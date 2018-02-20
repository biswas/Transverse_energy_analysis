// evolved from fit plotsFromResults3.cpp
// trying to get the TGraph for ETlambda/npart vs npart and 
 //interpolate between two points to get the ET for lambdas 
 // corresponding to the intermediate points
 // using  TGraph->Eval()
 // then scale the percentage uncertainties accordingly
 // FIXME: npart, not npart/2

#include "Riostream.h"
//#include <cstdio>
//#include <vector>
#include <string>
#include <fstream>
//#include "TString.h"
//#include "TMath.h"
//#include "TH1D.h"
#include <iostream> // to use cout for debugging
using namespace std;

std::string doubToString(Double_t);
std::string centIndToPercent(int centInd);
void formatGraph(TGraph* g, Double_t collEnArr[], int enInd);
// main function:

int plotsFromResults3(){

	ifstream in;
	string skipContent;// read and skip content that has no use
	const int cents = 7; // 7 different centralities for la only
	const int collEns = 5; // 5 different collision energies
	const int parts = 2; // 2 different particles: la and ala
	const int funcsOfCollEn = 1; // there is one quantity that
							// is to be plotted as a function of
							// collision energy:
							// (dET/dEta)/(N_part),
	// array for energy-dependent plots:
	// Don't think this is needed:
	// Double_t enPlotArr[cents][collEns][funcsOfCollEn];
	in.open(Form("./fitResults_la.dat"));
	if (!in.good()){
		cout << "Problem opening file!" << endl;
		return 1;
	}
	
	for(int i=0;i<47;i++){ // loop through 47 column names
		in >> skipContent;
		// (tested) print first and last header names to check:
		//if (i==0) cout << "Col. 1: " << skipContent << endl;
		//else if (i==45) cout << "Col. 45: " << skipContent << endl;
	}
	Double_t collEn;
	Double_t cent;// centrality value obtained from data file
	string part; // particle name
	Double_t dETdEtaSum[cents][collEns] = {0};// initialize element [0][0] to 0, 
											//and initialize all other elements as if 
											// they had static storage duration, hence
											// set them to zero as well
											
	Double_t dETdEtaSum_errSq[cents][collEns] = {0}; // errors added in quadrature
								// TODO don't forget to take the square root later
	Double_t dNchdEtaSum[cents][collEns] = {0};
	Double_t dNchdEtaSum_errSq[cents][collEns] = {0}; // errors added in quadrature
	Double_t Npart[cents][collEns];// Npart function of cent and en only
	Double_t Npart_err[cents][collEns];
	Double_t dETdEtaOverNpartBy2Sum[cents][collEns];
	Double_t dETdEtaOverNpartBy2Sum_err[cents][collEns]; // use formula involving...
					//partial derivatives to evaluate uncertainty
	Double_t dETdEtaOverdNchdEtaSum[cents][collEns];
	Double_t dETdEtaOverdNchdEtaSum_err[cents][collEns]; // 
	// Add elements below (for all 6 particles)
		// in order to get the above elements
	Double_t dETdEta[cents][collEns][parts];
	Double_t dETdEtaErr[cents][collEns][parts];
	Double_t dNchdEta[cents][collEns][parts];
	Double_t dNchdEtaErr[cents][collEns][parts];
	Double_t centArr[cents]; // need to pass to formatGraph function
	//Double_t dETdEtaOverNpartBy2[cents][collEns][parts];
	// enPlotArr element indexed as: 
	// enPlotArr[centIndex][enIndex][funcIndex]
	int centIndex;
	int enIndex;
	int partIndex;
	int funcIndex;
	for(int j=0; j<270; j++){// for 270 rows below header in the data file
		//for(int i=0; i<46; i++){
		in >> collEn; cout << collEn << "\t";
		in >> part; cout << part << "\t";
		in >> cent; cout << cent << "\t";
		for(int k=0; k<15;k++){// skip content in next 15 columns
		in >> skipContent;
		}
		
		// now adding things up
		
		// for each centrality there are 5 different energies
		// for each energy corresponding to a particular centrality,
			// there are 6 different particles, and hence
			// 6 different values of (dET/dEta)/(N_part/2)
		
		// for every collision energy, there are 9 Nparts 
			//(corresponding to 9 centralities)
		// add dETdEtaOverNpartby2 and dETdEtaOverdNchdEta values
			// corresponding to each Npart (cent) for a given collEn
			
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
		// particle set: {ka-,ka+,pi-,pi+,pro,pba}
		if 		(part == "pi-") partIndex = 0;
		else if (part == "pi+") partIndex = 1;
		else if (part == "ka-") partIndex = 2;
		else if (part == "ka+") partIndex = 3;
		else if (part == "pro") partIndex = 4;
		else if (part == "pba") partIndex = 5;
		
		in >> dETdEta[centIndex][enIndex][partIndex];
		cout << dETdEta[centIndex][enIndex][partIndex] << "\t";
		in >> dETdEtaErr[centIndex][enIndex][partIndex];
		for(int l=0; l<14; l++) in >> skipContent;
		in >> dNchdEta[centIndex][enIndex][partIndex];
		cout << dNchdEta[centIndex][enIndex][partIndex] << "\t";
		in >> dNchdEtaErr[centIndex][enIndex][partIndex];
		for(int l=0; l<8; l++) in >> skipContent;
		in >> Npart[centIndex][enIndex];
		cout << Npart[centIndex][enIndex] << "\t";
		in >> Npart_err[centIndex][enIndex];
		cout << Npart_err[centIndex][enIndex] << "\n";
		// TODO: what about lambdas while adding ET?
		// ET = 3ET_pi + 4ET_k + 4ET_p + 2ET_lam
		// however lambda spectra available for centralities different from the rest
		// at high energies, number of negative and positive charges in the collision
			// product are roughly the same
			// which is not true at low energies since the colliding nuclei have +ve nucleons
		// TODO: if the error bars don't look reasonable, cout results to check the errors
		if(partIndex==0 || partIndex==1){
			dETdEtaSum[centIndex][enIndex] 			+= (3.0/2.0)*dETdEta[centIndex][enIndex][partIndex];
			// ^ since number of pi0 = num of pi+ or pi-
			dETdEtaSum_errSq[centIndex][enIndex] 	+= ((3.0/2.0)*dETdEtaErr[centIndex][enIndex][partIndex])*
												 	   ((3.0/2.0)*dETdEtaErr[centIndex][enIndex][partIndex]);
			dETdEtaSum_err[centIndex][enIndex]		=  TMath::Sqrt(dETdEtaSum_errSq[centIndex][enIndex]);						 	   
			dNchdEtaSum[centIndex][enIndex] 		+= dNchdEta[centIndex][enIndex][partIndex];
			dNchdEtaSum_errSq[centIndex][enIndex] 	+= dNchdEtaErr[centIndex][enIndex][partIndex]*
													   dNchdEtaErr[centIndex][enIndex][partIndex];
			dNchdEtaSum_errcentIndex][enIndex]		=  TMath::Sqrt(dNchdEtaSum_errSq[centIndex][enIndex]);
		}
		else if(partIndex==2 || partIndex==3 || partIndex == 4 || partIndex == 5){
			dETdEtaSum[centIndex][enIndex] 			+= 2.0*dETdEta[centIndex][enIndex][partIndex];
			dETdEtaSum_errSq[centIndex][enIndex] 	+= (2.0*dETdEtaErr[centIndex][enIndex][partIndex])*
													(2.0*dETdEtaErr[centIndex][enIndex][partIndex]);
			// ^ since num of k0_s ~ num og k0_l = num of k+ or k-
				// also, num of p ~ num of pbar ~ num of n or nbar
			dETdEtaSum_err[centIndex][enIndex]		=  TMath::Sqrt(dETdEtaSum_errSq[centIndex][enIndex]);						 	   
			dNchdEtaSum[centIndex][enIndex] 		+= dNchdEta[centIndex][enIndex][partIndex];
			dNchdEtaSum_errSq[centIndex][enIndex] 	+= dNchdEtaErr[centIndex][enIndex][partIndex]*
													   dNchdEtaErr[centIndex][enIndex][partIndex];
			dNchdEtaSum_err[centIndex][enIndex]		=  TMath::Sqrt(dNchdEtaSum_errSq[centIndex][enIndex]);
		}
	} //end of for loop with index j - captured all 270 rows from data file
	in.close();// all information from file extracted, so it should be closed
	// print dETdEtaSum for 9 centralities and 5 collision energies
		// and assign values to dETdEtaOverNpartBy2Sum[i][j]
	// for quantity q = a/b (such as dETdEtaOverNpartBy2Sum)
	// q_err = sqrt((a_err/b)^2 + (-a*b_err/(b^2))^2)
		for(int i=0; i<cents; i++){
			for(int j=0; j<collEns; j++){
				cout << "dETdEtaSum["<<i<<"]["<<j<<"] 	= "<<dETdEtaSum[i][j]<<endl;
				cout << "dNchdEtaSum["<<i<<"]["<<j<<"] 	= "<<dNchdEtaSum[i][j]<<endl;
				dETdEtaOverNpartBy2Sum[i][j] 		= dETdEtaSum[i][j]/Npart[i][j];
				dETdEtaOverNpartBy2Sum_err[i][j]	= TMath::Sqrt(
														(dETdEtaSum_err[i][j]/Npart[i][j])*
														(dETdEtaSum_err[i][j]/Npart[i][j])
																 +
														(-dETdEtaSum[i][j]*Npart_err[i][j]/
															(Npart[i][j]*Npart[i][j]))*
														(-dETdEtaSum[i][j]*Npart_err[i][j]/
															(Npart[i][j]*Npart[i][j]))
																 );
				dETdEtaOverdNchdEtaSum[i][j] 		= dETdEtaSum[i][j]/dNchdEtaSum[i][j];// FIXME??
																	// graph does not look right
				dETdEtaOverdNchdEtaSum_err[i][j]	= TMath::Sqrt(
														(dETdEtaSum_err[i][j]/dNchdEtaSum[i][j])*
														(dETdEtaSum_err[i][j]/dNchdEtaSum[i][j])
																 +
														(-dETdEtaSum[i][j]*dNchdEtaSum_err[i][j]/
															(dNchdEtaSum[i][j]*dNchdEtaSum[i][j]))*
														(-dETdEtaSum[i][j]*dNchdEtaSum_err[i][j]/
															(dNchdEtaSum[i][j]*dNchdEtaSum[i][j]))
																 );
			}
		}
	Double_t collEnArr[5] = {7.7,11.5,19.6,27,39}; // To use in TGraphErrors

	/*
	for(int i=0; i<cents; i++){ // i is cent index
		for(int j=0; j<collEns;j++)
		Npart[i][j] = 
	}
	*/
	///////////////// HERE TODO TODO TODO TODO TODO TODO TODO TODO TODO
	/// ------ begin - plot all graphs at once -------------------------------//
	for(int centInd=0; centInd<cents; centInd++){ // loop through all centralities
		centArr[centInd] = centInd*1.0;
		Double_t dETdEtaOverNpartBy2SumCentByCent[collEns];// one graph per centInd
		Double_t dETdEtaOverdNchdEtaSumCentByCent[collEns];
		for(int i=0; i<collEns; i++){ // loop through all collEns for current centrality
		// because TGraphErrors does not take a 2d array as argument
			dETdEtaOverNpartBy2SumCentByCent[i] = dETdEtaOverNpartBy2Sum[centInd][i];
			dETdEtaOverdNchdEtaSumCentByCent[i] = dETdEtaOverdNchdEtaSum[centInd][i];
		}
		TGraph* g1;
		g1 = new TGraph(5, collEnArr, dETdEtaOverNpartBy2SumCentByCent);
		g1 -> SetName("dETdEtaOverNpartBy2Sum_vs_en"); // unique identifier to be used in function:
		formatGraph(g1, centArr, centInd);
		delete g1;	
	
		// TODO make a function to take care of different types of graph instead of creating new objects here
		TGraph* g2;
		g2 = new TGraph(5, collEnArr, dETdEtaOverdNchdEtaSumCentByCent);
		g2 -> SetName("dETdEtaOverdNchdEtaSum_vs_en"); // unique identifier to be used in function:
		formatGraph(g2, centArr, centInd);
		delete g2;
		
	} // end of for loop with index centInd
	/// ------ end - plot all snn graphs at once -------------------------------//
	
	Double_t NpartArrEnByEn[9]; // for each collEn, there is a unique Npart Array
	// unlike for the snn graphs in which 
	// for each cent, there is the same collEnArr
	// hence, the above array should be used in a loop instead of directly as in
	// the usage of collEnArr.
	// TODO current: populate NpartArr for each energy:
	/// ------ begin - plot all Npart graphs at once -------------------------------//
	for(int enInd=0; enInd<collEns; enInd++){ // loop through all coll Ens
	
		Double_t dETdEtaOverNpartBy2SumEnByEn[cents];// one graph per collEn;
		Double_t dETdEtaOverdNchdEtaSumEnByEn[cents];	// cents = num of Nparts
		for(int i=0; i<cents; i++){ // loop through all cents/Nparts for current collEn
		// because TGraphErrors does not take a 2d array as argument
			dETdEtaOverNpartBy2SumEnByEn[i] = dETdEtaOverNpartBy2Sum[i][enInd];
			dETdEtaOverdNchdEtaSumEnByEn[i] = dETdEtaOverdNchdEtaSum[i][enInd];
			NpartArrEnByEn[i]				= Npart[i][enInd];
		}
		TGraph* g1; // okay to use identifier g1 in local scope as was the case in previous use
		g1 = new TGraph(9, NpartArrEnByEn, dETdEtaOverNpartBy2SumEnByEn);
		g1 -> SetName("dETdEtaOverNpartBy2Sum_vs_Npart"); // unique identifier for graph
														// see use in function formatGraph
		formatGraph(g1, collEnArr, enInd);
		
		delete g1;
		
		TGraph* g2;
		g2 = new TGraph(9, NpartArrEnByEn, dETdEtaOverdNchdEtaSumEnByEn);
		g2 -> SetName("dETdEtaOverdNchdEtaSum_vs_Npart");
		formatGraph(g2, collEnArr, enInd);
		delete g2;
		
		
	} // end of for loop with index centInd
	/// ------ end - plot all graphs at once -------------------------------//
	
	return 0;
}

std::string doubToString(Double_t d){
	stringstream stream;
	stream << fixed << setprecision(1) << d;
	return stream.str();

}

void formatGraph(TGraph* g, Double_t collEn_Or_NpartArr[], int en_Or_centInd){
	// for reference:
		// g = new TGraph(9, NpartArrEnByEn, dETdEtaOverNpartBy2SumEnByEn);
	TCanvas* c = new TCanvas(/*"c","A Simple Graph Example",200,10,700,500*/);
	//c->SetLogx(); Npart graphs not in log scale
	TString xlabel;
	TString ylabel;
	string graphText;
	string graphName;
	string imgPathAndName;
	TLatex* t1= new TLatex();
	g->Draw("A*");
	g->SetMarkerSize(2);
	TString snn1("dETdEtaOverNpartBy2Sum_vs_en"); // name of first kind of
									// graph with sqrt(snn) in the x-axis
	TString snn2("dETdEtaOverdNchdEtaSum_vs_en"); // name of second kind of
									// graph with sqrt(snn) in the x-axis
	TString npart1("dETdEtaOverNpartBy2Sum_vs_Npart"); // name of first kind of
												// graph with Npart in the X-axis
	TString npart2("dETdEtaOverdNchdEtaSum_vs_Npart"); // name of second kind of
												// graph with Npart in the X-axis
	if (g -> GetName()==snn1){
		c -> SetLogx();
		g->SetMarkerStyle(29);
		g->SetMarkerColor(kBlue);
		xlabel = "#sqrt{#it{s}_{NN}} (GeV)";
		ylabel = "#LTd#it{E}_{T}/d#eta#GT/#LT#it{N}_{part}/2#GT (GeV)";
		graphText = centIndToPercent(en_Or_centInd)+" centrality";
		graphName = "dETdEtaOverNpartBy2SumCent" + std::to_string(en_Or_centInd);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/dETdEtaOverNpartBy2_En/"+graphName+".png";
	}
	else if (g -> GetName()==snn2){
		c -> SetLogx();
		g->SetMarkerStyle(29);
		g->SetMarkerColor(kOrange);
		xlabel = "#sqrt{#it{s}_{NN}} (GeV)";
		ylabel = "#LTd#it{E}_{T}/d#eta#GT/#LTd#it{N}_{ch}/d#eta#GT (GeV)";
		graphText = centIndToPercent(en_Or_centInd)+" centrality";
		graphName = "dETdEtaOverdNchdEtaSumCent" + std::to_string(en_Or_centInd);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/dETdEtaOverdNchdEta_En/"+graphName+".png";
	}
	else if (g -> GetName()==npart1){
		g->SetMarkerStyle(28);
		g->SetMarkerColor(kGreen);
		xlabel = "#it{N}_{part}";
		ylabel= "#LTd#it{E}_{T}/d#eta#GT/#LT#it{N}_{part}/2#GT (GeV)";
		graphText = "#sqrt{#it{s}_{NN}} ="+doubToString(collEn_Or_NpartArr[en_Or_centInd])+" GeV";
		graphName = "dETdEtaOverNpartBy2SumEn" + doubToString(collEn_Or_NpartArr[en_Or_centInd]);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/dETdEtaOverNpartBy2_Npart/"+graphName+".png";
	}
	else if (g -> GetName()==npart2){
		g->SetMarkerStyle(28);
		g->SetMarkerColor(kRed);
		xlabel = "#it{N}_{part}";
		ylabel= "#LTd#it{E}_{T}/d#eta#GT/#LTd#it{N}_{ch}/d#eta#GT (GeV)";
		graphText = "#sqrt{#it{s}_{NN}} ="+doubToString(collEn_Or_NpartArr[en_Or_centInd])+" GeV";
		graphName = "dETdEtaOverdNchdEtaSumEn" + doubToString(collEn_Or_NpartArr[en_Or_centInd]);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/dETdEtaOverdNchdEta_Npart/"+graphName+".png";
	}
	//cout << g->GetName() << endl;
	
	g->GetHistogram()-> SetTitle(0);
	g->GetHistogram()->GetXaxis()-> SetTitle(xlabel);
	g->GetHistogram()->GetXaxis()-> SetTitleOffset(1.05);
	g->GetHistogram()->GetXaxis()-> SetTitleSize(0.04);
	g->GetHistogram()->GetYaxis()-> SetTitle(ylabel);
	g->GetHistogram()->GetYaxis()-> SetTitleSize(0.04);
	g->GetHistogram()->GetYaxis()-> SetTitleOffset(1.05);
	const char* graphTextConstCharPtr = graphText.c_str();// required for TText constructor
	//TText* t1 = new TText(200,.62,graphTextNpartConstCharPtr);
	t1 -> SetNDC(kTRUE);
	t1 -> DrawLatex(0.2,0.8,graphTextConstCharPtr);
	
	//t1 -> SetTextAlign(22);
	t1 -> SetTextSize(0.05);
	//t1 -> DrawText();
	// FIXME t1 -> DrawText(0.2,0.8,graphTextNpartConstCharPtr);
	
				//c -> SaveAs("./fittedPlots/trial1.png");
	TImage *png = TImage::Create();//TODO try to use canvas method instead of png object
	png->FromPad(c);
	const char* imgPathAndNameConstCharPtr1 = imgPathAndName.c_str();
	png->WriteImage(imgPathAndNameConstCharPtr1);
	delete t1;
	delete c;
	delete png;
}

std::string centIndToPercent(int centInd){
	string centRange;
	if (centInd == 0) centRange = "0-5 %";
	else if (centInd == 1) centRange = "5-10 %";
	else if (centInd == 2) centRange = "10-20 %";
	else if (centInd == 3) centRange = "20-30 %";
	else if (centInd == 4) centRange = "30-40 %";
	else if (centInd == 5) centRange = "40-50 %";
	else if (centInd == 6) centRange = "50-60 %";
	else if (centInd == 7) centRange = "60-70 %";
	else if (centInd == 8) centRange = "70-80 %";
	else centRange = "Error: check centrality!";
	return centRange;	
}
