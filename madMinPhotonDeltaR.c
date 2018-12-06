#include "labelCMS.h"
#include <TChain.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TCut.h>
#include <iostream>

void madMinPhotonDeltaR()
{
   const TString dir = "/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV12/tree_GJet_CleanVars/";

   TChain * chain_GJets = new TChain("tree");
   chain_GJets->Add(dir + "tree_GJets_HT-100to200.root");
   chain_GJets->Add(dir + "tree_GJets_HT-200to400.root");
   chain_GJets->Add(dir + "tree_GJets_HT-400to600.root");
   chain_GJets->Add(dir + "tree_GJets_HT-600toInf.root");

   TChain * chain_QCD = new TChain("tree");
   chain_QCD->Add(dir + "tree_QCD_HT-200to300.root");
   chain_QCD->Add(dir + "tree_QCD_HT-300to500.root");
   chain_QCD->Add(dir + "tree_QCD_HT-500to700.root");
   chain_QCD->Add(dir + "tree_QCD_HT-700to1000.root");
   chain_QCD->Add(dir + "tree_QCD_HT-1000to1500.root");
   chain_QCD->Add(dir + "tree_QCD_HT-1500to2000.root");
   chain_QCD->Add(dir + "tree_QCD_HT-2000toInf.root");

   TChain * chain_GJets0p4 = new TChain("tree");
   chain_GJets0p4->Add(dir + "tree_GJets_DR-0p4_HT-100to200.root");
   chain_GJets0p4->Add(dir + "tree_GJets_DR-0p4_HT-200to400.root");
   chain_GJets0p4->Add(dir + "tree_GJets_DR-0p4_HT-400to600.root");
   chain_GJets0p4->Add(dir + "tree_GJets_DR-0p4_HT-600toInf.root");
 
   TH1D * hist = new TH1D("hist", ";min #DeltaR (#gamma, hard partons);events / 0.05", 16, 0., 0.8);
   const double max = 2000.;
   const TString savetag = "narrowrange";

   //TH1D * hist = new TH1D("hist", ";min #DeltaR (#gamma, hard partons);events / 0.1", 40, 0., 4.);
   //const double max = 8000.;
   //const TString savetag = "widerange";

   hist->SetLineWidth(2);
   TH1D * dr_GJets = (TH1D*)hist->Clone("dr_GJets");
   TH1D * dr_GJets0p4 = (TH1D*)hist->Clone("dr_GJets0p4");
   TH1D * dr_QCD = (TH1D*)hist->Clone("dr_QCD");

   const TCut cuts_photon = "@Photons->size()==1 && Photons[0].Pt()>=200. && Photons_fullID[0]==1";
   const TCut cuts_leptons = "@Electrons->size()==0 && @Muons->size()==0"; 
   const TCut cuts_tracks = "isoElectronTracks==0 && isoMuonTracks==0 && isoPionTracks==0";
   const TCut cuts_dphi = "DeltaPhi1>=0.5 && DeltaPhi2>=0.5 && DeltaPhi3>=0.3 && DeltaPhi4>=0.3";
   TCut cuts_baseline = "NJets>=2 && HT>=300. && MHT>=300. && JetID==1 && BTags==0";
   cuts_baseline = cuts_photon && cuts_leptons && cuts_tracks && cuts_dphi && cuts_baseline;
   
   char cuts_GJets[2000];
   sprintf(cuts_GJets, "Weight * 35862.824 * (%s)", TString(cuts_baseline).Data());
   std::cout << "filling from GJets..." << std::endl;
   chain_GJets->Project("dr_GJets", "madMinPhotonDeltaR", cuts_GJets);

   std::cout << "filling from GJets0p4..." << std::endl;
   chain_GJets0p4->Project("dr_GJets0p4", "madMinPhotonDeltaR", cuts_GJets);

   char cuts_QCD[2000];
   sprintf(cuts_QCD, "Weight * 35862.824 * (%s && madMinDeltaRStatus==1)", TString(cuts_baseline).Data());
   std::cout << "filling from QCD..." << std::endl;
   chain_QCD->Project("dr_QCD", "madMinPhotonDeltaR", cuts_QCD);

   TCanvas * c = new TCanvas("c", "", 400., 400.);
   dr_GJets->SetLineColor(8);
   dr_GJets0p4->SetLineColor(7);
   dr_QCD->SetLineColor(6);

   dr_GJets->Draw("HIST, E");
   dr_GJets->SetMinimum(0.);
   dr_GJets->SetMaximum(max);
   dr_GJets->SetStats(0);
   dr_GJets0p4->Draw("HIST, E, SAME");
   dr_QCD->Draw("HIST, E, SAME");

   TLegend * l = new TLegend(0.225, 0.725, 0.865, 0.89);
   l->SetNColumns(3);
   l->SetBorderSize(0);
   l->AddEntry(dr_GJets, "GJets_HT", "L");
   l->AddEntry(dr_GJets0p4, "GJets_DR-0p4_HT", "L");
   l->AddEntry(dr_QCD, "QCD_HT", "L");
   l->Draw();

   labelCMS();

   c->SaveAs("./plots/madMinPhotonDeltaR."+savetag+".pdf");
}

