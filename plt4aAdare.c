// macro to create the PHENIX part of the plot 4a from Adare et al. 2016
// for only 5 different collision energies relevant for comparison
// and save it in a root file:
// 7.7, 14.5, 19.6, 27, 39

int plt4aAdare()
{
	int const COLLENS = 5;
	double collEnArr[COLLENS] = {7.7, 14.5, 19.6, 27, 39};
	double dETdEtaOverNpartBy2[COLLENS] = {0.83,1.19,1.38,1.57,1.79};
	double dETdEtaOverNpartBy2_err[COLLENS] = {0.06,0.08,0.09,0.10,0.11};
	TFile* f = new TFile("plt4aAdare.root","RECREATE");
	TGraphErrors* g = new TGraphErrors(COLLENS, collEnArr, dETdEtaOverNpartBy2,
					NULL, dETdEtaOverNpartBy2_err);
	g->Write("plt4aAdare_PHENIX");
	
	delete g;
	delete f;
	
return 0;
}
