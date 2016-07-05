#!/bin/bash

# Submit efficiency ntuple jobs on the mu+jet skim
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 JOB_NAME"
fi

farmoutAnalysisJobs $1 \
  --infer-cmssw-path \
  --input-dbs-path=/SingleElectron/Run2016B-PromptReco-v2/MINIAOD \
  --assume-input-files-exist \
  --skip-existing-output \
  ./EGReco2016_submitUnpack_cfg.py \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

