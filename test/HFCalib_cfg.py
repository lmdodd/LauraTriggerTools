import FWCore.ParameterSet.Config as cms

process = cms.Process("HFCALIB")

## Import of standard configurations
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v0'

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/047E357A-E6C3-E511-9783-001F29082E76.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/047E357A-E6C3-E511-9783-001F29082E76.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/08B82CDF-ECC3-E511-B583-1418774108DF.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/08C92242-05C4-E511-8C46-44A842CF0627.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/0C256E01-FFC3-E511-AE78-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/12082B9E-EEC3-E511-8D64-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/125ADC2C-F8C3-E511-97D2-B083FED42B3B.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/1A1D376C-FCC3-E511-B095-7845C4FC346A.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/1CFEE202-FFC3-E511-983A-00266CF9ADA0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/2003A0AA-FAC3-E511-AA4E-782BCB1F0729.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/201C456D-FCC3-E511-AFC1-3417EBE6463F.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/349ADD76-E6C3-E511-AC26-44A842CF0627.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/34DB91F4-FAC3-E511-BF89-00266CF9BCC4.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/3C6AE649-F6C3-E511-9906-782BCB206470.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/3C8D74BF-F7C3-E511-B29F-782BCB53A09D.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/42C6C7E7-E2C3-E511-B5E3-3417EBE5280A.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/44DD6369-01C4-E511-B644-001CC4A6AD5E.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/5204D875-0AC4-E511-820F-782BCB20E908.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/561F014F-F3C3-E511-B118-00A0D1EE2A50.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/58E7D478-F4C3-E511-B286-00A0D1EE29D0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/5C5D8ACD-EDC3-E511-9D43-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/62CA80FE-E9C3-E511-B002-C81F66B7864D.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/6402965C-ECC3-E511-91C3-00266CF9B254.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/646D7702-F0C3-E511-AA22-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/74AA95BF-EFC3-E511-AB11-00266CF250C0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/80419CCF-F4C3-E511-BB00-A4BADB1E65B1.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8233C7EF-F6C3-E511-9F0E-782BCB206470.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8235A177-01C4-E511-8235-549F3525E81C.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8602B67E-F1C3-E511-9DF3-00266CF9C0AC.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8ACCB36B-01C4-E511-B762-90B11C0BB9FC.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8C11E52D-30C4-E511-A790-44A842CFC9B2.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8EC8E67E-FCC3-E511-987D-001C23C0DE8E.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/902FA94B-F5C3-E511-9B09-B499BAABF064.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/92956D6D-FCC3-E511-9E66-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/9621CF07-F8C3-E511-AE9E-00266CF9B970.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/A200537D-F0C3-E511-9282-00266CF250C0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/A280E5BB-F4C3-E511-866B-00A0D1EE2A50.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/A44EEB6F-FCC3-E511-A95A-00266CFAEA48.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/ACCBC67D-F7C3-E511-8556-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/B02E0D24-F9C3-E511-8654-00266CF2679C.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/B4DCC298-03C4-E511-A992-782BCB2100C5.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/B62E1F6D-01C4-E511-8AC7-A4BADB22A4AE.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CA8BF575-F2C3-E511-89A2-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CCE8E774-F2C3-E511-811F-549F3525AE58.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CE86D8E0-F0C3-E511-92F7-549F3525DD6C.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CE95ED08-F5C3-E511-89A6-00266CF9C0AC.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/D24E17D7-EFC3-E511-846A-A4BADB1E6F7A.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/DC90D32B-F2C3-E511-AA18-001EC94BF09D.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/E6B7B3CD-1BC4-E511-83D8-549F3525B9A0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/EA059C3E-EEC3-E511-A46B-B083FED024B2.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/EE068048-F6C3-E511-A72E-3417EBE6449E.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/F4FE5BAE-06C4-E511-B8D4-00266CF9B59C.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/047E357A-E6C3-E511-9783-001F29082E76.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/08B82CDF-ECC3-E511-B583-1418774108DF.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/08C92242-05C4-E511-8C46-44A842CF0627.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/0C256E01-FFC3-E511-AE78-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/12082B9E-EEC3-E511-8D64-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/125ADC2C-F8C3-E511-97D2-B083FED42B3B.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/1A1D376C-FCC3-E511-B095-7845C4FC346A.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/1CFEE202-FFC3-E511-983A-00266CF9ADA0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/2003A0AA-FAC3-E511-AA4E-782BCB1F0729.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/201C456D-FCC3-E511-AFC1-3417EBE6463F.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/349ADD76-E6C3-E511-AC26-44A842CF0627.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/34DB91F4-FAC3-E511-BF89-00266CF9BCC4.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/3C6AE649-F6C3-E511-9906-782BCB206470.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/3C8D74BF-F7C3-E511-B29F-782BCB53A09D.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/42C6C7E7-E2C3-E511-B5E3-3417EBE5280A.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/44DD6369-01C4-E511-B644-001CC4A6AD5E.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/5204D875-0AC4-E511-820F-782BCB20E908.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/561F014F-F3C3-E511-B118-00A0D1EE2A50.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/58E7D478-F4C3-E511-B286-00A0D1EE29D0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/5C5D8ACD-EDC3-E511-9D43-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/62CA80FE-E9C3-E511-B002-C81F66B7864D.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/6402965C-ECC3-E511-91C3-00266CF9B254.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/646D7702-F0C3-E511-AA22-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/74AA95BF-EFC3-E511-AB11-00266CF250C0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/80419CCF-F4C3-E511-BB00-A4BADB1E65B1.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8233C7EF-F6C3-E511-9F0E-782BCB206470.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8235A177-01C4-E511-8235-549F3525E81C.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8602B67E-F1C3-E511-9DF3-00266CF9C0AC.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8ACCB36B-01C4-E511-B762-90B11C0BB9FC.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8C11E52D-30C4-E511-A790-44A842CFC9B2.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/8EC8E67E-FCC3-E511-987D-001C23C0DE8E.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/902FA94B-F5C3-E511-9B09-B499BAABF064.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/92956D6D-FCC3-E511-9E66-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/9621CF07-F8C3-E511-AE9E-00266CF9B970.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/A200537D-F0C3-E511-9282-00266CF250C0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/A280E5BB-F4C3-E511-866B-00A0D1EE2A50.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/A44EEB6F-FCC3-E511-A95A-00266CFAEA48.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/ACCBC67D-F7C3-E511-8556-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/B02E0D24-F9C3-E511-8654-00266CF2679C.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/B4DCC298-03C4-E511-A992-782BCB2100C5.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/B62E1F6D-01C4-E511-8AC7-A4BADB22A4AE.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CA8BF575-F2C3-E511-89A2-00266CF9AB88.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CCE8E774-F2C3-E511-811F-549F3525AE58.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CE86D8E0-F0C3-E511-92F7-549F3525DD6C.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/CE95ED08-F5C3-E511-89A6-00266CF9C0AC.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/D24E17D7-EFC3-E511-846A-A4BADB1E6F7A.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/DC90D32B-F2C3-E511-AA18-001EC94BF09D.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/E6B7B3CD-1BC4-E511-83D8-549F3525B9A0.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/EA059C3E-EEC3-E511-A46B-B083FED024B2.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/EE068048-F6C3-E511-A72E-3417EBE6449E.root',
'/store/mc/RunIIFall15DR76/VBF_HToInvisible_M125_13TeV_powheg_pythia8/GEN-SIM-RAW/25nsPUfixed30NzshcalRaw_76X_mcRun2_asymptotic_v12-v1/00000/F4FE5BAE-06C4-E511-B8D4-00266CF9B59C.root',
        )
)


process.load('L1Trigger.RegionalCaloTrigger.rctDigis_cfi')
process.rctDigis.hcalDigis = cms.VInputTag(cms.InputTag("simHcalTriggerPrimitiveDigis"))

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.simHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag( cms.InputTag('simHcalUnsuppressedDigis'), cms.InputTag('simHcalUnsuppressedDigis') )
# process.simHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag( cms.InputTag('simHcalDigis'), cms.InputTag('simHcalDigis') )
process.simHcalTriggerPrimitiveDigis.FrontEndFormatError = cms.bool(False)

process.load("Configuration.Geometry.GeometryExtended2016Reco_cff")
process.XMLIdealGeometryESSource.geomXMLFiles.remove('Geometry/HcalCommonData/data/Phase0/hcalRecNumbering.xml')
process.XMLIdealGeometryESSource.geomXMLFiles.append('Geometry/HcalCommonData/data/Phase0/hcalRecNumberingRun2.xml')

process.es_pool = cms.ESSource("PoolDBESSource",
     process.CondDBSetup,
     timetype = cms.string('runnumber'),
     toGet = cms.VPSet(
         cms.PSet(record = cms.string("HcalLutMetadataRcd"),
             tag = cms.string("HcalLutMetadata_HFTP_1x1")
             )
         ),
     connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
     authenticationMethod = cms.untracked.uint32(0)
     )
process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string('analyze.root'))

#process.analyze = cms.EDAnalyzer("AnalyzeTP",
#        triggerPrimitives = cms.InputTag("simHcalTriggerPrimitiveDigis", "" , "HFCALIB"))
process.tree = cms.EDAnalyzer("JetHFCalib",
        triggerPrimitives = cms.InputTag("simHcalTriggerPrimitiveDigis", "" , "HFCALIB"),
        genSrc = cms.InputTag("ak4GenJetsNoNu","","HLT"),
        doClosure = cms.untracked.bool(True)
)



#process.p = cms.Path(process.simHcalTriggerPrimitiveDigis * process.analyze * process.analyzeOld)
process.p = cms.Path(process.simHcalTriggerPrimitiveDigis * process.tree )

# print process.dumpPython()
