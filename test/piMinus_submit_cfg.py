import os
import FWCore.ParameterSet.Config as cms
import EventFilter.L1TXRawToDigi.util as util

process = cms.Process("HFCALIB")

## Import of standard configurations
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v6'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
        $inputFileNames
               )
)

process.load("Configuration.Geometry.GeometryExtended2016Reco_cff")
# To get CaloTPGTranscoder
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")



process.es_pool = cms.ESSource("PoolDBESSource",
     process.CondDBSetup,
     timetype = cms.string('runnumber'),
     toGet = cms.VPSet(
         cms.PSet(record = cms.string("HcalLutMetadataRcd"),
             tag = cms.string("HcalLutMetadata_HFTP_1x1")
             ),
         cms.PSet(record = cms.string("HcalElectronicsMapRcd"),
             tag = cms.string("HcalElectronicsMap_HFTP_1x1")
             )
         ),
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
     authenticationMethod = cms.untracked.uint32(0)
     )
process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )



process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("$outputFileName")
)

process.tree = cms.EDAnalyzer("PionCalibrations",
        triggerPrimitives = cms.InputTag("simHcalTriggerPrimitiveDigis","","HFCALIB"),
        #triggerPrimitives = cms.InputTag("hcalDigis"),
        #eTriggerPrimitives = cms.InputTag("simEcalTriggerPrimitiveDigis"),
        eTriggerPrimitives = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
        genSrc = cms.InputTag("genParticles"),
        doClosure = cms.untracked.bool(False)
)


process.p = cms.Path(process.hcalDigis * process.ecalDigis * process.simHcalTriggerPrimitiveDigis *process.tree)
#print process.dumpPython()
