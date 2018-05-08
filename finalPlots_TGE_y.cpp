// use results from fits to produce individual plots
// (in rapidity coordinates rather than pseudorapidity)
// evolved from finalPlots_TGE.cpp

#include "Riostream.h"
#include <string>
#include <fstream>
#include "fitBESData5.h"
// ^ to use functions such as getNpartAndErr(Double_t en, string cent)
using namespace std;

std::string doubToString(Double_t);
std::string centIndToPercent(int centInd);
int centIndex(Double_t cent);
int enIndex(Double_t collEn);
int partIndex(string part);
void formatGraph(TGraphErrors* g, Double_t collEn_Or_NpartArr[], int en_Or_centInd);

Double_t collEnArr[5] = {7.7,11.5,19.6,27,39}; // To use in TGraphErrors

// main function:
int finalPlots_TGE_y(){
	
	ifstream in1; // input from dat file containing pi/k/p results
	TFile* f; // .root file to be created
	string skipContent;// read and skip content that has no use
	const int CENTS 		= 9; // 9 different centralities
	const int COLLENS 		= 5; // 5 different collision energies
	const int PARTS 		= 8; // 8 different particles
	Double_t collEn;
	Double_t cent;// centrality value obtained from data file
	string part; // particle name
	//Double_t mass; // just to debug
	Double_t dETdySum[CENTS][COLLENS] = {0.};// initialize element [0][0] to 0, 
											//and initialize all other elements as if 
											// they had static storage duration, hence
											// set them to zero as well
											
	Double_t dETdySum_errSq[CENTS][COLLENS] = {0}; // errors added in quadrature
	Double_t dETdySum_err[CENTS][COLLENS] = {0.}; // sq root of the above
	Double_t dNchdySum[CENTS][COLLENS] = {0.};
	Double_t dNchdySum_errSq[CENTS][COLLENS] = {0.}; // errors added in quadrature
	Double_t dNchdySum_err[CENTS][COLLENS] = {0.}; // sq root of the above
	Double_t Npart[CENTS][COLLENS] = {0.};// Npart function of cent and en only
	Double_t Npart_err[CENTS][COLLENS] = {0.};
	Double_t dETdyOverNpartBy2Sum[CENTS][COLLENS] = {0.};
	Double_t dETdyOverNpartBy2Sum_err[CENTS][COLLENS] = {0.}; // use formula involving...
					//partial derivatives to evaluate uncertainty
	Double_t dETdyOverdNchdySum[CENTS][COLLENS] = {0.};
	Double_t dETdyOverdNchdySum_err[CENTS][COLLENS] = {0.}; // 
	// Add elements below (for all the particles)
		// in order to get the above elements
	Double_t dETdy[CENTS][COLLENS][PARTS] = {0.};
	Double_t dETdyErr[CENTS][COLLENS][PARTS] = {0.};
	Double_t dNchdy[CENTS][COLLENS][PARTS] = {0.};
	Double_t dNchdyErr[CENTS][COLLENS][PARTS] = {0.};
	Double_t centArr[CENTS]; // need to pass to formatGraph function
	f = new TFile("crossCheckGraphs_y.root","RECREATE");
	for(int j=0; j<=270+70; j++){// for 270 rows below header in the data file in1
								// and 70 rows below header in data file in2
		if (j == 0) 
		{
			in1.open(Form("./fitResults5.dat"));
			if (!in1.good())
			{
				cout << "Problem opening file!" << endl;// tested
				return 3333;
			}
			for(int i=0;i<46;i++)
			{ // loop through 46 column names
				in1 >> skipContent;
				// (tested) print first and last header names to check:
				if (i==0) cout << "Col." << i+1<<": " << skipContent << endl;
				else if (i==45)cout<<"Col."<<i+1<<": "<< skipContent << endl;
			}
			//goto label;
		}
		else if (j == 271)
		{
			cout << "lambdas now*******************" << endl;
			in1.close();
			in1.open(Form("./fitResults_la.dat"));
			if (!in1.good())
			{
				cout << "Problem opening file!" << endl;// tested
				return 3333;
			}
			for(int i=0;i<47;i++)
			{ // loop through 46 column names
				in1 >> skipContent;
				// (tested) print first and last header names to check:
				if (i==0) cout << "Col." << i+1<<": " << skipContent << endl;
				else if (i==45)cout<<"Col."<<i+1<<": "<< skipContent << endl;
			}
		}

		cout << "j = " << j << endl;
		in1 >> collEn; cout << "collEn: " << collEn << "\t";
		in1 >> part; cout << "particle: " << part << "\t";
		in1 >> cent; cout << "centrality: " << cent << "\n";
		//in1 >> mass; cout << "mass: " << mass << "\n";// just to debug
		for(int k=0; k<23;k++){// skip content in next 23 columns
		in1 >> skipContent;
		}
		in1 >> dETdy[centIndex(cent)][enIndex(collEn)][partIndex(part)];
		cout << "dETdy: " << 
			dETdy[centIndex(cent)][enIndex(collEn)][partIndex(part)] << "\t";
		in1 >> dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)];
		for(int l=0; l<14; l++) in1 >> skipContent;
		in1 >> dNchdy[centIndex(cent)][enIndex(collEn)][partIndex(part)];
		cout << "dNchdy: " << 
			dNchdy[centIndex(cent)][enIndex(collEn)][partIndex(part)] << "\t";
		in1 >> dNchdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)];
		//for(int l=0; l<8; l++) in1 >> skipContent;
		in1 >> Npart[centIndex(cent)][enIndex(collEn)];
		cout << "Npart: " << 
			Npart[centIndex(cent)][enIndex(collEn)] << "\t";
		in1 >> Npart_err[centIndex(cent)][enIndex(collEn)];
		cout << Npart_err[centIndex(cent)][enIndex(collEn)] << "\n";
		if(j>=271) in1 >> 	skipContent;

		if(partIndex(part) == 6 || partIndex(part) == 7)
		{	// since lambdas aren't charged particles:
			dNchdy[centIndex(cent)][enIndex(collEn)][partIndex(part)]		= 0.;
			dNchdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)]	= 0.;
			if(centIndex(cent) == 5 || centIndex(cent) == 6)
			{// set below variables to zero, 
			  // since they don't represent the common centralities;
			  // the interpolated values corresponding to the common centralities...
			  // will be retrieved later 
				dETdy[centIndex(cent)][enIndex(collEn)][partIndex(part)] 		= 0.;
				dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)]	= 0.;
			
				Npart[centIndex(cent)][enIndex(collEn)]	= *getNpartAndErr(collEn,cent);
				Npart_err[centIndex(cent)][enIndex(collEn)]= *(getNpartAndErr(collEn,cent)+1);		
			}
		}
		cout << "dETdy: " << 
			dETdy[centIndex(cent)][enIndex(collEn)][partIndex(part)] << "\t";
		cout << "dNchdy: " << 
			dNchdy[centIndex(cent)][enIndex(collEn)][partIndex(part)] << "\t";
		cout << "Npart: " << 
			Npart[centIndex(cent)][enIndex(collEn)] << "\t";
		cout << Npart_err[centIndex(cent)][enIndex(collEn)] << "\n";
		// TODO: what about lambdas while adding ET?
		// ET = 3ET_pi + 4ET_k + 4ET_p + 2ET_lam
		// at high energies, number of negative and positive charges in the collision
			// product are roughly the same
			// which is not true at low energies since the colliding nuclei have +ve nucleons
		if(partIndex(part)==0 || partIndex(part)==1){
			dETdySum[centIndex(cent)][enIndex(collEn)] 
			 += (3.0/2.0)*dETdy[centIndex(cent)][enIndex(collEn)][partIndex(part)];
			// ^ since number of pi0 = num of pi+ or pi-
			dETdySum_errSq[centIndex(cent)][enIndex(collEn)]
			 += ((3.0/2.0)*dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)])*
			    ((3.0/2.0)*dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)]);
			dETdySum_err[centIndex(cent)][enIndex(collEn)]
			 =  TMath::Sqrt(dETdySum_errSq[centIndex(cent)][enIndex(collEn)]);
								 	   
			dNchdySum[centIndex(cent)][enIndex(collEn)]
			 += dNchdy[centIndex(cent)][enIndex(collEn)][partIndex(part)];
			dNchdySum_errSq[centIndex(cent)][enIndex(collEn)]
			 += dNchdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)]*
			    dNchdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)];
			dNchdySum_err[centIndex(cent)][enIndex(collEn)]
			 =  TMath::Sqrt(dNchdySum_errSq[centIndex(cent)][enIndex(collEn)]);
		}
		else if(partIndex(part)==2 || partIndex(part)==3 ||
				 partIndex(part)== 4 || partIndex(part)== 5){
			dETdySum[centIndex(cent)][enIndex(collEn)]
			 += 2.0*dETdy[centIndex(cent)][enIndex(collEn)][partIndex(part)];
			dETdySum_errSq[centIndex(cent)][enIndex(collEn)]
			 += (2.0*dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)])*
				(2.0*dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)]);
			// ^ since num of k0_s ~ num of k0_l = num of k+ or k-
				// also, num of p ~ num of pbar ~ num of n or nbar
			dETdySum_err[centIndex(cent)][enIndex(collEn)]
			 = TMath::Sqrt(dETdySum_errSq[centIndex(cent)][enIndex(collEn)]);						 	   
			dNchdySum[centIndex(cent)][enIndex(collEn)] 		
			 += dNchdy[centIndex(cent)][enIndex(collEn)][partIndex(part)];
			dNchdySum_errSq[centIndex(cent)][enIndex(collEn)] 	
			 += dNchdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)]*
				dNchdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)];
			dNchdySum_err[centIndex(cent)][enIndex(collEn)]		
			 = TMath::Sqrt(dNchdySum_errSq[centIndex(cent)][enIndex(collEn)]);
		}
		else if(partIndex(part)==6 || partIndex(part)==7){
			dETdySum[centIndex(cent)][enIndex(collEn)]
			 += dETdy[centIndex(cent)][enIndex(collEn)][partIndex(part)];
			dETdySum_errSq[centIndex(cent)][enIndex(collEn)]
			 += (dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)])*
				(dETdyErr[centIndex(cent)][enIndex(collEn)][partIndex(part)]);
			// ^ since num of k0_s ~ num of k0_l = num of k+ or k-
				// also, num of p ~ num of pbar ~ num of n or nbar
			dETdySum_err[centIndex(cent)][enIndex(collEn)]
			 = TMath::Sqrt(dETdySum_errSq[centIndex(cent)][enIndex(collEn)]);
		}
	} //end of for loop with index j - captured all 270+70 rows from data file
	in1.close();// all information from file extracted, so it should be closed
	
	// accomodate results for last 4 centralities for lambdas and antilambdas,
	 // i.e., cents 5, 6, 7 and 8:
	in1.open(Form("./lambdasInterpWithErr_y.dat"));
	// read and neglect the column headers of the...
	 // interpolated lambdas table in lambdasInterpWithErr.dat:
	for(int l=0;l<9;l++)
	{ // loop through 9 column names
		in1 >> skipContent; 
	}
	for(int j=0; j<5; j++)
	{
		// temporary double to hold input stream value
		Double_t dETdySumOrErr; 
		in1 >> collEn;
		for(int i=0; i<4; i++)
		{// first four data columns contain dETdy sum
			// for all the lambdas and the antilambdas
			in1 >> dETdySumOrErr;
			dETdySum[i+5][enIndex(collEn)] += dETdySumOrErr;
		}
		for(int i=0; i<4; i++)
		{// last four data columns contain dETdy_err (combined)
			// for all the lambdas and the antilambdas
			in1 >> dETdySumOrErr;
			dETdySum_errSq[i+5][enIndex(collEn)]
			 += dETdySumOrErr * dETdySumOrErr;
			dETdySum_err[i+5][enIndex(collEn)]
			 = TMath::Sqrt(dETdySum_errSq[i+5][enIndex(collEn)]);
		}
	}
	in1.close();
	// print dETdySum for 9 centralities and 5 collision energies
		// and assign values to dETdyOverNpartBy2Sum[i][j]
	// for quantity q = a/(b/2) (such as dETdyOverNpartBy2Sum)
	 // q_err = 2*sqrt((a_err/b)^2 + (-a*b_err/(b^2))^2)
		for(int i=0; i<CENTS; i++){
			for(int j=0; j<COLLENS; j++){
				cout << "dETdySum["<<i<<"]["<<j<<"] 	= "<<dETdySum[i][j]<<endl;
				cout << "dNchdySum["<<i<<"]["<<j<<"] 	= "<<dNchdySum[i][j]<<endl;
				dETdyOverNpartBy2Sum[i][j] 		= dETdySum[i][j]/(Npart[i][j]/2.);
				dETdyOverNpartBy2Sum_err[i][j]	= 2.*TMath::Sqrt(
														(dETdySum_err[i][j]/Npart[i][j])*
														(dETdySum_err[i][j]/Npart[i][j])
																 +
														(-dETdySum[i][j]*Npart_err[i][j]/
															(Npart[i][j]*Npart[i][j]))*
														(-dETdySum[i][j]*Npart_err[i][j]/
															(Npart[i][j]*Npart[i][j]))
																 );
				dETdyOverdNchdySum[i][j] 		= dETdySum[i][j]/dNchdySum[i][j];// FIXME??
																	// graph does not look right
				dETdyOverdNchdySum_err[i][j]	= TMath::Sqrt(
														(dETdySum_err[i][j]/dNchdySum[i][j])*
														(dETdySum_err[i][j]/dNchdySum[i][j])
																 +
														(-dETdySum[i][j]*dNchdySum_err[i][j]/
															(dNchdySum[i][j]*dNchdySum[i][j]))*
														(-dETdySum[i][j]*dNchdySum_err[i][j]/
															(dNchdySum[i][j]*dNchdySum[i][j]))
																 );
			}
		}
		
	/// ------ begin - plot all snn graphs at once -------------------------------//
	for(int centInd=0; centInd<CENTS; centInd++){ // loop through all centralities
		centArr[centInd] = centInd*1.0;
		Double_t dETdyOverNpartBy2SumCentByCent[COLLENS];// one graph per centInd
		Double_t dETdyOverNpartBy2SumCentByCent_err[COLLENS];
		Double_t dETdyOverdNchdySumCentByCent[COLLENS];
		Double_t dETdyOverdNchdySumCentByCent_err[COLLENS];
		for(int i=0; i<COLLENS; i++){ 
		// loop through all collEns for current centrality
		// because TGraphErrors does not take a 2d array as argument
			dETdyOverNpartBy2SumCentByCent[i] = dETdyOverNpartBy2Sum[centInd][i];
			dETdyOverNpartBy2SumCentByCent_err[i] = dETdyOverNpartBy2Sum_err[centInd][i];
			dETdyOverdNchdySumCentByCent[i] = dETdyOverdNchdySum[centInd][i];
			dETdyOverdNchdySumCentByCent_err[i] = dETdyOverdNchdySum_err[centInd][i];
		}
		TGraphErrors* g1;
		g1 = new TGraphErrors(COLLENS, collEnArr, dETdyOverNpartBy2SumCentByCent,
					NULL, dETdyOverNpartBy2SumCentByCent_err);
				 // ^ NULL for axis without available errors
		// unique identifier to be used in function:
		g1 -> SetName("dETdyOverNpartBy2Sum_vs_en"); 
		formatGraph(g1, centArr, centInd);
		delete g1;	
	
		// TODO make a function to take care of different types of graph instead of creating new objects here
		TGraphErrors* g2;
		g2 = new TGraphErrors(COLLENS, collEnArr, dETdyOverdNchdySumCentByCent,
					NULL, dETdyOverdNchdySumCentByCent_err);
		g2 -> SetName("dETdyOverdNchdySum_vs_en"); // unique identifier to be used in function:
		formatGraph(g2, centArr, centInd);
		delete g2;
		
	} // end of for loop with index centInd
	/// ------ end - plot all snn graphs at once -------------------------------//
	
	Double_t NpartArrEnByEn[CENTS]; // for each collEn, there is a unique Npart Array
	// unlike for the snn graphs in which 
	// for each cent, there is the same collEnArr
	// hence, the above array should be used in a loop instead of directly as in
	// the usage of collEnArr.
	Double_t NpartArrEnByEn_err[CENTS];
	
	
	/// ------ begin - plot all Npart graphs at once---------------------/// 
	for(int enInd=0; enInd<COLLENS; enInd++){ // loop through all coll Ens
		Double_t dETdyOverNpartBy2SumEnByEn[CENTS];// one graph per collEn;
		Double_t dETdyOverNpartBy2SumEnByEn_err[CENTS];
		Double_t dETdyOverdNchdySumEnByEn[CENTS];	// cents = num of Nparts
		Double_t dETdyOverdNchdySumEnByEn_err[CENTS];
		for(int i=0; i<CENTS; i++){ // loop through all cents/Nparts for current collEn
		// because TGraphErrors does not take a 2d array as argument
			dETdyOverNpartBy2SumEnByEn[i] = dETdyOverNpartBy2Sum[i][enInd];
			dETdyOverNpartBy2SumEnByEn_err[i] = dETdyOverNpartBy2Sum_err[i][enInd];
			dETdyOverdNchdySumEnByEn[i] = dETdyOverdNchdySum[i][enInd];
			dETdyOverdNchdySumEnByEn_err[i] = dETdyOverdNchdySum_err[i][enInd];
			NpartArrEnByEn[i]				= Npart[i][enInd];
			NpartArrEnByEn_err[i]			= Npart_err[i][enInd];
		}
		TGraphErrors* g1; // okay to use identifier g1 in local scope as was the case in previous use
		g1 = new TGraphErrors(CENTS, NpartArrEnByEn, dETdyOverNpartBy2SumEnByEn,
					NpartArrEnByEn_err, dETdyOverNpartBy2SumEnByEn_err);
		g1 -> SetName("dETdyOverNpartBy2Sum_vs_Npart"); // unique identifier for graph
														// see use in function formatGraph
		formatGraph(g1, collEnArr, enInd);
		
		delete g1;
		
		TGraphErrors* g2;
		g2 = new TGraphErrors(CENTS, NpartArrEnByEn, dETdyOverdNchdySumEnByEn,
						NpartArrEnByEn_err, dETdyOverdNchdySumEnByEn_err);
		g2 -> SetName("dETdyOverdNchdySum_vs_Npart");
		formatGraph(g2, collEnArr, enInd);
		delete g2;
		
		
	} // end of for loop with index enInd
	/// ------ end - plot all npart graphs at once -------------------------------//
	delete f;
	return 0;
} // end of main function

void formatGraph(TGraphErrors* g, Double_t collEn_Or_NpartArr[], int en_Or_centInd){
	// for reference:
		// g = new TGraph(9, NpartArrEnByEn, dETdyOverNpartBy2SumEnByEn);
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
	g->SetMarkerStyle(1);
	TString snn1("dETdyOverNpartBy2Sum_vs_en"); // name of first kind of
									// graph with sqrt(snn) in the x-axis
	TString snn2("dETdyOverdNchdySum_vs_en"); // name of second kind of
									// graph with sqrt(snn) in the x-axis
	TString npart1("dETdyOverNpartBy2Sum_vs_Npart"); // name of first kind of
												// graph with Npart in the X-axis
	TString npart2("dETdyOverdNchdySum_vs_Npart"); // name of second kind of
												// graph with Npart in the X-axis
	if (g -> GetName()==snn1){
		c -> SetLogx();
		g->SetMarkerColor(kBlue);
		g->GetHistogram()->GetYaxis()-> SetRangeUser(0., 2.50);
		xlabel = "#sqrt{#it{s}_{NN}} (GeV)";
		ylabel = "#LTd#it{E}_{T}/dy#GT/#LT#it{N}_{part}/2#GT (GeV)";
		graphText = centIndToPercent(en_Or_centInd)+" centrality";
		graphName = "dETdyOverNpartBy2SumCent" + std::to_string(en_Or_centInd);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/rapidity/dETdyOverNpartBy2_En/"+graphName+".png";
	}
	else if (g -> GetName()==snn2){
		c -> SetLogx();
		g->SetMarkerColor(kOrange);
		g->GetHistogram()->GetYaxis()-> SetRangeUser(0., 1.50);
		xlabel = "#sqrt{#it{s}_{NN}} (GeV)";
		ylabel = "#LTd#it{E}_{T}/dy#GT/#LTd#it{N}_{ch}/dy#GT (GeV)";
		graphText = centIndToPercent(en_Or_centInd)+" centrality";
		graphName = "dETdyOverdNchdySumCent" + std::to_string(en_Or_centInd);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/rapidity/dETdyOverdNchdy_En/"+graphName+".png";
	}
	else if (g -> GetName()==npart1){
		g->SetMarkerColor(kGreen);
		g->GetHistogram()->GetYaxis()-> SetRangeUser(0., 2.30);
		xlabel = "#it{N}_{part}";
		ylabel= "#LTd#it{E}_{T}/dy#GT/#LT#it{N}_{part}/2#GT (GeV)";
		graphText = "#sqrt{#it{s}_{NN}} ="+doubToString(collEn_Or_NpartArr[en_Or_centInd])+" GeV";
		graphName = "dETdyOverNpartBy2SumEn" + doubToString(collEn_Or_NpartArr[en_Or_centInd]);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/rapidity/dETdyOverNpartBy2_Npart/"+graphName+".png";
	}
	else if (g -> GetName()==npart2){
		g->SetMarkerColor(kRed);
		g->GetHistogram()->GetYaxis()-> SetRangeUser(0., 1.50);
		xlabel = "#it{N}_{part}";
		ylabel= "#LTd#it{E}_{T}/dy#GT/#LTd#it{N}_{ch}/dy#GT (GeV)";
		graphText = "#sqrt{#it{s}_{NN}} ="+doubToString(collEn_Or_NpartArr[en_Or_centInd])+" GeV";
		graphName = "dETdyOverdNchdySumEn" + doubToString(collEn_Or_NpartArr[en_Or_centInd]);
		imgPathAndName = 
		"./finalPlots/crossCheckPlots/rapidity/dETdyOverdNchdy_Npart/"+graphName+".png";
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
	const char* graphNameConstCharPtr = graphName.c_str();// required for f->Write(arg)
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
	// because graphs, unlike histograms, need to be explicitly written to TFile:
	g -> Write(graphNameConstCharPtr); 
	delete t1;
	delete c;
	delete png;
}

int enIndex(Double_t collEn)
{
	int enIndex;
	// energy set: {7.7,11.5,19.6,27,39}
	if (collEn == 7.7) enIndex = 0;
	else if (collEn == 11.5) enIndex = 1;
	else if (collEn == 19.6) enIndex = 2;
	else if (collEn == 27) enIndex = 3;
	else if (collEn == 39) enIndex = 4;
	else{cout << "Check collision energy value!" << endl; return 888;}
	return enIndex;
}

int centIndex(Double_t cent)
{
	int centIndex;
	if(cent == 0.) centIndex = 0;
	else if(cent == 1.) centIndex = 1;
	else if(cent == 2.) centIndex = 2;
	else if(cent == 3.) centIndex = 3;
	else if(cent == 4.) centIndex = 4;
	else if(cent == 5.) centIndex = 5;
	else if(cent == 6.) centIndex = 6;
	else if(cent == 7.) centIndex = 7;
	else if(cent == 8.) centIndex = 8;
	else{cout << "Check centrality value!" << endl; return 7777;}
	return centIndex;
}

int partIndex(string part)
{
	int partIndex;
	// particle set: {ka-,ka+,pi-,pi+,pro,pba}
	if 		(part == "pi-") partIndex = 0;
	else if (part == "pi+") partIndex = 1;
	else if (part == "ka-") partIndex = 2;
	else if (part == "ka+") partIndex = 3;
	else if (part == "pro") partIndex = 4;
	else if (part == "pba") partIndex = 5;
	else if (part == "la_") partIndex = 6;
	else if (part == "ala") partIndex = 7;
	return partIndex;
}
