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
   print 'Usage:python jetEfficiencyPlot.py RootFile2012.root ROOTFileSF.root label[optional]'
   exit()

infile1 = argv[1]
ntuple_file1 = ROOT.TFile(infile1)

######## LABEL & SAVE WHERE #########

if len(argv)>3:
   saveWhere='~/www/Research/'+argv[3]+'_'
else:
   saveWhere='~/www/Research/'

#saveWhere = './'



#####################################
#Get Effi NTUPLE                 #
#####################################


#ntuple = ntuple_file2.Get("tree2/Ntuple")
ntuple = ntuple_file1.Get("tree/Ntuple")

canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)

def make_plot(tree, variable, selection, binning, xaxis='', title=''):
    ''' Plot a variable using draw and return the histogram '''
    draw_string = "%s>>htemp(%s)" % (variable, ", ".join(str(x) for x in binning))
    tree.Draw(draw_string, selection, "goff")
    output_histo = ROOT.gDirectory.Get("htemp").Clone()
    output_histo.GetXaxis().SetTitle(xaxis)
    output_histo.SetTitle(title)
    return output_histo

def make_l1_efficiency(denom, num):
    ''' Make an efficiency graph with the "L1" style '''
    eff = ROOT.TGraphAsymmErrors(num, denom)
    eff.SetMarkerStyle(20)
    eff.SetMarkerColor(ROOT.EColor.kRed)
    eff.SetMarkerSize(1.5)
    eff.SetLineColor(ROOT.EColor.kBlack)
    return eff

def make_l1c_efficiency(denom, num):
    ''' Make an efficiency graph with the "UCT" style '''
    eff = ROOT.TGraphAsymmErrors(num, denom)
    eff.SetMarkerStyle(24)
    eff.SetMarkerColor(ROOT.EColor.kBlue)
    eff.SetMarkerSize(1.5)
    eff.SetLineColor(ROOT.EColor.kBlack)
    return eff


def compare_efficiencies(ntuple, variable, l1PtCut, binning, filename,
                         title='', xaxis='', showL1=False):
    ''' Returns a (L1, L1G) tuple of TGraphAsymmErrors '''
    denom = make_plot(
        ntuple, variable,
        "", # No selection
        binning
    )
    num_l1 = make_plot(
        ntuple, variable,
        "recoMatch && l1Pt > %0.2f" % (l1PtCut),
        binning
    )

    frame = ROOT.TH1F("frame", "frame", *binning)
    l1 = make_l1_efficiency(denom, num_l1)
    frame.SetMaximum(1.2)
    frame.SetTitle(title)
    frame.GetXaxis().SetTitle(xaxis)
    frame.Draw()
    l1.Draw('pe')
    #if showL1:
    #    l1.Draw('pe')
    legend = ROOT.TLegend(0.7, 0.2, 0.89, 0.4, "", "brNDC")
    legend.SetFillColor(ROOT.EColor.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(l1, "Test", "pe")
    #if showL1:
    #    legend.AddEntry(l1, "Current", "pe")
    legend.Draw()
    saveas = saveWhere+filename+'.png'
    print saveas
    canvas.SaveAs(saveas)

################################################################################
# Jet efficiency for a 20 GeV cut and 10 GeV cut on L1
################################################################################

compare_efficiencies(ntuple, 'recoPt',  30, [40, 0, 300],
                    'eg_save_as',
                     "EG 30 efficiency", "RECO p_{T} (GeV)",True)


# Resolutions
ntuple.Draw("(recoPt - (l1Pt))/recoPt>>l1gPtRes(150, -5, 5)","recoMatch==0&&recoPt>20","goff")
l1gPtRes = ROOT.gDirectory.Get("l1gPtRes")
l1gPtRes.SetLineColor(ROOT.EColor.kBlue)
l1gPtRes.Scale(1/l1gPtRes.Integral())
l1gPtRes.SetTitle('')
l1gPtRes.GetXaxis().SetTitle("(p_{T}^{RECO} - p_{T}^{TRG})/p_{T}^{RECO}")

ntuple_uncorr.Draw("(recoPt - l1Pt)/recoPt>>l1PtRes(150, -5, 5)", "recoMatch==0&&recoPt>20", "goff")
l1PtRes = ROOT.gDirectory.Get("l1PtRes")
l1PtRes.SetLineColor(ROOT.EColor.kRed)
l1PtRes.Scale(1/l1PtRes.Integral())
l1gPtRes.Draw()
l1PtRes.Draw("same")
legend1 = ROOT.TLegend(0.2, 0.2, 0.39, 0.4, "", "brNDC")
legend1.SetFillColor(ROOT.EColor.kWhite)
legend1.SetBorderSize(1)
legend1.AddEntry(l1gPtRes, "TPG Calibrations")
legend1.AddEntry(l1PtRes, "No Calibrations")
legend1.Draw("same")
saveas=saveWhere+'eg_res.png'
canvas.SaveAs(saveas)

