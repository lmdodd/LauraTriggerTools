#PiPl
farmoutAnalysisJobs \
   --merge \
   --input-files-per-job=2000 \
   --input-dir=root://cmsxrootd.hep.wisc.edu//store/user/laura/2015-02-25-MenuDir-AllPiPl_Float-makePionFloatTrees_cfg/ \
   mergeAllPiPlFloat \
   /cms/laura/MenusCurrent/CMSSW_6_2_5

farmoutAnalysisJobs \
   --merge \
   --input-files-per-job=2000 \
   --input-dir=root://cmsxrootd.hep.wisc.edu//store/user/laura/2015-02-25-MenuDir-AllPiPl_Int-makePionIntTrees_cfg/ \
   mergeAllPiPlInt \
   /cms/laura/MenusCurrent/CMSSW_6_2_5

farmoutAnalysisJobs \
   --merge \
   --input-files-per-job=2000 \
   --input-dir=root://cmsxrootd.hep.wisc.edu//store/user/laura/2015-02-25-MenuDir-AllPi0_Float-makePionFloatTrees_cfg/ \
   mergeAllPi0Float \
   /cms/laura/MenusCurrent/CMSSW_6_2_5

farmoutAnalysisJobs \
   --merge \
   --input-files-per-job=2000 \
   --input-dir=root://cmsxrootd.hep.wisc.edu//store/user/laura/2015-02-25-MenuDir-AllPi0_Int-makePionIntTrees_cfg/ \
   mergeAllPi0Int \
   /cms/laura/MenusCurrent/CMSSW_6_2_5
