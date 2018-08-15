#include <iostream>
#include <TLegend.h>
#include "labelCMS.h"
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TAxis.h>
#include "analysisTools.h"

/*void yields(const TString histname, const bool isWide = false, const bool doLog = false)
{
   char bufferGJets[1000];
   sprintf(bufferGJets, "s_%s_GJets", histname.Data());
   char bufferQCD[1000];
   sprintf(bufferQCD, "s_%s_QCD", histname.Data());

   TFile * f_0p4 = TFile::Open("fragmentation.hdp.0p4.root");
   TH1D * g_0p4 = (TH1D*)f_0p4->Get(bufferGJets);
   g_0p4->SetLineWidth(2);
   g_0p4->SetLineColor(6);
   TH1D * q_0p4 = (TH1D*)f_0p4->Get(bufferQCD);
   q_0p4->SetLineWidth(2);
   q_0p4->SetLineColor(6);

   TFile * f_0p3 = TFile::Open("fragmentation.hdp.0p3.root");
   TH1D * g_0p3 = (TH1D*)f_0p3->Get(bufferGJets);
   g_0p3->SetLineWidth(2);
   g_0p3->SetLineColor(4);
   TH1D * q_0p3 = (TH1D*)f_0p3->Get(bufferQCD);
   q_0p3->SetLineWidth(2);
   q_0p3->SetLineColor(4);

   TFile * f_0p2 = TFile::Open("fragmentation.hdp.0p2.root");
   TH1D * g_0p2 = (TH1D*)f_0p2->Get(bufferGJets);
   g_0p2->SetLineWidth(2);
   g_0p2->SetLineColor(8);
   TH1D * q_0p2 = (TH1D*)f_0p2->Get(bufferQCD);
   q_0p2->SetLineWidth(2);
   q_0p2->SetLineColor(8);

   TLegend * l = new TLegend(0.225, 0.175, 0.85, 0.35);
   l->SetBorderSize(0);
   l->SetNColumns(3);
   l->AddEntry(g_0p2, "#DeltaR = 0.2", "L");
   l->AddEntry(g_0p3, "#DeltaR = 0.3", "L");
   l->AddEntry(g_0p4, "#DeltaR = 0.4", "L");
   l->Draw();

   double w = 400.;
   if (isWide) w = 600.;
   TCanvas * c = new TCanvas ("c", "", w, 800.);
   c->Divide(1, 2);
   
   TPad * p1 = (TPad*)c->cd(1);
   g_0p4->Draw("E");
   g_0p4->SetStats(0);
   g_0p4->SetMinimum(0.);
   double s = 1.1;
   if (doLog) s = 10.;
   g_0p4->SetMaximum(s * TMath::Max(g_0p4->GetMaximum(), TMath::Max(g_0p3->GetMaximum(), g_0p2->GetMaximum())));
   g_0p3->Draw("E, SAME");
   g_0p2->Draw("E, SAME");
   if (doLog) {
      g_0p4->SetMinimum(0.1);
      p1->SetLogy();
   }
   l->Draw();
   labelCMS("35.9");
   
   TPad * p2 = (TPad*)c->cd(2);
   q_0p4->Draw("E");
   q_0p4->SetStats(0);
   q_0p4->SetMinimum(0.);
   q_0p4->SetMaximum(s * TMath::Max(q_0p4->GetMaximum(), TMath::Max(q_0p3->GetMaximum(), q_0p2->GetMaximum())));
   q_0p3->Draw("E, SAME");
   q_0p2->Draw("E, SAME");
   if (doLog) {
      q_0p4->SetMinimum(0.1);
      p2->SetLogy();
   }
   l->Draw();
   labelCMS("35.9");
   
   c->SaveAs("plots/yields_" + histname + ".pdf");
}*/

void runHist(const TString histname, const bool isWide=false)
{
   TFile * f_0p4 = TFile::Open("fragmentation.hdp.0p4.root");
   TGraphAsymmErrors * g_0p4 = (TGraphAsymmErrors*)f_0p4->Get(histname);

   TFile * f_0p3 = TFile::Open("fragmentation.hdp.0p3.root");
   TGraphAsymmErrors * g_0p3 = (TGraphAsymmErrors*)f_0p3->Get(histname);

   TFile * f_0p2 = TFile::Open("fragmentation.hdp.0p2.root");
   TGraphAsymmErrors * g_0p2 = (TGraphAsymmErrors*)f_0p2->Get(histname);

   const int n = g_0p4->GetN();
   TGraphErrors * df0p3 = new TGraphErrors(n);
   TGraphErrors * df0p2 = new TGraphErrors(n);

   char title[1000];
   sprintf(title, "%s;%s;%s", "", g_0p4->GetXaxis()->GetTitle(), "#DeltaF_{dir} / (1 - F_{dir})");
   df0p3->SetTitle(title);
   df0p2->SetTitle(title);
   
   for (int i = 0; i < n; ++i) {
      
      std::cout << "point " << i << std::endl;   

      double x0, y0;
      g_0p4->GetPoint(i, x0, y0);
      double y0err = g_0p4->GetErrorY(i);
      std::cout << y0 << " " << y0err << std::endl;

      double x1, y1;
      g_0p3->GetPoint(i, x1, y1);
      double y1err = g_0p3->GetErrorY(i);
      std::cout << y1 << " " << y1err << std::endl;

      double x2, y2;
      g_0p2->GetPoint(i, x2, y2);
      double y2err = g_0p2->GetErrorY(i);
      std::cout << y2 << " " << y2err << std::endl;
      
      double val1 = (y1-y0)/(1.-y0);
      df0p3->SetPoint(i, x0, val1);
      double err1 = pow(1.-y0, 2)*pow(y1err, 2) + pow(1.-y1, 2)*pow(y0err, 2);
      err1 = sqrt(err1)/pow(1.-y0, 2);
      df0p3->SetPointError(i, 0., err1);
      std::cout << val1 << " " << err1 << std::endl;

      double val2 = (y2-y0)/(1.-y0);
      df0p2->SetPoint(i, x0, val2);
      double err2 = pow(1.-y0, 2)*pow(y2err, 2) + pow(1.-y2, 2)*pow(y0err, 2);
      err2 = sqrt(err2)/pow(1.-y0, 2);
      df0p2->SetPointError(i, 0., err2);
      std::cout << val2 << " " << err2 << std::endl;
   
      std::cout << "" << std::endl;
   }

   double width = 400.;
   if (isWide) width = 600.;
   TCanvas * c = new TCanvas("c", "", width, 400.);

   df0p3->SetMarkerStyle(20);
   df0p3->SetMarkerColor(4);
   df0p3->SetLineColor(4);
   df0p3->Draw("APE");
   df0p3->SetMinimum(-1.);
   df0p3->SetMaximum(1.);

   if (isWide) drawLines46(-1., 1.);

   df0p2->SetMarkerStyle(20);
   df0p2->SetMarkerColor(8);
   df0p2->SetLineColor(8);
   df0p2->Draw("PE, SAME");

   TLegend * l = new TLegend(0.225, 0.77, 0.85, 0.89);
   l->SetBorderSize(0);
   l->SetNColumns(2);
   l->AddEntry(df0p2, "#DeltaR = 0.2", "P");
   l->AddEntry(df0p3, "#DeltaR = 0.3", "P");
   l->Draw();

   labelCMS();
   c->SaveAs("plots/dfover1mf." + histname + ".pdf");

   TCanvas * c2 = new TCanvas("c2", "", width, 400.);

   g_0p3->SetMarkerStyle(20);
   g_0p3->SetMarkerColor(4);
   g_0p3->SetLineColor(4);
   g_0p3->Draw("APE");
   g_0p3->SetMinimum(0.6);
   g_0p3->SetMaximum(1.05);

   if (isWide) drawLines46(0.6, 1.05);

   g_0p2->SetMarkerStyle(20);
   g_0p2->SetMarkerColor(8);
   g_0p2->SetLineColor(8);
   g_0p2->Draw("PE, SAME");

   g_0p4->SetMarkerStyle(20);
   g_0p4->SetMarkerColor(6);
   g_0p4->SetLineColor(6);
   g_0p4->Draw("PE, SAME");

   TLegend * l2 = new TLegend(0.225, 0.175, 0.85, 0.35);
   l2->SetBorderSize(0);
   l2->SetNColumns(3);
   l2->AddEntry(g_0p2, "#DeltaR = 0.2", "P");
   l2->AddEntry(g_0p3, "#DeltaR = 0.3", "P");
   l2->AddEntry(g_0p4, "#DeltaR = 0.4", "P");
   l2->Draw();

   labelCMS();
   c2->SaveAs("plots/frag." + histname + ".pdf");
}

void dfover1mf()
{
   runHist("HT");
   runHist("MHT");
   runHist("NJets");
   runHist("bin46_NJets789", true);
}
