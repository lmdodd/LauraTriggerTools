#!/bin/bash

# Submit efficiency ntuple jobs on the mu+jet skim
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 JOB_NAME"
fi

farmoutAnalysisJobs $1 \
  --infer-cmssw-path \
  --input-dbs-path=/SinglePiMinus_E1to1000_Eta5p2_13TeV_FlatRandom/RunIIFall15DR76-25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/GEN-SIM-RAW \
  --assume-input-files-exist \
  ./piMinus_submit_cfg.py  \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

