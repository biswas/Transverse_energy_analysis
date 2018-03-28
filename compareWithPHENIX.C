// macro to overlay two graphs on the same canvas for comparison

{
//Int_t ibin;
TCanvas *c1 = new TCanvas();
TFile* file1 = TFile::Open("./crossCheckGraphs.root");
TFile* file2 = TFile::Open("./plt4aAdare.root");
TH1D* g1 = (TH1D*)file1->Get("dETdEtaOverNpartBy2SumCent0");
TH1D* g2 = (TH1D*)file2->Get("plt4aAdare_PHENIX");
//TH1D *hna49 = new TH1D("hna49","dET/deta",100000,1.0,14000.0); // hna49
//ibin = hstar->FindBin(17.2);//collision energy is 17.2 GeV/c
//hna49->SetBinContent(ibin,1.50);//dET/deta/(npart/2) is 1.50 +/0.11

// get data points and errors for comparisons:
cout << "Number of bins: " << g1->GetNbinsX() << endl;
cout << "Exper.\t" << "SnnMarker\t" << "ET\t" << "ET_err\n";
for(Int_t i = 1; i< 6; i++) // 5 different energies, 2nd can't be compared
{	
	//if (i==2) continue;
	cout << "STAR:\t"<<g1->GetBinCenter(i)<<"\t"<<g1->GetBinContent(i)<<"\t"
			<<g1->GetBinError(i)<<"\n";
	cout << "PHENIX:\t"<<g1->GetBinCenter(i)<<"\t"<<g2->GetBinContent(i)<<"\t"
			<<g2->GetBinError(i)<<"\n";
}


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
delete c1;
}
