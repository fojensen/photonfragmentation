void weights1(const TString options)
{
   TH1D * num = new TH1D("num", "", 1, 0.5, 1.5);
   TH1D * denom = (TH1D*)num->Clone("denom");
   for (int i = 0; i < 20775; ++i) {
      num->Fill(1.);
      denom->Fill(1.);
   }
   for (int i = 0; i < 630; ++i) {
      denom->Fill(1.);
   }
   TGraphAsymmErrors * g = new TGraphAsymmErrors(num, denom, options);
   double x, y;
   g->GetPoint(0, x, y);
   double low = g->GetErrorYlow(0);
   double high = g->GetErrorYhigh(0);
   std::cout <<"options (" << options << ")  frag = " <<  y << " - " << low << " + " << high << std::endl;
   delete num;
}

void weights100(const TString options)
{  
   TH1D * num = new TH1D("num", "", 1, 0.5, 1.5);
   num->Sumw2();
   TH1D * denom = (TH1D*)num->Clone("denom");
   for (int i = 0; i < 20775; ++i) {
      num->Fill(1.);
      denom->Fill(1.);
   }
   for (int i = 0; i < 100; ++i) {
      denom->Fill(1., 6.3);
   }
   TGraphAsymmErrors * g = new TGraphAsymmErrors(num, denom, options);
   double x, y;
   g->GetPoint(0, x, y);
   double low = g->GetErrorYlow(0);
   double high = g->GetErrorYhigh(0);
   std::cout <<"options (" << options << ")  frag = " << y << " - " << low << " + " << high << std::endl;
   delete num;
}

void weights630(const TString options)
{  
   TH1D * num = new TH1D("num", "", 1, 0.5, 1.5);
   num->Sumw2();
   TH1D * denom = (TH1D*)num->Clone("denom");
   for (int i = 0; i < 20775; ++i) {
      num->Fill(1.);
      denom->Fill(1.);
   }
   denom->Fill(1., 630.);
   TGraphAsymmErrors * g = new TGraphAsymmErrors(num, denom, options);
   double x, y;
   g->GetPoint(0, x, y);
   double low = g->GetErrorYlow(0);
   double high = g->GetErrorYhigh(0);
   std::cout <<"options (" << options << ")  frag = " << y << " - " << low << " + " << high << std::endl;
   delete num;
}

void simpleExample()
{
   std::cout << "" << std::endl;
   
   std::cout << "qcd weight : 1" << std::endl;
   weights1("");
   weights1("n");
   weights1("pois");
   weights1("cl=0.683 b(0.5,0.5) mode");
   weights1("cl=0.683 b(1.,1.) mode");

   std::cout << "" << std::endl;
   
   std::cout << "qcd weight : 6.3" << std::endl;
   weights100("n");
   weights100("pois");
   weights100("cl=0.683 b(0.5,0.5) mode");
   weights100("cl=0.683 b(1.,1.) mode");
   
   std::cout << "" << std::endl;

   std::cout << "qcd weight : 630" << std::endl;
   weights630("n");
   weights630("pois");
   weights630("cl=0.683 b(0.5,0.5) mode");
   weights630("cl=0.683 b(1.,1.) mode");
}

