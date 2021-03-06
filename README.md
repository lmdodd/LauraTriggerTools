# LauraTriggerTools
Laura's Trigger Tools used for grabbing tpgs, regions, and RECO and matching.
```
cmsrel CMSSW_7_4_6_patch6
cd CMSSW_7_4_6_patch6/src/
cmsenv
git cms-init
git cms-addpkg L1Trigger/RegionalCaloTrigger     
git clone -b 746p6 https://github.com/lmdodd/LauraTriggerTools.git L1Trigger/LauraTriggerTools
export USER_CXXFLAGS="-Wno-delete-non-virtual-dtor -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=reorder -Wno-error=maybe-uninitialized"
scram b -j 8
```

To run a MC ntuple 

```
cd test/
cmsRun makeEGCalib.py #or submit via condor as in submitEG
```

To make plots
```
cd bin/ZEE
python pythonTPG_step1.py /path/to/rootfile.root #python pythonTPG_step1.py ../../test/tpg_eg_verification.root 
#Above produces outfile.root with histograms and a text file output v1.txt with the ECAL ScaleFactors
python plotECAL.py outfile.root #produces plot from above
#check saveWhere label in the above programs to where you want them to be saved.
```

To check applied scale factors run 

```
cd bin/ZEE
python pythonTPG_step2.py /path/to/rootfile.root #produces plots that ought to be close to 1
```

Similar method for Taus
