#include <TPaveText.h>

void labelCMS(const bool showlumi = false)
{
   TPaveText * label0 = new TPaveText(0.3, .905, 0.85, .945, "NDC, NB");
   label0->SetFillColor(0);
   label0->SetTextAlign(31);
   if (showlumi) {
      label0->AddText("CMS Simulation  #it{Preliminary}   35.9 fb^{1}  13 TeV");
   } else {
      label0->AddText("CMS Simulation  #it{Preliminary}   13 TeV");
   }
   label0->Draw();
}
