TH1F *GetCMSDataSqrtS(){
  //data from https://twiki.cern.ch/twiki/pub/CMSPublic/HIN11003Data/Fig3.txt
  TH1F *hcms = new TH1F("hcms","dET/deta",100000,1.0,14000.0); 
  ibin = hcms->FindBin(2760); 
//   hcms->SetBinContent(ibin,1657.1/382.8*2);
//   hcms->SetBinError(ibin,TMath::Sqrt(111.9*111.9+35.8*35.8)/382.8*2);
//   hcms->SetBinContent(ibin,10.8);//0-2.5%
//   hcms->SetBinError(ibin,0.55);
  hcms->SetBinContent(ibin,10.5);//0-5%
  hcms->SetBinError(ibin,0.5);
  hcms->SetMarkerStyle(21);
  hcms->SetMarkerColor(kGreen+2);// 416=kGreen
  hcms->SetLineColor(kGreen+2);
  hcms->SetMarkerSize(1.4);  
  return hcms;//alicehad
}


void etVsSqrts(Float_t et = 9.01999, Float_t etstaterr = 0.0306423, Float_t etsyserr = 0.501184, Bool_t is5TeV = kFALSE,Float_t et2015 = 12.0254, Float_t etstaterr2015 = 0.0181776, Float_t etsyserr2015 = 0.719316, Bool_t models = kFALSE, Bool_t isPrelim = kFALSE){
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  Bool_t addSTARToFit = kFALSE;
  Bool_t addNA49ToFit = kFALSE;
  Bool_t addWA98ToFit = kFALSE;
  Bool_t addE802ToFit = kFALSE;
  Bool_t addCMSToFit = kFALSE;
//cout<<"Frog!"<<endl;
    //}
  //void etVsSqrts(Float_t et, Float_t etstaterr, Float_t etsyserr) {
  float Marker_Size       = 1.4;
  cout<<"et for central point: "<<et<<" +/- "<<etstaterr<<" +/- "<<etsyserr<<endl;
  if(is5TeV){
    cout<<"et for central point: "<<et2015<<" +/- "<<etstaterr2015<<" +/- "<<etsyserr2015<<endl;
  }
  TGraphErrors *allpoints = new TGraphErrors();
  allpoints->SetMarkerStyle(20);
  allpoints->SetMarkerSize(2.0);
  Int_t allpointsNPoints = 0;
  allpoints->SetPoint(allpointsNPoints,2760,et);
  allpoints->SetPointError(allpointsNPoints,0,TMath::Sqrt(etstaterr*etstaterr+etsyserr*etsyserr));
  allpointsNPoints++;
  if(is5TeV){
    allpoints->SetPoint(allpointsNPoints,5020,et2015);
    allpoints->SetPointError(allpointsNPoints,0,TMath::Sqrt(etstaterr2015*etstaterr2015+etsyserr2015*etsyserr2015));
    allpointsNPoints++;
  }

  TCanvas *c = new TCanvas("c","dET/deta",600,400);
  c->SetLogx();
  c->SetLeftMargin(0.10);
  c->SetBottomMargin(0.14);
  c->SetBorderSize(0);
  c->SetFillColor(0);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetFrameFillColor(0);
  c->SetFrameBorderMode(0);
  c->SetTopMargin(0.026738);
  c->SetRightMargin(0.03);
  c->SetLeftMargin(0.112416);
  c->SetBottomMargin(0.17);
  Int_t ibin;
  // 
  TH1F *halicehad = new TH1F("halicehad","dET/deta",100000,1.0,14000.0); 
  ibin = halicehad->FindBin(2760); 
  halicehad->SetBinContent(ibin,et);
  halicehad->SetBinError(ibin,etsyserr);
  if(is5TeV && !isPrelim ){
    ibin = halicehad->FindBin(5020); 
    halicehad->SetBinContent(ibin,et2015);
    halicehad->SetBinError(ibin,etsyserr2015);
  }
  halicehad->SetMarkerStyle(20);
  halicehad->SetMarkerColor(2);
  halicehad->SetLineColor(2);
  halicehad->SetMarkerSize(Marker_Size);  
  halicehad->SetMarkerSize(Marker_Size);  
  if(isPrelim){
    halicehad->SetMarkerStyle(33);
    halicehad->SetMarkerSize(Marker_Size*1.5);  
    halicehad->SetMarkerSize(Marker_Size*1.5);  
  }
  TH1F *halicehadprelim = new TH1F("halicehadprelim","dET/deta",100000,1.0,14000.0); 
    ibin = halicehadprelim->FindBin(5020); 
    halicehadprelim->SetBinContent(ibin,et2015);
    halicehadprelim->SetBinError(ibin,etsyserr2015);
  halicehadprelim->SetMarkerStyle(20);
  halicehadprelim->SetMarkerColor(2);
  halicehadprelim->SetLineColor(2);
  halicehadprelim->SetMarkerSize(Marker_Size);  
  halicehadprelim->SetMarkerSize(Marker_Size);  
  //

// 200.0	      3.414		0.203		PHENIX//
// 130.0	      3.007		0.179		PHENIX//
// 62.4	      2.274		0.141		PHENIX//
// 39.0	      1.786		0.114		PHENIX//
// 27.0	      1.567		0.099		PHENIX/
// 19.6	      1.377		0.092		PHENIX//
// 14.5	      1.188		0.084		PHENIX
// 7.7	      0.829		0.056		PHENIX
  TH1F *hphenix = new TH1F("hphenix","dET/deta",100000,1.0,14000.0); 
  ibin = hphenix->FindBin(200.0); 
  hphenix->SetBinContent(ibin,3.414);
  hphenix->SetBinError(ibin,0.203);

  ibin = hphenix->FindBin(130.0); 
  hphenix->SetBinContent(ibin,3.007);
  hphenix->SetBinError(ibin,0.179);

  ibin = hphenix->FindBin(62.4); 
  hphenix->SetBinContent(ibin,2.274);
  hphenix->SetBinError(ibin,0.141);

  ibin = hphenix->FindBin(39.0); 
  hphenix->SetBinContent(ibin,1.786);
  hphenix->SetBinError(ibin,0.114);

  ibin = hphenix->FindBin(27.0); 
  hphenix->SetBinContent(ibin,1.567);
  hphenix->SetBinError(ibin,0.099);

  ibin = hphenix->FindBin(19.6); 
  hphenix->SetBinContent(ibin,1.377);
  hphenix->SetBinError(ibin,0.0992);

  ibin = hphenix->FindBin(14.5); 
  hphenix->SetBinContent(ibin,1.188);
  hphenix->SetBinError(ibin,0.084);

  ibin = hphenix->FindBin(7.7); 
  hphenix->SetBinContent(ibin,0.829);
  hphenix->SetBinError(ibin,0.06);
//   ibin = hphenix->FindBin(200.0); 
//   hphenix->SetBinContent(ibin,3.43);
//   hphenix->SetBinError(ibin,0.21);
//   ibin = hphenix->FindBin(130.0); 
//   hphenix->SetBinContent(ibin,3.01);
//   hphenix->SetBinError(ibin,0.18);
//   ibin = hphenix->FindBin(19.6); 
//   hphenix->SetBinContent(ibin,1.37);
//   hphenix->SetBinError(ibin,0.09);
  hphenix->SetMarkerStyle(25); 
  hphenix->SetMarkerColor(1);
  hphenix->SetMarkerSize(Marker_Size);  
  Float_t phenixenergies[] = {200,130,62.4,39.0,27,19.6,14.5,7.7};
  Int_t nphenixenergies = 8;
  for(Int_t i = 0;i<nphenixenergies;i++){
    Int_t bin = hphenix->FindBin(phenixenergies[i]);
    cout<<"i "<<i<<" bin "<<bin<<" Bin content "<<hphenix->GetBinContent(bin)<<" +/- "<<hphenix->GetBinError(bin)<<endl;
    allpoints->SetPoint(allpointsNPoints,phenixenergies[i],hphenix->GetBinContent(bin));
    allpoints->SetPointError(allpointsNPoints,0,hphenix->GetBinError(bin));//bin
    allpointsNPoints++;
  }

  //
  TH1F *hstar = new TH1F("hstar","dET/deta",100000,1.0,14000.0);
  ibin = hstar->FindBin(190.0);
  hstar->SetBinContent(ibin,3.51);
  hstar->SetBinError(ibin,0.19);
  hstar->SetMarkerStyle(29);  hstar->SetMarkerColor(kBlue);
  hstar->SetMarkerSize(Marker_Size * 1.5);  
  if(addSTARToFit){
    allpoints->SetPoint(allpointsNPoints,200,hstar->GetBinContent(ibin));
    allpoints->SetPointError(allpointsNPoints,0,hstar->GetBinError(ibin));
    allpointsNPoints++;
    
  }
  //
  TH1F *hna49 = new TH1F("hna49","dET/deta",100000,1.0,14000.0);
  ibin = hstar->FindBin(17.2);
  hna49->SetBinContent(ibin,1.50);
  hna49->SetBinError(ibin,0.11);
  ibin = hstar->FindBin(12.4);
  hna49->SetBinContent(ibin,1.16);
  hna49->SetBinError(ibin,0.09);
  ibin = hstar->FindBin(8.7);
  hna49->SetBinContent(ibin,0.94);
  hna49->SetBinError(ibin,0.07);
  hna49->SetMarkerStyle(26); hna49->SetMarkerColor(2);
  hna49->SetMarkerSize(Marker_Size);  
  if(addNA49ToFit){
    Float_t na49energies[] = {17.2,12.4,8.7};
    Int_t nna49energies = 3;
    for(Int_t i = 0;i<nna49energies;i++){
      Int_t bin = hna49->FindBin(na49energies[i]);
      //cout<<"i "<<i<<" bin "<<bin<<" Bin content "<<hna49->GetBinContent(bin)<<" +/- "<<hna49->GetBinError(bin)<<endl;
      allpoints->SetPoint(allpointsNPoints,na49energies[i],hna49->GetBinContent(bin));
      allpoints->SetPointError(allpointsNPoints,0,hna49->GetBinError(bin));//bin
      allpointsNPoints++;//phenix
  }


  }
  //
  TH1F *hwa98 = new TH1F("hwa98","dET/deta",100000,1.0,14000.0);
  ibin = hwa98->FindBin(17.0);
  hwa98->SetBinContent(ibin,2.0);
  hwa98->SetBinError(ibin,0.12);  
  hwa98->SetMarkerStyle(24); hwa98->SetMarkerColor(4);
  hwa98->SetMarkerSize(Marker_Size);  
  if(addSTARToFit){
    allpoints->SetPoint(allpointsNPoints,17.0,hwa98->GetBinContent(ibin));
    allpoints->SetPointError(allpointsNPoints,0,hwa98->GetBinError(ibin));
    allpointsNPoints++;
    
  }


  //
  TH1F *he802 = new TH1F("he802","dET/deta",100000,1.0,14000.0);
  ibin = he802->FindBin(4.84);
  he802->SetBinContent(ibin,0.579);
  he802->SetBinError(ibin,0.087);  
  ibin = he802->FindBin(4.27);
  he802->SetBinContent(ibin,0.498);
  he802->SetBinError(ibin,0.075);   
  ibin = he802->FindBin(3.81);
  he802->SetBinContent(ibin,0.405);
  he802->SetBinError(ibin,0.061);
  he802->SetMarkerStyle(32); he802->SetMarkerColor(4);
  he802->SetMarkerSize(Marker_Size);  
  if(addE802ToFit){
    Float_t e802energies[] = {4.84,4.27,3.81};
    Int_t ne802energies = 3;
    for(Int_t i = 0;i<ne802energies;i++){
      Int_t bin = he802->FindBin(e802energies[i]);
      //cout<<"i "<<i<<" bin "<<bin<<" Bin content "<<he802->GetBinContent(bin)<<" +/- "<<he802->GetBinError(bin)<<endl;
      allpoints->SetPoint(allpointsNPoints,e802energies[i],he802->GetBinContent(bin));
      allpoints->SetPointError(allpointsNPoints,0,he802->GetBinError(bin));//bin
      allpointsNPoints++;//phenix
    }
  }

  TH1F* hcms = GetCMSDataSqrtS();
  if(addCMSToFit){
    ibin = hcms->FindBin(2760); 
    allpoints->SetPoint(allpointsNPoints,2760,hcms->GetBinContent(ibin));
    allpoints->SetPointError(allpointsNPoints,0,hcms->GetBinError(ibin));
    allpointsNPoints++;

  }
  //
  hphenix->SetMaximum(16.0);
  //if(is5TeV)  hphenix->SetMaximum(18.0);
  hphenix->SetMarkerStyle(25);
  hphenix->SetStats(0); 
  hphenix->SetTitle(0);
  hphenix->GetXaxis()->SetTicks("+");
  hphenix->GetYaxis()->SetTitleSize(0.07);
  hphenix->GetXaxis()->SetTitleSize(0.07);
  hphenix->GetXaxis()->SetLabelSize(0.07);
  hphenix->GetYaxis()->SetLabelSize(0.07);
  hphenix->GetYaxis()->SetTitleOffset(0.7);
  hphenix->GetXaxis()->SetLabelOffset(0.0);
  hphenix->GetXaxis()->SetTitleOffset(1.1);
  hphenix->GetXaxis()->SetTitle("#sqrt{#it{s}_{NN}} (GeV)");
  hphenix->GetYaxis()->SetTitle("#LTd#it{E}_{T}/d#eta#GT/#LT#it{N}_{part}/2#GT (GeV)");
  hphenix->Draw("PE");
  hstar->Draw("PE same");
  hna49->Draw("PE same"); 
  hwa98->Draw("PE same");
  he802->Draw("PE same");
  halicehad->Draw("PE same");
  if(isPrelim) halicehadprelim->Draw("PE same");
  hcms->Draw("PE same");
  //
   Float_t textsize = 0.0588235;
//   TMarker *ma0 = new TMarker();
//   ma0->SetMarkerStyle(20); ma0->SetMarkerColor(2); 
//   ma0->SetMarkerSize(Marker_Size);  
//   ma0->SetX(2.0); ma0->SetY(15.0); ma0->Draw();
//   //  TText *ta0 = new TText(2.5,15.0,"ALICE");
//   TLatex *ta0 = new TLatex(2.5,15.0,"ALICE");
//   ta0->SetTextSize(textsize); ta0->SetTextAlign(12);
//   ta0->Draw();  

//   TMarker *m1 = new TMarker();
//   m1->SetMarkerStyle(21); m1->SetMarkerColor(1); 
//   m1->SetMarkerSize(Marker_Size);  
//   m1->SetX(2.0); m1->SetY(14.0); m1->Draw();
//   TText *t1 = new TText(2.5,14.0,"PHENIX");
//   t1->SetTextSize(textsize); t1->SetTextAlign(12);
//   t1->Draw();  

//   TMarker *m2 = new TMarker();
//   m2->SetMarkerStyle(29); m2->SetMarkerColor(TColor::kBlue); 
//   m2->SetMarkerSize(Marker_Size * 1.5);  
//   m2->SetX(2.0); m2->SetY(13); m2->Draw("same");
//   TText *t2 = new TText(2.5,13,"STAR");
//   t2->SetTextSize(textsize); t2->SetTextAlign(12);
//   t2->Draw();  

//   TMarker *m3 = new TMarker();
//   m3->SetMarkerStyle(26); m3->SetMarkerColor(2); 
//   m3->SetMarkerSize(Marker_Size);  
//   m3->SetX(2.0); m3->SetY(12.0); m3->Draw("same");
//   TText *t3 = new TText(2.5,12.0,"NA49");
//   t3->SetTextSize(textsize); t3->SetTextAlign(12);
//   t3->Draw();  

//   TMarker *m4 = new TMarker();
//   m4->SetMarkerStyle(24); m4->SetMarkerColor(4); 
//   m4->SetMarkerSize(Marker_Size);  
//   m4->SetX(2.0); m4->SetY(11.0); m4->Draw("same");
//   TText *t4 = new TText(2.5,11.0,"WA98");
//   t4->SetTextSize(textsize); t4->SetTextAlign(12);
//   t4->Draw();  

//   TMarker *m5 = new TMarker();
//   m5->SetMarkerStyle(25); m5->SetMarkerColor(4); 
//   m5->SetMarkerSize(Marker_Size);  
//   m5->SetX(2.0); m5->SetY(10.0); m5->Draw("same");
//   TText *t5 = new TText(2.5,10.0,"E802/917");
//   t5->SetTextSize(textsize); t5->SetTextAlign(12);
//   t5->Draw();  
//   //
//   // Functions 
//   // Linear fit
   //TF1 *f1 = new TF1("f1","-0.81187+0.7875*log(x)",3.5,5500); 
   TF1 *f1 = new TF1("f1","[0]*log(x/[1])",3.5,5500); 
   f1->SetParameter(0,0.73);
   f1->SetParameter(1,2.35);
  f1->SetLineWidth(2.0);
  f1->SetLineStyle(9);
  f1->SetLineColor(1);
  f1->Draw("same");
  // EKRT Nucl. Phys. A 696 (2001) 715, Fig. 4 
  TF1 *f4 = new TF1("f4","(1./185)*68*pow(x,0.475)",8,3300);
  f4->SetLineWidth(3);
  if(!is5TeV || models) f4->Draw("same");
  if(models){
    f4->SetLineColor(kBlue);
    f4->SetLineStyle(2);
  }
  //
  float yshift = 2.3;//1.8;
  TLine *l1 = new TLine(2.0,9.0-yshift,7.0,9.0-yshift); 
  l1->SetLineStyle(9); l1->SetLineWidth(2);
  l1->SetLineWidth(2);
  l1->SetLineColor(1);//TColor::kRed);
  l1->Draw();
  //  TLatex *t6 = new TLatex(7.5,9.0,"SPS+RHIC Fit: A+B*ln(#\sqrt{s})");
  TLatex *t6 = new TLatex(7.5,9.0-yshift,"a ln(#sqrt{#it{s}_{NN}}/b)"); // shorter text //-0.81187+0.7875*log(x)
  t6->SetTextSize(textsize); t6->SetTextAlign(12);
  t6->Draw(); 
  TLine *l2_1 = new TLine(2.0,8.0-yshift,7.0,8.0-yshift); 
  l2_1->SetLineStyle(1); l2_1->SetLineWidth(2);
  l2_1->SetLineColor(kRed);
  if(!is5TeV) l2_1->Draw();
  //delete l2_1;
  TLatex *t7_1 = new TLatex(7.5,8.0-yshift,"EKRT");
  t7_1->SetTextSize(textsize); t7_1->SetTextAlign(12);
  if(!is5TeV) t7_1->Draw();  
  if(is5TeV && models){
    TLatex *t7_1a = new TLatex(7.5,6.0-yshift,"EKRT");
    t7_1a->SetTextSize(textsize); t7_1a->SetTextAlign(12);
    t7_1a->Draw();
    TLine *l1a = new TLine(2.0,6.0-yshift,7.0,6.0-yshift); 
    l1a->SetLineStyle(2); l1a->SetLineWidth(2);
    l1a->SetLineWidth(2);
    l1a->SetLineColor(kBlue);
    l1a->Draw();
  }
//   //
//   /*
//   TLatex *t8 = new TLatex(3000,1.2,"LHC Run10");
//   t8->SetTextSize(0.036); t8->SetTextAlign(12);
//   t8->Draw();  
//   */
//   TLatex *t9 = new TLatex(120,16.5,"<dE_{T}/d#\eta> vs. #\sqrt{#it{s}_{NN}} for Central Au-Au/Pb#font[122]{-}Pb Collisions");
//   t9->SetTextSize(0.045); t9->SetTextAlign(21);
//   //t9->Draw();  
//   // Arrow
//   //  TArrow *a = new TArrow(2760.0,0.5,2760.0,5.0,0.03); a->Draw();

//   //gROOT->ProcessLine(".L ALICEPreliminaryPlot.C");
//   //ALICEPreliminaryPlot(c, 0.4, 0.55, 0.7, 0.83);
  TGraph *trento = new TGraph();
  Float_t trentoscale = 1.0;//0.7;
  trento->SetPoint(0,130,trentoscale*(862.052865+760.184352)/2.0/347.7*2);//using PHENIX npart
  trento->SetPoint(1,200,trentoscale*(988.211650+873.229411)/2.0/350.9*2);//using PHENIX npart
  trento->SetPoint(2,2760,trentoscale*(2698.379639+2398.537229)/2.0/((396.1+369.3)/2)*2);//using ALICE npart
  trento->SetPoint(3,5020,trentoscale*(3495.417284+3103.240247)/2.0/((398+372.2)/2.)*2);//using ALICE npart

  if(is5TeV){
    //trento->Draw("l same");
  }
  TLegend *leg1 = new TLegend(0.145973,0.553476,0.377517,0.962567);
  leg1->SetFillStyle(0);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.0534759);
  //leg1->AddEntry(halicehad,"ALICE 0-5% Pb#font[122]{-}Pb","P");
  if(isPrelim){
    leg1->AddEntry(halicehadprelim,"ALICE Preliminary","P");
    leg1->AddEntry(halicehad,"ALICE PRC94(2016)034903","P");
  }
  else{
    leg1->AddEntry(halicehad,"ALICE","P");
  }
  leg1->AddEntry(hcms,"CMS","P");
  leg1->AddEntry(hstar,"STAR","P");
  leg1->AddEntry(hphenix,"PHENIX","P");
  leg1->AddEntry(hna49,"NA49","P");
  leg1->AddEntry(he802,"E802/E917","P");
  leg1->AddEntry(hwa98,"WA98","P");
  //leg1->AddEntry(f1,"SPS+RHIC Fit: A+B*ln(#\sqrt{s})","l");
  //leg1->AddEntry(f4,"EKRT","l");
//   leg1->AddEntry(ETRA_MULT_SQRT_fopi,"FOPI estimate","P");
  leg1->Draw();

  TCanvas *c2 = new TCanvas("c2","dET/deta FIT",600,400);
  c2->SetLogx();
  c2->SetLeftMargin(0.10);
  c2->SetBottomMargin(0.14);
  c2->SetBorderSize(0);
  c2->SetFillColor(0);
  c2->SetFillColor(0);
  c2->SetBorderMode(0);
  c2->SetFrameFillColor(0);
  c2->SetFrameBorderMode(0);
  c2->SetTopMargin(0.026738);
  c2->SetRightMargin(0.03);
  c2->SetLeftMargin(0.112416);
  c2->SetBottomMargin(0.17);
  allpoints->Draw("AP");
  TF1 *func = new TF1("func","[0]*pow(x,2*[1])",0,6000);
  func->SetParameter(1,0.1);
  func->SetParLimits(1,0.1,0.5);
  allpoints->Fit(func);
  cout<<"Chi^2 "<<func->GetChisquare()<<" NDF "<<func->GetNDF()<<" Chi^2/NDF "<<func->GetChisquare()/func->GetNDF()<<endl;

  c->cd();
  if(is5TeV) func->Draw("same");

  yshift = 4.1;//1.8;
  TLine *l2 = new TLine(2.0,9.0+0.4-yshift,7.0,9.0+0.4-yshift); 
  l2->SetLineStyle(1); l1->SetLineWidth(2);
  l2->SetLineWidth(2);
  l2->SetLineColor(kRed);
    if(is5TeV)l2->Draw();
  //  TLatex *t6 = new TLatex(7.5,9.0,"SPS+RHIC Fit: A+B*ln(#\sqrt{s})");
  //delete t7;
  TLatex *t7 = new TLatex(7.5,9.0-yshift,"a s^{b}"); // shorter text //-0.81187+0.7875*log(x)
  t7->SetTextSize(textsize); t6->SetTextAlign(12);
  if(is5TeV)  t7->Draw();  
  if(is5TeV){
    if(models){
      c->SaveAs("./debugPlots/etVsSqrts2015Models.eps");
      c->SaveAs("./debugPlots/etVsSqrts2015Models.png");
      //  c->SaveAs("../pics/etVsSqrts.pdf"); // pdf misses logo unfortunately
      gSystem->Exec("./debugPlots/etVsSqrts2015Models.eps");
    }
    else{
      c->SaveAs("./debugPlots/pics/etVsSqrts2015.eps");
      c->SaveAs("./debugPlots/etVsSqrts2015.png");
      //  c->SaveAs("../pics/etVsSqrts.pdf"); // pdf misses logo unfortunately
      gSystem->Exec("./debugPlots/etVsSqrts2015.eps");
    }
  }
  else{
    c->SaveAs("./debugPlots/etVsSqrts.eps");
    c->SaveAs("./debugPlots/etVsSqrts.png");
    //  c->SaveAs("../pics/etVsSqrts.pdf"); // pdf misses logo unfortunately
    gSystem->Exec("./debugPlots/etVsSqrts.eps");
  }

}
