#include <TPaveText.h>

void labelCMS(const bool showlumi = false)
{
   TPaveText * l = new TPaveText(0.3, .905, 0.85, .945, "NDC, NB");
   l->SetFillColor(0);
   l->SetTextAlign(31);
   if (showlumi) {
      l->AddText("Simulation    41.86 fb^{1}  13 TeV");
   } else {
      l->AddText("Simulation                  13 TeV");
   }
   l->Draw();
}

