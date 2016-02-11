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
   print 'Usage:python jetEfficiencyPlot.py RootFile.root label[optional]'
   exit()

infile = argv[1]
ntuple_file = ROOT.TFile(infile)



#####################################
#Get Effi NTUPLE                 #
#####################################

ntuple = ntuple_file.Get("tree/matched")

canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)

ntuple.Draw("(gen_pt - l1_summed22)/gen_pt>>h1(150, -10, 3)", "gen_ieta!=-999&&l1_summed22>1&&gen_pt>5", "goff")
hist1 = ROOT.gDirectory.Get("h1")
hist1.SetLineColor(ROOT.kBlue-9)
hist1.Scale(1/hist1.Integral())
hist1.SetTitle('')
hist1.GetXaxis().SetTitle("(p_{T}^{GEN} JET - p_{T}^{TRG})/p_{T}^{GEN}")

ntuple.Draw("(gen_pt - l1_summed33)/gen_pt>>h2(150, -10, 3)", "gen_ieta!=-999&&l1_summed33>1&&gen_pt>5", "goff")
hist2 = ROOT.gDirectory.Get("h2")
hist2.SetLineColor(ROOT.kPink-3)
hist2.Scale(1/hist2.Integral())


ntuple.Draw("(gen_pt - l1_summed44)/gen_pt>>h3(150, -10, 3)", "gen_ieta!=-999&&l1_summed44>1&&gen_pt>5", "goff")
hist3 = ROOT.gDirectory.Get("h3")
hist3.SetLineColor(ROOT.kMagenta+2)
hist3.Scale(1/hist3.Integral())
maxi= hist3.GetMaximum()
hist1.GetYaxis().SetRangeUser(0,maxi*1.2)


hist1.Draw()
hist2.Draw("same")
hist3.Draw("same")
legend1 = ROOT.TLegend(0.2, 0.2, 0.39, 0.4, "", "brNDC")
legend1.SetFillColor(ROOT.kWhite)
legend1.SetBorderSize(1)
legend1.AddEntry(hist1, "3x3")
legend1.AddEntry(hist2, "5x5")
legend1.AddEntry(hist3, "7x7")
legend1.Draw("same")
saveas='jet_Resolution.png'
canvas.SaveAs(saveas)

canvas

###########PT Distributions

ntuple.Draw("l1_summed22>>h1(150,0,150)", "gen_ieta!=-999&&l1_summed22>1&&gen_et>5", "goff")
hist1 = ROOT.gDirectory.Get("h1")
hist1.SetLineColor(ROOT.kBlue-9)
hist1.Scale(1/hist1.Integral())
hist1.SetTitle('')
hist1.GetXaxis().SetTitle("p_{T} HF JET ")

ntuple.Draw("l1_summed33>>h2(150,0, 150)", "gen_ieta!=-999&&l1_summed33>1&&gen_et>5", "goff")
hist2 = ROOT.gDirectory.Get("h2")
hist2.SetLineColor(ROOT.kPink-3)
hist2.Scale(1/hist2.Integral())


ntuple.Draw("l1_summed44>>h3(150, 0, 150)", "gen_ieta!=-999&&l1_summed44>1&&gen_et>5", "goff")
hist3 = ROOT.gDirectory.Get("h3")
hist3.SetLineColor(ROOT.kMagenta+2)
hist3.Scale(1/hist3.Integral())

ntuple.Draw("gen_et>>g1(150, 0, 150)", "gen_ieta!=-999&&gen_et>5", "goff")
hist4 = ROOT.gDirectory.Get("g1")
hist4.SetLineColor(ROOT.kBlack)
hist4.Scale(1/hist4.Integral())
#maxi= hist4.GetMaximum()
#hist1.GetYaxis().SetRangeUser(0,maxi*1.2)


hist1.Draw()
hist2.Draw("same")
hist3.Draw("same")
hist4.Draw("same")
legend1 = ROOT.TLegend(0.7, 0.7, 0.9, 0.9, "", "brNDC")
legend1.SetFillColor(ROOT.kWhite)
legend1.SetBorderSize(1)
legend1.AddEntry(hist1, "l1 3x3")
legend1.AddEntry(hist2, "l1 5x5")
legend1.AddEntry(hist3, "l1 7x7")
legend1.AddEntry(hist4, "gen pt")
legend1.Draw("same")
saveas='jet_Pt.png'
canvas.SaveAs(saveas)

canvas

######### ETA

ntuple.Draw("l1_summed22/gen_et>>h1(100,0, 10)", "gen_ieta!=-999&&l1_summed22>1&&gen_et>5", "goff")
hist1 = ROOT.gDirectory.Get("h1")
hist1.SetLineColor(ROOT.kBlue-9)
hist1.Scale(1/hist1.Integral())
hist1.SetTitle('')
hist1.GetXaxis().SetTitle("HF JET P_{T}^{L1}/P_{T}^{GEN} Response")

ntuple.Draw("l1_summed33/gen_et>>h2(100,0, 10)", "gen_ieta!=-999&&l1_summed33>1&&gen_et>5", "goff")
hist2 = ROOT.gDirectory.Get("h2")
hist2.SetLineColor(ROOT.kPink-3)
hist2.Scale(1/hist2.Integral())


ntuple.Draw("l1_summed44/gen_et>>h3(100, 0, 10)", "gen_ieta!=-999&&l1_summed44>1&&gen_et>5", "goff")
hist3 = ROOT.gDirectory.Get("h3")
hist3.SetLineColor(ROOT.kMagenta+2)
hist3.Scale(1/hist3.Integral())

maxi= hist3.GetMaximum()
hist1.GetYaxis().SetRangeUser(0,maxi*1.2)


hist1.Draw()
hist2.Draw("same")
hist3.Draw("same")
legend1 = ROOT.TLegend(0.7, 0.7, 0.9, 0.9, "", "brNDC")
legend1.SetFillColor(ROOT.kWhite)
legend1.SetBorderSize(1)
legend1.AddEntry(hist1, "l1 3x3")
legend1.AddEntry(hist2, "l1 5x5")
legend1.AddEntry(hist3, "l1 7x7")
legend1.Draw("same")
saveas='jet_PtResponse.png'
canvas.SaveAs(saveas)

