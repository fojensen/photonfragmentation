#include <TStyle.h>
#include "labelCMS.h"
#include <TPaveStats.h>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TH1D.h>
#include <TCanvas.h>

void scatter()
{
   TFile * f_0p4 = TFile::Open("fragmentation.hdp.0p4.root");
   TGraphAsymmErrors * g_0p4 = (TGraphAsymmErrors*)f_0p4->Get("bin46_NJets789");

   TFile * f_0p3 = TFile::Open("fragmentation.hdp.0p3.root");
   TGraphAsymmErrors * g_0p3 = (TGraphAsymmErrors*)f_0p3->Get("bin46_NJets789");

   TFile * f_0p2 = TFile::Open("fragmentation.hdp.0p2.root");
   TGraphAsymmErrors * g_0p2 = (TGraphAsymmErrors*)f_0p2->Get("bin46_NJets789");

   TH1D * h3 = new TH1D("h3", ";#DeltaF_{dir} / (1 - F_{dir});number of bins", 20, -1., 1.);
   h3->SetLineWidth(2);
   h3->SetLineColor(4);
   TH1D * h2 = new TH1D("h2", ";#DeltaF_{dir} / (1 - F_{dir});number of bins;", 20, -1., 1.);
   h2->SetLineWidth(2);
   h2->SetLineColor(8);
 
   for (int i = 0; i < 38; ++i) {
   
      double x0, y0;
      g_0p4->GetPoint(i, x0, y0);
      double y0err = g_0p4->GetErrorY(i);

      double x1, y1;
      g_0p3->GetPoint(i, x1, y1);
      double y1err = g_0p3->GetErrorY(i);

      double x2, y2;
      g_0p2->GetPoint(i, x2, y2);
      double y2err = g_0p2->GetErrorY(i);
      
      double val1 = (y1-y0)/(1.-y0);
      //double err1 = pow(1.-y0, 2)*pow(y1err, 2) + pow(1.-y1, 2)*pow(y0err, 2);
      //err1 = sqrt(err1)/pow(1.-y0, 2);
      h3->Fill(val1);

      double val2 = (y2-y0)/(1.-y0);
      //double err2 = pow(1.-y0, 2)*pow(y2err, 2) + pow(1.-y2, 2)*pow(y0err, 2);
      //err2 = sqrt(err2)/pow(1.-y0, 2);
      h2->Fill(val2);
   }

   gStyle->SetOptStat("mr");

   TCanvas * c1 = new TCanvas("c1", "", 800., 400.);
   c1->Divide(2, 1);
   c1->cd(1);
   h3->Draw();
   labelCMS();
   c1->cd(2);
   h2->Draw();
   labelCMS();

   c1->Update();
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

   //TPaveStats *st21 = (TPaveStats*)h3->FindObject("stats");
   //TPaveStats *st22 = (TPaveStats*)h2->FindObject("stats");
   double height = st22->GetY2NDC()-st22->GetY1NDC();
   st22->SetY2NDC(st21->GetY1NDC()-.005);
   st22->SetY1NDC(st22->GetY2NDC()-height);
   st21->Draw();
   st22->Draw();

   labelCMS("", true);
   c2->SaveAs("plots/fragSystematics46Scatter.pdf");  
}

