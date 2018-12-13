#include <TH1D.h>
#include <TStyle.h>
#include "labelCMS.h"
#include <TPaveStats.h>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>

void scatter()
{
   TFile * f_0p4 = TFile::Open("fragmentation.hdp.sp0p4.root");
   TH1D * g_0p4 = (TH1D*)f_0p4->Get("bin46_NJets8910");

   TFile * f_0p3 = TFile::Open("fragmentation.hdp.sp0p3.root");
   TH1D * g_0p3 = (TH1D*)f_0p3->Get("bin46_NJets8910");

   TFile * f_0p2 = TFile::Open("fragmentation.hdp.sp0p2.root");
   TH1D * g_0p2 = (TH1D*)f_0p2->Get("bin46_NJets8910");

   TH1D * h3 = new TH1D("h3", ";#DeltaF_{dir} / (1 - F_{dir});number of bins", 20, -1., 1.);
   h3->SetLineWidth(2);
   h3->SetLineColor(4);
   TH1D * h2 = new TH1D("h2", ";#DeltaF_{dir} / (1 - F_{dir});number of bins;", 20, -1., 1.);
   h2->SetLineWidth(2);
   h2->SetLineColor(8);
 
   for (int i = 0; i < 38; ++i) {
  
      const double x = g_0p4->GetBinCenter(i); 
 
      double y4 = g_0p4->GetBinContent(i);
      double y4err = g_0p4->GetBinError(i);

      double y3 = g_0p3->GetBinContent(i);
      double y1err = g_0p3->GetBinError(i);

      double y2 = g_0p2->GetBinContent(i);
      double y2err = g_0p2->GetBinError(i);
      
      double val3 = (y3-y4)/(1.-y4);
      //double err1 = pow(1.-y0, 2)*pow(y1err, 2) + pow(1.-y1, 2)*pow(y0err, 2);
      //err1 = sqrt(err1)/pow(1.-y0, 2);
      h3->Fill(val3);

      double val2 = (y2-y4)/(1.-y4);
      //double err2 = pow(1.-y0, 2)*pow(y2err, 2) + pow(1.-y2, 2)*pow(y0err, 2);
      //err2 = sqrt(err2)/pow(1.-y0, 2);
      h2->Fill(val2);
   }

   gStyle->SetOptStat("mr");

   // c1 is a hack to get the stats boxes
   TCanvas c1("c1", "", 800., 400.);
   c1.Divide(2, 1);
   c1.cd(1);
   h3->Draw();
   c1.cd(2);
   h2->Draw();
   c1.Update();
   gPad->Update();
   TPaveStats *st21 = (TPaveStats*)h3->FindObject("stats");
   st21->SetLineColor(4);
   st21->SetTextColor(4);
   TPaveStats *st22 = (TPaveStats*)h2->FindObject("stats");
   st22->SetLineColor(8);
   st22->SetTextColor(8);

   TCanvas * c2 = new TCanvas("c2", "", 400., 400.);
   h3->Draw();
   h2->Draw("SAME");

   double height = st22->GetY2NDC()-st22->GetY1NDC();
   st22->SetY2NDC(st21->GetY1NDC()-.005);
   st22->SetY1NDC(st22->GetY2NDC()-height);
   st21->Draw();
   st22->Draw();

   labelCMS();
   c2->SaveAs("plots/fragSystematics46Scatter.pdf");
}

