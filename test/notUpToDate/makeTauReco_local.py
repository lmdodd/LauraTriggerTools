import FWCore.ParameterSet.Config as cms
import os
#from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = 'root://cmsxrootd.fnal.gov//store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/643/00000/F05E88C1-762C-E511-B786-02163E011B6C.root'

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
process.GlobalTag.globaltag = 'GR_P_V56'
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
# Load the correct global tag, based on the release
# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/521/00000/425B1189-6729-E511-AF38-02163E013728.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/522/00000/F6930521-5D29-E511-B517-02163E011D37.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/548/00000/44CE0650-EF29-E511-BA0D-02163E012601.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/559/00000/3C6564E0-A62C-E511-A11A-02163E012603.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/560/00000/A886DB09-E029-E511-8A06-02163E0125C8.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/561/00000/F6A7CE0F-132A-E511-A423-02163E011D88.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/562/00000/7CC342FD-A02A-E511-BE8D-02163E01182A.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/562/00000/96297F29-862A-E511-85C1-02163E013901.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/562/00000/C6637305-A12A-E511-AD71-02163E0138F6.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/562/00000/E0561625-7C2A-E511-9016-02163E0133D1.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/562/00000/EA8BA8F7-A02A-E511-A45F-02163E013473.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/604/00000/1606A3BF-972A-E511-86A7-02163E013515.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/612/00000/7A0CE8FF-A72A-E511-B7DC-02163E011D1C.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/638/00000/10C07DF0-FA2A-E511-846A-02163E01474A.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/642/00000/D20F8A8A-DE2A-E511-9D16-02163E0133FF.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/643/00000/1E72D617-BE2C-E511-96A0-02163E0139A2.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/643/00000/3C563818-BE2C-E511-993B-02163E0144D6.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/643/00000/C061E81E-BE2C-E511-AA5F-02163E01208E.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/643/00000/C6F9761A-BE2C-E511-932C-02163E011D30.root',
       '/store/data/Run2015B/SingleMuon/MINIAOD/PromptReco-v1/000/251/643/00000/CC199B16-BE2C-E511-B1A5-02163E012B30.root'
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
process.source.lumisToProcess = LumiList.LumiList(filename = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt').getVLuminosityBlockRange()




# Load emulation and RECO sequences
process.load("L1Trigger.LauraTriggerTools.emulation_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")

# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'
# Tree producers

# ECAL TPG Producer
#process.load("Geometry.EcalMapping.EcalMapping_cfi")
#process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")

# ECAL TPG Analyzer
#process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
#process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#
#process.ecalTriggerPrimitiveDigis = cms.EDProducer("EcalTrigPrimProducer",
#   InstanceEB = cms.string('ebDigis'),
#   InstanceEE = cms.string('eeDigis'),
#   Label = cms.string('ecalDigis'),
#
#   BarrelOnly = cms.bool(False),
#   Famos = cms.bool(False),
#   TcpOutput = cms.bool(False),
#
#   Debug = cms.bool(False),
#
#   binOfMaximum = cms.int32(6), ## optional from release 200 on, from 1-10
#
#)

#process.load("Geometry.CaloEventSetup.CaloTopology_cfi")





process.tree = cms.EDAnalyzer(
    "TauRecoCalib",
    taus = cms.InputTag("slimmedTaus"),
    TPGSF1 = TPG_SF_E_Full,
    TPGSFp = TPG_SF_H_Current,
    regionLSB = cms.double(0.5),
    egammaLSB = cms.double(1.0), # This has to correspond with the value from L1CaloEmThreshold
    egammaSeed = cms.int32(2),	
    ECALOn = cms.bool(False),	
    v_off = cms.bool(False),	
    v1 = cms.bool(True),	
    v3 = cms.bool(False)#only use for checking hcal
)



process.p1 = cms.Path(
    process.ecalDigis*
    #process.ecalTriggerPrimitiveDigis*
    process.emulationSequence*
    process.scalersRawToDigi
)


process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

