// macro to overlay two graphs on the same canvas for comparison

{
TCanvas *c1 = new TCanvas()
TFile* file1 = TFile::Open("./crossCheckGraphs.root")
TFile* file2 = TFile::Open("./plt4aAdare.root")
TGraphErrors* g1 = (TGraphErrors*)file1->Get("dETdEtaOverNpartBy2SumCent0")
TGraphErrors* g2 = (TGraphErrors*)file2->Get("plt4aAdare_PHENIX")
g1->Draw("A*")
g2->Draw("*Same")
TLegend* leg = new TLegend(0.1,0.7,0.48,0.9)
g1 -> SetName("g1")
g2 -> SetName("g2")
leg -> AddEntry("g1", "STAR 0-5% Au+Au (Spectral)")
leg -> AddEntry("g2", "PHENIX 0-5% Au+Au (Calorimetric)")
leg->Draw()
}
