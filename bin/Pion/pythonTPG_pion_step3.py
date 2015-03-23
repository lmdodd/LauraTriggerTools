from __future__ import division
#from decimal import *
from sys import argv, exit, stdout, stderr
import sys
import ROOT

text_file = open("v3.txt","w")


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
   saveWhere='~/www/Research/v3_pipl'

########### MAke Hist arrays #########
histos = []
hist_ptb = []
for i in range(0,9):
    hname_ptb = "hist_ptb%d" %(i)
    hist_ptb.append(ROOT.TH1F(hname_ptb,"",56,0,56))
    histos.append([])
    for j in range(0,56):
        hname = "histos%d_%d" % (i, j) # Each histogram must have a unique name
        histos[i].append( ROOT.TH1F(hname,"",100,0,5) )

for event in ntuple:
    for ptb in range(0,9):
        if event.ptbin==ptb:
           for eta in range(0,56):
               SF=1.0
               if event.TPGh5x5>3 and event.TPGDiff==1:
                  #print ("TPG5x5: %.2f" % event.TPG5x5)
                  #print ("TPGSum: %.2f" % event.TPGSum)
                  #print ("veto: %.2f" % veto)
                  #print ("cTPGE: %.2f" % event.cTPGE)
                  #print ("TPGE: %.2f" % event.TPGE)
                  #print ("TPGH: %.2f" % event.TPGH)
                  if event.genPt>0 and event.TPGDiff==1 and event.TPG5x5_tpgeta==eta:
                     gen=event.genPt
		     #print ("gen: %.2f" % gen)
                     tpg=event.cTPGe5x5+event.TPGh5x5
		     #print ("tpg: %.2f" % tpg)
                     SF = gen/tpg
		     #print ("SF: %.2f" % SF)
                     histos[ptb][eta].Fill(SF)
    
print 'mean'
for ptb in range(0,9):
    print '\t'
    for eta in range(0,56):
        Mean =histos[ptb][eta].GetMean()
        print ("Ptb:%d eta:%d SF:%.2f" %(ptb,eta,Mean))
        text_file.write("%f, " % Mean)
        MeanError =histos[ptb][eta].GetMeanError()
        bin=eta+1
        hist_ptb[ptb].SetBinContent(bin,Mean)
        hist_ptb[ptb].SetBinError(bin,MeanError)
    save = 'hist_ptb%d.png' % ptb 
    hist_ptb[ptb].Draw("pE1")
    canvas.SaveAs(saveWhere+save) 


#
file=ROOT.TFile("outfile_v3.root","RECREATE")
file.cd()
#

for ptb in range(0,9):
  hist_ptb[ptb].Write()
  for eta in range(0,56):
      histos[ptb][eta].Write()                 


