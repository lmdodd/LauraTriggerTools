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
  --input-file-list=DoubleElectron.txt \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --assume-input-files-exist \
  ./makeEGReco_data.py  \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'
