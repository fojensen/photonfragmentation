#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include "analysisTools.h"
#include <TChain.h>
#include <TLorentzVector.h>
#include <TH1D.h>

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
      double DeltaPhi1, DeltaPhi2, DeltaPhi3, DeltaPhi4;
      double Weight;
      std::vector<TLorentzVector> *Electrons, *Muons, *Photons;
      std::vector<bool> *Photons_fullID;
      double madMinPhotonDeltaR;
      int madMinDeltaRStatus;

      TH1D *h_inc[3];
      TH1D *h_HT[3], *h_MHT[3], *h_NJets[3];
      TH1D *h_bin50[3], *h_bin65[3];
      TH1D *h_bin50_NJets789[3], *h_bin65_NJets789[3];
};

void fragmentation::initChain(TChain * chain)
{
   chain->SetBranchAddress("HT", &HT);
   chain->SetBranchAddress("MHT", &MHT);
   chain->SetBranchAddress("NJets", &NJets);
   chain->SetBranchAddress("BTags", &BTags);
   chain->SetBranchAddress("isoElectronTracks", &isoElectronTracks);
   chain->SetBranchAddress("isoMuonTracks", &isoMuonTracks);
   chain->SetBranchAddress("isoPionTracks", &isoPionTracks);
   chain->SetBranchAddress("DeltaPhi1", &DeltaPhi1);
   chain->SetBranchAddress("DeltaPhi2", &DeltaPhi2);
   chain->SetBranchAddress("DeltaPhi3", &DeltaPhi3);
   chain->SetBranchAddress("DeltaPhi4", &DeltaPhi4);
   chain->SetBranchAddress("Weight", &Weight);
   chain->SetBranchAddress("JetID", &JetID);
   chain->SetBranchAddress("madMinPhotonDeltaR", &madMinPhotonDeltaR);
   chain->SetBranchAddress("madMinDeltaRStatus", &madMinDeltaRStatus);
   chain->SetBranchAddress("Electrons", &Electrons);
   chain->SetBranchAddress("Muons", &Muons);
   chain->SetBranchAddress("Photons", &Photons);
   chain->SetBranchAddress("Photons_fullID", &Photons_fullID);
   Electrons = Muons = Photons = 0;
   Photons_fullID = 0;
}

bool fragmentation::passBaseline()
{ 
   if (NJets>=2 && HT>=300. && MHT>=250. && JetID==1 && BTags==0) {
      if (isoElectronTracks==0 && isoMuonTracks==0 && isoPionTracks==0) {
         if (Electrons->size()==0 && Muons->size()==0 && Photons->size()==1) {
            if (Photons->at(0).Pt()>=200. && Photons_fullID->at(0)==1) {
               if (whichBin_HTMHT13(HT, MHT)>0) {
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
      if (MHT<250.) continue;

      if (hid==0) {
         if (madMinDeltaRStatus!=1) continue;
         if (madMinPhotonDeltaR>=sp) continue;
      }
      if (hid==1 && madMinPhotonDeltaR<0.4) continue; // hard cutoff set by MC production
      if (hid==2 && madMinPhotonDeltaR<sp) continue;
  
      Weight = Weight * 35862.824;
 
      h_bin65[hid]->Fill(whichBin_65(NJets, HT, MHT), Weight);
      h_MHT[hid]->Fill(MHT, Weight);
      for (int j = 1; j <= 65; ++j) {
         if (whichBin_65_NJets789(NJets, HT, MHT, j)) {
            h_bin65_NJets789[hid]->Fill(j, Weight);
         }
      }
      if (MHT<300.) continue;
      h_inc[hid]->Fill(1., Weight);
      h_HT[hid]->Fill(HT, Weight);
      h_NJets[hid]->Fill(NJets, Weight);
      h_bin50[hid]->Fill(whichBin_50(NJets, HT, MHT), Weight);
      for (int j = 1; j <= 50; ++j) { 
         if (whichBin_50_NJets789(NJets, HT, MHT, j)) {
            h_bin50_NJets789[hid]->Fill(j, Weight);
         } 
      }
   }
}

TGraphAsymmErrors * divideHists(TH1D * hist[3], const TString name)
{
   TH1D * num = (TH1D*)hist[1]->Clone("num_"+name);
   TH1D * denom = (TH1D*)hist[0]->Clone("denom_"+name);
   denom->Add(hist[2]);
   TGraphAsymmErrors * graph = new TGraphAsymmErrors(num, denom, "n");
   graph->SetName(name);
   char buffer[1000];
   sprintf(buffer, ";%s;fragmentation fraction", hist[0]->GetXaxis()->GetTitle());
   graph->SetTitle(buffer);
   return graph;
}

void fragmentation::run()
{
   const TString dir = "/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/tree_GJet_CleanVars/";
   const TString dirTag = "hdp";

   //const TString dir = "/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/tree_GJetLDP_CleanVars/";
   //const TString dirTag = "ldp";

   //const double sp = 0.3; const TString spTag = "sp0p3";
   const double sp = 0.4; const TString spTag = "sp0p4";
   //const double sp = 0.5; const TString spTag = "sp0p5";

   TChain * cQCD = new TChain("tree");
   cQCD->Add(dir + "tree_QCD_HT-200to300.root");
   cQCD->Add(dir + "tree_QCD_HT-300to500.root");
   cQCD->Add(dir + "tree_QCD_HT-500to700.root");
   cQCD->Add(dir + "tree_QCD_HT-700to1000.root");
   cQCD->Add(dir + "tree_QCD_HT-1000to1500.root");
   cQCD->Add(dir + "tree_QCD_HT-1500to2000.root");
   cQCD->Add(dir + "tree_QCD_HT-2000toInf.root");
   initChain(cQCD);

   TChain * cGJets = new TChain("tree");
   cGJets->Add(dir + "tree_GJets_HT-100to200.root");
   cGJets->Add(dir + "tree_GJets_HT-200to400.root");
   cGJets->Add(dir + "tree_GJets_HT-400to600.root");
   cGJets->Add(dir + "tree_GJets_HT-600toInf.root");
   initChain(cGJets);
    
   TString l[3] = {"QCD_0tosp", "GJets_spto0p4", "GJets_gtsp"};
   for (int i = 0; i < 3; ++i) {
      h_inc[i] = new TH1D("h_inc_"+l[i], ";the unit bin;events", 1, 0.5, 1.5);
      h_HT[i] = new TH1D("h_HT_"+l[i], ";HT [GeV];events / 100 GeV", 10, 300., 1300.);
      h_MHT[i] = new TH1D("h_MHT_"+l[i], ";MHT [GeV];events / 100 GeV", 10, 300., 1300.);
      h_NJets[i] = new TH1D("h_NJets_"+l[i], ";NJets;events / 1", 9, 1.5, 10.5);
      h_bin50[i] = new TH1D("h_bin50_"+l[i], ";50-bin NJets-HT-MHT analysis plane;events / bin", 50, 0.5, 50.5);
      h_bin65[i] = new TH1D("h_bin65_"+l[i], ";65-bin NJets-HT-MHT analysis plane;events / bin", 65, 0.5, 65.5);
      h_bin50_NJets789[i] = new TH1D("h_bin50_NJets789_"+l[i], ";50-bin NJets-HT-MHT analysis plane;events / bin", 50, 0.5, 50.5);
      h_bin65_NJets789[i] = new TH1D("h_bin65_NJets789_"+l[i], ";65-bin NJets-HT-MHT analysis plane;events / bin", 65, 0.5, 65.5);
   }

   std::cout << "filling QCD hists..." << std::endl;
   fillHists(cQCD, 0, sp); // values less than sp
   std::cout << "filling GJets hists..." << std::endl;
   fillHists(cGJets, 1, sp); // values greater than 0p4
   std::cout << "filling GJets hists..." << std::endl;
   fillHists(cGJets, 2, sp); // values greater than sp
  
   TH1D *h_bin46[3], *h_bin59[3];
   TH1D *h_bin46_NJets789[3], *h_bin59_NJets789[3];
   for (int i = 0; i < 3; ++i) {
      h_bin46[i] = binShifts46(h_bin50[i], "h_bin46_"+l[i]);
      h_bin59[i] = binShifts59(h_bin65[i], "h_bin59_"+l[i]);
      h_bin46_NJets789[i] = binShifts46(h_bin50_NJets789[i], "h_bin46_NJets789_"+l[i]);
      h_bin59_NJets789[i] = binShifts59(h_bin65_NJets789[i], "h_bin59_NJets789_"+l[i]);
   }

   char buffer[200];
   sprintf(buffer, "fragmentation.%s.%s.root", dirTag.Data(), spTag.Data());
   TFile * f = new TFile(buffer, "RECREATE");

   TGraphAsymmErrors * g_inc = divideHists(h_inc, "inc");
   g_inc->Write();

   TGraphAsymmErrors * g_HT = divideHists(h_HT, "HT");
   g_HT->Write();

   TGraphAsymmErrors * g_MHT = divideHists(h_MHT, "MHT");
   for (int i = 0; i < 3; ++i) h_MHT[i]->Write();
   g_MHT->Write();

   TGraphAsymmErrors * g_NJets = divideHists(h_NJets, "NJets");
   g_NJets->Write();

   TGraphAsymmErrors * g_bin46 = divideHists(h_bin46, "bin46");
   g_bin46->Write(); 
 
   TGraphAsymmErrors * g_bin46_NJets789 = divideHists(h_bin46_NJets789, "bin46_NJets789");
   g_bin46_NJets789->Write();
 
   TGraphAsymmErrors * g_bin59 = divideHists(h_bin59, "bin59");
   g_bin59->Write();

   TGraphAsymmErrors * g_bin59_NJets789 = divideHists(h_bin59_NJets789, "bin59_NJets789");
   g_bin59_NJets789->Write();

   f->Close();
}
 
