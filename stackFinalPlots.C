// macro to create all the final plots, appropriately stacked
/*
1. dETdEtaOverNpartBy2SumCent[i]
2. dETdEtaOverdNchdEtaSumCent[i]
*/

#include "fitBESData5.h"
std::string doubToString(Double_t);
std::string centIndToPercent(int centInd);

void stackFinalPlots()
{

TFile* file1 = TFile::Open("./crossCheckGraphs.root");
//TH1D* g1;
TGraphErrors* g1;
string graphName;
string graphText;
string imgPathAndName;

const int CENTS = 9;
const int COLLENS = 5;
Double_t collEnArr[5] = {7.7,11.5,19.6,27,39};

// to check power law in ET/Npart vs Snn plot:
TF1 *func = new TF1("func","[0]*pow(x,2*[1])",0,40);
func->SetParameter(1,0.1);
func->SetParLimits(1,0.1,0.5);
func->SetLineStyle(9);
func->SetLineColor(1);
//gStyle -> SetErrorX(0.);
// 1 and 2
for(int graphInd = 0; graphInd < 2; graphInd++) // two different snn graphs
{
	TCanvas *c1 = new TCanvas();
	TLegend* leg = new TLegend(0.1,0.5,0.5,0.9);

	for(int centInd = 0; centInd < CENTS; centInd++)
	{
		if (graphInd == 0)
		{
			graphName = "dETdEtaOverNpartBy2SumCent" + std::to_string(centInd);
			//g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.5, 2.00);
		}
		else
		{ 
			graphName = "dETdEtaOverdNchdEtaSumCent" + std::to_string(centInd);
			g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.6, 1.00);
		}
		const char* graphNameConstCharPtr = graphName.c_str();
		graphText = centIndToPercent(centInd);
		const char* graphTextConstCharPtr = graphText.c_str();
		cout << "graphName: " << graphName << endl;
		g1 = (TGraphErrors*)file1->Get(graphNameConstCharPtr);
		if (graphInd == 0 && centInd==0)
		{
			g1->Fit(func);
	  		cout<<graphName<<": Chi^2 "<<func->GetChisquare()
	  			<<" NDF "<<func->GetNDF()<<" Chi^2/NDF "
	  			<<func->GetChisquare()/func->GetNDF()<<endl;
	  		//gStyle->SetOptFit(0111);
		}
		g1->SetMarkerSize(1);
		if (centInd == 0) g1->Draw("A*X");
		g1->SetMarkerColor(1+centInd); 
		g1->SetFillColor(1+centInd);
		g1 -> SetMarkerStyle(20+centInd);
		c1 -> SetLogx();
		g1 -> Draw("PX");
		//g1 -> Draw("PE");
		g1 -> SetName("g1");
		leg -> AddEntry(g1, graphTextConstCharPtr, "p");
		leg -> SetFillStyle(0);
		leg -> SetFillColor(0);
		leg -> SetBorderSize(0);
		leg -> Draw();
		
	} // end of for loop with index centInd
						
	// PHENIX plot for comparison:
	if (graphInd == 0)
	{
		TFile* file2 = TFile::Open("./plt4aAdare.root");
		g1 = (TGraphErrors*)file2->Get("plt4aAdare_PHENIX");
		g1->SetFillColorAlpha(kBlue-10, 0.75);
   		g1->SetFillStyle(3002);
		g1->Draw("3");
		cout << "********************************" << endl;
	}
	
	imgPathAndName = 
			"./publication/Biswas/figures/finalStacked/"+graphName+"s.png";
	c1 -> Update();
	const char* imgPathAndNameConstCharPtr1 = imgPathAndName.c_str();
	c1->SaveAs(imgPathAndNameConstCharPtr1);
	delete c1;
	delete leg;
} // end of for loop with index graphInd

for(int graphInd = 2; graphInd < 4; graphInd++) // two different npart graphs
{
	TCanvas *c1 = new TCanvas();
	TLegend* leg = new TLegend(0.7,0.1,0.9,0.4);
	for(int enInd = 0; enInd < COLLENS; enInd++)
	{
		
		if (graphInd == 2)
		{
			graphName = "dETdEtaOverNpartBy2SumEn" + doubToString(collEnArr[enInd]);
			//g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.5, 2.00);
		}
		else
		{ 
			graphName = "dETdEtaOverdNchdEtaSumEn" + doubToString(collEnArr[enInd]);
			g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.4, 1.00);
		}
		const char* graphNameConstCharPtr = graphName.c_str();
		graphText = doubToString(collEnArr[enInd]) + " GeV";
		const char* graphTextConstCharPtr = graphText.c_str();
		cout << "graphName: " << graphName << endl;
		g1 = (TGraphErrors*)file1->Get(graphNameConstCharPtr);
		// suppress error bars along X-axis:
		//gStyle -> SetErrorX(0.0001); // does not work
		//c1->Update();		
		for(int j=0;j<g1->GetN();j++){g1->SetPointError(j,0,g1->GetErrorY(j));}
		g1->SetMarkerSize(1);
		if (enInd == 0) g1 -> Draw("AP");
		g1->SetMarkerColor(1+enInd); g1 -> SetLineColor(1+enInd);
		g1 -> SetMarkerStyle(20+enInd);
		//c1 -> SetLogx();
		g1 -> Draw("P");
		g1 -> SetName("g1");
		leg -> AddEntry(g1, graphTextConstCharPtr, "p");
		leg -> SetFillStyle(0);
		leg -> SetFillColor(0);
		leg -> SetBorderSize(0);
		leg -> Draw();
		
	} // end of for loop with index enInd

	// save stuff:
	imgPathAndName = 
			"./publication/Biswas/figures/finalStacked/"+graphName+"s.png";
	c1 -> Update();
	const char* imgPathAndNameConstCharPtr1 = imgPathAndName.c_str();
	c1->SaveAs(imgPathAndNameConstCharPtr1);
	delete c1;
	delete leg;
} // end of for loop with index graphInd
delete func;


}
