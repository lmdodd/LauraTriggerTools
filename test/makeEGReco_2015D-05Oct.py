import FWCore.ParameterSet.Config as cms
import os
#from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = '/store/data/Run2015D/DoubleEG/MINIAOD/PromptReco-v4/000/258/159/00000/34346FDB-306C-E511-9A4C-02163E013641.root'

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

process = cms.Process("L1Digis")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '74X_dataRun2_reMiniAOD_v0'
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
# Load the correct global tag, based on the release
# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


#secondary files
from L1Trigger.LauraTriggerTools.Sec_Oct05_cfi import secondaryMap

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles),
    secondaryFileNames = cms.untracked.vstring(secondaryMap[options.inputFiles[0]]),
    duplicateCheckMode = cms.untracked.string('checkAllFilesOpened')
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt').getVLuminosityBlockRange()




# Load emulation and RECO sequences
process.load("L1Trigger.LauraTriggerTools.emulation_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")

# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'
# Tree producers


process.tree = cms.EDAnalyzer(
    "EGRecoCalib",
    recoSrc = cms.InputTag("slimmedElectrons"),
    eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
    eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
    eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
    TPGSF1 = TPG_SF_E_Full, #current settings
    TPGSF2 = TPG_SF_E_Full, #current settings
    TPGSFp = TPG_SF_H_Current, #current settings
    TPGSFp1 = TPG_SF_H_Current, #current settings
    regionLSB = cms.double(0.5),
    egammaLSB = cms.double(1.0), # This has to correspond with the value from L1CaloEmThreshold
    egammaSeed = cms.int32(2),	
    ECALOn = cms.bool(True),	
    v_off = cms.bool(True),	
    v1 = cms.bool(False),	
    v2 = cms.bool(False),	
    v3 = cms.bool(False),	
    v4 = cms.bool(False)
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
    process.ecalDigis*
    process.emulationSequence*
    process.scalersRawToDigi*
    process.egmGsfElectronIDSequence 
)


process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

