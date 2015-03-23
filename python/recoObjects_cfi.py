#flake8: noqa
import FWCore.ParameterSet.Config as cms

'''

Select good RECO-level electrons

Authors: Laura Dodd, Maria Cepeda (UW Madison)

'''


#########################################################################
##-------- Find Isolated Muons and Electrons


#recoElecs = cms.EDFilter(
#    "GsfElectronSelector",
#    src = cms.InputTag( "gsfElectrons" ),
#    cut = cms.string(
#        " (et>8.0)"
#        " && (gsfTrack.trackerExpectedHitsInner.numberOfHits==0 && !(-0.02<convDist<0.02 && -0.02<convDcot<0.02))"
#        " && (  (isEB"
#        " && abs(sigmaIetaIeta)<0.01 &&  abs(deltaPhiSuperClusterTrackAtVtx)<0.03 && abs(deltaEtaSuperClusterTrackAtVtx)<0.004 )"
#        " || (isEE"
#        " && abs(sigmaIetaIeta)<0.03 &&   abs(deltaPhiSuperClusterTrackAtVtx)<0.02 &&abs(deltaEtaSuperClusterTrackAtVtx)<0.005 )"
#        ")"
#    ),
#    filter = cms.bool(False),
#)

#isoElecs = cms.EDFilter(
#    "GsfElectronSelector",
#    src = cms.InputTag('recoElecs'),
#    cut = cms.string(
#        "(dr03TkSumPt +  dr03EcalRecHitSumEt+  dr03HcalTowerSumEt)/pt  < 0.15 "
#    ),
#    filter = cms.bool(False),
#)


recoElecs = cms.EDFilter("GsfElectronSelector",
                                 src = cms.InputTag("gsfElectrons"),
                                 cut = cms.string('pt>8&&((isEB()&&abs(deltaEtaSuperClusterTrackAtVtx())<0.004&&abs(deltaPhiSuperClusterTrackAtVtx())<0.06&&hcalOverEcal()<0.04&&abs(sigmaIetaIeta())<0.01)||(isEE()&&abs(deltaEtaSuperClusterTrackAtVtx())<0.007&&abs(deltaPhiSuperClusterTrackAtVtx())<0.03&&hcalOverEcal()<0.025&&abs(sigmaIetaIeta())<0.03))&&(pfIsolationVariables().chargedHadronIso()+pfIsolationVariables().photonIso()+pfIsolationVariables().neutralHadronIso())/pt()<0.15'),
                                 filter = cms.bool(True)  
                             )


recoObjects = cms.Sequence(
    recoElecs
)
