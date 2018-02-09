// modification of BESLambdasToRootFile.cpp

#include "Riostream.h"
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include "TString.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TVectorDfwd.h"
#include <iostream> // to use cout for debugging
using namespace std;

string concatenategraphname(string,string,string,string);
vector<Double_t> xValsToBinEdges(const vector<Double_t> &xV);

int BESLambdasToRootFile_TH1(){ // main
	string 				myString;// temporary string to hold ifstream instance	
	string				collidingSpecies;/////////////
	string 				collisionEnergy;
	string 				collisionEnergyStr;////////////////////
	string 				particleName;// eg. pi+
	Float_t 			myFloat; //temporary float to hold energy value
	Double_t			myDouble; // temporary double to hold values from histo row
	Int_t				graphnum = 0; // track number of histos created/parsed

	ifstream 			in;
	
	in.open(Form("./BES_lambdas.txt"));
	// ^ data file with BES strangeness data (in this case lambdas)
	TFile* f = new TFile("BESLambdas_TH1.root","RECREATE");// .root file to be created
	
	while(in >> collidingSpecies){
		while(collidingSpecies!="Au+Au") {in >> collidingSpecies;}
		in >> collisionEnergy;
		for(int i = 0; i<5; i++) in >> myString; // skipping 5 strings
		// single graph creation:
		for(int cent = 0; cent<7; cent++){// iterate 7X for 7 centralities
			std::vector<Double_t> xVec;
			std::vector<Double_t> yVec;
			std::vector<Double_t> yVecErrStat;
			//////..........std::vector<Double_t> yVecErrSys;
			
			in >> particleName;
			if(!in.good()) cout<< "particleName not read correctly"<< endl;
			else cout << particleName << endl;
			in >> myString;// read & skip string corresponding to centrality
			Int_t xNum = 0;
			Double_t tempDouble; /////////// flag used in debugging
			////////......Double_t binHighEdge; // only pushed_back after completion of while loop
									// to avoid redundancy
			while(in>>myDouble){// get single content from each of the 3 columns, and repeat
									// until in.fail(), i,e. fail bit encountered
								// fstream malfunction if input stream statement within loop body
									// instead of in loop condition
				xNum++;// first iteration gets first point, etc.
				xVec.push_back(myDouble);
				///////.........in >> binHighEdge;// not pushed_back until the while loop terminates
				in >> myDouble;
				yVec.push_back(myDouble);
				in >> myDouble;
				yVecErrStat.push_back(myDouble);
				////////.......in >> myDouble;
				//////////.......yVecErrSys.push_back(myDouble);
				cout << "bin: " << xNum << " " << xVec[xNum-1] << " " << yVec[xNum-1] << endl;
			}// end of while loop to capture every histo bin
			//TODO:
			/*if (in.fail())cout << "Error! Fail bit!"<< endl; // this is what happens
			else if (in.bad())cout << "Error! Not fail bit but bad bit!"<<endl;
			else cout << "Error! But neither fail bit nor bad bit!"<< endl;*/
			in.clear(); // to restore input stream in a good state after failbit
			in >> myString;// read end of histo: ---------------- and do nothing
			//////////........xVec.push_back(binHighEdge);
			string centString = std::to_string(cent);////////////////////
			/*
			// declare and define pointer to array holding bin edges:
			Double_t* binEdgesPtr;
			binEdgesPtr = xValsToBinEdges(xVec);
			*/
// cent$centrality$_$particleName$_$collidingSpecies$_$collisionEnergy$// graphname format
			string graphname = concatenategraphname(centString,particleName,collidingSpecies,collisionEnergy);
			// TGraphErrors name argument in the Write() method should be const char*, not string:
			const char* graphnameConstCharPtr = graphname.c_str();// convert string to const char*
			graphnum++;// just to track
			cout << "histoname: " << graphname << ", histonum: "<< graphnum<< endl;
			//for(int c = 0; c<xNum; c++){cout<< binEdgesVec[c]<<endl;}return 0;//////////////
			// fourth arg in TH1D constructor is a pointer 
			//to list (of bin edges), in this case a vector:
			////////........Double_t* a = &xVec[0];
			std::vector<Double_t> binEdgesVector = xValsToBinEdges(xVec);
			cout << "line 92" << endl;
			//Double_t* a = &binEdgesVector[0];
			TH1D *h = new TH1D(graphnameConstCharPtr,graphnameConstCharPtr, xNum, &binEdgesVector[0]);
			// ^ here graphname needs to be a const char*
			
			// check if pointers for two histos are not different:
			cout << "his. pointer " << graphnum <<":" << h << endl;
			cout << "arr. pointer " << graphnum <<":" << &binEdgesVector[0] << endl;
			////////////........cout << "arr. pointer " << graphnum <<":" << /*a*/&xVec[0] << endl;
			//////////.....h->GetXaxis()->SetRangeUser(0.,10.);

			for(int j = 1; j<=xNum; j++){// fill in bin content and error for every bin
				// Y-axis of data transformed to match BGBW fit y-axis:
				h->SetBinContent(j,
							yVec[j-1]);
				
				// only statistical uncertainty available:
				h->SetBinError(j,yVecErrStat[j-1]);
			}

			cout <<"--------------------------------------------"<< endl<< endl;
		}// end of for loop to capture each centrality
	}// end of global while loop to capture each collisionEnergy
	in.close();
	f->Write(); // writes objects in memory (TH1D objects in this case) to TFile
	delete f; // instead of f->Close(), in order to automatically delete all objects
				// owned by this file
	return 0;
}
string concatenategraphname(string centStr,string pName,string colSp,string colEn){
	string initText = "cent";
	string undScr = "_";//underscore
	string addedString = initText+centStr+undScr+pName+undScr+colSp+undScr+colEn;
	return addedString; //type: const char*: to be done later
}

// takes constant reference to vector with...
// x-values as an argument and...
// returns pointer to array with bin edges
vector<Double_t> xValsToBinEdges(const vector<Double_t> &xV){
	cout << "function has been called" << endl;
	const int N = xV.size(); // number of bins equal to number of data points
	cout << "number of bins: " << N << endl;
	std::vector<Double_t> binEdgesVec(N+1, 0.);

	// postulate that the width of the second last bin is equal to the...
	// distance between the second-last and the third-last x-values: 
	Double_t width_secondLastBin = xV[N-2] - xV[N-3];
	cout << "3rd-last point: " << xV[N-3] << " 2nd-last point: " << xV[N-2] << endl;
	// ^ because the last element is xV[N-1];
	
	// declare/initialize variables to use in recursion:
	Double_t wRight = width_secondLastBin;// width of bin to the right
	cout << "width_secondLastBin: " << wRight << endl;
	Double_t wCurrent = 0.; // width of bin corresponding to the current index
	for(int i = N-2; i >= 1; --i) // iterate from the 3rd-last bin to the first bin
	{
		cout << "working on bin: " << i << " centered at: "<< xV[i] <<endl;
		wCurrent 			= ((xV[i] - xV[i-1]) - 0.5 *wRight)*2;
		cout << "wCurrent: " << wCurrent << endl;
		binEdgesVec[i-1] = xV[i-1] - 0.5 * wCurrent;
		cout << "binEdgesVec[" << i-1 << "]: " << binEdgesVec[i-1] << endl;
		wRight 				= wCurrent; //for next iteration
	}
	binEdgesVec[N-2]	= binEdgesVec[N-3] + (xV[N-3] - binEdgesVec[N-3])*2;
	binEdgesVec[N-1]	= binEdgesVec[N-2] + width_secondLastBin;
	binEdgesVec[N]		= xV[N-1] + (xV[N-1] - binEdgesVec[N-1]);
	cout << "right edge of last bin: " << binEdgesVec[N] << endl;
	cout << "vector of bin edges: " << endl;
	for(i = 0; i <= N+1; i++)
	{
		cout << binEdgesVec[i] << endl;
	}
	cout << "*********************" << endl;
return binEdgesVec;
}
