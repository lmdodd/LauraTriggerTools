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


ntuple = ntuple_file.Get("tree/matched")

canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)

######## LABEL & SAVE WHERE #########

########### MAke Hist arrays #########
histos = []
hist_ptb = []
for i in range(0,6):
    hname_ptb = "hist_ptb%d" %(i)
    hist_ptb.append(ROOT.TH1F(hname_ptb,"",82,0,82))
    histos.append([])
    for j in range(0,82):
        hname = "histos%d_%d" % (i, j) # Each histogram must have a unique name
        histos[i].append( ROOT.TH1F(hname,"",100,0,5) )

for event in ntuple:
    if event.gen_ieta==-999:
       continue
    if event.l1_summed44<2:
       continue
    if event.gen_pt<20:
       continue
    if event.gen_pt>20 and event.gen_pt<30:
       ptb=0
    elif event.gen_pt>30 and event.gen_pt<50:
       ptb=1
    elif event.gen_pt>50 and event.gen_pt<80:
       ptb=2
    elif event.gen_pt>80 and event.gen_pt<100:
       ptb=3
    elif event.gen_pt>100 and event.gen_pt<200:
       ptb=4
    elif event.gen_pt>200:
       ptb=5
    SF=0.0
    reco=event.gen_et
    tpg=event.l1_summed44
    SF = reco/tpg
    #print 'gen_ieta'
    #print event.gen_ieta
    if event.gen_ieta>0:
       eta= event.gen_ieta+40
    if event.gen_ieta<0:
       eta= event.gen_ieta+41
    histos[ptb][eta].Fill(SF)
    
print 'mean'
for ptb in range(0,6):
    print '\t'
    for eta in range(0,82):
        Mean =histos[ptb][eta].GetMean()
        print ("Ptb:%d eta:%d SF:%.2f" %(ptb,eta,Mean))
        text_file.write("%f, " % Mean)
        MeanError =histos[ptb][eta].GetMeanError()
        bin=eta+1
        hist_ptb[ptb].SetBinContent(bin,Mean)
        hist_ptb[ptb].SetBinError(bin,MeanError)
    save = 'hist_ptb%d.png' % ptb 
    hist_ptb[ptb].Draw("pE1")
    canvas.SaveAs(save) 


#
file=ROOT.TFile("outfile.root","RECREATE")
file.cd()
#

for ptb in range(0,6):
  hist_ptb[ptb].Write()
  for eta in range(0,82):
      histos[ptb][eta].Write()                 


