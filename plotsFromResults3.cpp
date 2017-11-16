/* in this version .....
	automatized npart graph formatting
	next: automatize snn graph formatting
*/


#include "Riostream.h"
//#include <cstdio>
//#include <vector>
#include <string>
#include <fstream>
//#include "TString.h"
//#include "TMath.h"
//#include "TH1D.h"
#include <iostream> // to use cout for debugging
using namespace std;

std::string doubToString(Double_t);
void formatGraph(TGraph* g1, Double_t collEnArr[], int enInd);
// main function:

int plotsFromResults3(){

	ifstream in;
	string skipContent;// read and skip content that has no use
	const int cents = 9; // 9 different centralities
	const int collEns = 5; // 5 different collision energies
	const int parts = 6; // 6 different particles
	const int funcsOfCollEn = 2; // there are two quantities that
							// are to be plotted as functions of
							// collision energy:
							// (dET/dEta)/(N_part/2), and
							// (dET/dEta)/(dN_ch/dEta)
					// TODO actually 4, but try two first
	// array for energy-dependent plots:
	Double_t enPlotArr[cents][collEns][funcsOfCollEn];
	in.open(Form("/home/bsharma/rhip/analysisCodes/fitResults5.dat"));
	if (!in.good()){
		cout << "Problem opening file!" << endl;
		return 1;
	}
	
	for(int i=0;i<46;i++){ // loop through 46 column names
		in >> skipContent;
		// (tested) print first and last header names to check:
		//if (i==0) cout << "Col. 1: " << skipContent << endl;
		//else if (i==45) cout << "Col. 45: " << skipContent << endl;
	}
	Double_t collEn;
	Double_t cent;// centrality value obtained from data file
	string part; // particle name
	Double_t dETdEtaSum[cents][collEns] = {0};// initialize element [0][0] to 0, 
											//and initialize all other elements as if 
											// they had static storage duration, hence
											// set them to zero as well
											
	Double_t dETdEtaErrSum[cents][collEns] = {0};
	Double_t dNchdEtaSum[cents][collEns] = {0};
	Double_t dNchdEtaErrSum[cents][collEns] = {0};
	Double_t Npart[cents][collEns];// Npart function of cent and en only
	Double_t dETdEtaOverNpartBy2Sum[cents][collEns];
	Double_t dETdEtaOverdNchdEtaSum[cents][collEns];
	// Add elements below (for all 6 particles)
		// in order to get the above elements
	Double_t dETdEta[cents][collEns][parts];
	Double_t dETdEtaErr[cents][collEns][parts];
	Double_t dNchdEta[cents][collEns][parts];
	Double_t dNchdEtaErr[cents][collEns][parts];
	//Double_t dETdEtaOverNpartBy2[cents][collEns][parts];
	// enPlotArr element indexed as: 
	// enPlotArr[centIndex][enIndex][funcIndex]
	int centIndex;
	int enIndex;
	int partIndex;
	int funcIndex;
	for(int j=0; j<270; j++){// for 270 rows below header in the data file
		//for(int i=0; i<46; i++){
		in >> collEn; cout << collEn << "\t";
		in >> part; cout << part << "\t";
		in >> cent; cout << cent << "\t";
		for(int k=0; k<15;k++){// skip content in next 15 columns
		in >> skipContent;
		}
		
		// now adding things up
		
		// for each centrality there are 5 different energies
		// for each energy corresponding to a particular centrality,
			// there are 6 different particles, and hence
			// 6 different values of (dET/dEta)/(N_part/2)
		
		// for every collision energy, there are 9 Nparts 
			//(corresponding to 9 centralities)
		// add dETdEtaOverNpartby2 and dETdEtaOverdNchdEta values
			// corresponding to each Npart (cent) for a given collEn
			
		if(cent == 0.) centIndex = 0;
		else if(cent == 1.) centIndex = 1;
		else if(cent == 2.) centIndex = 2;
		else if(cent == 3.) centIndex = 3;
		else if(cent == 4.) centIndex = 4;
		else if(cent == 5.) centIndex = 5;
		else if(cent == 6.) centIndex = 6;
		else if(cent == 7.) centIndex = 7;
		else if(cent == 8.) centIndex = 8;
		else{cout << "Check centrality value!" << endl; return 2;}
		// energy set: {7.7,11.5,19.6,27,39}
		if (collEn == 7.7) enIndex = 0;
		else if (collEn == 11.5) enIndex = 1;
		else if (collEn == 19.6) enIndex = 2;
		else if (collEn == 27) enIndex = 3;
		else if (collEn == 39) enIndex = 4;
		// particle set: {ka-,ka+,pi-,pi+,pro,pba}
		if (part =="pi-") partIndex = 0;
		else if (part == "pi+") partIndex = 1;
		else if (part == "ka-") partIndex = 2;
		else if (part == "ka+") partIndex = 3;
		else if (part == "pro") partIndex = 4;
		else if (part == "pba") partIndex = 5;
		
		in >> dETdEta[centIndex][enIndex][partIndex];
		cout << dETdEta[centIndex][enIndex][partIndex] << "\t";
		in >> dETdEtaErr[centIndex][enIndex][partIndex];
		for(int l=0; l<14; l++) in >> skipContent;
		in >> dNchdEta[centIndex][enIndex][partIndex];
		cout << dNchdEta[centIndex][enIndex][partIndex] << "\t";
		in >> dNchdEtaErr[centIndex][enIndex][partIndex];
		for(int l=0; l<8; l++) in >> skipContent;
		in >> Npart[centIndex][enIndex];
		cout << Npart[centIndex][enIndex] << "\n";
		in >> skipContent;
		// TODO: what about lambdas while adding ET?
		// ET = 3ET_pi + 4ET_k + 4ET_p + 2ET_lam
		// at high energies, numer of negative and positive charges in the collision
			// product are roughly the same
			// which is not true at low energies since the colliding nuclei have +ve nucleons
		//FIXME: fix the sum formulas below:
		if(partIndex==0 || partIndex==1){
			dETdEtaSum[centIndex][enIndex] += (3.0/2.0)*dETdEta[centIndex][enIndex][partIndex];
			// ^ since number of pi0 = num of pi+ or pi-
			dNchdEtaSum[centIndex][enIndex] += dNchdEta[centIndex][enIndex][partIndex];
		}
		else if(partIndex==2 || partIndex==3 || partIndex == 4 || partIndex == 5){
			dETdEtaSum[centIndex][enIndex] += 2.0*dETdEta[centIndex][enIndex][partIndex];
			// ^ since num of k0_s ~ num og k0_l = num of k+ or k-
				// also, num of p ~ num of pbar ~ num of n or nbar
			dNchdEtaSum[centIndex][enIndex] += dNchdEta[centIndex][enIndex][partIndex];
		}
		/*
		if (i==0) cout << headerBuffer << "\t";
		else if (i==1) cout << headerBuffer << "\t";
		if (i==44) cout << headerBuffer << "\t";
		else if (i==45)cout << headerBuffer << "\n";
		*/
		//}//end of for loop with index i
	} //end of for loop with index j - captured all 270 rows from data file
	in.close();// all information from file extracted, so it should be closed
	// print dETdEtaSum for 9 centralities and 5 collision energies
		// and assign values to dETdEtaOverNpartBy2Sum[i][j]
		for(int i=0; i<cents; i++){
			for(int j=0; j<collEns; j++){
				cout << "dETdEtaSum["<<i<<"]["<<j<<"] = "<<dETdEtaSum[i][j]<<endl;
				cout << "dNchdEtaSum["<<i<<"]["<<j<<"] = "<<dNchdEtaSum[i][j]<<endl;
				dETdEtaOverNpartBy2Sum[i][j] = dETdEtaSum[i][j]/Npart[i][j];
				dETdEtaOverdNchdEtaSum[i][j] = dETdEtaSum[i][j]/dNchdEtaSum[i][j];
			}
		}
	Double_t collEnArr[5] = {7.7,11.5,19.6,27,39}; // To use in TGraphErrors

	/*
	for(int i=0; i<cents; i++){ // i is cent index
		for(int j=0; j<collEns;j++)
		Npart[i][j] = 
	}
	*/
	
	/// ------ begin - plot all graphs at once -------------------------------//
	for(int centInd=0; centInd<cents; centInd++){ // loop through all centralities
	
		Double_t dETdEtaOverNpartBy2SumCentByCent[collEns];// one graph per centInd
		Double_t dETdEtaOverdNchdEtaSumCentByCent[collEns];
		for(int i=0; i<collEns; i++){ // loop through all collEns for current centrality
		// because TGraphErrors does not take a 2d array as argument
			dETdEtaOverNpartBy2SumCentByCent[i] = dETdEtaOverNpartBy2Sum[centInd][i];
			dETdEtaOverdNchdEtaSumCentByCent[i] = dETdEtaOverdNchdEtaSum[centInd][i];
		}
		TGraph* g1;
		//Double_t* collEnPtr = &collEnArr[0];
		//Double_t* dETdEtaOverNpartBy2SumCent0Ptr = &dETdEtaOverNpartBy2SumCent0[0];
		//g = new TGraph(5, collEnPtr, dETdEtaOverNpartBy2SumCent0Ptr);// TGraph constructor
		g1 = new TGraph(5, collEnArr, dETdEtaOverNpartBy2SumCentByCent);
		TCanvas *c1 = new TCanvas(/*"c1","A Simple Graph Example",200,10,700,500*/);
		c1->SetLogx();
		g1->Draw("A*");
		g1->SetMarkerStyle(29);
		g1->SetMarkerColor(kBlue);
		g1->SetMarkerSize(2);
		TString xlabel_snn = "#sqrt{#it{s}_{NN}} (GeV)";
		TString ylabel_etOverEtaOverNpartOver2 = "#LTd#it{E}_{T}/d#eta#GT/#LT#it{N}_{part}/2#GT (GeV)";
		g1->GetHistogram()->GetXaxis()-> SetTitle(xlabel_snn);
		g1->GetHistogram()->GetYaxis()-> SetTitle(ylabel_etOverEtaOverNpartOver2);
		string graphName1 = "dETdEtaOverNpartBy2SumCent" + std::to_string(centInd);
		string imgPathAndName1 = 
		"./finalPlots/crossCheckPlots/dETdEtaOverNpartBy2_En/"+graphName1+".png";
					//c1 -> SaveAs("./fittedPlots/trial1.png");
		TImage *png1 = TImage::Create();//TODO try to use canvas method instead of png object
		png1->FromPad(c1);
		const char* imgPathAndNameConstCharPtr1 = imgPathAndName1.c_str();
		png1->WriteImage(imgPathAndNameConstCharPtr1);
		delete g1;
		delete png1;
		delete c1;
	
		// TODO make a function to take care of different types of graph instead of creating new objects here
		TGraph* g2;
		g2 = new TGraph(5, collEnArr, dETdEtaOverdNchdEtaSumCentByCent);
		TCanvas *c2 = new TCanvas(/*"c1","A Simple Graph Example",200,10,700,500*/);
		c2->SetLogx();
		g2->Draw("A*");
		g2->SetMarkerStyle(29);
		g2->SetMarkerColor(kBlue);
		g2->SetMarkerSize(2);
		TString xlabel_snn_2 = "#sqrt{#it{s}_{NN}} (GeV)";
		TString ylabel_etOverEtaOverNchOverEta = "#LTd#it{E}_{T}/d#eta#GT/#LTd#it{N}_{ch}/d#eta#GT (GeV)";
		g2->GetHistogram()->GetXaxis()-> SetTitle(xlabel_snn_2);
		g2->GetHistogram()->GetYaxis()-> SetTitle(ylabel_etOverEtaOverNchOverEta);
		string graphName2 = "dETdEtaOverdNchdEtaSumCent" + std::to_string(centInd);
		string imgPathAndName2 = 
					"./finalPlots/crossCheckPlots/dETdEtaOverdNchdEta_En/"+graphName2+".png";
					//c1 -> SaveAs("./fittedPlots/trial1.png");
		TImage *png2 = TImage::Create();// TODO try to use canvas method instead of png object
		png2->FromPad(c2);
		const char* imgPathAndNameConstCharPtr2 = imgPathAndName2.c_str();
		png2->WriteImage(imgPathAndNameConstCharPtr2);
		delete g2;
		delete png2;
		delete c2;
		/* //TODO: stuff vs Npart: 9 centralities times 5 energies -> 45 Nparts
		// each energy gets a different plot
		
		TGraph* g3;
		g3 = new TGraph(5, collEnArr, dETdEtaOverdNchdEtaSumCentByCent);
		TCanvas *c2 = new TCanvas();
		c2->SetLogx();
		g2->Draw();
		string graphName2 = "dETdEtaOverNpartBy2SumCent" + std::to_string(centInd);
		string imgPathAndName2 = 
					"./finalPlots/crossCheckPlots/dETdEtaOverdNchdEta_En/"+graphName2+".png";
					//c1 -> SaveAs("./fittedPlots/trial1.png");
		TImage *png2 = TImage::Create();// TODO try to use canvas method instead of png object
		png2->FromPad(c2);
		const char* imgPathAndNameConstCharPtr2 = imgPathAndName2.c_str();
		png2->WriteImage(imgPathAndNameConstCharPtr2);
		delete g2;
		delete png2;
		delete c2;
		*/
		
	} // end of for loop with index centInd
	/// ------ end - plot all snn graphs at once -------------------------------//
	
	Double_t NpartArrEnByEn[9]; // for each collEn, there is a unique Npart Array
	// unlike for the snn graphs in which 
	// for each cent, there is the same collEnArr
	// hence, the above array should be used in a loop instead of directly as in
	// the usage of collEnArr.
	// TODO current: populate NpartArr for each energy:
	/// ------ begin - plot all Npart graphs at once -------------------------------//
	for(int enInd=0; enInd<collEns; enInd++){ // loop through all coll Ens
	
		Double_t dETdEtaOverNpartBy2SumEnByEn[cents];// one graph per collEn;
		Double_t dETdEtaOverdNchdEtaSumEnByEn[cents];	// cents = num of Nparts
		for(int i=0; i<cents; i++){ // loop through all cents/Nparts for current collEn
		// because TGraphErrors does not take a 2d array as argument
			dETdEtaOverNpartBy2SumEnByEn[i] = dETdEtaOverNpartBy2Sum[i][enInd];
			dETdEtaOverdNchdEtaSumEnByEn[i] = dETdEtaOverdNchdEtaSum[i][enInd];
			NpartArrEnByEn[i]				= Npart[i][enInd];
		}
		TGraph* g1; // okay to use identifier g1 in local scope as was the case in previous use
		g1 = new TGraph(9, NpartArrEnByEn, dETdEtaOverNpartBy2SumEnByEn);
		g1 -> SetName("dETdEtaOverNpartBy2Sum_vs_Npart"); // unique identifier for graph
														// see use in function formatGraph
		formatGraph(g1, collEnArr, enInd);
		/* // trying to put the following in a function
		TCanvas *c1 = new TCanvas();
		//c1->SetLogx(); Npart graphs not in log scale
		g1->Draw("A*");
		g1->SetMarkerStyle(28);
		g1->SetMarkerColor(kGreen);
		g1->SetMarkerSize(2);
		TString xlabel_Npart = "#it{N}_{part}";
		TString ylabel_etOverEtaOverNpartOver2 = "#LTd#it{E}_{T}/d#eta#GT/#LT#it{N}_{part}/2#GT (GeV)";
		g1->GetHistogram()->GetXaxis()-> SetTitle(xlabel_Npart);
		g1->GetHistogram()->GetYaxis()-> SetTitle(ylabel_etOverEtaOverNpartOver2);
		string graphTextNpart = "#sqrt{#it{s}_{NN}} ="+doubToString(collEnArr[enInd])+" GeV";
		const char* graphTextNpartConstCharPtr = graphTextNpart.c_str();// required for TText constructor
		//TText* t1 = new TText(200,.62,graphTextNpartConstCharPtr);
		TLatex* t1= new TLatex();;
		t1 -> SetNDC(kTRUE);
		t1 -> DrawLatex(0.2,0.8,graphTextNpartConstCharPtr);
		
		//t1 -> SetTextAlign(22);
		t1 -> SetTextSize(0.05);
		//t1 -> DrawText();
		// FIXME t1 -> DrawText(0.2,0.8,graphTextNpartConstCharPtr);
		string graphName1 = "dETdEtaOverNpartBy2SumEn" + std::to_string(collEnArr[enInd]);
		string imgPathAndName1 = 
		"./finalPlots/crossCheckPlots/dETdEtaOverNpartBy2_Npart/"+graphName1+".png";
					//c1 -> SaveAs("./fittedPlots/trial1.png");
		TImage *png1 = TImage::Create();//TODO try to use canvas method instead of png object
		png1->FromPad(c1);
		const char* imgPathAndNameConstCharPtr1 = imgPathAndName1.c_str();
		png1->WriteImage(imgPathAndNameConstCharPtr1);
		delete t1;
		delete c1;
		delete png1;
		*/
		
		delete g1;
		
		
	
		// TODO make a function to take care of different types of graph instead of creating new objects here
		TGraph* g2;
		g2 = new TGraph(9, NpartArrEnByEn, dETdEtaOverdNchdEtaSumEnByEn);
		g2 -> SetName("dETdEtaOverdNchdEtaSum_vs_Npart");
		formatGraph(g2, collEnArr, enInd);
		/*
		TCanvas *c2 = new TCanvas();
		//c2->SetLogx();
		g2->Draw("A*");
		g2->SetMarkerStyle(28);
		g2->SetMarkerColor(kGreen);
		g2->SetMarkerSize(2);
		TString xlabel_Npart_2 = "#it{N}_{part}";
		TString ylabel_etOverEtaOverNchOverEta = "#LTd#it{E}_{T}/d#eta#GT/#LTd#it{N}_{ch}/d#eta#GT (GeV)";
		g2->GetHistogram()->GetXaxis()-> SetTitle(xlabel_Npart_2);
		g2->GetHistogram()->GetYaxis()-> SetTitle(ylabel_etOverEtaOverNchOverEta);
		string graphName2 = "dETdEtaOverdNchdEtaSumEn" + std::to_string(collEnArr[enInd]);
		string imgPathAndName2 = 
					"./finalPlots/crossCheckPlots/dETdEtaOverdNchdEta_Npart/"+graphName2+".png";
					//c1 -> SaveAs("./fittedPlots/trial1.png");
		TImage *png2 = TImage::Create();// TODO try to use canvas method instead of png object
		png2->FromPad(c2);
		const char* imgPathAndNameConstCharPtr2 = imgPathAndName2.c_str();
		png2->WriteImage(imgPathAndNameConstCharPtr2);
		delete png2;
		delete c2;
		*/
		delete g2;
		
		
	} // end of for loop with index centInd
	/// ------ end - plot all graphs at once -------------------------------//
	
	return 0;
}

std::string doubToString(Double_t d){
	stringstream stream;
	stream << fixed << setprecision(1) << d;
	return stream.str();

}

void formatGraph(TGraph* g1, Double_t collEnArr[], int enInd){
	// for reference:
		// g1 = new TGraph(9, NpartArrEnByEn, dETdEtaOverNpartBy2SumEnByEn);
	TCanvas *c1 = new TCanvas(/*"c1","A Simple Graph Example",200,10,700,500*/);
	//c1->SetLogx(); Npart graphs not in log scale
	TString xlabel_Npart;
	TString ylabel;
	string imgPathAndName1;
	g1->Draw("A*");
	g1->SetMarkerSize(2);
	TString npart1("dETdEtaOverNpartBy2Sum_vs_Npart"); // name of first kind of
												// graph with Npart in the X-axis
	TString npart2("dETdEtaOverdNchdEtaSum_vs_Npart"); // name of second kind of
												// graph with Npart in the X-axis
	if (g1 -> GetName()==npart1){
		g1->SetMarkerStyle(28);
		g1->SetMarkerColor(kGreen);
		ylabel= "#LTd#it{E}_{T}/d#eta#GT/#LT#it{N}_{part}/2#GT (GeV)";
		string graphName1 = "dETdEtaOverNpartBy2SumEn" + doubToString(collEnArr[enInd]);
		imgPathAndName1 = 
		"./finalPlots/crossCheckPlots/dETdEtaOverNpartBy2_Npart/"+graphName1+".png";
	}
	else if (g1 -> GetName()==npart2){
		g1->SetMarkerStyle(28);
		g1->SetMarkerColor(kRed);
		ylabel= "#LTd#it{E}_{T}/d#eta#GT/#LTd#it{N}_{ch}/d#eta#GT (GeV)";
		string graphName1 = "dETdEtaOverdNchdEtaSumEn" + doubToString(collEnArr[enInd]);
		imgPathAndName1 = 
		"./finalPlots/crossCheckPlots/dETdEtaOverdNchdEta_Npart/"+graphName1+".png";
	}
	xlabel_Npart = "#it{N}_{part}";
	//cout << g1->GetName() << endl;
	
	
	g1->GetHistogram()->GetXaxis()-> SetTitle(xlabel_Npart);
	g1->GetHistogram()->GetYaxis()-> SetTitle(ylabel);
	string graphTextNpart = "#sqrt{#it{s}_{NN}} ="+doubToString(collEnArr[enInd])+" GeV";
	const char* graphTextNpartConstCharPtr = graphTextNpart.c_str();// required for TText constructor
	//TText* t1 = new TText(200,.62,graphTextNpartConstCharPtr);
	TLatex* t1= new TLatex();;
	t1 -> SetNDC(kTRUE);
	t1 -> DrawLatex(0.2,0.8,graphTextNpartConstCharPtr);
	
	//t1 -> SetTextAlign(22);
	t1 -> SetTextSize(0.05);
	//t1 -> DrawText();
	// FIXME t1 -> DrawText(0.2,0.8,graphTextNpartConstCharPtr);
	
				//c1 -> SaveAs("./fittedPlots/trial1.png");
	TImage *png1 = TImage::Create();//TODO try to use canvas method instead of png object
	png1->FromPad(c1);
	const char* imgPathAndNameConstCharPtr1 = imgPathAndName1.c_str();
	png1->WriteImage(imgPathAndNameConstCharPtr1);
	delete t1;
	delete c1;
	delete png1;
}
