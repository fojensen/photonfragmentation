#include <TCanvas.h>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include "labelCMS.h"

void drawLines46(const double min, const double max)
{
   TLine * line[6];
   line[0] = new TLine(0.5, min, 0.5, max);
   line[1] = new TLine(10.5, min, 10.5, max);
   line[2] = new TLine(20.5, min, 20.5, max);
   line[3] = new TLine(30.5, min, 30.5, max);
   line[4] = new TLine(38.5, min, 38.5, max);
   line[5] = new TLine(46.5, min, 46.5, max);
   for (int i = 0; i < 6; ++i) {
      line[i]->SetLineStyle(2);
      line[i]->SetLineColor(2);
      line[i]->Draw();
   }
}

void fragPlot()
{
   TFile * f = TFile::Open("fragmentation.0p4.GJetsHT.root");

   TCanvas * c1 = new TCanvas("c1", "", 600., 400.); 
   TGraphAsymmErrors * g_bin46_NJets789 = (TGraphAsymmErrors*)f->Get("bin46_NJets789");
   g_bin46_NJets789->SetMarkerStyle(20);
   g_bin46_NJets789->Draw("APE");
   g_bin46_NJets789->SetMinimum(0.7);
   g_bin46_NJets789->SetMaximum(1.01);
   drawLines46(g_bin46_NJets789->GetMinimum(), g_bin46_NJets789->GetMaximum());
   labelCMS();
   c1->SaveAs("plots/fragmentation.pdf");

   TCanvas * c2 = new TCanvas("c2", "", 1200., 400.);
   c2->Divide(3, 1);

   c2->cd(1);
   TGraphAsymmErrors * g_HT = (TGraphAsymmErrors*)f->Get("HT");
   g_HT->SetMarkerStyle(20);
   g_HT->Draw("APE");
   g_HT->SetMinimum(0.7);
   g_HT->SetMaximum(1.01);
   labelCMS();

   c2->cd(2);
   TGraphAsymmErrors * g_MHT = (TGraphAsymmErrors*)f->Get("MHT");
   g_MHT->SetMarkerStyle(20);
   g_MHT->Draw("APE");
   g_MHT->SetMinimum(0.7);
   g_MHT->SetMaximum(1.01);
   labelCMS();

   c2->cd(3);
   TGraphAsymmErrors * g_NJets = (TGraphAsymmErrors*)f->Get("NJets");
   g_NJets->SetMarkerStyle(20);
   g_NJets->Draw("APE");
   g_NJets->SetMinimum(0.7);
   g_NJets->SetMaximum(1.01);
   labelCMS();

   c2->SaveAs("plots/fragmentation.vars.pdf");
}

