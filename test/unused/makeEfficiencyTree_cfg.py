import FWCore.ParameterSet.Config as cms
import os
from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = 'file:/hdfs/store/user/laura/2012-08-01-CRAB_ZEESkim/skim_12_1_oNf.root'
#soon to be data
#options.inputFiles = '/store/user/laura/SinglePiPlusPt20/SinglePiPlusPt20-0093.root'

options.outputFile = "efficiency.root"


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
    'Set to 1')


options.parseArguments()

process = cms.Process("L1Digis")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
# Load the correct global tag, based on the release
if 'CMSSW_7' in os.environ['CMSSW_VERSION']:
    process.GlobalTag.globaltag = 'POSTLS162_V2::All'
    print "Using global tag for upgrade MC: %s" % process.GlobalTag.globaltag
    if not options.isMC:
        raise ValueError("There is no data in CMSSW 6, you must mean isMC=1")
else:
    if not options.isMC:
        # CMSSW 5 data
        process.GlobalTag.globaltag = 'GR_H_V28::All'
    else:
        # CMSSW 5 MC
        process.GlobalTag.globaltag = 'START53_V7B::All'
        raise ValueError("If MC-- you must mean CMSSW_7 isMC=1")
    process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
    process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
    print "Using global tag for 52X data: %s" % process.GlobalTag.globaltag

# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
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
    #get MC efficicency working
    #process.load("LauraTriggerTools.emulationMC_cfi")
process.load("L1Trigger.LauraTriggerTools.recoObjects_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")

# Determine which calibration to use
from L1Trigger.LauraTriggerTools.emulation_cfi import \
        eg_calib_v1, eg_calib_v3, eg_calib_v4

calib_map = {
    'CALIB_V1': eg_calib_v1,
    'CALIB_V3': eg_calib_v3,
    'CALIB_V4': eg_calib_v4
}


# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'

# Tree producers
if options.isPi0:
   process.tree = cms.EDAnalyzer(
       "pionCalibImpl",
       TPGSF1 = TPG_SF_1_veto,
       TPGSF2 = TPG_SF_v2_off,
       TPGSFp = TPG_SF_3_veto,
       TPGSFp1 = TPG_SF_v3_off,
       regionLSB = cms.double(0.5),
       egammaLSB = cms.double(1.0), # This has to correspond with the value from L1CaloEmThreshold
       egammaSeed = cms.int32(2),	
       ECALOn = cms.bool(True)	
   )

#process.EgEffi = cms.EDAnalyzer(
process.tree = cms.EDAnalyzer(
    "MiniEffiTree",
    src = cms.VInputTag("recoElecs"),
    recoSrc = cms.VInputTag(cms.InputTag("l1ExtraLayer2","NonIsolated")),
    srcJet = cms.VInputTag("ak5PFJets"),
    recoSrcJet = cms.VInputTag(cms.InputTag("l1ExtraLayer2","Central")),
    metSrc = cms.VInputTag("tcMet"),
    recoMetSrc = cms.VInputTag(cms.InputTag("l1ExtraLayer2","MET"))
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
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

