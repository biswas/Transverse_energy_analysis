// stack: data created spefically for function automatically deleted when function returns
// heap: data will remain until manually deleted 
/// FIXME: each iteration of the key (histogram loop) consumes
	// an additional ~30 MB of memory!!!
	// even after: 
		// removing Tcanvas class
		// following Rademakers' technique involving declarations ouside the loop
	/// -> try nestedly looping using histogram name
			// instead of key
	/// is it because the fitting and integral functions 
		// implicitly create new objects and don't delete them?
			
			
// TODO: after calculating all the integrals, find the sums...
		// dN_ch/dEta, dN_ch/dyetc, N_part 
// FIXME current debugging location around line 108
#include <iostream>
#include <string>
#include "TKey.h"
#include <fstream>
#include "fitBESData3.h"
using namespace std;
// DEBUG note: taking out ofstream functionality does not prevent memory leak
	// getting rid of TImage object doesn't solve the problem either
	// next: TODO !!!! try making everything modular and see if 
		// transferring varibles from stack to heap fixes the issue
////// FIXME: verify integral values from wolframalpha
			// make functions for dNdEta and dNdy

// forward declarations for methods in fitBESData.h:
Double_t getdNdptOverptIntegrand(Double_t* rad, Double_t* par);
Double_t getdNdpt(Double_t* pT, Double_t* params);
string concatenateHistoname(string,string,string,string);
Double_t* getIntegralsAndErrorsFromData(TH1D*, Double_t, Double_t);
/////Double_t* getIntegralsAndErrorsFromFit(Double_t* myPt, Double_t* par);
Double_t getdNdpt(Double_t* pT, Double_t* params);
Double_t getdETdyIntegrand(Double_t* myPt, Double_t* par);
Double_t getdETdEtaIntegrand(Double_t* myPt, Double_t* par);

// main function:
int fitBESData3Test(){
	std::ofstream datFile ("fitResults3.dat", std::ofstream::out);
	datFile << "ColE" << "\t"
			<< "part" << "\t"
			<< "mass" << "\t"
			<< "beta" <<"\t"
			<< "beta_err" <<"\t"
			<< "temp" <<"\t"
			<< "temp_err"<<"\t"
			<< "n(v-prof.)" <<"\t"
			<< "n_err" <<"\t"
			<< "norm" <<"\t"
			<< "norm_err" <<"\t"
			<< "dETdEta_d" << "\t"
			<< "dETdEta_d_err" << "\t"
			<< "dETdEta_left" << "\t"
			<< "dETdEta_leftErr" << "\t"
			<< "dETdEta_right" << "\t"
			<< "dETdEta_rightErr" << "\n";
	TFile* myFile = new TFile("BESData.root");
	TIter next(myFile->GetListOfKeys());
	TKey* mikey;
	TH1D* h;
	//for(int cent=0;cent<9;cent++){// centrality loop
	TCanvas* c1;
	TClass* class1;
	TF1* funcBGBW;
	TF1* dETdEtaIntegrandFunc;
	TF1* dETdyIntegrandFunc;
	int breakOutForTesting =0;
	int stop =4; // breakOut after this many iterations (if achieved)
	while((mikey=(TKey*)next())){// FIXME!!! delete mikey at the end of every loop
	gROOT->Reset();
		
		class1 = gROOT->GetClass(mikey->GetClassName());
		if(!class1->InheritsFrom("TH1")){ /*FIXME delete all objects in memory before continuing*/
			delete class1;// when out of the if statement, this gives seg fault
			mikey->DeleteBuffer();
			continue;
		}	
		c1 = new TCanvas(); // definition here, declaration outside, a la Rademakers
		funcBGBW = new TF1("getdNdpt",getdNdpt,0.00000000000001,10.,5);
		dETdEtaIntegrandFunc = new TF1("dETdEtaIntegrand", 
									getdETdEtaIntegrand, 
									0, 10, 6 );// function goes from 0 to 10
										// and has 6 parameters"
										// mass, beta, temp, n, norm, type
		dETdyIntegrandFunc = new TF1("dETdyIntegrand",
								  getdETdyIntegrand,
								  0,10,6);
		gPad->SetLogy();
		gStyle->SetOptFit();// display fit parameters; customizable
		gStyle->SetOptDate();// display date (at bottom left)
		gROOT-> SetBatch(kTRUE);// save canvases without displaying them
		c1->Update();
		// read histogram object for current iteration of key:
		//TH1F* h = (TH1F*)mikey->ReadObj();// FIXME! use a method analogous to clear instead of delete coz of how root works
		h = (TH1D*)mikey->ReadObj();
		//h = mikey->ReadObj();
		string histoName = h->GetName();
		//get first three characters of particle name from histoName:
		string particleID = histoName.substr(6,3); 
		//cout << "particle ID: "<<particleID<<endl;
		//return 1;
		
		//------------ Assign mass & type to particle -----------------//
		Double_t mass; // in GeV
		// type Double_t instead of Int_t 
		 //to use as argument in TF1 method SetParameters()
		Double_t type;// 0 for mesons, -1 for baryons, 1 for antibaryons
		if		(particleID=="pi-"||particleID=="pi+")
				{mass = 0.13957; type = 0.;}
		else if	(particleID=="ka-"||particleID=="ka+")
				{mass = 0.49368; type = 0.;}
		else if	(particleID=="pro")
				{mass = 0.93827; type = -1.;}
		else if	(particleID=="pba")
				{mass = 0.93827; type = 1.;}
		else {cout << "Check particle: "
				<< particleID<<endl;return 1;}
		
		
		
		
		
		Double_t* integralPtr;
		// TODO : need to fix what function this should be:
		integralPtr = getIntegralsAndErrorsFromData(h,type,mass);
					// ^ method verified!!!
		for(int i=0; i<8; i++){
			
			cout<<"Int result "<<i+1<<": "<<*(integralPtr+i)<<endl;
		}
		
		
		//------------- Begin BGBW fit --------------------------//
		//FIXME when you delete, use the "C"? option to delete all the inherited objects as well
		if (	histoName == "cent7_ka-_Au+Au_7.7"
			|| 	histoName == "cent7_ka-_Au+Au_11.5"
			||	histoName == "cent7_pi+_Au+Au_7.7"
			||	histoName == "cent8_ka+_Au+Au_7.7"
			||	histoName == "cent4_pi-_Au+Au_19.6"
			||	histoName == "cent5_ka+_Au+Au_27"
			||	histoName == "cent5_ka-_Au+Au_7.7"
			||	histoName == "cent6_pi+_Au+Au_11.5"){
			funcBGBW->SetParameters(0.1,0.9,0.03,0.01,10000.);
			}			
		else{
			funcBGBW->SetParameters(0.1,0.95,0.05,0.1,1000000.);
			}
		funcBGBW->SetParNames("mass","beta (c)","temp","n","norm");
		funcBGBW->SetParLimits(1,0.5,0.999999999999999999999);//param 1

		funcBGBW->FixParameter(0,mass);// mass in GeV
		
		
		
		
		

		h->Fit("getdNdpt","","",0.00000000000001,10.);
		
		
		breakOutForTesting++;
		if(breakOutForTesting>=stop) break;
		continue;
		
		
		
		// FIXME!!!!!! most of the memory has been consumed
		// as soon as the above method is invoked
				
				
				
				
				
				
		// show 130% of max y value in pad, so the curve isn't cropped out:
		//Double_t maxY = 1.3*(h->GetMaximum());
		//h-> GetYaxis()->SetRangeUser(0.,maxY);
		h-> GetXaxis()->SetRangeUser(0.,10.);
		TString xlabel = "p_{T}";
		TString ylabel = "#frac{d^{2}N}{dydp_{T}}";
		h-> SetXTitle(xlabel);
		h-> SetYTitle(ylabel);
		Double_t beta = funcBGBW->GetParameter(1);
		Double_t temp = funcBGBW->GetParameter(2);
		Double_t n	  = funcBGBW->GetParameter(3);
		Double_t norm = funcBGBW->GetParameter(4);
		Double_t betaErr = funcBGBW->GetParError(1);
		Double_t tempErr = funcBGBW->GetParError(2);
		Double_t nErr = funcBGBW->GetParError(3);
		Double_t normErr = funcBGBW->GetParError(4);

		//------------- end BGBW fit ----------------------------
		
		
		//-------- Find integrals left and right of data points -------//
		funcBGBW->SetParameters(mass,beta,temp,n,norm);
// see FIXME 6		
		dETdEtaIntegrandFunc -> SetParameters(mass,beta,temp,n,norm,type);
// see FIXME 6		
		dETdyIntegrandFunc -> SetParameters(mass,beta,temp,n,norm,type);
		//////Double_t SixParamsArr[6] = [mass,beta,temp,n,norm,type];
		Double_t* integrandPtr;
				//// FIXME integrandPtr = getIntegralsAndErrorsFromFit(myPt, 6paramsArr);
				// myPt should be the first element of the pt array
					// FIXME FIXME FIXME how to prepare the parameters array correctly?
					// -> create a TF1 object and pass function getIntegralsAndErrorsFromFit
						//->as a parameter
						// however, that is not robust since this function would have to be
						// called 4 times to calculate all the different integrals
						// to not complicate things, just create four different 
						// TF1 objects for the four different integrals
						// it was not difficult to not do that for the discrete integral
						// though because it did not require the Integral method
							//Assuming the x-axis array is set up not manually but
							// automatically through the knowledge of the first and
							// the last elements of the array
				// second parameter is the pointer to the parameters array
							// ^ method verified!!!
				for(int i=0; i<8; i++){
			
					cout<<"Int result "<<i+1<<": "<<*(integralPtr+i)<<endl;
				}	
		Int_t totBins = h->GetNbinsX();		
		Int_t binx1 = 0;
		Int_t binx2 = totBins+1;
		
		Double_t leftCut = h->GetXaxis()->GetBinLowEdge(binx1+2); 
		Double_t rightCut = h->GetXaxis()->GetBinUpEdge(binx2-1); 
		
// FIXME 6 uncomment below block after testing


		Double_t dETdEtaLeft 	= dETdEtaIntegrandFunc -> Integral(0.,leftCut);
		Double_t dETdEtaRight 	= dETdEtaIntegrandFunc -> Integral(rightCut,10.);
		Double_t dETdyLeft 		= dETdyIntegrandFunc -> Integral(0.,leftCut);
		Double_t dETdyRight 	= dETdyIntegrandFunc -> Integral(rightCut,10.);
		cout<< "binx1: "<< binx1+2 << " content: "<< h->GetBinContent(binx1+2)<< endl;
		// ^ find out why it has to be binx1+2
		cout<< "binx2: "<< binx2-1 << " content: "<< h->GetBinContent(binx2-1)<< endl;
		cout<< "leftcut: "<< leftCut << endl;
		cout<< "rightcut: "<< rightCut << endl;
		cout << "left(eta): "<< dETdEtaLeft<< endl;
		cout << "right(eta):"<< dETdEtaRight<< endl;	
		cout << "left(y): "<< dETdyLeft<< endl;
		cout << "right(y):"<< dETdyRight<< endl;
		cout <<"Integral from data for histo "<<breakOutForTesting+1<<": "<<*(integralPtr+0)<<endl;// 357.633 for pi minus cent 0
		cout<< "Error: "<<*(integralPtr+1)<<endl<<"-----------------------------------"<<endl;				
		//------ end Find integrals left and right of data points ----//
		
		//-- Output results to file-----------------------------
		datFile << 0.0 << "\t"	
		<< particleID << "\t"
		<< mass << "\t"
		<< beta <<"\t"
		<< betaErr <<"\t"
		<< temp <<"\t"
		<< tempErr <<"\t"
		<< n <<"\t"
		<< nErr <<"\t"
		<< norm <<"\t"
		<< normErr <<"\t"
		<< *(integralPtr+0) << "\t" //
		<< *(integralPtr+1)<< "\t" //
		<< dETdEtaLeft << "\t"
		<< "ETlErr" << "\t"
		<< dETdEtaRight << "\t"
		<< "ETrErr" << "\n";
		
		//-- end- output results to file------------------------
		c1->Update();
		Double_t chi2BGBW = funcBGBW->GetChisquare();
		Double_t nDFBGBW = funcBGBW->GetNDF();
		Double_t p2 = funcBGBW->GetParameter(2);
		Double_t e2 = funcBGBW->GetParError(2);
 // FIXME 6 uncomment above block after testing


	
		//cout << "chi2: " << chi2BGBW << "\nndf: " 
			//<< nDFBGBW<< "\nchi2/ndf: " << chi2BGBW/nDFBGBW <<endl;
	
		/* FIXME 
		string imgPathAndName = "./fittedPlots3/"+histoName+".png";
				//c1 -> SaveAs("./fittedPlots/trial1.png");
		TImage *png = TImage::Create();// FIXME try to use canvas method instead of png object
		png->FromPad(c1);
		const char* imgPathAndNameConstCharPtr = imgPathAndName.c_str();
		png->WriteImage(imgPathAndNameConstCharPtr);
		*/
		//cout << "Draw class here: \n";
		//h-> DrawClass();
		/////h->Delete();// works but does not free memory!
		///////////FIXME c1->Clear();// 
		/// sometimes when you delete objects, they stay in the program stack
		//FIXME delete png;
		mikey->DeleteBuffer();// works!
		
		h->BufferEmpty(1);
		gSystem->ProcessEvents();
		delete h;
		delete funcBGBW;
		delete dETdEtaIntegrandFunc;
		delete dETdyIntegrandFunc;
		delete c1;	// Rademakers
		gObjectTable->Print();
		//delete mikey; // FIXME 9 segmentation violation
		//delete class1; // segmentation violation
	}
	//}// end of while loop to iterate through every key
	/////////////delete c1;
	//delete mikey;
	//delete h;
	//delete class1;
	//delete funcBGBW;
	//delete dETdEtaIntegrandFunc;
	//delete dETdyIntegrandFunc;
	gObjectTable->Print();
	delete myFile;
	datFile.close();
	gDirectory->GetList()->Delete();
return 0;
}


