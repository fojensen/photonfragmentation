#include <TFile.h>
#include <iostream>
#include "labelCMS.h"
#include <TChain.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TCut.h>

void deltaR()
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

   TChain * chain_GJets0p4[4];
   for (int i = 0; i < 4; ++i) chain_GJets0p4[i] = new TChain("tree");
   double wGJets0p4[4];
   chain_GJets0p4[0]->Add(dir + "tree_GJets_DR-0p4_HT-100to200.root"); wGJets0p4[0] = 5391./14758163.;
   chain_GJets0p4[1]->Add(dir + "tree_GJets_DR-0p4_HT-200to400.root"); wGJets0p4[1] = 1168./49572400.;
   chain_GJets0p4[2]->Add(dir + "tree_GJets_DR-0p4_HT-400to600.root"); wGJets0p4[2] = 132.5/11680386.;
   chain_GJets0p4[3]->Add(dir + "tree_GJets_DR-0p4_HT-600toInf.root"); wGJets0p4[3] = 44.05/11639826.;
   for (int i = 0; i < 4; ++i)std::cout << wGJets0p4[i] << std::endl;
 
   TChain * chain_GJets0p4OLDXS = new TChain("tree");
   chain_GJets0p4OLDXS->Add(dir + "tree_GJets_DR-0p4_HT-100to200.root");
   chain_GJets0p4OLDXS->Add(dir + "tree_GJets_DR-0p4_HT-200to400.root");
   chain_GJets0p4OLDXS->Add(dir + "tree_GJets_DR-0p4_HT-400to600.root");
   chain_GJets0p4OLDXS->Add(dir + "tree_GJets_DR-0p4_HT-600toInf.root");

   TH1D * deltaR_GJets = new TH1D("deltaR_GJets", ";min #DeltaR (#gamma, hard partons);events / 0.1", 40, 0., 4.);
   const double max = 8000.;
   const TString saveTag = "4";

   //TH1D * deltaR_GJets = new TH1D("deltaR_GJets", ";min #DeltaR (#gamma, hard partons);events / 0.05", 16, 0., 0.8); 
   //const double max = 2000.;
   //const TString saveTag = "0p8";
  
   TH1D * deltaR_GJets0p4 = (TH1D*)deltaR_GJets->Clone("deltaR_GJets0p4");
   TH1D * deltaR_GJets0p4OLDXS = (TH1D*)deltaR_GJets->Clone("deltaR_GJets0p4OLDXS");
   TH1D * deltaR_QCD = (TH1D*)deltaR_GJets->Clone("deltaR_QCD");
 
   TString baseline = "NJets>=2 && HT>=300. && MHT>=300. && JetID==1 && BTags==0";
   baseline += " && isoElectronTracks==0 && isoMuonTracks==0 && isoPionTracks==0";
   baseline += " && DeltaPhi1>=0.5 && DeltaPhi2>=0.5 && DeltaPhi3>=0.3 && DeltaPhi4>=0.3";
   baseline += " && @Electrons->size()==0 && @Muons->size()==0 && @Photons->size()==1";
   baseline += " && Photons[0].Pt()>=200. && Photons_fullID[0]==1";
 
   std::cout << "QCD..." << std::endl;
   char cutQCD[2000];
   sprintf(cutQCD, "Weight * 35862.824 * (%s && madMinDeltaRStatus==1)", baseline.Data());
   chain_QCD->Project("deltaR_QCD", "madMinPhotonDeltaR", cutQCD);

   std::cout << "GJets..." << std::endl;
   char cutGJets[2000];
   sprintf(cutGJets, "Weight * 35862.824 * (%s)", baseline.Data());
   chain_GJets->Project("deltaR_GJets", "madMinPhotonDeltaR", cutGJets);

   std::cout << "GJets0p4..." << std::endl;
   for (int i = 0; i < 4; ++i) {
      char cutGJets0p4[2000];
      sprintf(cutGJets0p4, "%f * 35862.824 * (%s)", wGJets0p4[i], baseline.Data());
      chain_GJets0p4[i]->Project("+deltaR_GJets0p4", "madMinPhotonDeltaR", cutGJets0p4);
   }

   std::cout << "GJets0p4OLDXS..." << std::endl;
   chain_GJets0p4OLDXS->Project("deltaR_GJets0p4OLDXS", "madMinPhotonDeltaR", cutGJets);
   
   /*char buffer1[200];
   sprintf(buffer1, "madMinPhotonDeltaR%s.root", saveTag.Data());
   TFile * f = new TFile(buffer1, "RECREATE");
   deltaR_GJets->Write();
   deltaR_GJets0p4->Write();
   deltaR_QCD->Write();
   f->Close();*/

   TCanvas * c = new TCanvas("c", "", 400., 400.);
   deltaR_GJets0p4->SetLineColor(7);
   deltaR_GJets0p4->SetLineWidth(2);
   deltaR_GJets->SetLineColor(8);
   deltaR_GJets->SetLineWidth(2);
   deltaR_QCD->SetLineColor(6);
   deltaR_QCD->SetLineWidth(2);

   deltaR_GJets0p4->Draw("HIST, E");
   deltaR_GJets0p4->SetMinimum(0.);
   deltaR_GJets0p4->SetMaximum(max);
   deltaR_GJets0p4->SetStats(0);
   deltaR_GJets->Draw("HIST, E, SAME");
   deltaR_QCD->Draw("HIST, E, SAME");

   //deltaR_GJets0p4OLDXS->SetLineColor(7);
   //deltaR_GJets0p4OLDXS->SetLineWidth(2);
   //deltaR_GJets0p4OLDXS->SetLineStyle(2);
   //deltaR_GJets0p4OLDXS->Draw("HIST, E, SAME");

   TLegend * l = new TLegend(0.225, 0.725, 0.865, 0.89);
   l->SetNColumns(3);
   l->SetBorderSize(0);
   l->AddEntry(deltaR_GJets0p4, "GJets_DR-0p4_HT", "L");
   //l->AddEntry(deltaR_GJets0p4OLDXS, "GJets_DR-0p4_HT OLDXS", "L"); 
   l->AddEntry(deltaR_GJets, "GJets_HT", "L");
   l->AddEntry(deltaR_QCD, "QCD_HT", "L");
   l->Draw();

   labelCMS();

   char buffer2[200];
   sprintf(buffer2, "plots/madMinPhotonDeltaR%s.pdf", saveTag.Data());
   c->SaveAs(buffer2);
}
