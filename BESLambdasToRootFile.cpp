// modification of BESDataToRootFile.cpp

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

/*const char* */ string concatenategraphname(string,string,string,string);

int BESLambdasToRootFile(){ // main
	string 				myString;// temporary string to hold ifstream instance	
	string				collidingSpecies;/////////////
	string 				collisionEnergy;
	string 				collisionEnergyStr;////////////////////
	string 				particleName;// eg. pi+
	Float_t 			myFloat; //temporary float to hold energy value
	Double_t			myDouble; // temporary double to hold values from histo row
	Int_t				graphnum = 0; // track number of histos created/parsed

	ifstream 			in;
	
	in.open(Form("/home/bsharma/rhip/analysisCodes/BES_lambdas.txt"));
	// ^ data file with BES strangeness data (in this case lambdas)
	TFile* f = new TFile("BESLambdas.root","RECREATE");// .root file to be created
	//const char* collidingSpeciesPtr = NULL;
	
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
			in >> myString;// read & skip string corresponding to centrality
			Int_t xNum = 0;
			Double_t tempDouble; /////////// flag used in debugging
			////////......Double_t binHighEdge; // only pushed_back after completion of while loop
									// to avoid redundancy
			/////// FIXME xVec.push_back(0.0);// to set the left-most edge to zero
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

// cent$centrality$_$particleName$_$collidingSpecies$_$collisionEnergy$// graphname format
			string graphname = concatenategraphname(centString,particleName,collidingSpecies,collisionEnergy);
			// TGraphErrors name argument in the Write() method should be const char*, not string:
			const char* graphnameConstCharPtr = graphname.c_str();// convert string to const char*
			graphnum++;// just to track
			cout << "graphname: " << graphname << ", graphnum: "<< graphnum<< endl;
			//for(int c = 0; c<xNum; c++){cout<< binEdgesArr[c]<<endl;}return 0;//////////////
			// fourth arg in TH1D constructor is a pointer 
			//to list (of bin edges), in this case a vector:
			////////........Double_t* a = &xVec[0];
			/////////......TH1D *h = new TH1D(graphnameConstCharPtr,graphnameConstCharPtr, xNum, a);
			// ^ here graphname needs to be a const char*
			
			// TGraphErrors constructor (6/9 from documentation): // did not work 
			/*TVectorD<Double_t> xTVec(xVec.size(), &xVec[0]);
			TVectorD<Double_t> yTVec(yVec.size(), &yVec[0]);
			TVectorD<Double_t> yTVecErrStat(yVecErrStat.size(), &yVecErrStat[0]);
			TGraphErrors *tg = new TGraphErrors(xTVec, yTVec, NULL, yTVecErrStat);
			*/
			// TGraphErrors constructor (4/9 from documentation):
			TGraphErrors *tg = new TGraphErrors(xNum, &xVec[0], &yVec[0], NULL, &yVecErrStat[0]);
			// need to explicitly write TGraphErrors object to TFile:
			tg -> Draw("AP"); // default is "ALP" that gives connecting lines between points
			tg -> SetTitle(graphnameConstCharPtr);
			tg -> Write(graphnameConstCharPtr);
			// check if pointers for two histos are not different:
			cout << "gr. pointer " << graphnum <<":" << tg << endl;
			////////////........cout << "arr. pointer " << graphnum <<":" << /*a*/&xVec[0] << endl;
			//////////.....h->GetXaxis()->SetRangeUser(0.,10.);
			cout << "point num: " << xNum << endl;
			/*//////............
			for(int j = 1; j<=xNum; j++){// fill in bin content and error for every bin
				// Y-axis of data transformed to match BGBW fit y-axis:
				h->SetyVec(j,
							yVec[j-1]);//times2*TMath::Pi());//times (xVec[j]+xVec[j+1])/2);
				// ^ (data y-value multiplied by 2pi// not:*pt; pt = ptLow+0.5 of binWidth)
				
				// add errors in quadrature:
				h->SetBinError(j,TMath::Sqrt(yVecErrStat[j-1]*yVecErrStat[j-1]
								+yVecErrSys[j-1]*yVecErrSys[j-1]));
				
			}
			//////////........ */
			cout <<"--------------------------------------------"<< endl<< endl;
		}// end of for loop to capture each centrality
	}// end of global while loop to capture each collisionEnergy
	in.close();
	/////////.........f->Write(); // writes objects in memory (TH1D objects in this case) to TFile
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
