#flake8: noqa
'''

Grab the L1 primitives, and put them in the event.

Authors: Laura

'''

import FWCore.ParameterSet.Config as cms


from Configuration.StandardSequences.RawToDigi_Data_cff import *
from L1Trigger.LauraTriggerTools.LutSF import *

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
    queryIntervalInLS = cms.uint32(100)#,
)


l1DigiStep = cms.Sequence(
    # Only do the digitization of objects that we care about
    #RawToDigi
    gctDigis
    * gtDigis
    * ecalDigis
    * hcalDigis
)


emulationSequence = cms.Sequence(l1DigiStep * l1Digis)
