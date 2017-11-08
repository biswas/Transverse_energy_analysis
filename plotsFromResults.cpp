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

// variables to calculate:
// 1. dETdEtaOverNpartBy2_cent0, etc.
// 2. collEn_cent0, etc.
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

	

	return 0;
}
