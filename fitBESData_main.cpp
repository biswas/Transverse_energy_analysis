#include <iostream>
#include <string>
#include "TKey.h"
#include <fstream>
using namespace std;

////// FIXME: verify integral values from wolframalpha
			// make functions for dNdEta and dNdy

// forward declarations:
Double_t getdNdptOverptIntegrand(Double_t* rad, Double_t* par);
Double_t getdNdpt(Double_t* pT, Double_t* params);
string concatenateHistoname(string,string,string,string);
//This function may not be used:
Double_t getdNdptOver2pipt(Double_t* pT, Double_t* params);
Double_t getdETdyIntegrand(Double_t* myPt, Double_t* par);
Double_t getdETdEtaIntegrand(Double_t* myPt, Double_t* par);
void printHeader(std::ofstream& datFile);
void printHeader(); // overloaded
//// to use supervised and unsupervised machine learning
 // to predict particle id using good-fit parameters:
void outputDatFile(/*std::ofstream& datFile,*/ string centrality, string particleID,
					Double_t mass, Double_t beta, Double_t betaErr,
					  Double_t temp, Double_t tempErr, Double_t n, Double_t nErr,
					  Double_t norm, Double_t normErr,Double_t dE_tdEtaIntegralData,
					  Double_t dE_tdEtaIntegralDataError, Double_t dETdEtaLeft,
					  Double_t dETdEtaRight);
void outputDatFile(); // overloaded
// subroutine for above routine to classify particles using k-means clustering:
void classifyParticleKmeans();//FIXME

// main function:
int fitBESData_main(){
	//std::ofstream* datFile = new std::ofstream("fitResults2.dat", std::ofstream::out);
	//std::ofstream datFile1("fitResults2.dat", std::ofstream::out);
	//if(!datFile.open("fitResults2.dat")) return 42;
	
	//printHeader(datFile1); // FIXME: pass datFile object to method?
	std::ofstream datFile("fitResults2.dat", std::ofstream::out);
	//printHeader();
	
	datFile << "E" << "\t"// collision energy
			<< "cent" << "\t"
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
			<< "dETdEta_d" << "\t" // TODO: transverse energy from data
			<< "dETdEta_d_err" << "\t" // TODO: transverse from data error
			<< "dETdEta_left" << "\t"
			<< "dETdEta_leftErr" << "\t"
			<< "dETdEta_right" << "\t"
			<< "dETdEta_rightErr" << "\n";
	
	
	
	//datFile1.close();
	
	TFile* myFile = new TFile("BESData.root");
	TIter next(myFile->GetListOfKeys());
	TKey* mikey;
	//for(int cent=0;cent<9;cent++){// centrality loop
	
	int breakOutForTesting =0;
	while((mikey=(TKey*)next())){
		TCanvas* c1 = new TCanvas;
		gPad->SetLogy();
		gStyle->SetOptFit();// display fit parameters; customizable
		gStyle->SetOptDate();// display date (at bottom left)
		gROOT-> SetBatch(kTRUE);// save canvases without displaying them
		c1->Update();

		TClass* class1 = gROOT->GetClass(mikey->GetClassName());
		if(!class1->InheritsFrom("TH1")) continue;

		// read histogram object for current iteration of key:
		TH1F* h = (TH1F*)mikey->ReadObj();
		string histoName = h->GetName();
		//get first three characters of particle name from histoName:
		string particleID = histoName.substr(6,3); 
		//cout << "particle ID: "<<particleID<<endl;
		//return 1;
		string centrality = histoName.substr(4,1);
		
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
		/*if (particleID=="pi-"||particleID=="ka-"||particleID=="pba") type = -1;
		else if (particleID=="pi+"||particleID=="ka+"||particleID=="pro") type=1;
		else {cout << "Check particle type: " << particleID << endl; return 2;}*/
		//---------End - Assign mass & type to particle -----------------//
		
		//------------- Find Et integral from data points -------//
		Double_t dx; // gets defined in the loop
		//Double_t integralData = 0.;
		Double_t dE_tdEtaIntegralData= 0.; // dE_T/dEta from data points
		Double_t dE_tdyIntegralData= 0.; // dE_T/dy from data points
		//Double_t igerr2 = 0.; // is this error? why 2? square?
		Int_t binx1 = 0;
		Int_t binx2 = 1000;
		Double_t dE_tdEtaIntegralDataError = 0.;
		Double_t dE_tdyIntegralDataError = 0.;
		Double_t jMass = mass;
		Int_t totBins = h -> GetNbinsX();
		if (binx1 < 0) binx1 = 0; // sanity check
	   	if (binx2 > totBins+1 || binx2 < 0) binx2 = totBins+1;
		for(Int_t binx = binx1; binx <= binx2; binx++){
			float pt = (h->GetXaxis()->GetBinLowEdge(binx)+
						h->GetXaxis()->GetBinUpEdge(binx))/2;// avg of bin edges
			//float pt = h->GetXaxis()->GetBinLowEdge(binx);
			// calculate E_T needed for dE_T/dy:
			float et = TMath::Sqrt(pt*pt+mass*mass)+type*mass;
			// ^ sin(theta)=1 at midrapidity
			// calculate J*E_T needed for dE_T/dEta:
			float JTimeset = pt/(TMath::Sqrt(pt*pt+mass*mass))*et; 
			dx = h->GetXaxis()->GetBinWidth(binx);
			//cout << "bin width from bin " << binx << ": " << dx << endl;
			//cout << "content in bin " << binx << ": " << h->GetBinContent(binx) << endl;
			dE_tdEtaIntegralData += h->GetBinContent(binx)*JTimeset*dx;
			dE_tdyIntegralData += h->GetBinContent(binx)*et*dx;
			// checking with et0:
			//if (width) integralData += h->GetBinContent(binx)*et0*dx; 
			dE_tdEtaIntegralDataError += h->GetBinError(binx)*dx*JTimeset;
			dE_tdyIntegralDataError += h->GetBinError(binx)*dx*et;
			//igerr2 += h->GetBinError(binx)*dx*et; //// !!look up details later
		}
			// ^ if the errors are completely correlated
			// if uncorrelated: take the square root of igerr2
		
		//---------End- Find Et integral from data points ------//
		
		
		//------------- Begin BGBW fit --------------------------//
		TF1* funcBGBW = new TF1("getdNdpt",getdNdpt,0.00000000000001,10.,5);
		if (	histoName == "cent7_ka-_Au+Au_7.7"
			|| 	histoName == "cent7_ka-_Au+Au_11.5"
			||	histoName == "cent7_pi+_Au+Au_7.7"
			||	histoName == "cent8_ka+_Au+Au_7.7"
			||	histoName == "cent4_pi-_Au+Au_19.6"
			||	histoName == "cent5_ka+_Au+Au_27"
			||	histoName == "cent5_ka-_Au+Au_7.7"
			||	histoName == "cent6_pi+_Au+Au_11.5"){
			funcBGBW->SetParameters(0.1,0.9,0.03,0.01,10000.);
			//cout << "Histoname: " << histoName << endl;
			//breakOutForTesting = 281;// to break out of the loop
			}			
		else{
			funcBGBW->SetParameters(0.1,0.95,0.05,0.1,1000000.);
			}
		//TODO funcBGBW->SetParameters(0.1,0.6,	0.07,	2.,100000.);
		funcBGBW->SetParNames("mass","beta (c)","temp","n","norm");
		funcBGBW->SetParLimits(1,0.5,0.999999999999999999999);//param 1

		funcBGBW->FixParameter(0,mass);// mass in GeV
		h->Fit("getdNdpt","","",0.00000000000001,10.);
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
		//c1 -> SaveAs("./fittedPlots/trial1.png");
		//------------- end BGBW fit ----------------------------
		
		
		//-------- Find integrals left and right of data points -------//
		funcBGBW->SetParameters(mass,beta,temp,n,norm);
		TF1* dETdEtaIntegrandFunc = new TF1("dETdEtaIntegrand", 
									getdETdEtaIntegrand, 
									0, 10, 6 );// function goes from 0 to 10
										// and has 6 parameters"
										// mass, beta, temp, n, norm, type
		dETdEtaIntegrandFunc -> SetParameters(mass,beta,temp,n,norm,type);
		TF1* dETdyIntegrandFunc = new TF1("dETdyIntegrand",
								  getdETdyIntegrand,
								  0,10,6);
		dETdyIntegrandFunc -> SetParameters(mass,beta,temp,n,norm,type);			  
		Double_t leftCut = h->GetXaxis()->GetBinLowEdge(binx1+2); // TODO:check if bin is right
		Double_t rightCut = h->GetXaxis()->GetBinUpEdge(binx2-1); // TODO:check if bin is right
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
		cout<<"Integral from data for histo "<<breakOutForTesting+1<<": "<<dE_tdEtaIntegralData<<endl;// 357.633 for pi minus cent 0
		cout<< "Error: "<< dE_tdEtaIntegralDataError<<endl<<"-----------------------------------"<<endl;				
		//------ end Find integrals left and right of data points ----//
		
		//-- Output results to file-----------------------------
		datFile << 0.0 << "\t"	//collision energy
			<< centrality << "\t"
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
			<< dE_tdEtaIntegralData << "\t" //
			<< dE_tdEtaIntegralDataError << "\t" //
			<< dETdEtaLeft << "\t"
			<< "ETlErr" << "\t"
			<< dETdEtaRight << "\t"
			<< "ETrErr" << "\n";
		// FIXME outputDatFile(datFile, centrality, particleID, mass, beta, betaErr,
					 // temp, tempErr, n, nErr, norm, normErr,
					  //dE_tdEtaIntegralData, dE_tdEtaIntegralDataError,
					  //dETdEtaLeft, dETdEtaRight);
		/*
		outputDatFile(//datFile,
					centrality, particleID,
					mass, beta, betaErr,
					temp, tempErr, n, nErr,
					norm, normErr, dE_tdEtaIntegralData,
					dE_tdEtaIntegralDataError, dETdEtaLeft,
					dETdEtaRight);
		*/
		
		//-- end- output results to file------------------------
		c1->Update();
		Double_t chi2BGBW = funcBGBW->GetChisquare();
		Double_t nDFBGBW = funcBGBW->GetNDF();
		Double_t p2 = funcBGBW->GetParameter(2);
		Double_t e2 = funcBGBW->GetParError(2);
	
		//cout << "chi2: " << chi2BGBW << "\nndf: " 
			//<< nDFBGBW<< "\nchi2/ndf: " << chi2BGBW/nDFBGBW <<endl;
	
		string imgPathAndName = "./fittedPlots2/"+histoName+".png";
		TImage* png = TImage::Create();
		png->FromPad(c1);
		const char* imgPathAndNameConstCharPtr = imgPathAndName.c_str();
		png->WriteImage(imgPathAndNameConstCharPtr);
		delete h;
		delete png;
		delete class1;
		delete c1;
		//delete funcBGBW;
		//delete dETdEtaIntegrandFunc;
		//delete dETdyIntegrandFunc;
		//delete imgPathAndNameConstCharPtr;
		breakOutForTesting++;
		if(breakOutForTesting>280) break;
				
	}
	//}// end of while loop to iterate through every key
	delete mikey;
	myFile->Close();
	delete myFile;
	datFile.close();
return 0;
}









//method to return 1/pt dN/dpt integrand
Double_t getdNdptOverptIntegrand(Double_t* rad, Double_t* par){
	// (dN/dpt)/pt= r*dr*mt*I0((pt*sinh(rho))/T)*K1((mt*cosh(rho))/T)
	// rho=arctanh(beta); beta=betaMax*(r/R)^n
	//Double_t pT		= pt[0];
	Double_t r 			= rad[0];
	// parameters used to fit: mass, beta, temp, n, norm
	Double_t mass 		= par[0];// not mT
	Double_t pt			= par[1];
	Double_t betaMax 	= par[2];
	Double_t temp 		= par[3];
	Double_t n 			= par[4];

	Double_t beta = betaMax*TMath::Power(r,n);
	if(beta > 0.99999999999999999999) beta = 0.99999999999999999999;
	Double_t mT 	= TMath::Sqrt(mass*mass + pt*pt);
	Double_t rho0 	= TMath::ATanH(beta);
	Double_t avoidFPE = pt*TMath::SinH(rho0)/temp;
	if(avoidFPE > 700.) avoidFPE = 700.;
	Double_t bk1arg = mT*TMath::CosH(rho0)/temp;
	Double_t integrand = /*definition*/
	r*mT*TMath::BesselI0(avoidFPE)*TMath::BesselK1(bk1arg);
	return integrand;
}// end of method getdNdptOverptIntegrand

// method to return dN/dpt
Double_t getdNdpt(Double_t* pT, Double_t* params){
	TF1* dNdptOverptIntegrandFunc = new TF1("integrandFunc", 
									getdNdptOverptIntegrand, 
									0, 1, 5 );
	
	Double_t pt		= pT[0];
	Double_t mass 	= params[0];// not mT
	Double_t beta 	= params[1];
	Double_t temp 	= params[2];
	Double_t n 		= params[3];
	Double_t norm	= params[4];
	dNdptOverptIntegrandFunc->SetParameters(mass,pt,beta,temp,n);

	Double_t dNdptOverpt 	= dNdptOverptIntegrandFunc->Integral(0,1);
	// ^ normalized r goes from 0 to 1 instead of from 0 to R
	Double_t dNdpt_normalized			= 2 * TMath::Pi() * pt * norm * dNdptOverpt;

	return dNdpt_normalized;
}
Double_t getdETdyIntegrand(Double_t* myPt, Double_t* par){
	Double_t pt   = myPt[0]; // x-axis of integration
	Double_t getdNdptPars[5];
	getdNdptPars[0]=par[0];
	getdNdptPars[0]=par[1];
	getdNdptPars[0]=par[2];
	getdNdptPars[0]=par[3];
	getdNdptPars[0]=par[4];
	Double_t mass = par[0];
	//Double_t beta = par[1];
	//Double_t temp = par[2];
	//Double_t n    = par[3];
	//Double_t norm = par[4];
	Double_t type = par[5];
	//funcBGBW-> SetParameters(mass,beta,temp,n,norm);
	Double_t funcVal = getdNdpt(myPt,par);
	cout<< "funcVal: " << funcVal<< "pt: "<< pt<< endl;
	Double_t integrand = funcVal*(TMath::Sqrt(pt*pt+mass*mass)+type*mass);
	
	return integrand;
}
Double_t getdETdEtaIntegrand(Double_t* myPt, Double_t* par){
	Double_t pt   = myPt[0]; // x-axis of integration
	Double_t getdNdptPars[5];
	getdNdptPars[0]=par[0];
	getdNdptPars[0]=par[1];
	getdNdptPars[0]=par[2];
	getdNdptPars[0]=par[3];
	getdNdptPars[0]=par[4];
	Double_t mass = par[0];
	//Double_t beta = par[1];
	//Double_t temp = par[2];
	//Double_t n    = par[3];
	//Double_t norm = par[4];
	Double_t type = par[5];
	//funcBGBW-> SetParameters(mass,beta,temp,n,norm);
	Double_t funcVal = getdNdpt(myPt,par);
	cout<< "funcVal: " << funcVal<< " pt: "<< pt<< endl;
	Double_t integrand = funcVal*(pt/TMath::Sqrt(pt*pt+mass*mass))*(TMath::Sqrt(pt*pt+mass*mass)+type*mass);
	
	return integrand;
}

/*Double_t use_funcBGBW(TF1* funcObj, Double_t* pt){
	return funcObj->Eval(pt);
}*/
string concatenateHistoname(string centStr,string pName,string colSp,string colEn){
	string initText = "cent";
	string undScr = "_";//underscore
	//string enUnits = "GeV";
	string addedString = initText+centStr+undScr+pName+undScr+colSp+undScr+colEn;//+enUnits;
	return addedString; //type: const char*: to be done later
}

// results: par1,par2,par3,par4,transEn, 
	//transEnErr, collEn
// args 2, 3: centrality, partName

//FIXME: actually, this function is probably not needed
		// just outstream this info everytime a fit is good?
		// TODO: convert function to ifstream from ofstream
void outputDatFile(/*std::ofstream& datFile,*/ string centrality, string particleID,
					Double_t mass, Double_t beta, Double_t betaErr,
					  Double_t temp, Double_t tempErr, Double_t n, Double_t nErr,
					  Double_t norm, Double_t normErr,Double_t dE_tdEtaIntegralData,
					  Double_t dE_tdEtaIntegralDataError, Double_t dETdEtaLeft,
					  Double_t dETdEtaRight){
	std::ofstream datFile2("fitResults2.dat",
					/*std::ofstream::out | */std::ofstream::app);
	datFile2 << 0.0 << "\t"	//collision energy
			<< centrality << "\t"
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
			<< dE_tdEtaIntegralData << "\t" //
			<< dE_tdEtaIntegralDataError << "\t" //
			<< dETdEtaLeft << "\t"
			<< "ETlErr" << "\t"
			<< dETdEtaRight << "\t"
			<< "ETrErr" << "\n";
	//datFile2.close();

/*	std::ofstream datFile ("fitResults.dat", std::ofstream::out);
	datFile << "par1\t"<<"par2\t"<<"par3\t"<<"par4\t"
			<<"transEn\t"<<"transEnErr\t"<<"collEn\t"
			<<"centrality\t"<<"partName\n";
	for(int i=0; i<270; i++){// all these indices can't be i:
		datFile << results[i]<<"\t"<< results[i]<<"\t"
				<< results[i]<<"\t"<< results[i]<<"\t"
				<< results[i]<<"\t"<< results[i]<<"\t"
				<< results[i]<<"\t"<< results[i]<<"\n";
	
	}			
	datFile.close();	
*/
}
void classifyParticleKmeans(){

	
}

void printHeader(/*std::ofstream& datFile*/){
	std::ofstream datFile("fitResults2.dat",
					std::ofstream::out);
	datFile << "E" << "\t"// collision energy
			<< "cent" << "\t"
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
			<< "dETdEta_d" << "\t" // TODO: transverse energy from data
			<< "dETdEta_d_err" << "\t" // TODO: transverse from data error
			<< "dETdEta_left" << "\t"
			<< "dETdEta_leftErr" << "\t"
			<< "dETdEta_right" << "\t"
			<< "dETdEta_rightErr" << "\n";
	//datFile.close();
}
