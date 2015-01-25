#flake8: noqa
import FWCore.ParameterSet.Config as cms

'''

Select good RECO-level muons, electrons, taus, and jets for matching.

Authors: Laura Dodd, Nate Woods, Maria Cepeda, Evan Friis (UW Madison)

'''


#########################################################################
##-------- Find Isolated Muons and Electrons


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



recoObjects = cms.Sequence(
    recoElecs 
)


recoObjects_truthMatched = cms.Sequence(
    recoObjects 
)

