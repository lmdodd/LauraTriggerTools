import FWCore.ParameterSet.Config as cms
import os
#from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')
# Set useful defaults
#options.inputFiles = '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/725/00000/B0A0345C-8E20-E611-A0B5-02163E01472B.root'
#options.inputFiles = '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/158/00000/1CCC1100-0E1A-E611-98C7-02163E014332.root'

options.outputFile = "tpg_eg_verification.root"


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
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v9'

# Load the correct global tag, based on the release
# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


#secondary files
from L1Trigger.LauraTriggerTools.Sec_2016_cfi import secondaryMap


process.source = cms.Source(
    "PoolSource",
    #fileNames = cms.untracked.vstring(options.inputFiles),
    fileNames = cms.untracked.vstring(
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/58BED76E-A51A-E611-BAD8-02163E01432C.root',
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/600F510E-A21A-E611-BE99-02163E011FA2.root',
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/6ECF7869-A61A-E611-A2F8-02163E012330.root',
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/9634CC5D-A41A-E611-8FD6-02163E011BBB.root',
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/96E2E896-AF1A-E611-834D-02163E013858.root',
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/9E59B5C8-B91A-E611-9EFD-02163E0139B9.root',
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/D85EB64A-A71A-E611-8935-02163E014424.root',
       '/store/data/Run2016B/SingleElectron/MINIAOD/PromptReco-v2/000/273/302/00000/DCE1F9C9-A81A-E611-A86C-02163E01474E.root',
    ) #,
    #duplicateCheckMode = cms.untracked.string('checkAllFilesOpened')
)



def dasQuery(queryString, entryTitle) :
    import das_client
    dasinfo = das_client.get_data('https://cmsweb.cern.ch', queryString, 0, 0, False)
    if dasinfo['status'] != 'ok' :
        raise Exception('DAS query failed.\nQuery: %s\nDAS Status returned: %s' % (queryString, dasinfo['status']))

    for entry in dasinfo['data'] :
        if len(entry[entryTitle]) > 0:
           yield entry[entryTitle][0] 

def getSecondaryFiles(primaryFileList) :
    secondaryFiles = []
    for primaryFile in primaryFileList :
        # TODO: Trim to LFN, e.g. /store/user/...
        query = 'parent file=%s' % primaryFile
        for entry in dasQuery(query, 'parent') :
            secondaryFiles.append(entry['name'].encode('ascii','ignore'))
    return secondaryFiles

process.source.secondaryFileNames = cms.untracked.vstring(getSecondaryFiles(process.source.fileNames))


process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
)

import FWCore.PythonUtilities.LumiList as LumiList
process.source.lumisToProcess = LumiList.LumiList(filename = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-273450_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt').getVLuminosityBlockRange()


process.tree = cms.EDAnalyzer(
    "EGRecoCalib",
    recoSrc = cms.InputTag("slimmedElectrons"),
    ecalSrc = cms.InputTag("ecalDigis:EcalTriggerPrimitives"), 
    hcalSrc = cms.InputTag("hcalDigis"), 
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
    process.ecalDigis*
    process.hcalDigis*
    process.egmGsfElectronIDSequence 
)


process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

