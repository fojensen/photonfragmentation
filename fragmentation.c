#include <TFile.h>
#include <iostream>
#include "analysisTools.h"
#include <TChain.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TGraphAsymmErrors.h>

class fragmentation
{
   public:
      fragmentation(){};
      void run();
   private:
      bool passBaseline();
      void initChain(TChain * chain);
      void fillHists(TChain * chain, const int hid, const double sp);

      double HT, MHT;
      int NJets, JetID, BTags;
      int isoElectronTracks, isoMuonTracks, isoPionTracks;
      double Weight;
      std::vector<TLorentzVector> *Electrons, *Muons, *Photons;
      std::vector<bool> *Photons_fullID;
      double madMinPhotonDeltaR;
      int madMinDeltaRStatus;
      std::vector<double> *Photons_hasPixelSeed;

      TH1D *h_inc[3];
      TH1D *h_HT[3], *h_MHT[3], *h_NJets[3], *h_BTags[3];
      TH1D *h_HTMHT[3], *h_bin50[3], *h_bin65[3];
      TH1D *h_bin50_NJets8910[3], *h_bin65_NJets8910[3];
};

void fragmentation::initChain(TChain * chain)
{
   Electrons = Muons = Photons = 0;
   Photons_fullID = 0;
   Photons_hasPixelSeed = 0;
   chain->SetBranchAddress("HT", &HT);
   chain->SetBranchAddress("MHT", &MHT);
   chain->SetBranchAddress("NJets", &NJets);
   chain->SetBranchAddress("BTags", &BTags);
   chain->SetBranchAddress("isoElectronTracks", &isoElectronTracks);
   chain->SetBranchAddress("isoMuonTracks", &isoMuonTracks);
   chain->SetBranchAddress("isoPionTracks", &isoPionTracks);
   chain->SetBranchAddress("Weight", &Weight);
   chain->SetBranchAddress("JetID", &JetID);
   chain->SetBranchAddress("madMinPhotonDeltaR", &madMinPhotonDeltaR);
   chain->SetBranchAddress("madMinDeltaRStatus", &madMinDeltaRStatus);
   chain->SetBranchAddress("Electrons", &Electrons);
   chain->SetBranchAddress("Muons", &Muons);
   chain->SetBranchAddress("Photons", &Photons);
   chain->SetBranchAddress("Photons_fullID", &Photons_fullID);
   chain->SetBranchAddress("Photons_hasPixelSeed", &Photons_hasPixelSeed);
}

bool fragmentation::passBaseline()
{ 
   if (HT>=300. && NJets>=2 && JetID==1 && MHT>=300.) {
      if (isoElectronTracks==0 && isoMuonTracks==0 && isoPionTracks==0) {
         if (Electrons->size()==0 && Muons->size()==0 && Photons->size()==1) {
            if (Photons->at(0).Pt()>=200. && Photons_fullID->at(0)==1 && Photons_hasPixelSeed->at(0)==0) {
               if (whichBin_HTMHT13(HT, MHT, NJets)>0) {
                  return true;
               }
            }
         }
      }
   }
   return false;
}

void fragmentation::fillHists(TChain * chain, const int hid, const double sp)
{
   for (int i = 0; i < chain->GetEntries(); ++i) {
      chain->GetEntry(i);
      if (!passBaseline()) continue;

      // QCD
      if (hid==0) {
         if (madMinDeltaRStatus!=1) continue;
         if (madMinPhotonDeltaR>=sp) continue;
      }
      // GJets
      if (hid==1 && madMinPhotonDeltaR<0.4) continue;
      if (hid==2 && madMinPhotonDeltaR<sp) continue;
  
      if (MHT<250.) continue; 
      h_bin65[hid]->Fill(whichBin_65(HT, MHT, NJets), Weight);
      h_MHT[hid]->Fill(MHT, Weight);
      for (int j = 1; j <= 65; ++j) {
         if (whichBin_65_NJets8910(HT, MHT, NJets, j)) {
            h_bin65_NJets8910[hid]->Fill(j, Weight);
         }
      }
      if (MHT<300.) continue;
      h_inc[hid]->Fill(1., Weight);
      h_HT[hid]->Fill(HT, Weight);
      h_NJets[hid]->Fill(NJets, Weight);
      h_BTags[hid]->Fill(BTags, Weight);
      h_HTMHT[hid]->Fill(whichBin_HTMHT(HT, MHT, NJets), Weight);  
      h_bin50[hid]->Fill(whichBin_50(HT, MHT, NJets), Weight);
      for (int j = 1; j <= 50; ++j) { 
         if (whichBin_50_NJets8910(HT, MHT, NJets, j)) {
            h_bin50_NJets8910[hid]->Fill(j, Weight);
         } 
      }
   }

    addOverflow(h_HT[hid]);
    addOverflow(h_MHT[hid]);
    addOverflow(h_NJets[hid]);
    addOverflow(h_BTags[hid]);
}

/*TGraphAsymmErrors * divideHists(TH1D * hist[3], const TString name)
{
   TH1D * num = (TH1D*)hist[1]->Clone("num_"+name);
   TH1D * denom = (TH1D*)hist[0]->Clone("denom_"+name);
   denom->Add(hist[2]);
   TGraphAsymmErrors * graph = new TGraphAsymmErrors(num, denom, "");
   graph->SetName(name);
   char buffer[1000];
   sprintf(buffer, ";%s;fragmentation fraction", hist[0]->GetXaxis()->GetTitle());
   graph->SetTitle(buffer);
   return graph;
}*/

TGraphAsymmErrors * divideHists(TH1D * hist[3], const TString name)
{
   TH1D * num = (TH1D*)hist[1]->Clone("num_"+name);
   TH1D * denom = (TH1D*)hist[0]->Clone("denom_"+name);
   denom->Add(hist[2]);
   const int nbins = hist[0]->GetNbinsX();
   //TGraphAsymmErrors * graph = new TGraphAsymmErrors(num, denom, "");
   TGraphAsymmErrors * graph = new TGraphAsymmErrors(nbins);
   for (int i = 1; i <= nbins; ++i) {
      const int x = denom->GetBinCenter(i);
      const double denom1 = denom->GetBinContent(i);
      if (denom1>0.) {
          const double num1 = num->GetBinContent(i);
          const double num1err = num->GetBinError(i);
          const double denom1err = denom->GetBinError(i);
          TH1D h_num1("h_num1", ";;", 1, 0.5, 1.5);
          h_num1.SetBinContent(i, num1);
          h_num1.SetBinError(i, num1err);
          TH1D h_denom1("h_denom1", ";;", 1, 0.5, 1.5);
          h_denom1.SetBinContent(i, denom1);
          h_denom1.SetBinError(i, denom1err);
          TGraphAsymmErrors g1(&h_num1, &h_denom1);
          double x[1], f[1];
          g1.GetPoint(0, x[0], f[0]);
          graph->SetPoint(i-1, *x, *f);
          graph->SetPointError(i-1, 0., 0., g1.GetErrorYlow(0), g1.GetErrorYhigh(0));
      } else {
         std::cout << "screwy point; bin: " << i << std::endl;
         graph->SetPoint(i-1, x, 1.);
         graph->SetPointError(i-1, 0., 0., 1., 0.);
      }
   }

   graph->SetName(name);
   char buffer[1000];
   sprintf(buffer, ";%s;fragmentation fraction", hist[0]->GetXaxis()->GetTitle());
   graph->SetTitle(buffer);
   return graph;
}

/*TH1D * divideHists(TH1D * hist[3], const TString name)
{
   TH1D * num = (TH1D*)hist[1]->Clone("num");
   num->SetName(name);
   TH1D * denom = (TH1D*)hist[0]->Clone("denom");
   denom->Add(hist[2]);
   num->Divide(num, denom, 1, 1, "B");
   char buffer[1000];
   sprintf(buffer, ";%s;fragmentation fraction", hist[0]->GetXaxis()->GetTitle());
   num->SetTitle(buffer);
   return num;
}*/

void fragmentation::run()
{
   //const double sp = 0.2; const TString spTag = "sp0p2";
   //const double sp = 0.3; const TString spTag = "sp0p3";
   const double sp = 0.4; const TString spTag = "sp0p4";

   // nominal high-delta phi region
   const TString dir = "/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV16/tree_GJet_CleanVars/";
   const TString dirTag = "hdp";
   
   // low-delta phi region for QCD estimate
   //const TString dir = "/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV16/tree_GJetLDP_CleanVars/";
   //const TString dirTag = "ldp";

   char buffer[200];
   sprintf(buffer, "fragmentation.%s.%s", dirTag.Data(), spTag.Data());

   TChain * cQCD = new TChain("tree");
   cQCD->Add(dir + "tree_QCD_HT-200to300_MC2017.root");
   cQCD->Add(dir + "tree_QCD_HT-300to500_MC2017.root");
   cQCD->Add(dir + "tree_QCD_HT-500to700_MC2017.root");
   cQCD->Add(dir + "tree_QCD_HT-700to1000_MC2017.root");
   cQCD->Add(dir + "tree_QCD_HT-1000to1500_MC2017.root");
   cQCD->Add(dir + "tree_QCD_HT-1500to2000_MC2017.root");
   cQCD->Add(dir + "tree_QCD_HT-2000toInf_MC2017.root");
   initChain(cQCD);

   TChain * cGJets = new TChain("tree");
   cGJets->Add(dir + "tree_GJets_HT-100to200_MC2017.root");
   cGJets->Add(dir + "tree_GJets_HT-200to400_MC2017.root");
   cGJets->Add(dir + "tree_GJets_HT-400to600_MC2017.root");
   cGJets->Add(dir + "tree_GJets_HT-600toInf_MC2017.root");
   initChain(cGJets);
   
   const int nNJets = 5; const double xNJets[nNJets+1] = {2, 4, 6, 8, 10, 12};
   const int nHT = 6; const double xHT[nHT+1] = {300., 350., 600., 850., 1200., 1700., 2300.};
   const int nMHT = 4; const double xMHT[nMHT+1] = {300., 350., 600., 850., 1100.};
  
   TString l[3] = {"QCD_0tosp", "GJets_spto0p4", "GJets_gtsp"};
   for (int i = 0; i < 3; ++i) {
      h_inc[i] = new TH1D("h_inc_"+l[i], ";the unit bin;events", 1, 0.5, 1.5);
      h_HT[i] = new TH1D("h_HT_"+l[i], ";HT [GeV];events / bin", nHT, xHT);
      h_MHT[i] = new TH1D("h_MHT_"+l[i], ";MHT [GeV];events / bin", nMHT, xMHT);
      h_HTMHT[i] = new TH1D("h_HTMHT_"+l[i], ";10-bin HT-MHT plane;events / bin", 11, -0.5, 10.5);
      h_NJets[i] = new TH1D("h_NJets_"+l[i], ";NJets;events / bin", nNJets, xNJets);
      h_BTags[i] = new TH1D("h_BTags_"+l[i], ";BTags;events / bin", 4, -0.5, 3.5);
      h_bin50[i] = new TH1D("h_bin50_"+l[i], ";50-bin NJets-HT-MHT plane;events / bin", 51, -0.5, 50.5);
      h_bin65[i] = new TH1D("h_bin65_"+l[i], ";65-bin NJets-HT-MHT plane;events / bin", 66, -0.5, 65.5);
      h_bin50_NJets8910[i] = new TH1D("h_bin50_NJets8910_"+l[i], ";50-bin NJets-HT-MHT plane;events / bin", 51, -0.5, 50.5);
      h_bin65_NJets8910[i] = new TH1D("h_bin65_NJets8910_"+l[i], ";65-bin NJets-HT-MHT plane;events / bin", 66, -0.5, 65.5);
   }

   std::cout << "filling QCD hists..." << std::endl;
   fillHists(cQCD, 0, sp); // values less than sp
   std::cout << "filling GJets hists..." << std::endl;
   fillHists(cGJets, 1, sp); // values greater than 0p4
   std::cout << "filling GJets hists..." << std::endl;
   fillHists(cGJets, 2, sp); // values greater than sp
  
   TH1D *h_bin46[3], *h_bin59[3];
   TH1D *h_bin46_NJets8910[3], *h_bin59_NJets8910[3];
   for (int i = 0; i < 3; ++i) {
      h_bin46[i] = binShifts46(h_bin50[i], "h_bin46_"+l[i]);
      h_bin59[i] = binShifts59(h_bin65[i], "h_bin59_"+l[i]);
      h_bin46_NJets8910[i] = binShifts46(h_bin50_NJets8910[i], "h_bin46_NJets8910_"+l[i]);
      h_bin59_NJets8910[i] = binShifts59(h_bin65_NJets8910[i], "h_bin59_NJets8910_"+l[i]);
   }

   TFile * f = new TFile(TString(buffer)+".root", "RECREATE");   

   TGraphAsymmErrors * g_inc = divideHists(h_inc, "g_inc"); g_inc->Write();
   TGraphAsymmErrors * g_HT = divideHists(h_HT, "g_HT"); g_HT->Write();
   TGraphAsymmErrors * g_MHT = divideHists(h_MHT, "g_MHT"); g_MHT->Write();
   TGraphAsymmErrors * g_NJets = divideHists(h_NJets, "g_NJets"); g_NJets->Write();
   TGraphAsymmErrors * g_HTMHT = divideHists(h_HTMHT, "g_HTMHT"); g_HTMHT->Write();
   TGraphAsymmErrors * g_BTags = divideHists(h_BTags, "g_BTags"); g_BTags->Write();
   TGraphAsymmErrors * g_bin46 = divideHists(h_bin46, "g_bin46"); g_bin46->Write();
   TGraphAsymmErrors * g_bin46_NJets8910 = divideHists(h_bin46_NJets8910, "g_bin46_NJets8910"); g_bin46_NJets8910->Write();
   TGraphAsymmErrors * g_bin59 = divideHists(h_bin59, "g_bin59"); g_bin59->Write();
   TGraphAsymmErrors * g_bin59_NJets8910 = divideHists(h_bin59_NJets8910, "g_bin59_NJets8910"); g_bin59_NJets8910->Write();

   f->Close();
}
 
