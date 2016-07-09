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
  --vsize-limit=4000 \
  --input-file-list=Electron2015D-Prv4.txt \
  --job-count=5 \
  --input-files-per-job=1 \
  --assume-input-files-exist \
  ./makeEGReco_2015D-PRv4.py  \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1 \
  --infer-cmssw-path \
  --vsize-limit=4000 \
  --input-file-list=Electron2015D-05Oct.txt \
  --job-count=5 \
  --input-files-per-job=1 \
  --assume-input-files-exist \
  ./makeEGReco_2015D-05Oct.py  \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

