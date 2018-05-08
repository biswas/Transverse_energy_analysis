// macro to overlay two graphs on the same canvas for comparison

{
TCanvas *c1 = new TCanvas();
TFile* file1 = TFile::Open("./crossCheckGraphs.root");
TFile* file2 = TFile::Open("./plt4aAdare.root");
TH1D* g1 = (TH1D*)file1->Get("dETdEtaOverNpartBy2SumCent0");
TH1D* g2 = (TH1D*)file2->Get("plt4aAdare_PHENIX");

TF1 *func = new TF1("func","[0]*pow(x,[1])",0,40);
func->SetParameter(1,0.1);
func->SetParLimits(1,0.1,0.5);
func->SetLineStyle(9);
func->SetLineColor(1);
g1->Fit(func);
	  		cout<<graphName<<": Chi^2 "<<func->GetChisquare()
	  			<<" NDF "<<func->GetNDF()<<" Chi^2/NDF "
	  			<<func->GetChisquare()/func->GetNDF()<<endl;

//g1->GetXaxis()->SetTicks("+");
g1->SetMarkerColor(7); g1->SetMarkerSize(2);
g2 -> SetMarkerStyle(kFullCircle); g2->SetMarkerColor(4); g2->SetMarkerSize(2);
g1->Draw("A*");
g1 -> SetMarkerStyle(kFullStar);
g1->Draw("same PE");
g2->Draw("same PE");
g1 -> SetName("g1");
g2 -> SetName("g2");

TLegend* leg = new TLegend(0.1,0.7,0.48,0.9);

leg -> Draw();
leg -> AddEntry("g1", "STAR 0-5% Au+Au (Spectral)", "p");
leg -> AddEntry("g2", "PHENIX 0-5% Au+Au (Calorimetric)", "p");
leg -> SetFillStyle(0);
leg -> SetFillColor(0);
leg -> SetBorderSize(0);
leg -> SetTextSize(0.0534759);

c1 -> Update();
c1->SaveAs("./PHENIX_comparison2.png");
}
