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
   saveWhere='~/www/Research/'

#####################################
#Get NTUPLE                 #
#####################################

hist_ptb3 = ntuple_file.Get("hist_ptb3")
hist_ptb4 = ntuple_file.Get("hist_ptb4")
hist_ptb5 = ntuple_file.Get("hist_ptb5")
hist_ptb6 = ntuple_file.Get("hist_ptb6")
hist_ptb7 = ntuple_file.Get("hist_ptb7")
hist_ptb8 = ntuple_file.Get("hist_ptb8")

canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)
canvas
hist_ptb3.SetMarkerColor(ROOT.EColor.kPink-3)
hist_ptb4.SetMarkerColor(ROOT.EColor.kBlue-6)
hist_ptb5.SetMarkerColor(ROOT.EColor.kYellow+3)
hist_ptb6.SetMarkerColor(ROOT.EColor.kAzure+6)
hist_ptb7.SetMarkerColor(ROOT.EColor.kRed-7)
hist_ptb8.SetMarkerColor(ROOT.EColor.kMagenta+2)
hist_ptb3.SetLineColor(ROOT.EColor.kPink-3)
hist_ptb4.SetLineColor(ROOT.EColor.kBlue-6)
hist_ptb5.SetLineColor(ROOT.EColor.kYellow+3)
hist_ptb6.SetLineColor(ROOT.EColor.kAzure+6)
hist_ptb7.SetLineColor(ROOT.EColor.kRed-7)
hist_ptb8.SetLineColor(ROOT.EColor.kMagenta+2)
hist_ptb3.SetMarkerStyle(23)
hist_ptb4.SetMarkerStyle(23)
hist_ptb5.SetMarkerStyle(23)
hist_ptb6.SetMarkerStyle(23)
hist_ptb7.SetMarkerStyle(23)
hist_ptb8.SetMarkerStyle(23)
hist_ptb3.GetXaxis().SetTitle("TPG Eta")
hist_ptb3.GetYaxis().SetTitle("Reco/TPG Value")
hist_ptb3.GetYaxis().SetRangeUser(0.7,2.1)
hist_ptb3.SetTitle('recoPt/TPGPt vs TPG Eta')
hist_ptb3.Draw("pE1")
hist_ptb4.Draw("pE1same")
hist_ptb5.Draw("pE1same")
hist_ptb6.Draw("pE1same")
hist_ptb7.Draw("pE1same")
hist_ptb8.Draw("pE1same")

legend1 = ROOT.TLegend(0.7, 0.82, 0.99, 0.99, "", "brNDC")
legend1.SetFillColor(ROOT.EColor.kWhite)
legend1.SetBorderSize(1)
legend1.AddEntry(hist_ptb3, "recoEg Pt=25 GeV")
legend1.AddEntry(hist_ptb4, "recoEg Pt=30 GeV")
legend1.AddEntry(hist_ptb5, "recoEg Pt=35 GeV")
legend1.AddEntry(hist_ptb6, "recoEg Pt=40 GeV")
legend1.AddEntry(hist_ptb7, "recoEg Pt=45 GeV")
legend1.AddEntry(hist_ptb8, "recoEg Pt=50 GeV")
legend1.Draw("same")
saveas=saveWhere+'SF_trigger.png'
canvas.SaveAs(saveas)
canvas


