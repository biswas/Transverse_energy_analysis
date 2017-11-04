#ifndef fitBESData3_H
#define fitBESData3_H

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
void outputDatFile(Double_t* results, int centra, string partName){
	std::ofstream datFile ("fitResults.dat", std::ofstream::out);
	datFile << "par1\t"<<"par2\t"<<"par3\t"<<"par4\t"
			<<"transEn\t"<<"transEnErr\t"<<"collEn\t"
			<<"centrality\t"<<"partName\n";
	for(int i=0; i<270; i++){// FIXME: all these indices can't be i:
		datFile << results[i]<<"\t"<< results[i]<<"\t"
				<< results[i]<<"\t"<< results[i]<<"\t"
				<< results[i]<<"\t"<< results[i]<<"\t"
				<< results[i]<<"\t"<< results[i]<<"\n";
	
	}			
	datFile.close();	
}
void classifyParticleKmeans(){

	
}

#endif
