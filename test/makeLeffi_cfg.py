import FWCore.ParameterSet.Config as cms
import os
from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.emulation_cfi import *
from FWCore.ParameterSet.VarParsing import VarParsing
#from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = 'file:/hdfs/store/user/laura/2012-08-01-CRAB_ZEESkim/skim_12_1_oNf.root'
#soon to be data
#options.inputFiles = '/store/user/laura/SinglePiPlusPt20/SinglePiPlusPt20-0093.root'

options.outputFile = "leffi.root"

options.register(
                 'isMC',
                 0,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 'Set to 1 for simulated samples - updates GT, emulates HCAL TPGs.')

options.parseArguments()

process = cms.Process("L1Effi")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
# Load the correct global tag, based on the release
if 'CMSSW_6' in os.environ['CMSSW_VERSION']:
    raise ValueError("USe CMSSW_5 for this")
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

# Load Geometry and RECO sequences
process.load("L1Trigger.LauraTriggerTools.recoObjects_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")


# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'


process.load("L1Trigger.LauraTriggerTools.emulation_cfi")





#process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
#process.load("L1Trigger.Configuration.L1Reco_cff")

#'''This remakes L1 regions'''
process.l1Digis = cms.EDProducer(
    "L1RCTProducer",
    hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis")),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives")),
    BunchCrossings = cms.vint32(0),
    getFedsFromOmds = cms.bool(False),
    queryDelayInLS = cms.uint32(10),
    queryIntervalInLS = cms.uint32(100)#,
)

#process.p = cms.Path(
#    process.gctDigis
#    * process.gtDigis
#    * process.ecalDigis
#    * process.hcalDigis
#)
#process.l1DigiStep = cms.Sequence(
    # Only do the digitization of objects that we care about
    #RawToDigi
#    process.gctDigis
#    * process.gtDigis
#    * process.ecalDigis
#    * process.hcalDigis
#)
#process.emulationSequence = cms.Sequence(process.l1DigiStep * process.l1Digis)


# Tree producers
process.tree = cms.EDAnalyzer(
                              "LEffiTree",
                              recoSrc = cms.VInputTag("recoElecs"),
                              l1Src = cms.VInputTag(cms.InputTag("L1RCTProducer","l1Digis")),
                              regionLSB = cms.double(0.5),
                              egammaLSB = cms.double(1.0) # This has to correspond with the value from L1CaloEmThreshold
                              )

reco_step=process.recoObjects

#process.p1 = cms.Path(
#    reco_step *
#    process.emulationSequence
#)

process.p1 = cms.Path(
                      process.emulationSequence *
                      process.l1Digis *
                      process.scalersRawToDigi *
                      reco_step 
                      )

process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

