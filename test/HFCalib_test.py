import FWCore.ParameterSet.Config as cms

process = cms.Process("HFCALIB")

## Import of standard configurations
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v6'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("CondCore.DBCommon.CondDBCommon_cfi")


process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
'file:/hdfs/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/40000/06A3BC65-BFEC-E511-9A08-0025905C95F8.root'
        )
)



process.load("Configuration.Geometry.GeometryExtended2016Reco_cff")

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.simHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag( cms.InputTag('simHcalUnsuppressedDigis'), cms.InputTag('simHcalUnsuppressedDigis') )
process.simHcalTriggerPrimitiveDigis.FrontEndFormatError = cms.bool(False)

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
        fileName = cms.string("output_HF.root"))

process.tree = cms.EDAnalyzer("JetHFCalib",
        triggerPrimitives = cms.InputTag("simHcalTriggerPrimitiveDigis", "" , "HFCALIB"),
        eTriggerPrimitives = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
        genSrc = cms.InputTag("ak4GenJetsNoNu","","HLT"),
        doClosure = cms.untracked.bool(True)
)



#process.p = cms.Path(process.simHcalTriggerPrimitiveDigis * process.analyze * process.analyzeOld)
process.p = cms.Path(process.ecalDigis * process.hcalDigis * process.simHcalTriggerPrimitiveDigis * process.tree )

# print process.dumpPython()
