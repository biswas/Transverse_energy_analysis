// use covariance matrix obtained from the fit: see TH1 class reference
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
			
// TODO:// check if dNdyIntegrandFunc is equivalent to funcBGBW in terms of integration 
		//after calculating all the integrals, find the sums...
			// dN_ch/dEta, dN_ch/dyetc, N_part 
// FIXME current debugging location around line 108
#include <iostream>
#include <string>
#include "TKey.h"
#include <fstream>
#include "fitBESData4_oneByOne.h"
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
Double_t getdETdEtaIntegrand(Double_t* myPt, Double_t* par);
Double_t getdETdyIntegrand(Double_t* myPt, Double_t* par);
Double_t getdNdEtaIntegrand(Double_t* myPt, Double_t* par);
Double_t getdNdyIntegrand(Double_t* myPt, Double_t* par);

// main function:
int fitBESData4_oneByOne(){
	int breakOutForTesting =0;
	// now debugging: 18
	int stop = 17;// histo num to test: 1 for histo #1
	
	std::ofstream datFile ("fitResults4_oneByOne.dat", std::ofstream::out);
	datFile << "CollEn"<< "\t"	
			<< "particle" << "\t"
			<< "mass" << "\t"
			<< "beta" <<"\t"
			<< "betaErr" <<"\t"
			<< "temp" <<"\t"
			<< "tempErr" <<"\t"
			<< "n-v_prof" <<"\t"
			<< "nErr" <<"\t"
			<< "norm" <<"\t"
			<< "normErr" <<"\t"
			<< "dETdEta_data" << "\t" //dETdEta_d
			<< "dETdEta_d_err"<< "\t" //dETdEta_d_err
			<< "dETdEtaLeft" << "\t"
			<< "dETdEtaLErr" << "\t"
			<< "dETdEtaRight" << "\t"
			<< "dETdEtaRErr" << "\t"
			<< "dETdEtaTotal" << "\t" // dETdEtaTotal
			<< "dETdEtaTErr" << "\t"
			<< "dETdy_d" << "\t" //dETdy_d
			<< "dETdy_d_err"<< "\t" //dETdy_d_err
			<< "dETdyLeft" << "\t"
			<< "dETdyLErr" << "\t"
			<< "dETdyRight" << "\t"
			<< "dETdyRErr" << "\t"
			<< "dETdyTotal" << "\t" // dETdyTotal
			<< "dETdyTErr" << "\t"
			<< "dNdEta_d" << "\t" //dNdEta_d
			<< "dNdEta_d_err"<< "\t" //dNdEta_d_err
			<< "dNdEtaLeft" << "\t"
			<< "dNdEtaLErr" << "\t"
			<< "dNdEtaRight" << "\t"
			<< "dNdEtaRErr" << "\t"
			<< "dNdEtaTotal" << "\t" // dNdEtaTotal
			<< "dNdEtaTErr" << "\t"
			<< "dNdy_d" << "\t" //dNdy_d
			<< "dNdy_d_err"<< "\t" //dNdy_d_err
			<< "dNdyLeft" << "\t"
			<< "dNdyLErr" << "\t"
			<< "dNdyRight" << "\t"
			<< "dNdyRErr" << "\t"
			<< "dNdyTotal" << "\t" // dNdyTotal
			<< "dNdyTErr" << "\n";
	TFile* myFile = new TFile("BESData.root");
	TIter next(myFile->GetListOfKeys());
	TKey* mikey;
	TH1D* h;
	//for(int cent=0;cent<9;cent++){// centrality loop
	TCanvas* c1;
	TClass* class1;
	TF1* funcBGBW;
	TF1* funcBGBW_top;
	TF1* funcBGBW_bottom;
	TF1* dETdEtaIntegrandFunc;
	TF1* dETdEtaIntegrandFunc_top;
	TF1* dETdEtaIntegrandFunc_bottom;
	TF1* dETdyIntegrandFunc;
	TF1* dNdEtaIntegrandFunc;
	TF1* dNdyIntegrandFunc;
	while((mikey=(TKey*)next())){// FIXME!!! delete mikey at the end of every loop
		breakOutForTesting++;
		if (breakOutForTesting<stop) continue;
		class1 = gROOT->GetClass(mikey->GetClassName());
		if(!class1->InheritsFrom("TH1")){
			delete class1;
			mikey->DeleteBuffer();
			continue;
		}
			
		c1 = new TCanvas(); // a la Rademakers
		funcBGBW = new TF1("getdNdpt",getdNdpt,0.00000000000001,10.,5);
		funcBGBW_top = new TF1("getdNdpt_top",getdNdpt,0.00000000000001,10.,5);
		funcBGBW_bottom = new TF1("getdNdpt_bottom",getdNdpt,0.00000000000001,10.,5);
		
		dETdEtaIntegrandFunc = new TF1("dETdEtaIntegrand", 
									getdETdEtaIntegrand, 
									0, 10, 6 );// function goes from 0 to 10
										// and has 6 parameters"
										// mass, beta, temp, n, norm, type
		dETdEtaIntegrandFunc_top = new TF1("dETdEtaIntegrand_top", 
									getdETdEtaIntegrand, 
									0, 10, 6 );
		dETdEtaIntegrandFunc_bottom = new TF1("dETdEtaIntegrand_bottom", 
									getdETdEtaIntegrand, 
									0, 10, 6 );
		dETdyIntegrandFunc = new TF1("dETdyIntegrand",
								  getdETdyIntegrand,
								  0,10,6);
		dNdEtaIntegrandFunc = new TF1("dETdyIntegrand",
								  getdNdEtaIntegrand,
								  0,10,5); // 5 parameters:m,b,t,n,norm
		dNdyIntegrandFunc = new TF1("dETdyIntegrand",
								  getdNdEtaIntegrand,
								  0,10,5);
		gPad->SetLogy();
		gStyle->SetOptFit(1111);// display fit parameters; customizable
		gStyle->SetOptDate();// display date (at bottom left)
		gROOT-> SetBatch(kTRUE);// save canvases without displaying them
		c1->Update();

		// read histogram object for current iteration of key:
		//TH1F* h = (TH1F*)mikey->ReadObj();// FIXME! use a method analogous to clear instead of delete coz of how root works
		h = (TH1D*)mikey->ReadObj();
		//h = mikey->ReadObj();
		string histoName = h->GetName();
		Double_t collEn = 0.;
		//cent8_ka+_Au+Au_7.7
		if(histoName.substr( histoName.length() - 4 ) == "_7.7") collEn = 7.7;
		else if(histoName.substr( histoName.length() - 4 ) == "11.5") collEn = 11.5;
		else if(histoName.substr( histoName.length() - 4 ) == "19.6") collEn = 19.6;
		else if(histoName.substr( histoName.length() - 4 ) == "u_27") collEn = 27;
		else if(histoName.substr( histoName.length() - 4 ) == "u_39") collEn = 39;
		//get first three characters of particle name from histoName:
		string particleID = histoName.substr(6,3); 
		//cout << "particle ID: "<<particleID<<endl;
		//return 1;
		
		//------------ Assign mass & type to particle -----------------//
		Double_t mass; // in GeV
		//Double_t mass_top; // parameter for data+error
		//Double_T mass_bottom; // parameter for data-error 
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
		
		Double_t* integralDataPtr;
		// TODO : need to fix what function this should be:
		integralDataPtr = getIntegralsAndErrorsFromData(h,type,mass);
					// ^ method verified!!!
		
		
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
		TFitResultPtr r = h->Fit("getdNdpt","S","",0.00000000000001,10.);
		//Double_t chi2Prob = funcBGBW->GetProb();
		///gStyle->SetOptFit(1111);
		//cout << "chi-sq prob: " << chi2Prob << endl;
		// show 130% of max y value in pad, so the curve isn't cropped out:
		//Double_t maxY = 1.3*(h->GetMaximum());
		//h-> GetYaxis()->SetRangeUser(0.,maxY);
		//TMatrixDSym cov = r->GetCovarianceMatrix();
		h->SetMaximum(5*(h->GetMaximum()));
		h-> GetXaxis()->SetRangeUser(0.,10.);
		TString xlabel = "p_{T}";
		TString ylabel = "#frac{d^{2}N}{dydp_{T}}";
		h-> SetXTitle(xlabel);
		h-> SetYTitle(ylabel);
		Double_t beta 			= funcBGBW->GetParameter(1);
		
		//Double_t beta_bottom 	= funcBGBW_bottom->GetParameter(1);
		Double_t temp 			= funcBGBW->GetParameter(2);
		
		//Double_t temp_bottom 	= funcBGBW_bottom->GetParameter(2);
		Double_t n	  			= funcBGBW->GetParameter(3);

		//Double_t n_bottom	  	= funcBGBW_bottom->GetParameter(3);
		Double_t norm 			= funcBGBW->GetParameter(4);
		
		//Double_t norm_bottom 	= funcBGBW_bottom->GetParameter(4);
		Double_t betaErr 		= funcBGBW->GetParError(1);
		//Double_t betaErr_top 	= funcBGBW_top->GetParError(1);
		//Double_t betaErr_bottom = funcBGBW_bottom ->GetParError(1);
		Double_t tempErr 		= funcBGBW->GetParError(2);
		//Double_t tempErr_top 	= funcBGBW_top->GetParError(2);
		//Double_t tempErr_bottom = funcBGBW_bottom ->GetParError(2);
		Double_t nErr 			= funcBGBW->GetParError(3);
		//Double_t nErr_top 		= funcBGBW_top->GetParError(3);
		//Double_t nErr_bottom  	= funcBGBW_bottom ->GetParError(3);
		Double_t normErr 		= funcBGBW->GetParError(4);
		//Double_t normErr_top 	= funcBGBW_top->GetParError(4);
		//Double_t normErr_bottom = funcBGBW_bottom ->GetParError(4);
		
		Double_t beta_top 		= beta+betaErr;
		Double_t temp_top 		= temp+tempErr;
		Double_t n_top	  		= n+nErr;
		Double_t norm_top 		= norm+normErr;
		Double_t beta_bottom 	= beta-betaErr;
		Double_t temp_bottom 	= temp-tempErr;
		Double_t n_bottom	  	= n-nErr;
		Double_t norm_bottom 	= norm-normErr;
		//------------- end BGBW fit ----------------------------
		
		
		//-------- Find integrals left and right of data points -------//
		funcBGBW->SetParameters(mass,beta,temp,n,norm);
		funcBGBW_top->SetParameters(mass,beta_top,temp_top,n_top,norm_top);
		funcBGBW_top->Draw("SAME");
		funcBGBW_top->SetLineColor(4);
		funcBGBW_top->SetLineStyle(2);
		funcBGBW_bottom->SetParameters(mass,beta_bottom,temp_bottom,n_bottom,norm_bottom);
		funcBGBW_bottom->Draw("SAME");
		funcBGBW_bottom->SetLineColor(4);
		funcBGBW_bottom->SetLineStyle(2);
		//gStyle->SetLineStyle(2); this changed the stats frame
		dETdEtaIntegrandFunc -> SetParameters(mass,beta,temp,n,norm,type);
		dETdEtaIntegrandFunc->FixParameter(5,type);
		dETdEtaIntegrandFunc->FixParameter(0,mass);
		dETdEtaIntegrandFunc_top -> SetParameters(mass,beta+betaErr,temp+tempErr,n+nErr,norm+normErr,type);
		dETdEtaIntegrandFunc_top->FixParameter(5,type);
		dETdEtaIntegrandFunc_top->FixParameter(0,mass);
		dETdEtaIntegrandFunc_bottom -> SetParameters(mass,beta-betaErr,temp-tempErr,n-nErr,norm-normErr,type);
		dETdEtaIntegrandFunc_bottom->FixParameter(5,type);
		dETdEtaIntegrandFunc_bottom->FixParameter(0,mass);
		dETdyIntegrandFunc -> SetParameters(mass,beta,temp,n,norm,type);
		dETdyIntegrandFunc->FixParameter(5,type);
		dETdyIntegrandFunc->FixParameter(0,mass);
		dNdEtaIntegrandFunc -> SetParameters(mass,beta,temp,n,norm);
		dNdEtaIntegrandFunc->FixParameter(0,mass);
		dNdyIntegrandFunc -> SetParameters(mass,beta,temp,n,norm);
		dNdyIntegrandFunc->FixParameter(0,mass);
		//FIXME ----- Add to fitBESData4_2.cpp ---///// 

		Int_t totBins = h->GetNbinsX();		
		Int_t binx1 = 0;
		Int_t binx2 = totBins+1;
		
		Double_t leftCut = h->GetXaxis()->GetBinLowEdge(binx1+2); 
		Double_t rightCut = h->GetXaxis()->GetBinUpEdge(binx2-1); 
		
		Double_t dETdEtaLeft 	= dETdEtaIntegrandFunc -> Integral(0.,leftCut);
		Double_t dETdEtaLeft_top 	= dETdEtaIntegrandFunc_top -> Integral(0.,leftCut);
		Double_t dETdEtaLeft_bottom 	= dETdEtaIntegrandFunc_bottom -> Integral(0.,leftCut);
		Double_t dETdEtaRight 	= dETdEtaIntegrandFunc -> Integral(rightCut,10.);
		Double_t dETdyLeft 		= dETdyIntegrandFunc -> Integral(0.,leftCut);
		Double_t dETdyRight 	= dETdyIntegrandFunc -> Integral(rightCut,10.);
		Double_t dNdEtaLeft 	= dNdEtaIntegrandFunc -> Integral(0.,leftCut);
		Double_t dNdEtaRight 	= dNdEtaIntegrandFunc -> Integral(rightCut,10.);
		Double_t dNdyLeft 		= dNdyIntegrandFunc -> Integral(0.,leftCut);
		Double_t dNdyRight 		= dNdyIntegrandFunc -> Integral(rightCut,10.);
		// Errors:
		cout << "Number of parameters: " << dETdEtaIntegrandFunc->GetNpar()<<endl;
		Double_t dETdEtaLErr	= 
		dETdEtaIntegrandFunc->IntegralError(0.00000000001,leftCut,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
		
		//TMatrixD matrix0(6,6);
		//gMinuit->mnemat(matrix0.GetMatrixArray(),6);
		cout << "Cov. Matrix without trick: " << endl;
		r->Print("V");
		//matrix0.Print();
		Double_t dETdEtaLErr2 = dETdEtaLeft_top-dETdEtaLeft_bottom;
		// --- Begin: debug -nan for histo 18 ---- //
		cout << "leftCut: " << leftCut << endl;
		cout << "Parameters: "<< endl;
		/*for(int p=1; p<=5; p++){
			cout << r->GetParam(p-1)
					<< "\t";
		}*/
		/*
		cout << endl;
		cout << "Cov. matrix array: \n";
		for(int covEl=1; covEl<=25; covEl++){
			cout << *(r->GetCovarianceMatrix().GetMatrixArray()+covEl-1)
					<< "\t";
			if (covEl%5==0) cout << endl;
		}
		*/
		// ----- End: debug -nan for histo 18 ---- //
		
		Double_t dETdEtaRErr	= 
		dETdEtaIntegrandFunc->IntegralError(rightCut,10.,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
		Double_t dETdyLErr	= 
		dETdyIntegrandFunc->IntegralError(0.,leftCut,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
		Double_t dETdyRErr	= 
		dETdyIntegrandFunc->IntegralError(rightCut,10.,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
		Double_t dNdEtaLErr	= 
		dNdEtaIntegrandFunc->IntegralError(0.,leftCut,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
		Double_t dNdEtaRErr	= 
		dETdEtaIntegrandFunc->IntegralError(rightCut,10.,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
		Double_t dNdyLErr	= 
		dNdyIntegrandFunc->IntegralError(0.,leftCut,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
		Double_t dNdyRErr	= 
		dNdyIntegrandFunc->IntegralError(rightCut,10.,
								r->GetParams(),
								r->GetCovarianceMatrix().GetMatrixArray());
								
		Double_t dETdEta_d = *(integralDataPtr+0);
		Double_t dETdEta_d_err = *(integralDataPtr+1);
		Double_t dETdEtaTotal = dETdEtaLeft+dETdEta_d+dETdEtaRight;
		Double_t dETdEtaTErr = dETdEtaLErr+dETdEta_d_err+dETdEtaRErr;
		
		Double_t dETdy_d = *(integralDataPtr+2);
		Double_t dETdy_d_err = *(integralDataPtr+3);
		Double_t dETdyTotal = dETdyLeft+dETdy_d+dETdyRight;
		Double_t dETdyTErr = dETdyLErr+dETdy_d_err+dETdyRErr;
		
		Double_t dNdEta_d = *(integralDataPtr+4);
		Double_t dNdEta_d_err = *(integralDataPtr+5);
		Double_t dNdEtaTotal = dNdEtaLeft+dNdEta_d+dNdEtaRight;
		Double_t dNdEtaTErr = dNdEtaLErr+dNdEta_d_err+dNdEtaRErr;
		
		Double_t dNdy_d = *(integralDataPtr+6);
		Double_t dNdy_d_err = *(integralDataPtr+7);
		Double_t dNdyTotal = dNdyLeft+dNdy_d+dNdyRight;
		Double_t dNdyTErr = dNdyLErr+dNdy_d_err+dNdyRErr;
		
		cout <<"dETdEtaL for histo "<<breakOutForTesting<<" = " 
				<<histoName<<": "<<dETdEtaLeft<<endl;
		cout <<"dETdEtaLErr for histo "<<breakOutForTesting<<" = " 
				<<histoName<<": "<<dETdEtaLErr<<endl; // -nan?
		cout <<"dETdEtaLErr2 for histo "<<breakOutForTesting<<" = " 
				<<histoName<<": "<<dETdEtaLErr2<<endl;
		cout<<"-----------------------------------"<<endl;				
		//------ end Find integrals left and right of data points ----//
		
		//-- Output results to file-----------------------------
		datFile << collEn << "\t"	
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
			<< dETdEta_d << "\t" //dETdEta_d
			<< dETdEta_d_err<< "\t" //dETdEta_d_err
			<< dETdEtaLeft << "\t"
			<< dETdEtaLErr << "\t"
			<< dETdEtaRight << "\t"
			<< dETdEtaRErr << "\t"
			<< dETdEtaTotal<< "\t" // dETdEtaTotal
			<< dETdEtaTErr << "\t"
			<< dETdy_d << "\t" //dETdy_d
			<< dETdy_d_err<< "\t" //dETdy_d_err
			<< dETdyLeft << "\t"
			<< dETdyLErr << "\t"
			<< dETdyRight << "\t"
			<< dETdyRErr << "\t"
			<< dETdyTotal<< "\t" // dETdyTotal
			<< dETdyTErr << "\t"
			<< dNdEta_d << "\t" //dNdEta_d
			<< dNdEta_d_err<< "\t" //dNdEta_d_err
			<< dNdEtaLeft << "\t"
			<< dNdEtaLErr << "\t"
			<< dNdEtaRight << "\t"
			<< dNdEtaRErr << "\t"
			<< dNdEtaTotal << "\t" // dNdEtaTotal
			<< dNdEtaTErr << "\t"
			<< dNdy_d << "\t" //dNdy_d
			<< dNdy_d_err<< "\t" //dNdy_d_err
			<< dNdyLeft << "\t"
			<< dNdyLErr << "\t"
			<< dNdyRight << "\t"
			<< dNdyRErr << "\t"
			<< dNdyTotal << "\t" // dNdyTotal
			<< dNdyTErr << "\n";
		
		//-- end- output results to file------------------------
		c1->Update();
		Double_t chi2BGBW = funcBGBW->GetChisquare();
		Double_t nDFBGBW = funcBGBW->GetNDF();
		Double_t p2 = funcBGBW->GetParameter(2);
		Double_t e2 = funcBGBW->GetParError(2);
		


	
		//cout << "chi2: " << chi2BGBW << "\nndf: " 
			//<< nDFBGBW<< "\nchi2/ndf: " << chi2BGBW/nDFBGBW <<endl;
	
		/* FIXME */
		string imgPathAndName = "./debugPlots/"+histoName+".png";
				//c1 -> SaveAs("./fittedPlots/trial1.png");
		TImage *png = TImage::Create();// FIXME try to use canvas method instead of png object
		png->FromPad(c1);
		const char* imgPathAndNameConstCharPtr = imgPathAndName.c_str();
		png->WriteImage(imgPathAndNameConstCharPtr);
		/**/
		//cout << "Draw class here: \n";
		//h-> DrawClass();
		///////h->Delete();// works
		///////////FIXME c1->Clear();// 
		/// sometimes when you delete objects, they stay in the program stack
		//FIXME delete png;
		mikey->DeleteBuffer();// works!
		//breakOutForTesting++;
		if(breakOutForTesting==stop) break;
		
		gSystem->ProcessEvents();
		delete h;
		delete funcBGBW;
		delete funcBGBW_top;
		delete funcBGBW_bottom;
		delete dETdEtaIntegrandFunc;
		delete dETdEtaIntegrandFunc_top;
		delete dETdEtaIntegrandFunc_bottom;
		delete dETdyIntegrandFunc;
		delete dNdEtaIntegrandFunc;
		delete dNdyIntegrandFunc;
		delete c1;	// Rademakers
		//delete mikey; // segmentation violation
		//delete class1; // segmentation violation
	} // End of while loop to iterate through every key 
	//gObjectTable->Print();
	delete myFile;
	datFile.close();
return 0;
}






