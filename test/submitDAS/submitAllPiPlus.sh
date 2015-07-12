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
  --input-file-list=submitpiplus/PiPlusPt10.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt15 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt15.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt20 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt20.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt25 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt25.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt30 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt30.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt35 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt35.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-PiPlus_Pt40 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt40.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'


farmoutAnalysisJobs $1-PiPlus_Pt45 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt45.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-PiPlus_Pt50 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt50.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-PiPlus_Pt55 \
  --infer-cmssw-path \
  --input-file-list=submitpiplus/PiPlusPt55.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=0\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

