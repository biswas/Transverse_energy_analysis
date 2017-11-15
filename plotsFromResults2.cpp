/* in this version, got rid of functionality to produce
	a single trial graph and replaced it with the functionality
	to produce many graphs at the same time;
	and added other functionalities
*/

// cross check plots:
	// a) 4 energy-dependent plots * 9 centrality bins 	= 36 plots
	// b) 4 N_part-dependent plots * 5 energies			= 20 plots
// new plots:
	// c) one for each species: 36 plots*3 species		= 108 plots
	
// a) : {(dET/dEta)/(N_part/2),(dET/dEta)/(dN_ch/dEta),(dET/dy)/(N_part/2),(dET/y)/(dN_ch/dEta)} w.r.t collEn
//		X
//		{0,1,2,3,4,5,6,7,8}
// b) : {(dET/dEta)/(N_part/2),(dET/dEta)/(dN_ch/dEta),(dET/dy)/(N_part/2),(dET/y)/(dN_ch/dEta)} w.r.t N_part
//		X
//		{7.7,11.5,19.6,27,39}


//______________________________________________________________

// Read fitResults5.dat
	// manipulate the data according to what plots are needed
// write new table in format suitable to easily create required plots

// content to be created:

/*
1. 5by3 matrix for a particular centrality, say 0:
	1.1 column with energy values, i.e., 5 rows
	1.2	corresponding to each energy, add the 
		values of (dET/dEta) and divide the sum by (Npart/2)
		This quotient goes into the corresponding row in the
		second column
	1.3 corresponding to each energy, add the 
		values of (dET/dEta) and divide the sum by the sum of
		dN/dEta
		This quotient goes into the corresponding row in the
		third column

*/


#include "Riostream.h"
//#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include "TString.h"
#include "TMath.h"
#include "TH1D.h"
#include <iostream> // to use cout for debugging
using namespace std;

// main function:

int plotsFromResults2(){

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
	} //end of for loop with index j
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
	Double_t collEnArr[5] = {7.7,11.5,19.6,27,39};	
	
	/// ------ begin - plot all graphs at once -------------------------------//
	for(int centInd=0; centInd<cents; centInd++){ // loop through all centralities
	
		Double_t dETdEtaOverNpartBy2SumCentByCent[collEns];// one centrality per centInd
		Double_t dETdEtaOverdNchdEtaSumCentByCent[collEns];
		for(int i=0; i<collEns; i++){
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
		TString ylabel_etOverEtaOverNpartOver2_2 = "#LTd#it{E}_{T}/d#eta#GT/#LTd#it{N}_{ch}/d#eta#GT (GeV)";
		g2->GetHistogram()->GetXaxis()-> SetTitle(xlabel_snn_2);
		g2->GetHistogram()->GetYaxis()-> SetTitle(ylabel_etOverEtaOverNpartOver2_2);
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
	/// ------ begin - plot all graphs at once -------------------------------//
	//delete g;
	
	return 0;
}
