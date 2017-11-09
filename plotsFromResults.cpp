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

int plotsFromResults(){

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
	Double_t Npart[cents][collEns];// Npart function of cent and en only
	Double_t dETdEtaOverNpartBy2Sum[cents][collEns];
	// Add elements below (for all 6 particles)
		// in order to get the above elements
	Double_t dETdEta[cents][collEns][parts];
	Double_t dETdEtaErr[cents][collEns][parts];
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
		else if (part == "pi+") partIndex = 0;
		else if (part == "ka-") partIndex = 2;
		else if (part == "ka+") partIndex = 3;
		else if (part == "pro") partIndex = 4;
		else if (part == "pba") partIndex = 5;
		
		in >> dETdEta[centIndex][enIndex][partIndex];
		cout << dETdEta[centIndex][enIndex][partIndex] << "\t";
		in >> dETdEtaErr[centIndex][enIndex][partIndex];
		for(int l=0; l<24; l++) in >> skipContent;
		in >> Npart[centIndex][enIndex];
		cout << Npart[centIndex][enIndex] << "\n";
		in >> skipContent;
		dETdEtaSum[centIndex][enIndex] += dETdEta[centIndex][enIndex][partIndex];
		
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
				dETdEtaOverNpartBy2Sum[i][j] = dETdEtaSum[i][j]/Npart[i][j];
			}
		}
	Double_t collEnArr[5] = {7.7,11.5,19.6,27,39};
	// TODO the following code is only a trial, after whose completion
	 // make a loop to produce all the plots at once
	 // using TGraphErrors
	Double_t dETdEtaOverNpartBy2SumCent0[collEns];
	for(int i=0; i<collEns; i++){
		dETdEtaOverNpartBy2SumCent0[i] = dETdEtaOverNpartBy2Sum[0][i];
	}
	TGraph* g;
	Double_t* collEnPtr = &collEnArr[0];
	Double_t* dETdEtaOverNpartBy2SumCent0Ptr = &dETdEtaOverNpartBy2SumCent0[0];
	g = new TGraph(5, collEnPtr, dETdEtaOverNpartBy2SumCent0Ptr);// TGraph constructor
	TCanvas *c = new TCanvas(/*"c1","A Simple Graph Example",200,10,700,500*/);
	c->SetLogx();
	g->Draw();
	// string imgPathAndName = "./"+histoName+".png";
	string imgPathAndName = "./dETdEtaOverNpartBy2SumCent0.png";
				//c1 -> SaveAs("./fittedPlots/trial1.png");
	TImage *png = TImage::Create();// FIXME try to use canvas method instead of png object
	png->FromPad(c);
	const char* imgPathAndNameConstCharPtr = imgPathAndName.c_str();
	png->WriteImage(imgPathAndNameConstCharPtr);
	//delete g;
	delete png;
	delete c;
	return 0;
}
