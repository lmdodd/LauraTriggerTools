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
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/105B3BC2-2BED-E511-A51F-008CFA56D6F4.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/1086966F-39ED-E511-ACE1-008CFA197D2C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/12EE2FE4-39ED-E511-B177-00266CFBE88C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/141A1434-28ED-E511-BED6-00266CFCCB8C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/188C0960-31ED-E511-9BFB-008CFA0648FC.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/189066C3-2BED-E511-BFAA-008CFA56D870.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/1CF6B2E1-2EED-E511-B574-A0369F7F9DE0.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/1ED7067A-28ED-E511-A454-001E67A40604.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/22301599-2AED-E511-9B8B-549F35AD8B6E.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/226A51A0-25ED-E511-8935-001E67DDD0AA.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/2CA56DC0-2BED-E511-873F-549F358EB755.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/30B673FA-30ED-E511-A991-008CFA05E898.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/38B39F39-2FED-E511-A87A-001E67A400F0.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/42D239B6-26ED-E511-88A1-90B11C050429.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/483DED91-26ED-E511-82F0-A0369F7FC540.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/48754171-29ED-E511-B3F3-008CFA166014.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/48A4C643-2FED-E511-95C3-001E67A404B5.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/4A0FC4E7-2FED-E511-8AB8-008CFA165FD0.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/4AD3C408-27ED-E511-865D-A0369F7FC4E8.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/4CBD652E-33ED-E511-8B6E-001E675A6630.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/508359B1-38ED-E511-8746-001517F7F410.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/5A50482E-28ED-E511-B213-008CFA1C907C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/5A76B21D-35ED-E511-9BA1-001E67A3FEB1.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/5AA10863-2DED-E511-B1E3-001E675A69DC.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/62376E1C-34ED-E511-98A9-A0369F7FC54C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/64CA147A-2CED-E511-82D8-001E675A659A.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/64FF2665-27ED-E511-9236-90B11C08CDC7.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/66BDB9ED-32ED-E511-924B-008CFA05E898.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/6827304E-36ED-E511-95AE-008CFA05E898.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/6CB14289-32ED-E511-A140-008CFA06488C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/6CFD0AA8-36ED-E511-8E43-A0369F7FC4E8.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/744959EC-36ED-E511-85BE-001517F7F410.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/768DC390-2DED-E511-AD7D-A0369F7FC210.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/7880B68F-2DED-E511-8AAD-A0369F7F9DE0.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/7C5C22A4-36ED-E511-9E7E-008CFA166014.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/820E4CFF-2BED-E511-AC19-549F35AD8B6E.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/8C74C200-35ED-E511-A050-008CFA05E898.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/98087D65-2AED-E511-80A6-001E67A40514.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/9CDF8E6C-27ED-E511-8BBE-001E67A40604.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/A63C77E1-35ED-E511-977E-001E67A40514.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/A6FE836D-2BED-E511-B178-001E67DDD22B.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/AC5CAFB6-34ED-E511-9F36-008CFA166014.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/B86F6FAD-36ED-E511-B165-A0369F7F9314.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/BE3A9366-29ED-E511-A36D-001E67A3FE66.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/C4097092-37ED-E511-8C02-008CFA197D2C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/C4FDFFAF-36ED-E511-BA45-008CFA05E8EC.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/CC8EF942-34ED-E511-8AEE-D4AE52AAF583.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/CE063321-34ED-E511-880C-008CFA06488C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/CE482D78-37ED-E511-A440-001E675A69DC.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/CE79A6E1-32ED-E511-8107-00266CFCE03C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/CEC142AD-32ED-E511-B9CD-008CFA05E978.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/D29C7164-31ED-E511-8AAC-001E67DDD0B9.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/DA3DA57A-2FED-E511-9D4F-008CFA0516BC.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/E03649DE-32ED-E511-B290-008CFA05E8EC.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/E437B0BF-30ED-E511-A62A-008CFA05E978.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/F0C6A580-2AED-E511-B6BF-008CFA06488C.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/F24FCA92-2CED-E511-957C-001E67A3F3DF.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/F4891E75-28ED-E511-8938-001E67A3F8A8.root',
       '/store/mc/RunIIFall15DR76/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/GEN-SIM-RAW/25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/60000/F815ABC4-34ED-E511-BEFF-00266CFCC860.root'
        )
)

process.load("Configuration.Geometry.GeometryExtended2016Reco_cff")
# To get CaloTPGTranscoder
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")


process.load("Configuration.Geometry.GeometryExtended2016Reco_cff")

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
        fileName = cms.string('analyzePion.root')
)

process.tree = cms.EDAnalyzer("PionCalibrations",
        triggerPrimitives = cms.InputTag("simHcalTriggerPrimitiveDigis","","HFCALIB"),
        #triggerPrimitives = cms.InputTag("hcalDigis"),
        #eTriggerPrimitives = cms.InputTag("simEcalTriggerPrimitiveDigis"),
        eTriggerPrimitives = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
        genSrc = cms.InputTag("genParticles"),
        doClosure = cms.untracked.bool(False)
)


#process.p = cms.Path(process.hcalDigis * process.ecalDigis * process.tree )
#process.p = cms.Path(process.hcalDigis * process.simHcalTriggerPrimitiveDigis * process.ecalDigis * process.tree )
#process.p = cms.Path(process.l1tCaloLayer1Digis * process.tree)
process.p = cms.Path(process.hcalDigis * process.ecalDigis * process.simHcalTriggerPrimitiveDigis *process.tree)

#process.p = cms.Path(process.simHcalTriggerPrimitiveDigis * process.l1tCaloLayer1Digis* process.tree)

#process.p = cms.Path(process.l1tCaloLayer1Digis*process.simCaloStage2Layer1Digis*process.rctDigis * process.tree)
#print process.dumpPython()
