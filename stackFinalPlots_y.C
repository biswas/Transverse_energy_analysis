// ecolved from stackFinalPlots.C
// macro to create all the final plots, appropriately stacked
// relevant to rapidity, y, instead of pseudorapidity
/*
1. dETdyOverNpartBy2SumCent[i]
2. dETdyOverdNchdySumCent[i]
*/

#include "fitBESData5.h"
std::string doubToString(Double_t);
std::string centIndToPercent(int centInd);

void stackFinalPlots_y()
{

TFile* file1 = TFile::Open("./crossCheckGraphs_y.root");
//TH1D* g1;
TGraphErrors* g1;
string graphName;
string graphText;
string imgPathAndName;

const int CENTS = 9;
const int COLLENS = 5;
Double_t collEnArr[5] = {7.7,11.5,19.6,27,39};
// 1 and 2
for(int graphInd = 0; graphInd < 2; graphInd++) // two different snn graphs
{
	TCanvas *c1 = new TCanvas();
	TLegend* leg = new TLegend(0.1,0.5,0.5,0.9);
	for(int centInd = 0; centInd < CENTS; centInd++)
	{
		if (graphInd == 0)
		{
			graphName = "dETdyOverNpartBy2SumCent" + std::to_string(centInd);
			//g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.5, 2.00);
		}
		else
		{ 
			graphName = "dETdyOverdNchdySumCent" + std::to_string(centInd);
			g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.6, 1.05);
		}
		const char* graphNameConstCharPtr = graphName.c_str();
		graphText = centIndToPercent(centInd);
		const char* graphTextConstCharPtr = graphText.c_str();
		cout << "graphName: " << graphName << endl;
		g1 = (TGraphErrors*)file1->Get(graphNameConstCharPtr);
		g1->SetMarkerSize(1);
		if (centInd == 0) g1->Draw("A*X");
		g1->SetMarkerColor(1+centInd); 
		g1 -> SetMarkerStyle(20+centInd);
		c1 -> SetLogx();
		g1 -> Draw("PX");
		g1 -> SetName("g1");
		leg -> AddEntry(g1, graphTextConstCharPtr, "p");
		leg -> SetFillStyle(0);
		leg -> SetFillColor(0);
		leg -> SetBorderSize(0);
		leg -> Draw();
		imgPathAndName = 
			"./publication/Biswas/figures/finalStacked/"+graphName+"s.png";
		c1 -> Update();
	} // end of for loop with index centInd
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
			graphName = "dETdyOverNpartBy2SumEn" + doubToString(collEnArr[enInd]);
			g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.0, 2.50);
		}
		else
		{ 
			graphName = "dETdyOverdNchdySumEn" + doubToString(collEnArr[enInd]);
			g1->GetHistogram()->GetYaxis()-> SetRangeUser(0.5, 1.10);
		}
		const char* graphNameConstCharPtr = graphName.c_str();
		graphText = doubToString(collEnArr[enInd]) + " GeV";
		const char* graphTextConstCharPtr = graphText.c_str();
		cout << "graphName: " << graphName << endl;
		g1 = (TGraphErrors*)file1->Get(graphNameConstCharPtr);
		// get rid of x-errors:
		for(int j=0;j<g1->GetN();j++){g1->SetPointError(j,0,g1->GetErrorY(j));}
		g1->SetMarkerSize(1);
		if (enInd == 0) g1->Draw("AP");
		g1->SetMarkerColor(1+enInd); g1 -> SetLineColor(1+enInd);
		g1 -> SetMarkerStyle(20+enInd); g1 -> SetLineStyle(9-enInd*2);
		//c1 -> SetLogx();
		g1 -> Draw("P");
		g1 -> SetName("g1");
		leg -> AddEntry(g1, graphTextConstCharPtr, "p");
		leg -> SetFillStyle(0);
		leg -> SetFillColor(0);
		leg -> SetBorderSize(0);
		leg -> Draw();
		imgPathAndName = 
			"./publication/Biswas/figures/finalStacked/"+graphName+"s.png";
		c1 -> Update();
	} // end of for loop with index centInd
	const char* imgPathAndNameConstCharPtr1 = imgPathAndName.c_str();
	c1->SaveAs(imgPathAndNameConstCharPtr1);
	delete c1;
	delete leg;
} // end of for loop with index graphInd



}
