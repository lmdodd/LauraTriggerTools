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
  --job-count=10 \
  --input-file-list=Tau-2015D.txt \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --assume-input-files-exist \
  ./makeTauReco_2015D.py  \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'
