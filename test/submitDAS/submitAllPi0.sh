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
  --input-file-list=submitpi0/Pi0Pt10.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt15 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt15.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt20 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt20.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt25 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt25.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt30 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt30.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt35 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt35.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName' 

farmoutAnalysisJobs $1-Pi0_Pt40 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt40.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'


farmoutAnalysisJobs $1-Pi0_Pt45 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt45.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-Pi0_Pt50 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt50.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

farmoutAnalysisJobs $1-Pi0_Pt55 \
  --infer-cmssw-path \
  --input-file-list=submitpi0/Pi0Pt55.txt \
  --assume-input-files-exist \
  --input-dir=root://cmsxrootd.fnal.gov/ \
  --input-files-per-job=1 \
  ./makePionTree_cfg.py isMC=1 isPi0=1\
    'inputFiles=$inputFileNames' 'outputFile=$outputFileName'

