#flake8: noqa
'''

Grab the L1 primitives, and put them in the event.

Authors: Laura

'''

import FWCore.ParameterSet.Config as cms

from L1Trigger.L1TCalorimeter.L1TRerunHCALTP_FromRaw_cff import *

#from Configuration.StandardSequences.RawToDigi_Data_cff import ecalDigis


# RCT
'''This remakes L1 regions'''
l1Digis = cms.EDProducer(
    "L1RCTProducer",
    hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis")),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives")),
    BunchCrossings = cms.vint32(0),
    getFedsFromOmds = cms.bool(False),
    queryDelayInLS = cms.uint32(10),
    queryIntervalInLS = cms.uint32(100),
    conditionsLabel = cms.string("") 
)



L1TCaloStage1_PPFromRaw = cms.Sequence(
    L1TRerunHCALTP_FromRAW
    #+ecalDigis
)


emulationSequence = cms.Sequence(L1TCaloStage1_PPFromRaw + l1Digis)
