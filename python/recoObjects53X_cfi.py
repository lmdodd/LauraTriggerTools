#flake8: noqa
import FWCore.ParameterSet.Config as cms

'''

Select good RECO-level muons, electrons, taus, and jets for matching.

Authors: Laura Dodd, Nate Woods, Maria Cepeda, Evan Friis (UW Madison)

'''


#########################################################################
##-------- Find Isolated Electrons


recoElecs = cms.EDFilter(
    "GsfElectronSelector",
    src = cms.InputTag( "gsfElectrons" ),
    cut = cms.string(
        " (et>20.0)"
        " && (gsfTrack.trackerExpectedHitsInner.numberOfHits==0 && !(-0.02<convDist<0.02 && -0.02<convDcot<0.02))"
        " && (  (isEB"
        " && abs(sigmaIetaIeta)<0.01 &&  abs(deltaPhiSuperClusterTrackAtVtx)<0.03 && abs(deltaEtaSuperClusterTrackAtVtx)<0.004 )"
        " || (isEE"
        " && abs(sigmaIetaIeta)<0.03 &&   abs(deltaPhiSuperClusterTrackAtVtx)<0.02 &&abs(deltaEtaSuperClusterTrackAtVtx)<0.005 )"
        ")"
    ),
    filter = cms.bool(False),
)




##-------- Find Isolated Taus     
# Rerun the PFTau sequence
from Configuration.StandardSequences.MagneticField_cff import *
from RecoTauTag.Configuration.RecoPFTauTag_cff import *

# Select good taus
dmTaus = cms.EDFilter(
    "PFTauSelector",
    src = cms.InputTag("hpsPFTauProducer"),
    #cut = cms.string("pt > 10 & abs(eta) < 2.5"),
    cut = cms.string("pt > 10 & abs(eta) < 2.5"),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator=cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
            selectionCut=cms.double(0.5)
        ),
    ),
    filter = cms.bool(False)
)

isoTaus = cms.EDFilter(
    "PFTauSelector",
    src = cms.InputTag("hpsPFTauProducer"),
    cut = cms.string("pt > 10 & abs(eta) < 2.5"),
    #cut = cms.string("pt > 10 & abs(eta) < 2.5"),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator=cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
            selectionCut=cms.double(0.5)
        ),
        cms.PSet(
            discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseIsolationDBSumPtCorr"),
            selectionCut=cms.double(0.5)
        ),
    ),
    filter = cms.bool(False)
)

recoTaus = cms.EDFilter(
    "PFTauSelector",
    src = cms.InputTag("hpsPFTauProducer"),
    cut = cms.string("pt > 10 & abs(eta) < 2.5"),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator=cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
            selectionCut=cms.double(0.5)
        ),
        cms.PSet(
            discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseIsolationDBSumPtCorr"),
            selectionCut=cms.double(0.5)
        ),
        cms.PSet(
            discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection"),
            selectionCut=cms.double(0.5)
        ),
        cms.PSet(
            discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseElectronRejection"),
            selectionCut=cms.double(0.5)
        ),
    ),
    filter = cms.bool(False)
)


recoObjects = cms.Sequence(
    recoElecs*
    recoTauClassicHPSSequence *
    dmTaus *
    isoTaus*
    recoTaus
) 


recoObjects_truthMatched = cms.Sequence(
    recoObjects 
)

