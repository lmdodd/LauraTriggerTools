from __future__ import division
#from decimal import *
from sys import argv, exit, stdout, stderr
import sys
import ROOT


text_file = open("v1.txt","w")

# So things don't look like crap.
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(1011)
######## File #########
if len(argv) < 2:
   print 'Usage:python pythontest.py RootFile.root SaveLabel[optional]'
   exit()

infile = argv[1]
ntuple_file = ROOT.TFile(infile)


ntuple = ntuple_file.Get("tree/Ntuple")

canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)

######## LABEL & SAVE WHERE #########

if len(argv)>2:
   saveWhere='~/www/Research/'+argv[2]+'_'
else:
   saveWhere=''

########### MAke Hist arrays #########
histos = []
hist_ptb = []
for i in range(0,9):
    hname_ptb = "hist_ptb%d" %(i)
    hist_ptb.append(ROOT.TH1F(hname_ptb,"",56,0,56))
    histos.append([])
    for j in range(0,28):
        hname = "histos%d_%d" % (i, j) # Each histogram must have a unique name
        histos[i].append( ROOT.TH1F(hname,"",100,0,5) )

for event in ntuple:
    nentries = len(event.ptbin)
    for i in range(0,nentries):    
          for ptb in range(0,9):
              if event.ptbin[i]==ptb:
                 for eta in range(0,28):
                     #bin=abs(eta-28)
                     SF=0.0
                     maxTpgPt = event.TPGVeto[i]*event.TPG5x5[i]
                     if event.recoPt[i]>0 and event.TPGe5x5[i]>0 and abs(event.TPG5x5_tpgeta_[i]-28) == eta and event.TPGVeto[i]>.25:
                        reco=event.recoPt[i]
                        tpg=event.TPGe5x5[i]
                        SF = reco/tpg
                        #print("recoPt: %.2f" % event.recoPt[i])
                        #print("reco: %.2f" % reco)
                        #print("tpg5x5Pt: %.2f" % event.TPG5x5[i])
                        #print("tpg: %.2f" % tpg)
                        #print("SF: %.2f" % SF)
                        histos[ptb][eta].Fill(SF)
print 'mean'
for ptb in range(0,9):
    print '\t'
    text_file.write("\n")
    for eta in range(0,28):
        Mean =histos[ptb][eta].GetMean()
        print ("Ptb:%d eta:%d SF:%.2fi \n" %(ptb,eta,Mean))
        text_file.write("%f, " % Mean)
        MeanError =histos[ptb][eta].GetMeanError()
        bin=eta+1
        hist_ptb[ptb].SetBinContent(bin,Mean)
        hist_ptb[ptb].SetBinError(bin,MeanError)
    save = 'hist_ptb%d.png' % ptb 
    hist_ptb[ptb].Draw("pE1")
    canvas.SaveAs(saveWhere+save) 

#
#print 'mean'
#for ptb in range(0,9):
#    print '\t'
#    for eta in range(0,28):
#        #bin=abs(eta-28)
#        bin=eta+1
#        Mean =histos[ptb][eta].GetMean()
#        print ("Ptb:%d eta:%d SF:%.2f" %(ptb,eta,Mean))
#        text_file.write("%f, " % Mean)
#        MeanError =histos[ptb][eta].GetMeanError()
#        hist_ptb[ptb].SetBinContent(bin,Mean)
#        hist_ptb[ptb].SetBinError(bin,MeanError)
#    save = 'hist_ptb%d.png' % ptb 
#    hist_ptb[ptb].Draw("pE1")
#    canvas.SaveAs(saveWhere+save) 
#

#
file=ROOT.TFile("outfile.root","RECREATE")
file.cd()
#

for ptb in range(0,9):
  hist_ptb[ptb].Write()
  for eta in range(0,28):
      histos[ptb][eta].Write()                 


