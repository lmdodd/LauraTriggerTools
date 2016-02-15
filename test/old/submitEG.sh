#!/bin/bash

# Submit efficiency ntuple jobs on the mu+jet skim
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 JOB_NAME"
  exit 1
fi

farmoutAnalysisJobs $1 \
  --infer-cmssw-path \
  --vsize-limit=8000 \
  --input-file-list=DoubleEGPrompt.txt \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --assume-input-files-exist \
  ./makeEGReco_submit.py  \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'
