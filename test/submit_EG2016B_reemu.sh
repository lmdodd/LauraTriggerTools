#!/bin/bash

# Submit efficiency ntuple jobs on the mu+jet skim
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 JOB_NAME"
fi

farmoutAnalysisJobs $1 \
  --infer-cmssw-path \
  --input-file-list=SingleElectron2016.txt \
  --assume-input-files-exist \
  ./EGReco2016_submitReEmu_cfg.py \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

