#include <iostream>
#include <string>
using namespace std;

// forward declarations:
Double_t getdNdptOverptIntegrand(Double_t* rad, Double_t* par);
Double_t getdNdpt(Double_t* pT, Double_t* params);
string concatenateHistoname(string,string,string,string);

// main function:
int fitBESoneByone(){
	//-------------- Customize histogram name --------------//
	int cent 						= 8;// 0 to 8
	std::string collidingSpecies 	= "Au+Au";
	std::string collisionEnergy 	= "11.5";// 7.7, 11.5, 19.6, 27, 39
	std::string particleName 		= "ka+";	
	// ----------- End - Customize histogram name-----------//
	//------------ Assign mass to particle -----------------//
	Double_t mass; // in GeV
	if		(particleName=="pi-"||particleName=="pi+") mass = 0.13957;
	else if	(particleName=="ka-"||particleName=="ka+") mass = 0.49368;
	else if	(particleName=="pbar"||particleName=="proton") mass = 0.93827;
	else {cout << "Invalid particle: "
			<< particleName<<endl;return 0;};
	//---------End - Assign mass to particle -----------------//
	string centString = std::to_string(cent);
	string histoname = concatenateHistoname(centString,particleName,collidingSpecies,collisionEnergy);

	//FIXME:TFile* myFile = new TFile("BESData_transformed.root");
	TFile* myFile = new TFile("BESData.root");

	//------------- Begin BGBW fit --------------------------
	// 	TH1F* hpx2 = (TH1F*)myFile->Get("cent0_pi-_Au+Au_7.7");// eg. name
	const char* histonameConstCharPtr = histoname.c_str();//
	TF1* funcBGBW = new TF1("getdNdpt",getdNdpt,0.00000000000001,5.,5);
	TCanvas * c1;
	gStyle->SetOptFit();
	gStyle->SetOptDate();
	//gStyle->SetOptLogy();
	// for cent7_ka-_Au+Au_7.7 and cent8_ka+_Au+Au_7.7:
	//funcBGBW->SetParameters(0.1,0.9,0.03,0.01,10000.);
	//funcBGBW->SetParameters(0.1,0.95,0.05,0.1,1000000.);
	funcBGBW->SetParameters(0.1,0.6,	0.07,	2.,100000.);
	
	funcBGBW->SetParNames("mass","beta (c)","temp","n","norm");
	funcBGBW->SetParLimits(1,0.5,0.999999999999999999999);
	funcBGBW->FixParameter(0,mass);// mass in GeV
	//funcBGBW->FixParameter();
	TH1F* hpx2 = (TH1F*)myFile->Get(histonameConstCharPtr);
	/// FIXME hpx2->Fit("getdNdpt","","",0.00000000000001,5.);
	
	
	
	
	
	
	
	TFitResultPtr r = hpx2->Fit("getdNdpt","S","",0.00000000000001,5.);
	TMatrixDSym cov = r->GetCovarianceMatrix();
	//Double_t dNdptOverpt 	= dNdptOverptIntegrandFunc->Integral(0,1);
	Double_t BGBWIntegral = funcBGBW->Integral(0.,10.);
	cout << "Integral: " << BGBWIntegral << endl;
	Double_t BGBWIntErr = funcBGBW->IntegralError(0.,10.,
									r->GetParams(),
									r->GetCovarianceMatrix().GetMatrixArray());
	cout << "Integral Err: " << BGBWIntErr << endl;
	
	
	
	
	
	
	
	Double_t chi2BGBW = funcBGBW->GetChisquare();
	Double_t nDFBGBW = funcBGBW->GetNDF();
	Double_t p2 = funcBGBW->GetParameter(2);
	Double_t e2 = funcBGBW->GetParError(2);
	cout << "chi2: " << chi2BGBW << "\nndf: " 
		<< nDFBGBW<< "\nchi2/ndf: " << chi2BGBW/nDFBGBW <<endl;

//------------- end BGBW fit ----------------------------

return 0;
}

//method to return 1/pt dN/dpt integrand
Double_t getdNdptOverptIntegrand(Double_t* rad, Double_t* par){
	// dN/dpt= r*dR*pt*mt*I0((pt*sinh(rho))/T)*K1((mt*cosh(rho))/T)
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
	Double_t dNdpt			= 2 * TMath::Pi() * pt * norm * dNdptOverpt;

	return dNdpt;
}

string concatenateHistoname(string centStr,string pName,string colSp,string colEn){
	string initText = "cent";
	string undScr = "_";//underscore
	string addedString = initText+centStr+undScr+pName+undScr+colSp+undScr+colEn;
	return addedString; //type: const char*: to be done later
}



