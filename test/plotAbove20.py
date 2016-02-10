'''
Usage:python jetEfficiencyPlot.py RootFile.root label[optional]

Script to make some quick efficiency plots to test ntuple generation.


Author: L. Dodd, UW Madison

'''

from subprocess import Popen
from sys import argv, exit, stdout, stderr

import ROOT

# So things don't look like crap.
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

######## File #########
if len(argv) < 2:
   print 'Usage:python file.py RootFile.root Folder[optional]'
   exit()

infile = argv[1]

ntuple_file = ROOT.TFile(infile)

######## LABEL & SAVE WHERE #########

if len(argv)>2:
   saveWhere='~/www/Research/'+argv[2]+'_'
else:
   saveWhere='.'

#####################################
#Get NTUPLE                 #
#####################################

hist_ptb2 = ntuple_file.Get("hist_ptb0")
hist_ptb3 = ntuple_file.Get("hist_ptb1")
hist_ptb4 = ntuple_file.Get("hist_ptb2")
hist_ptb5 = ntuple_file.Get("hist_ptb3")
hist_ptb6 = ntuple_file.Get("hist_ptb4")
hist_ptb7 = ntuple_file.Get("hist_ptb5")

canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)
canvas
hist_ptb3.SetMarkerColor(ROOT.kPink-3)
hist_ptb2.SetMarkerColor(ROOT.kMagenta-6)
hist_ptb4.SetMarkerColor(ROOT.kBlue-6)
hist_ptb5.SetMarkerColor(ROOT.kYellow+3)
hist_ptb6.SetMarkerColor(ROOT.kAzure+6)
hist_ptb7.SetMarkerColor(ROOT.kRed-7)
hist_ptb3.SetLineColor(ROOT.kPink-3)
hist_ptb2.SetLineColor(ROOT.kMagenta-6)
hist_ptb4.SetLineColor(ROOT.kBlue-6)
hist_ptb5.SetLineColor(ROOT.kYellow+3)
hist_ptb6.SetLineColor(ROOT.kAzure+6)
hist_ptb7.SetLineColor(ROOT.kRed-7)
hist_ptb3.SetMarkerStyle(23)
hist_ptb2.SetMarkerStyle(23)
hist_ptb4.SetMarkerStyle(23)
hist_ptb5.SetMarkerStyle(23)
hist_ptb6.SetMarkerStyle(23)
hist_ptb7.SetMarkerStyle(23)
hist_ptb3.GetXaxis().SetTitle("TPG Eta")
hist_ptb3.GetYaxis().SetTitle("Gen/TPG Value")
hist_ptb3.GetYaxis().SetRangeUser(0,3)
hist_ptb3.SetTitle('gen Jet Et/(TPGEt)  vs TPG iEta')
hist_ptb3.Draw("pE1")
hist_ptb2.Draw("pE1same")
hist_ptb4.Draw("pE1same")
hist_ptb5.Draw("pE1same")
hist_ptb6.Draw("pE1same")
hist_ptb7.Draw("pE1same")

legend1 = ROOT.TLegend(0.7, 0.82, 0.99, 0.99, "", "brNDC")
legend1.SetFillColor(ROOT.kWhite)
legend1.SetBorderSize(1)
legend1.AddEntry(hist_ptb3, "Gen Jet Pt=30-50 GeV")
legend1.AddEntry(hist_ptb2, "Gen Jet Pt=20-50 GeV")
legend1.AddEntry(hist_ptb4, "Gen Jet Pt=50-80 GeV")
legend1.AddEntry(hist_ptb5, "Gen Jet Pt=80-100 GeV")
legend1.AddEntry(hist_ptb6, "Gen Jet Pt=100-200 GeV")
legend1.AddEntry(hist_ptb7, "Gen Jet Pt=200 GeV")
legend1.Draw("same")
saveas='SF_trigger.png'
canvas.SaveAs(saveas)
canvas


