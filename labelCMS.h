#include <TPaveText.h>

void labelCMS()
{
   TPaveText * label0 = new TPaveText(0.3, .905, 0.85, .945, "NDC, NB");
   label0->SetFillColor(0);
   label0->SetTextAlign(31);
   label0->AddText("CMS Simulation  #it{Preliminary}   13 TeV");
   label0->Draw();
}

/*

void labelCMS(const bool isSim=false)
{
   TPaveText * label0 = new TPaveText(0.6, .905, 0.9, .945, "NDC, NB");
   label0->SetFillColor(0);
   label0->SetTextAlign(31);
   if (isSim) {
      label0->AddText("Simulation 13 TeV");
   } else {
      label0->AddText("13 TeV  35.9 fb^{-1}");
   }
   label0->Draw();

   //TPaveText * label1 = new TPaveText(0.22, 0.81, 0.4, .89, "NDC, NB");
   TPaveText * label1 = new TPaveText(0.3, 0.905, 0.55, .945, "NDC, NB");
   label1->SetFillColor(0);
   //label1->SetTextAlign(13);
   label1->SetTextAlign(31);
   //label1->AddText("#splitline{CMS}{#it{Preliminary}}");
   label1->AddText("CMS #it{Preliminary}"); 
   label1->Draw();
}

void labelCMS2d(const bool isSim = false)
{
   TPaveText * label0 = new TPaveText(0.225, .905, 0.8, .945, "NDC, NB");
   label0->SetFillColor(0);
   label0->SetTextAlign(31);
   TString words = "CMS #it{Preliminary}    13 TeV  35.9 fb^{-1}";
   if (isSim) words = "CMS Simulation #it{Preliminary}    13 TeV  35.9 fb^{-1}";
   label0->AddText(words);
   label0->Draw();
}
*/
