#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TH2.h>
#include <TH1D.h>
#include <iostream>
#include "analysisTools.h"

class sf
{
   public:
      sf(){};
      void run(const TString who);
   private:
      void initChain(TChain * chain);
      bool pass_baseline();
      bool pass_photon();
      bool pass_diMuon();
      bool pass_diElectron();
      void getbins_2d(TH2 * h2, int * bx, int * by, const double x, const double y);

      double HT, MHT;
      int NJets, JetID, BTags;
      int isoElectronTracks, isoMuonTracks, isoPionTracks;
      double DeltaPhi1, DeltaPhi2, DeltaPhi3, DeltaPhi4;
      int NElectrons, NMuons;
      std::vector<TLorentzVector> *Photons, *Electrons, *Muons;
      std::vector<bool> *Photons_fullID, *Photons_hasPixelSeed;
      double Weight;
 
      TH2 *h2_reco, *h2_reco_temp;
      TH2 *h2_id, *h2_id_temp;
      TH2 *h2_iso, *h2_iso_temp;
};

void sf::initChain(TChain * chain)
{
   chain->SetBranchAddress("HT", &HT); 
   chain->SetBranchAddress("MHT", &MHT);   
   chain->SetBranchAddress("NJets", &NJets);
   chain->SetBranchAddress("BTags", &BTags); 
   chain->SetBranchAddress("Weight", &Weight);
   chain->SetBranchAddress("isoElectronTracks", &isoElectronTracks); 
   chain->SetBranchAddress("isoMuonTracks", &isoMuonTracks); 
   chain->SetBranchAddress("isoPionTracks", &isoPionTracks); 
   chain->SetBranchAddress("DeltaPhi1", &DeltaPhi1); 
   chain->SetBranchAddress("DeltaPhi2", &DeltaPhi2); 
   chain->SetBranchAddress("DeltaPhi3", &DeltaPhi3); 
   chain->SetBranchAddress("DeltaPhi4", &DeltaPhi4); 
   chain->SetBranchAddress("JetID", &JetID);
   chain->SetBranchAddress("Photons", &Photons);
   chain->SetBranchAddress("Electrons", &Electrons);
   chain->SetBranchAddress("Muons", &Muons);
   chain->SetBranchAddress("Photons_fullID", &Photons_fullID);
   chain->SetBranchAddress("Photons_hasPixelSeed", &Photons_hasPixelSeed);
   chain->SetBranchAddress("NMuons", &NMuons);
   chain->SetBranchAddress("NElectrons", &NElectrons);

   Photons_fullID = Photons_hasPixelSeed = 0;
   Photons = Electrons = Muons = 0;
}

bool sf::pass_diElectron()
{
   if (Muons->size()==0 && Electrons->size()==2) {
      return true;
   }
   return false;
}

bool sf::pass_diMuon()
{
   if (Muons->size()==2 && Electrons->size()==0) {
      return true;
   }
   return false;
}

bool sf::pass_photon()
{
   if (BTags==0) {
      if (Electrons->size()==0 && Muons->size()==0) {
         if (Photons->at(0).Pt()>=200. && Photons_fullID->at(0)==1 && Photons_hasPixelSeed->at(0)==0) {
            return true;
         }
      }
   }
   return false;
}

bool sf::pass_baseline()
{
   if (NJets>=2 && HT>=300. && MHT>=300. && JetID==1) {
      if (isoElectronTracks==0 && isoMuonTracks==0 && isoPionTracks==0) {
         if (DeltaPhi1>=0.5 && DeltaPhi2>=0.5 && DeltaPhi3>=0.3 && DeltaPhi4>=0.3) {
            if (whichBin_HTMHT(HT, MHT, NJets)>0) {
               return true;
            }
         }
      }
   }
   return false;
}

void sf::getbins_2d(TH2 * h2, int * bx, int * by, const double x, const double y)
{
   // get bin numbers
   const int bin = h2->Fill(x, y);
   int binx, biny, binz;
   h2->GetBinXYZ(bin, binx, biny, binz);
   // correct for overflows
   const int binsX = h2->GetXaxis()->GetNbins();
   const int binsY = h2->GetYaxis()->GetNbins();
   if (binx == 0) binx = 1;
   if (binx == binsX+1) binx = binsX;
   if (biny == 0) biny=1;
   if (biny == binsY+1) biny = binsY;
   // return bin numbers
   const int tempx = binx;
   const int tempy = biny;
   *bx = tempx;
   *by = tempy;
}

TH1D * makeSF(TH1D * h[3], const TString name)
{
   TH1D * g = (TH1D*)h[0]->Clone(name);
   g->Reset("MICES");  
   
   char buffer[1000];
   sprintf(buffer, ";%s;%s", h[0]->GetXaxis()->GetTitle(), "average scale factor");
   g->SetTitle(buffer);

   for (int i = 0; i < h[0]->GetNbinsX(); ++i) {
      const double norm = h[2]->GetBinContent(i+1);
      if (norm>0.) {
         g->SetBinContent(i+1, h[0]->GetBinContent(i+1)/norm);
         g->SetBinError(i+1, h[1]->GetBinContent(i+1)/norm);
      } else {
         g->SetBinContent(i+1, 0.);
         g->SetBinError(i+1,  0.);
      }
   }

   return g;
}

void sf::run(const TString who)
{
   TChain * chain = new TChain("tree");
   const TString dir = "/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV16/";
  
   if (who=="photon") {
      chain->Add(dir + "tree_GJet_CleanVars/tree_GJets_HT-*_MC2017.root");
   } else if (who=="diElectron") {
      chain->Add(dir + "tree_DYe_CleanVars/tree_DYJetsToLL_M-50_HT-*_MC2017.root");
   } else if (who=="diMuon") {
       chain->Add(dir + "tree_DYm_CleanVars/tree_DYJetsToLL_M-50_HT-*_MC2017.root");
   } else {
      std::cout << "Who?" << std::endl;
      return;
   } 
   initChain(chain);

   TH1D *h_inc[3];
   TH1D *h_HT[3], *h_MHT[3], *h_NJets[3];
   TH1D *h_BTags[3];

   const int nBinsHT = 6; const double xBinsHT[7] = {300., 350., 600., 850., 1200., 1700., 2700.};
   const int nBinsMHT = 4; const double xBinsMHT[5] = {300., 350., 600., 850., 1350.};
   const int nBinsNJets = 5; const double xBinsNJets[6] = {2, 4, 6, 8, 10, 14};

   const TString l[3] = {"val", "err", "norm"};
   for (int i = 0; i < 3; ++i) {
      h_inc[i] = new TH1D("h_inc_"+l[i], ";the unit bin;events", 1, 0.5, 1.5);
      h_HT[i] = new TH1D("h_HT_"+l[i], ";HT [GeV];events / bin", nBinsHT, xBinsHT);
      h_MHT[i] = new TH1D("h_MHT_"+l[i], ";MHT [GeV];events / bin", nBinsMHT, xBinsMHT);
      h_NJets[i] = new TH1D("h_NJets_"+l[i], ";NJets;events / bin", nBinsNJets, xBinsNJets);
      h_BTags[i] = new TH1D("h_BTags_"+l[i], ";BTags;events / bin",4, -0.5, 3.5);
   }

   if (who=="photon") {
      // id + iso
      TFile * f_id = TFile::Open("./pogfiles/2017_PhotonsLoose.root");
      h2_id = (TH2F*)f_id->Get("EGamma_SF2D");
      h2_id_temp = (TH2F*)h2_id->Clone("h2_id_temp");
   }
   if (who=="diElectron") {
      // reco
      TFile * f_reco = TFile::Open("./pogfiles/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root");
      h2_reco = (TH2F*)f_reco->Get("EGamma_SF2D");
      h2_reco_temp = (TH2F*)h2_reco->Clone("h2_reco_temp");
      // id
      TFile * f_id = TFile::Open("./pogfiles/ElectronScaleFactors_Run2017.root");
      h2_id = (TH2F*)f_id->Get("Run2017_CutBasedVetoNoIso94XV2");
      h2_id_temp = (TH2F*)h2_id->Clone("h2_id_temp");
      // iso
      TFile * f_iso = TFile::Open("./pogfiles/ElectronScaleFactors_Run2017.root");
      h2_iso = (TH2F*)f_iso->Get("Run2017_MVAVLooseTightIP2DMini");
      h2_iso_temp = (TH2F*)h2_iso->Clone("h2_iso_temp");
   }
   if (who=="diMuon") {
      // reco + id
      TFile * f_id = TFile::Open("./pogfiles/RunBCDEF_SF_ID.root");
      h2_id = (TH2F*)f_id->Get("NUM_MediumID_DEN_genTracks_pt_abseta");
      h2_id_temp = (TH2F*)h2_id->Clone("h2_id_temp");
      // iso
      TFile * f_iso = TFile::Open("./pogfiles/SF.root");
      h2_iso = (TH2F*)f_iso->Get("TnP_MC_NUM_MiniIso02Cut_DEN_MediumID_PAR_pt_eta");
      h2_iso_temp = (TH2F*)h2_iso->Clone("h2_iso_temp");
   }
   
   h2_reco_temp->StatOverflows();
   h2_id_temp->StatOverflows();
   h2_iso_temp->StatOverflows();
   
   for (int i = 0; i < chain->GetEntries(); ++i) {
      chain->GetEntry(i);
      
      if (!pass_baseline()) continue;
      if (who=="Photon" && !pass_photon()) continue;
      if (who=="diElectron" && !pass_diElectron()) continue;
      if (who=="diMuon" && !pass_diMuon()) continue;
      
      Weight = Weight * 41860.;
 
      double val = 1.;
      double err = 0.;

      if (who=="photon") {
         int binx = 0;
         int biny = 0;
         getbins_2d(h2_id_temp, &binx, &biny, Photons->at(0).Eta(), Photons->at(0).Pt());
         val = h2_id->GetBinContent(binx, biny);
         err = h2_id->GetBinError(binx, biny);
      }
      if (who=="diElectron") {
         for (int j = 0; j < 2; ++j) {
            int binx_reco, biny_reco, binx_id, biny_id, binx_iso, biny_iso;
            binx_reco = biny_reco = binx_id = biny_id = binx_iso = biny_iso = 0;
            getbins_2d(h2_reco_temp, &binx_reco, &biny_reco, Electrons->at(j).Eta(), Electrons->at(j).Pt());
            getbins_2d(h2_id_temp,   &binx_id,   &biny_id,   Electrons->at(j).Eta(), Electrons->at(j).Pt());
            getbins_2d(h2_iso_temp,  &binx_iso,  &biny_iso,  Electrons->at(j).Eta(), Electrons->at(j).Pt());
            val *= h2_reco->GetBinContent(binx_reco, biny_reco) * h2_id->GetBinContent(binx_id, biny_id) * h2_iso->GetBinContent(binx_iso, biny_iso);
            err += pow(h2_reco->GetBinError(binx_reco, biny_reco), 2) * pow(h2_id->GetBinError(binx_id, biny_id), 2) + pow(h2_iso->GetBinError(binx_iso, biny_iso), 2);
         }
         err = sqrt(err);
      }
      if (who=="diMuon") {
          for (int j = 0; j < 2; ++j) {
             int binx_id, biny_id, binx_iso, biny_iso;
             binx_id = biny_id = binx_iso = biny_iso = 0; 
             getbins_2d(h2_id_temp,  &binx_id,  &biny_id,  Muons->at(j).Pt(), TMath::Abs(Muons->at(j).Eta()));
             getbins_2d(h2_iso_temp, &binx_iso, &biny_iso, Muons->at(j).Pt(), TMath::Abs(Muons->at(j).Eta()));
             val *= h2_id->GetBinContent(binx_id, biny_id) * h2_iso->GetBinContent(binx_iso, biny_iso);
             err += pow(h2_id->GetBinError(binx_id, biny_id), 2) + pow(h2_iso->GetBinError(binx_iso, biny_iso), 2);
         }
         err = sqrt(err);
      }

      h_inc[0]->Fill(1., Weight*val);
      h_inc[1]->Fill(1., Weight*err);
      h_inc[2]->Fill(1., Weight);
            
      h_HT[0]->Fill(HT, Weight*val);
      h_HT[1]->Fill(HT, Weight*err);
      h_HT[2]->Fill(HT, Weight);

      h_MHT[0]->Fill(MHT, Weight*val);
      h_MHT[1]->Fill(MHT, Weight*err);
      h_MHT[2]->Fill(MHT, Weight);

      h_NJets[0]->Fill(NJets, Weight*val);
      h_NJets[1]->Fill(NJets, Weight*err);
      h_NJets[2]->Fill(NJets, Weight);

      h_BTags[0]->Fill(BTags, Weight*val);
      h_BTags[1]->Fill(BTags, Weight*err);
      h_BTags[2]->Fill(BTags, Weight);
   }

   TFile * file = new TFile("./sf."+who+".root", "RECREATE");
   TH1D * g_inc = makeSF(h_inc, "h_inc"); g_inc->Write();
   TH1D * g_HT = makeSF(h_HT, "h_HT"); g_HT->Write();
   TH1D * g_MHT = makeSF(h_MHT, "h_MHT"); g_MHT->Write();
   TH1D * g_NJets = makeSF(h_NJets, "h_NJets"); g_NJets->Write();
   TH1D * g_BTags = makeSF(h_BTags, "h_BTags"); g_BTags->Write();
   std::cout << "inclusive sf: " << g_inc->GetBinContent(1) << " +- " << g_inc->GetBinError(1) << std::endl;
   file->Close();
}

