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
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt10/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt15 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt15/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt20 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt20/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt25 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt25/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt30 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt30/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/  \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt35 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt35/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt40 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt40/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-Pi0_Pt45 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt45/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-Pi0_Pt50 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt50/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-Pi0_Pt55 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePi0Pt55/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_MCRUN2_73_V10-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

