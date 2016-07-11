import FWCore.ParameterSet.Config as cms
import os
#from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = '/store/data/Run2016B/Tau/MINIAOD/PromptReco-v2/000/275/001/00000/5E3E354F-4F34-E611-AD7E-02163E014660.root'

#options.outputFile = "tpg_eg_verification.root"
options.outputFile = "tpg_hcal_verification.root"


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
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v9'
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


# Load the correct global tag, based on the release
# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/data/Run2016B/Tau/MINIAOD/PromptReco-v2/000/275/001/00000/5E3E354F-4F34-E611-AD7E-02163E014660.root',
      '/store/data/Run2016B/Tau/MINIAOD/PromptReco-v2/000/275/001/00000/66B34328-4F34-E611-97F5-02163E011A4E.root',
      '/store/data/Run2016B/Tau/MINIAOD/PromptReco-v2/000/275/001/00000/68AE4A2E-4F34-E611-865A-02163E0143AC.root',
      '/store/data/Run2016B/Tau/MINIAOD/PromptReco-v2/000/275/001/00000/B427591E-4F34-E611-9493-02163E014735.root'
    ),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
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
process.source.lumisToProcess = LumiList.LumiList(filename = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt').getVLuminosityBlockRange()





process.tree = cms.EDAnalyzer(
    "TauRecoCalib",
    taus = cms.InputTag("slimmedTaus"),
    TPGSF1 = TPG_SF_E_Full,
    TPGSFp = TPG_SF_H_Current,
    regionLSB = cms.double(0.5),
    ECALOn = cms.bool(False),	
    v_off = cms.bool(False),	
    v1 = cms.bool(True),	
    v3 = cms.bool(False)#only use for checking hcal
)



process.p1 = cms.Path(
    process.ecalDigis*
    process.hcalDigis*
    process.scalersRawToDigi
)


process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

