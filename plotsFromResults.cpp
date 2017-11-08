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
		if (i==0) cout << "Col. 1: " << skipContent << endl;
		else if (i==45) cout << "Col. 45: " << skipContent << endl;
	}
	Double_t collEn;
	Double_t cent;// centrality value obtained from data file
	string part; // particle name
	Double_t dETdEta;
	Double_t dETdEtaErr;
	Double_t Npart;
	// enPlotArr element indexed as: 
	// enPlotArr[centIndex][enIndex][funcIndex]
	int centIndex;
	int enIndex;
	int funcIndex;
	for(int j=0; j<270; j++){// for 270 rows in the data file
		//for(int i=0; i<46; i++){
		in >> collEn; cout << collEn << "\t";
		in >> part; cout << part << "\t";
		in >> cent; cout << cent << "\t";
		for(int k=0; k<15;k++){// skip content in next 15 columns
		in >> skipContent;
		}
		in >> dETdEta;cout << dETdEta << "\t";
		in >> dETdEtaErr;
		for(int l=0; l<24; l++) in >> skipContent;
		in >> Npart; cout << Npart << "\n";
		in >> skipContent;
		
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
		
		
		/*
		if (i==0) cout << headerBuffer << "\t";
		else if (i==1) cout << headerBuffer << "\t";
		if (i==44) cout << headerBuffer << "\t";
		else if (i==45)cout << headerBuffer << "\n";
		*/
		//}//end of for loop with index i
	} //end of for loop with index j
	// create 5by3 (3 cols, 5 rows) array of Doubles:
	//Double_t colEnPlotsArr[5][3];
	
	in.close();

	return 0;
}
