// macro to overlay two graphs on the same canvas for comparison

{
TCanvas *c1 = new TCanvas();
//c1 -> DrawFrame(0.000000000000001, 0.000000000000000001, 2800., 12.);
TFile* file1 = TFile::Open("./crossCheckGraphs.root");
TFile* file2 = TFile::Open("./plt4aAdare.root");
TH1D* g1 = (TH1D*)file1->Get("dETdEtaOverNpartBy2SumCent0");
TH1D* g2 = (TH1D*)file2->Get("plt4aAdare_PHENIX");

///////// power law fit
TF1 *func = new TF1("func","[0]*pow(x,[1])",0,2800);
func->SetParameter(1,0.1);
func->SetParLimits(1,0.1,1.);
func->SetLineStyle(9);
func->SetLineColor(1);
g1->Fit("func");
	  		cout<<"Chi^2 "<<func->GetChisquare()
	  			<<" NDF "<<func->GetNDF()<<" Chi^2/NDF "
	  			<<func->GetChisquare()/func->GetNDF()<<endl;

/////////////

//TH1D* gCMS = new TH1D("CMS",NULL,100000,1.0,14000.0); // add CMS data point for comparison
//TH1D* gALICE = new TH1D("ALICE",NULL,100000,1.0,14000.0); // add ALICE data point for comparison
Double_t CMSarrX[1] = {2760};
Double_t CMSarrY[1] = {10.5};
Double_t CMSarrYerr[1] = {0.5};
Double_t ALICEarrX[1] = {2760};
Double_t ALICEarrY[1] = {9};
Double_t ALICEarrYerr[1] = {0.5};
//////g1->SetPoint(1,2760,10.5);
//////g1->SetPoint(1,2760,9);
///////////////^^^
TGraphErrors* gCMS = new TGraphErrors(1,CMSarrX,CMSarrY,NULL,CMSarrYerr); // add CMS and ALICE data point for comparison
TGraphErrors* gALICE = new TGraphErrors(1,ALICEarrX,ALICEarrY,NULL,ALICEarrYerr);
gCMS -> Draw("A*");
gALICE -> Draw("A*");
//gCMSnALICE -> GetHistogram() -> GetXaxis() -> SetRangeUser(0.,2800.);
/////////TGraphErrors* gCMS = new TGraphErrors(); // add CMS data point for comparison
/////////TGraphErrors* gALICE = new TGraphErrors(); // add ALICE data point for comparison
//g1->GetXaxis()->SetTicks("+");
c1 -> DrawFrame(0., 0.1, 5000., 15.);
g1->SetMarkerColor(1); g1->SetMarkerSize(1);
g2 -> SetMarkerStyle(kFullCircle); g2->SetMarkerColor(4); g2->SetMarkerSize(1);
/////////gCMS-> SetMarkerStyle(kFullTriangleDown); gCMS->SetMarkerColor(2); gCMS->SetMarkerSize(2);
/////////gALICE -> SetMarkerStyle(kFullTriangleUp); gALICE->SetMarkerColor(3); gALICE->SetMarkerSize(2);
//g1->GetYaxis()-> SetRangeUser(0.5, 110);

///////////////////g1->Draw("A*");


g1->Draw("same PE");
g1 -> SetMarkerStyle(kFullStar); /// needs to be called after Draw("A*")
g1->Draw("same PE");
g2->Draw("same PE");
g1 -> SetName("g1");
g2 -> SetName("g2");

gCMS -> SetMarkerStyle(kFullTriangleDown); gCMS->SetMarkerColor(2);
gCMS -> Draw("same PE");
gALICE -> SetMarkerStyle(kFullTriangleUp); gALICE->SetMarkerColor(3);
gALICE -> Draw("same PE");
gCMS -> SetName("gCMS");
gALICE -> SetName("gALICE");
func -> Draw("same");
/////////////////////////////////// CMS and ALICE

/*
Int_t ibin = gCMS->FindBin(2760);//collision energy is 2760 GeV/c
gCMS->SetBinContent(ibin,10.5);//dET/deta/(npart/2) is 10.5 +/0.11
gCMS->SetBinError(ibin,0.5);
gCMS->SetBinContent(ibin,9);//dET/deta/(npart/2) is 10.5 +/0.11
gCMS->SetBinError(ibin,0.5);

gCMS->Draw("same PE");
gCMS -> SetMarkerStyle(kFullStar);
gCMS->Draw("same PE");
gALICE->Draw("same PE");
gCMS -> SetName("gCMS");
gALICE -> SetName("gALICE");
*/
/*
gCMS->SetPoint(1,2760,10.5);
gALICE->SetPoint(1,2760,9);
gCMS->SetPointError(1,0,0.5);
gALICE->SetPointError(1,0,0.5);
gCMS->Draw("same PE");
gALICE->Draw("same PE");
gCMS -> SetName("gCMS");
gALICE -> SetName("gALICE");
*/
///////////////////////////////////



TLegend* leg = new TLegend(0.5,0.15,0.7,0.3);

leg -> Draw();
leg -> AddEntry("g1", "STAR 0-5% Au+Au (Spectral)", "p");
leg -> AddEntry("g2", "PHENIX 0-5% Au+Au (Calorimetric)", "p");
leg -> AddEntry("gCMS", "CMS 0-5% Au+Au at 2.76 TeV", "p");
leg -> AddEntry("gALICE", "ALICE 0-5% Au+Au at 2.76 TeV", "p");
leg -> SetFillStyle(0);
leg -> SetFillColor(0);
leg -> SetBorderSize(0);
leg -> SetTextSize(0.0334759);
c1->SetLogx(); c1->SetLogy();

c1 -> Update();
c1->SaveAs("./STAR_P_C_Acomparison.png");// STAR, PHENIX, CMS, ALICE
return c1;
}
