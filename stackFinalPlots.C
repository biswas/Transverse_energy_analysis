// macro to create all the final plots, appropriately stacked
/*
1. dETdEtaOverNpartBy2SumCent[i]
2. dETdEtaOverdNchdEtaSumCent[i]
*/

#include "fitBESData5.h"
void stackFinalPlots()
{

TFile* file1 = TFile::Open("./crossCheckGraphs.root");
//TH1D* g1;
TGraph* g1;
string graphName;
string graphText;
string imgPathAndName;

const int CENTS = 9;
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
		cout << "graphName: " << graphName << endl;
		//g1 = (TH1D*)file1->Get(graphNameConstCharPtr);
		g1 = (TGraph*)file1->Get(graphNameConstCharPtr);
		g1->SetMarkerSize(1);
		if (centInd == 0) g1->Draw("A*X");
		g1->SetMarkerColor(1+centInd); 
		//g2 -> SetMarkerStyle(kFullCircle); g2->SetMarkerColor(4); g2->SetMarkerSize(2);
		g1 -> SetMarkerStyle(20+centInd);
		c1 -> SetLogx();
		//g1->Draw("same PE");
		g1 -> Draw("PX");
		//g2->Draw("same PE");
		g1 -> SetName("g1");

		//g2 -> SetName("g2");
		graphText = centIndToPercent(centInd);
		const char* graphTextConstCharPtr = graphText.c_str();
		//g1 -> SetTitle(graphTextConstCharPtr);	
	
		leg -> AddEntry(g1, graphTextConstCharPtr, "p");
		leg -> SetFillStyle(0);
		leg -> SetFillColor(0);
		leg -> SetBorderSize(0);
		leg -> Draw();
		//leg -> SetTextSize(0.0534759);
		imgPathAndName = 
			"./publication/Biswas/figures/finalStacked/"+graphName+"s.png";

		c1 -> Update();
	} // end of for loop with index centInd
	const char* imgPathAndNameConstCharPtr1 = imgPathAndName.c_str();
	c1->SaveAs(imgPathAndNameConstCharPtr1);
	delete c1;
	delete leg;
}

}
