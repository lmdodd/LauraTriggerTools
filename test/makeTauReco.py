import FWCore.ParameterSet.Config as cms
import os
from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
#options.inputFiles = 'file:/hdfs/store/user/laura/2012-08-01-CRAB_ZEESkim/skim_12_1_oNf.root'
options.inputFiles = 'file:/hdfs/store/user/ldodd/DYJetsToLL_M-50_13TeV-pythia6/DYJetsToLL_M-50_13TeV-pythia6_Fall13dr-tsg_PU20bx25_POSTLS162_V2-v1/05a81b8d696d27a5c3c2ca036967addd/skim_4_1_OgG.root'
#soon to be data
#options.inputFiles = '/store/user/laura/SinglePiPlusPt20/SinglePiPlusPt20-0093.root'

options.outputFile = "tpg_tau_verification.root"
#options.outputFile = "tpg_hcal_verification.root"


options.register(
    'isMC',
    1,
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

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
# Load the correct global tag, based on the release
if 'CMSSW_6' in os.environ['CMSSW_VERSION']:
    process.GlobalTag.globaltag = 'POSTLS162_V2::All'
    print "Using global tag for upgrade MC: %s" % process.GlobalTag.globaltag
    if not options.isMC:
        raise ValueError("There is no data in CMSSW 6, you must mean isMC=1")
else:
    #raise ValueError("Why aren't you using CMSSW 6?")
    if not options.isMC:
        # CMSSW 5 data
        process.GlobalTag.globaltag = 'GR_H_V28::All'
    else:
        # CMSSW 5 MC
        process.GlobalTag.globaltag = 'START53_V7B::All'
    process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
    process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
    print "Using global tag for 52X data: %s" % process.GlobalTag.globaltag

# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
)

# Load emulation and RECO sequences
process.load("L1Trigger.LauraTriggerTools.emulation_cfi")
process.load("L1Trigger.LauraTriggerTools.recoObjects53X_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")

# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'

# Tree producers
process.tree = cms.EDAnalyzer(
    "EGRecoCalib",
    recoSrc = cms.VInputTag("recoTaus"),
    TPGSF1 = TPG_SF_1_veto,
    TPGSF2 = TPG_SF_v2_int,
    TPGSFp = TPG_SF_v3_off,
    #TPGSFp = TPG_SF_3_veto,
    TPGSFp1 = TPG_SF_v3_off,
    regionLSB = cms.double(0.5),
    egammaLSB = cms.double(1.0), # This has to correspond with the value from L1CaloEmThreshold
    egammaSeed = cms.int32(2),	
    ECALOn = cms.bool(False),	
    v_off = cms.bool(False),	
    v1 = cms.bool(True),	
    v2 = cms.bool(False),	
    v3 = cms.bool(False),	
    v4 = cms.bool(False)
)

reco_step=process.recoObjects

process.p1 = cms.Path(
    process.emulationSequence *
    process.scalersRawToDigi *
    reco_step 
)


process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(True)
   #wantSummary = cms.untracked.bool(True),
   #SkipEvent = cms.untracked.vstring('ProductNotFound')
)
