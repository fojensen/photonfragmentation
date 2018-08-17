#include <TPaveText.h>

void labelCMS()
{
   TPaveText * label0 = new TPaveText(0.3, .905, 0.85, .945, "NDC, NB");
   label0->SetFillColor(0);
   label0->SetTextAlign(31);
   label0->AddText("CMS Simulation  #it{Preliminary}   13 TeV");
   label0->Draw();
}
