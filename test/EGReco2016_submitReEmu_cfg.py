import FWCore.ParameterSet.Config as cms
import os
#from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/725/00000/B0A0345C-8E20-E611-A0B5-02163E01472B.root'
#options.inputFiles = '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/158/00000/1CCC1100-0E1A-E611-98C7-02163E014332.root'

options.outputFile = "tpg_eg_verification.root"
#options.outputFile = "tpg_hcal_verification.root"


options.register(
    'isMC',
    0,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    'Set to 1 for simulated samples - updates GT, emulates HCAL TPGs.')

options.register(
    'isPi0',
    0,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    'Set to 1 for running over pi0 samples. Find Max TPG in ECAL.')


options.parseArguments()

from Configuration.StandardSequences.Eras import eras

process = cms.Process("L1TCaloSummaryTest",eras.Run2_2016)
# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8'

# Load the correct global tag, based on the release
# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


#secondary files
from L1Trigger.LauraTriggerTools.Sec_2016_cfi import secondaryMap


# To get L1 CaloParams
process.load('L1Trigger.L1TCalorimeter.caloStage2Params_cfi')
# To get CaloTPGTranscoder
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

process.HcalTPGCoderULUT.LUTGenerationMode = cms.bool(False)

process.load("Configuration.Geometry.GeometryExtended2016Reco_cff")


from L1Trigger.Configuration.customiseReEmul import L1TEventSetupForHF1x1TPs,L1TReEmulFromRAW,L1TReEmulFromRAWsimTP 
process = L1TEventSetupForHF1x1TPs(process)

process.load('L1Trigger.Configuration.SimL1Emulator_cff')
process.load('L1Trigger.Configuration.CaloTriggerPrimitives_cff')
process.simEcalTriggerPrimitiveDigis.Label = 'ecalDigis'
process.simHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag(
    cms.InputTag('hcalDigis'),
    cms.InputTag('hcalDigis')
    )
process.L1TReEmul = cms.Sequence(process.simHcalTriggerPrimitiveDigis * process.simEcalTriggerPrimitiveDigis)


process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles),
    secondaryFileNames = cms.untracked.vstring(secondaryMap[options.inputFiles[0]]) #,
)



process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-273450_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt').getVLuminosityBlockRange()

process.tree = cms.EDAnalyzer(
    "EGRecoCalib",
    recoSrc = cms.InputTag("slimmedElectrons"),
    ecalSrc = cms.InputTag("simEcalTriggerPrimitiveDigis"), 
    hcalSrc = cms.InputTag("simHcalTriggerPrimitiveDigis"), 
    eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
    eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
    eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
    TPGSF1 = TPG_SF_E_Full, #current settings
    TPGSFp = TPG_SF_H_Current, #current settings
    regionLSB = cms.double(0.5),
    v_off = cms.bool(True),	
    v1 = cms.bool(False),	
    v3 = cms.bool(False)	
)


from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.MiniAOD

switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)


process.p1 = cms.Path(
    process.RawToDigi*process.L1TReEmul*
    process.egmGsfElectronIDSequence 
)


process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

