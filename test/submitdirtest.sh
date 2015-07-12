#!/bin/bash

# Submit rate ntuple jobs on the ZeroBias3 dataset (2012C)
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 JOB_NAME"
  exit 1
fi

farmoutAnalysisJobs $1-Pi0_Pt10 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt10/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'
