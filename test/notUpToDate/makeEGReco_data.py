import FWCore.ParameterSet.Config as cms
import os
#from L1Trigger.LauraTriggerTools.Lut import *
from L1Trigger.LauraTriggerTools.TPG_cfi import *
# Get command line options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# Set useful defaults
options.inputFiles = '/store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/162/00000/9CC606D8-4127-E511-8F35-02163E013830.root'

options.outputFile = "tpg_eg_verificationREEMU.root"
#options.outputFile = "tpg_hcal_verification.root"


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

from L1Trigger.LauraTriggerTools.TPG_cfi import *

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = 'GR_P_V56'
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.GlobalTag.toGet = cms.VPSet( 
     cms.PSet(record = cms.string("EcalTPGLinearizationConstRcd"), 
              tag = cms.string("EcalTPGLinearizationConst_weekly_test2_hlt"), 
              connect =cms.untracked.string('frontier://FrontierPrep/CMS_CONDITIONS') 
              ) 
 )  

# Load the correct global tag, based on the release
# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source(
    "PoolSource",
    #fileNames = cms.untracked.vstring(options.inputFiles),
    fileNames = cms.untracked.vstring(
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/096/00000/8A2D533C-5626-E511-AF3C-02163E011FAB.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/161/00000/AC857A3B-9C26-E511-B32E-02163E012704.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/162/00000/BE906A2C-4327-E511-8014-02163E0121CC.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/163/00000/9CB965BF-9F26-E511-8FB1-02163E012040.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/164/00000/2CA2349A-A526-E511-A371-02163E0138D0.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/167/00000/C446EC67-A726-E511-81C1-02163E0119E7.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/168/00000/747F782A-BB26-E511-BA24-02163E011EE9.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/6A0A8868-4B27-E511-B3F8-02163E011BD1.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/7E66D014-D628-E511-9D1E-02163E012603.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/84B7599F-4E27-E511-9DEC-02163E014509.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/C47A9CF9-6227-E511-908E-02163E014509.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/E8FEB382-3228-E511-8B4D-02163E0133E3.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/244/00000/F2D83438-6927-E511-A22B-02163E01182A.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/251/00000/4AA28544-9027-E511-AEF5-02163E01472B.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/251/00000/E6F7FA3F-7E27-E511-B271-02163E014181.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/252/00000/2672D59D-B827-E511-BB31-02163E01358B.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/252/00000/3866DC22-8627-E511-8700-02163E01463E.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/252/00000/4A3B62E0-8D27-E511-9EC6-02163E0140E1.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/252/00000/8A8CCAA5-8827-E511-A64E-02163E014558.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/252/00000/8CE81C36-8427-E511-9817-02163E0127D3.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/252/00000/F0686F9B-BA27-E511-82C7-02163E0142FD.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/491/00000/EAD8F705-C628-E511-A141-02163E0133B6.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/493/00000/324A1F04-C928-E511-AE77-02163E011ABC.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/496/00000/024C9298-912C-E511-A109-02163E0144D6.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/497/00000/66FC7B3B-E028-E511-AB85-02163E01463E.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/498/00000/6A6EB96C-EC28-E511-8B71-02163E01340C.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/499/00000/D89CF3FD-F728-E511-AC0C-02163E0136CF.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/500/00000/3E3DEF56-2429-E511-8E40-02163E0128FE.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/521/00000/7CA3211F-8F29-E511-BE9E-02163E0133E3.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/522/00000/7853D0B3-8B29-E511-8B13-02163E012073.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/548/00000/14AF4497-132A-E511-BF13-02163E0121BD.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/559/00000/30C4B973-A72C-E511-8BA8-02163E013542.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/560/00000/ECAEB6CB-F429-E511-A6D1-02163E01457B.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/561/00000/0CEAA0EF-492A-E511-806F-02163E0133F0.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/561/00000/50B8C3F1-8A2A-E511-AE28-02163E014166.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/561/00000/8EB8538F-642B-E511-992E-02163E01457B.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/561/00000/BAE3F236-592A-E511-9F1C-02163E0144D6.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/561/00000/C2300D47-E22B-E511-B6EA-02163E0126E1.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/561/00000/E454DDF2-3F2A-E511-B46E-02163E011A29.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/561/00000/EE25CDA5-352A-E511-8C17-02163E01369B.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/04EB3F26-492A-E511-994F-02163E0133AD.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/125577E7-5F2A-E511-B651-02163E01208E.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/181FCE1C-542A-E511-BBB6-02163E01412F.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/2898E65B-642A-E511-AF62-02163E01299A.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/28BB98B7-6A2A-E511-AD9B-02163E0133BA.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/28F0FA63-722A-E511-9130-02163E011861.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/36C1E05E-E42A-E511-B50C-02163E011DE5.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/38A35AAD-5B2A-E511-B5AB-02163E0128B9.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/40BC5DC5-422A-E511-B333-02163E011DCE.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/4225C576-822A-E511-A4E9-02163E01338A.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/44B38FF2-3D2A-E511-8BE9-02163E0127DF.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/44F8BDC0-382A-E511-8233-02163E01208E.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/500AD530-482A-E511-9562-02163E014241.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/5A22D222-4B2A-E511-863B-02163E011BD1.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/60240C09-6E2A-E511-B696-02163E01241A.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/7AC612FA-782A-E511-92C2-02163E01414A.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/842125B7-5D2A-E511-86E5-02163E011C7F.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/8E27C7CB-462A-E511-B23D-02163E01267F.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/8E5C4F4C-592A-E511-A05C-02163E0126A0.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/98CB4FB3-502A-E511-B6DA-02163E0136E2.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/AAED62CE-612A-E511-923C-02163E01399F.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/BACEEB7E-552A-E511-827B-02163E011A46.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/BC0E9C49-4D2A-E511-87A0-02163E01412F.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/BE7B13AA-F62A-E511-9D0B-02163E012601.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/C2AC6086-572A-E511-A7BB-02163E01208E.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/C476CE2C-4F2A-E511-BCEA-02163E01208E.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/CCEFEFB1-402A-E511-BF33-02163E0136E2.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/D4AAE55B-4C2A-E511-B026-02163E0126E1.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/E0806DA0-522A-E511-9725-02163E012857.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/E648F383-662A-E511-B1F8-02163E0137FC.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/562/00000/F2E3B5FB-432A-E511-9772-02163E01379D.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/604/00000/D02F3FAD-932A-E511-8196-02163E012B30.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/612/00000/B0976CB6-A12A-E511-BEE5-02163E012601.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/628/00000/901BCAD1-B52A-E511-99B0-02163E0146D1.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/638/00000/BEB8ACAE-F82A-E511-A5CB-02163E013793.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/642/00000/6C9BEE18-DA2A-E511-A902-02163E0133FF.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/643/00000/62942F94-912C-E511-80E0-02163E01340A.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/643/00000/F05E88C1-762C-E511-B786-02163E011B6C.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/716/00000/FEEE8ED5-C32B-E511-9562-02163E013728.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/721/00000/CA2AB92F-192C-E511-B266-02163E011DCE.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/781/00000/E6B7BCB8-B02C-E511-ADF5-02163E011D30.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/251/883/00000/DA459E92-4F2D-E511-AF66-02163E01474A.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/252/102/00000/743C3E35-D92F-E511-810C-02163E0139B0.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/252/116/00000/10293DB4-7730-E511-82D6-02163E0117FF.root',
       '/store/data/Run2015B/DoubleEG/MINIAOD/PromptReco-v1/000/252/126/00000/DA16B12A-E930-E511-8AD3-02163E011A02.root'
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
#process.load("LauraTriggerTools.emulationMC_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")

# Read inst. lumi. info from the scalers
process.load("EventFilter.ScalersRawToDigi.ScalersRawToDigi_cfi")
process.scalersRawToDigi.scalersInputTag = 'rawDataCollector'
# Tree producers

# ECAL TPG Producer
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")

# ECAL TPG Analyzer
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.ecalTriggerPrimitiveDigis = cms.EDProducer("EcalTrigPrimProducer",
   InstanceEB = cms.string('ebDigis'),
   InstanceEE = cms.string('eeDigis'),
   Label = cms.string('ecalDigis'),

   BarrelOnly = cms.bool(False),
   Famos = cms.bool(False),
   TcpOutput = cms.bool(False),

   Debug = cms.bool(False),

   binOfMaximum = cms.int32(6), ## optional from release 200 on, from 1-10

)

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")


process.tree = cms.EDAnalyzer(
    "EGRecoCalib",
    recoSrc = cms.InputTag("slimmedElectrons"),
    eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-loose"),
    eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-medium"),
    eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-tight"),
    TPGSF1 = TPG_SF_1_227,
    #TPGSF1 = TPG_SF_v1_veto,
    TPGSF2 = TPG_SF_2_227,
    TPGSFp = TPG_SF_v3_off,
    #TPGSFp = TPG_SF_3_veto,
    TPGSFp1 = TPG_SF_v3_off,
    regionLSB = cms.double(0.5),
    egammaLSB = cms.double(1.0), # This has to correspond with the value from L1CaloEmThreshold
    egammaSeed = cms.int32(2),	
    ECALOn = cms.bool(True),	
    v_off = cms.bool(True),	
    v1 = cms.bool(False),	
    v2 = cms.bool(False),	
    v3 = cms.bool(False),	
    v4 = cms.bool(False)
)


from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.MiniAOD

switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V2_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

process.p1 = cms.Path(
    process.ecalDigis*
    process.ecalTriggerPrimitiveDigis*
    process.emulationSequence*
    process.scalersRawToDigi*
    process.egmGsfElectronIDSequence 
)


process.p1 += process.tree

# Make the framework shut up.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

#print out all processes used when running- useful check to see if module ran
#UNCOMMENT BELOW
dump_file = open('dump.py','w')
dump_file.write(process.dumpPython())

