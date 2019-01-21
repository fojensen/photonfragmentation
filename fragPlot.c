#include <TH1D.h>
#include <TCanvas.h>
#include <TFile.h>
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
   TFile * f = TFile::Open("fragmentation.hdp.sp0p4.2016..root");
   //TFile * f = TFile::Open("fragmentation.ldp.sp0p4.root");

   TCanvas * c1 = new TCanvas("c1", "", 600., 400.); 
   TH1D * g_bin46_NJets8910 = (TH1D*)f->Get("bin46_NJets8910");
   g_bin46_NJets8910->SetMarkerStyle(20);
   g_bin46_NJets8910->Draw("PE");
   g_bin46_NJets8910->SetMinimum(0.7);
   g_bin46_NJets8910->SetMaximum(1.01);
   drawLines46(g_bin46_NJets8910->GetMinimum(), g_bin46_NJets8910->GetMaximum());
   g_bin46_NJets8910->SetStats(0);
   labelCMS();
   c1->SaveAs("plots/fragmentation.2016.pdf");

   TCanvas * c2 = new TCanvas("c2", "", 800, 800);
   c2->Divide(2, 2);

   const double ymin = 0.85;
   const double ymax = 1.01;

   c2->cd(1);
   TH1D * g_HT = (TH1D*)f->Get("HT");
   g_HT->SetMarkerStyle(20);
   g_HT->Draw("PE");
   g_HT->SetMinimum(ymin);
   g_HT->SetMaximum(ymax);
   g_HT->SetStats(0);
   labelCMS();

   c2->cd(2);
   TH1D * g_MHT = (TH1D*)f->Get("MHT");
   g_MHT->SetMarkerStyle(20);
   g_MHT->Draw("PE");
   g_MHT->SetMinimum(ymin);
   g_MHT->SetMaximum(ymax);
   g_MHT->SetStats(0);
   labelCMS();

   c2->cd(3);
   TH1D * g_NJets = (TH1D*)f->Get("NJets");
   g_NJets->SetMarkerStyle(20);
   g_NJets->Draw("PE");
   g_NJets->SetMinimum(ymin);
   g_NJets->SetMaximum(ymax);
   g_NJets->SetStats(0);
   labelCMS();

   c2->cd(4);
   TH1D * g_HTMHT = (TH1D*)f->Get("HTMHT");
   g_HTMHT->SetMarkerStyle(20);
   g_HTMHT->Draw("PE");
   g_HTMHT->SetMinimum(ymin);
   g_HTMHT->SetMaximum(ymax);
   g_HTMHT->SetStats(0);
   labelCMS();

   c2->SaveAs("plots/fragmentation.vars.2016.pdf");
}

