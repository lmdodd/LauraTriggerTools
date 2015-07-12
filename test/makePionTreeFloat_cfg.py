import FWCore.ParameterSet.Config as cms
import os
from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
#options.inputFiles = '/store/mc/Fall14DR73/SinglePi0Pt10/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/20000/4E7080EA-95C8-E411-A682-0025905B85D8.root'
options.inputFiles = 'file:/hdfs/store/mc/Fall14DR73/SinglePiPlusPt15/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/40000/C677C925-92C8-E411-A40B-0025905A6082.root' 

options.outputFile = "tpg_73x_verification.root"
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
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
# Load the correct global tag, based on the release
if 'CMSSW_7' in os.environ['CMSSW_VERSION']:
    process.GlobalTag.globaltag = 'MCRUN2_73_V12::All'
    print "Using global tag for upgrade MC: %s" % process.GlobalTag.globaltag
    if not options.isMC:
        raise ValueError("There is no data in CMSSW 7, you must mean isMC=1")
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
if not options.isMC:
    process.load("L1Trigger.LauraTriggerTools.emulation_cfi")
else:
    process.load("L1Trigger.LauraTriggerTools.emulation_cfi")
    #process.load("LauraTriggerTools.emulationMC_cfi")
process.load("L1Trigger.LauraTriggerTools.recoObjects53X_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")


# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'

# Tree producers
if options.isPi0:
   process.tree = cms.EDAnalyzer(
       "pionCalibImplF",
       TPGSF1 = TPG_SF_1_veto,
       TPGSF2 = TPG_SF_v2_off,
       TPGSFp = TPG_SF_3_veto,
       TPGSFp1 = TPG_SF_v3_off,
       regionLSB = cms.double(0.5),
       egammaLSB = cms.double(1.0), # This has to correspond with the value from L1CaloEmThreshold
       egammaSeed = cms.int32(2),	
       ECALOn = cms.bool(True)	
   )
else:
   process.tree = cms.EDAnalyzer(
       "pionCalibImplF",
       TPGSF1 = TPG_SF_1_veto,
       TPGSF2 = TPG_SF_v2_off,
       TPGSFp = TPG_SF_3_veto,
       TPGSFp1 = TPG_SF_v3_off,
       regionLSB = cms.double(0.5),
       egammaLSB = cms.double(1.0), # This has to correspond with the value from L1CaloEmThreshold
       egammaSeed = cms.int32(2),	
       ECALOn = cms.bool(False)	
   )

process.p1 = cms.Path(
    process.emulationSequence
    * process.scalersRawToDigi 
)

process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

