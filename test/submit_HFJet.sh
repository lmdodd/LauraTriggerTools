#!/bin/bash

# Submit efficiency ntuple jobs on the mu+jet skim
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 JOB_NAME"
fi

farmoutAnalysisJobs $1 \
  --infer-cmssw-path \
  --input-dbs-path=/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIIFall15DR76-25nsNoPUZsecalNzshcalRaw_76X_mcRun2_asymptotic_2016EcalTune_30fb_v1-v1/GEN-SIM-RAW
  --assume-input-files-exist \
  ./HFCalib_submit_cfg.py  \
  'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

