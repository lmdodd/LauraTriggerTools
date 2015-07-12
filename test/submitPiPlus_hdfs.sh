#!/bin/bash

# Submit rate ntuple jobs on the ZeroBias3 dataset (2012C)
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: $0 JOB_NAME"
  exit 1
fi

farmoutAnalysisJobs $1-PiPlus_Pt10 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt10/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt15 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt15/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt20 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt20/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt25 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt25/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt30 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt30/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/  \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt35 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt35/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt40 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt40/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-PiPlus_Pt45 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt45/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-PiPlus_Pt50 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt50/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-PiPlus_Pt55 \
  --infer-cmssw-path \
  --input-dir=/store/mc/Fall14DR73/SinglePiPlusPt55/GEN-SIM-RAW/PF_NoPU_RAW_GSFromWinter15_tsg_MCRUN2_73_V11-v1/ \
  --input-files-per-job=5 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

