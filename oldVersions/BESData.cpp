#include "Riostream.h"
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include "TString.h"
#include "TH1D.h"
#include <iostream> // to use cout for debugging
using namespace std;

/*const char* */ string concatenateHistoname(string,string,string,string);

int BESData(){ // main
	string 				myString;// temporary string to hold ifstream instance	
	string				collidingSpecies;/////////////
	//string 				collidingSpecies;// eg. Au+Au
	//char				collidingSpeciesTrial; // didn't work
	//Float_t 			collisionEnergy;// eg. 39 GeV////////////////////
	//float collisionEnergy;/////////////////
	string 				collisionEnergy;
	string 				collisionEnergyStr;////////////////////
	string 				particleName;// eg. pi+
	Float_t 			myFloat; //temporary float to hold energy value
	Double_t			myDouble; // temporary double to hold values from histo row
	//string 			histoname;
	//char				histoname[30];// needs to be a char to be used in sprintf///////////////
	
	std::vector<TH1D> 	histoList; // vector (list) of histograms
	std::vector<Double_t> binEdgesVec;
	std::vector<Double_t> binContent;
	std::vector<Double_t> binContentErrStat;
	std::vector<Double_t> binContentErrSys;
	ifstream 			in;
	
	in.open(Form("/home/bsharma/rhip/trialCodes/BESData.txt"));
	// ^ data file with Beam Scan Energy data
	TFile* f = new TFile("BESData.root","RECREATE");// .root file to be created
	const char* collidingSpeciesPtr = NULL;
	
/*
nested loop structure:

	while(not eof){
		while

	}
*/	
	//while(!in.eof()){
	Int_t testVar = 0;
	while(in >> collidingSpecies){
		testVar++;
		cout << "test var: "<< testVar << endl;//FIXME: program terminates within a single iteration of this loop
		//in >> collidingSpecies;// initialize
		while(collidingSpecies!="Au+Au") {in >> collidingSpecies;}
		//while(collidingSpeciesTrial!="Au+Au") {in >> collidingSpeciesTrial;}
		// use regex to make it more general in the future
//////////// int *p = &x; means p is a pointer to x's address, where x is an integer
//////////// aka p is a variable that contains the address to x, an integer
//////////// so *p means "contents of p", which is the value of the variable x
//////////// hence, *p = 90 sets x=90
//////////// c_str() returns a pointer to an array that contains a null-terminated
				// sequence of characters representing the current value of the
				// string object
		//collidingSpeciesPtr = &collidingSpeciesTrial;
		//const char* collidingSpeciesPtr = collidingSpecies.c_str();
		cout << "Collision of: "<<collidingSpecies<< endl;//Au+Au etc.
		//in >> myFloat;//////////////
		//collisionEnergy = myFloat;///////////////
		in >> collisionEnergy;
		cout << "at energy: "<< collisionEnergy<< " GeV" << endl;// 7.7 etc.
		string tempString;/////////// flag used in debugging
		for(int i = 0; i<8; i++) in >> myString; // skipping 8 strings
		// single histogram creation:
		for(int cent = 0; cent<3; cent++){// iterate 9X for 9 centralities
			in >> particleName;////////
			//in >> myString;
			if(!in.good()) cout<< "particleName not read correctly"<< endl;
			//const char* particleNamePtr = particleName.c_str();// eg. pbar/////////////////
			cout << "particle: " << particleName/*Ptr*/ << endl; // pi- etc.
			in >> myString;// read & skip string corresponding to centrality
			tempString = myString;//////////// flag
			cout << "Centrality: "<< tempString<< endl; ///////////// no error

			//Bool_t endFound(0); // trigger to mark end of histogram
			//cout << "endFound: "<< endFound << endl;///////////////// no error

			Int_t binNum = 0;
			Double_t tempDouble; /////////// flag used in debugging
			Double_t binHighEdge; // only pushed_back after completion of while loop
									// to avoid redundancy
			Bool_t ignoreBin(false);
			while(in>>myDouble){// get single content from each of the 5 columns
	//FIXME: condition turns false before second iteration of loop
				//in >> myDouble;//////////// no error!!
				//cout << "fstream good if this is a double: "<< myDouble<< endl;
				//if(!in.good()){cout << "!!!! End of histo !!!"<< endl;/*break;}*/} // break while loop if eof reached or histo complete
				binNum++;
				//cout << "bin number: "<< binNum << endl; //////////////
				binEdgesVec.push_back(myDouble);
/////////// FIXME: need loop structure to check if a bin edge is repeated, in which case, ignore the bin
/*				for(int binIter = 1; binIter<binNum; binIter++){
					if (binEdgesVec[binNum]==binEdgesVec[binIter-1]){
						binEdgesVec.pop_back();
						binNum--;
						cout << "Found conflict in elements"<< binNum <<" and "<< binIter-1<< endl; return 0;
						goto label; // skip unnecessary bin
						//ignoreBin;
						//break;
						}
				}
				//if (ignoreBin){
					//break;
				//}
*/
//////////// TODO: go to fixme above////////////////////////////////////////////////////////////////////
				//cout << "low edge for bin "<<binNum<<": " <<binEdgesVec[binNum-1]<< endl;////////////
				in >> binHighEdge;// not pushed_back until the while loop terminates
				//cout << "high edge for bin "<<binNum<<": " <<binHighEdge<< endl;////////////
				//in >> myDouble;
				//binEdgesVec.push_back(myDouble);
				//in >> binEdgesVec[binNum+1];
				in >> myDouble;
				binContent.push_back(myDouble);
				//cout << "content for bin "<<binNum<<": " <<binContent[binNum-1]<< endl;////////////
				in >> myDouble;
				binContentErrStat.push_back(myDouble);
				//cout<<"errStat for bin "<<binNum<<": "<<binContentErrStat[binNum-1]<< endl;////////////
				in >> myDouble;
				binContentErrSys.push_back(myDouble);
				//cout <<"errSys for bin "<<binNum<<": " <<binContentErrSys[binNum-1]<< endl;////////////
						// use TH1D pointers array (vector)?
			}// end of while loop to capture every histo bin
			if (in.fail())cout << "Error! Fail bit!"<< endl; // this is what happens
			else if (in.bad())cout << "Error! Not fail bit but bad bit!"<<endl;
			else cout << "Error! But neither fail bit nor bad bit!"<< endl;
			in.clear(); // to restore input stream in a good state after failbit
			in >> myString;// read end of histo: ---------------- and do nothing
			binEdgesVec.push_back(binHighEdge);
			//cout << "high edge for last bin: "<< binEdgesVec[binNum]<< endl;

//TH1D *cent0_pion_minus__3 = new TH1D("cent0_pion_minus__3","0-5%",52, xAxis2);
// cent_$centrality$_$particleName$_$collidingSpecies$_$collisionEnergy$
			string centString = std::to_string(cent);////////////////////
			cout << "Centrality as string: " << centString << endl;//////////////
			//string collisionEnergyString = std::to_string(collisionEnergy);////////////////////
			/*const char* */ string histoname = concatenateHistoname(centString,particleName,collidingSpecies,collisionEnergy);
			const char* histonameConstCharPtr = histoname.c_str();
			/*sprintf(histoname,
					"cent%i_%s_%s_%f",
					cent,particleNamePtr,collidingSpeciesPtr,collisionEnergy);*/////////////////////////
			
			cout << "Histoname: " << histoname << endl;
///////////// FIXME Runtime error after this point!!!!!!///////////////////////////////////
///////////// source of error: bins are not in increasing order
			//const Double_t binEdgesArr = binEdgesVec[0];// TH1D constructor needs this
			// can't just use the pointer, need to copy the content of vector to array
			//Double_t binEdgesArr[binNum];
			//std::copy(binEdgesVec.begin(),binEdgesVec.end(),binEdgesArr);
			cout << "(sanity check) bin num (26?): "<< binNum<< endl;
			//for(int c = 0; c<binNum; c++){cout<< binEdgesArr[c]<<endl;}return 0;//////////////
			Double_t* a = &binEdgesVec[0];
			TH1D *h = new TH1D(histonameConstCharPtr,/*histoname*/ histonameConstCharPtr, binNum, a/*binEdgesArr*/);
			// ^ here histoname needs to be a const char*
			cout <<"--------------------------------------------"<< endl<< endl;
			for(int j = 1; j<=binNum; j++){
				h->SetBinContent(j,binContent[j]);
				h->SetBinError(j,binContentErrStat[j]+binContentErrSys[j]);
			}
			histoList.push_back(*h);			
			//h[histoNum]->Write();
			//break;
		}// end of for loop to capture each centrality
	}// end of global while loop to capture each collisionEnergy
	in.close();
	f->Write();
	return 0;
}
string concatenateHistoname(string centStr,string pName,string colSp,string colEn){
	string initText = "cent";
	string undScr = "_";//underscore
	string addedString = initText+centStr+undScr+pName+undScr+colSp+undScr+colEn;
	return addedString/*addedString.c_str()*/; //type: const char*: to be done later
}
