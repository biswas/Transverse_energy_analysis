// macro to check if all the lambda TGraphErrors have first or last two...
// ... bins that are equal in width. If not, the algorithm to create...
// ... TH1D objects using the TGraphErrors objects will be a...
// ... little bit more complicatied

// conclusion: they are not equal in width; however...
// ... The last bins have the least effect on the results, and...
// ... and it was calculated that the difference between the... 
// ... distance between the third-last and the fourth-last ...
// ... x-values and the distance between the second-last... 
// ... and the third-last x-values was very low...
// ... for each of the spectra (last x-value was not considered...
// ... because it might have been used to represent a bin encompassing...
// ... "everything to the right of" the second-last bin), 

// ... so the distance between the second-last and the third-last...
// ... x-values will be used as the size of the second-last bin,...
// ... after which the width of each of the bin to its left can...
// ... be computed using a simple algorithm:

// Algorithm:
// ... width(i-1) = {dist_x-vals(i, i-1) - 0.5*width(i)}*2, where...

// ... width(i-1) is the width of the (i-1)th bin,...
// ... dist_x-vals(i, i-1) is the distance between the ith and the...
// ... (i-1)th x-values, the first i in the recursion representing...
// ... the second-last x-value. The last bin-width is then obtained...
// ... as: width(n) = {dist_x-vals(n, n-1) - 0.5*(width(n-1))}*2

// !Algorithm

{
// iterate through all the TGraphErrors objects in a given TFile
// objects required: TFile, TKey, TGraphErrors
// first declare everything and then define as required (a la Rademakers)

TFile* rootFile;
TKey* mikey;
TGraphErrors* gr;
int i;

i = 0;
rootFile = new TFile("BESLambdas.root");
TIter next(rootFile->GetListOfKeys());
cout << "#\t" << "Graph Name\t\t"<< "x(N)\t" << "x(N-1)\t" 
	<< "x(N-2)\t" << "T or F\t" << "d(N, N-1)\t"
	<< "d(N-1, N-2)\t" << "diff" << endl;
while((mikey = (TKey*)next()))
{
	gr = (TGraphErrors*)mikey->ReadObj();
	string graphname = gr->GetName();
	Double_t* x1ptr; // pointer to first x-value
	Int_t N; // number of points in the graph
	x1ptr = gr->GetX();
	N = gr->GetN();
	string tOrF;
	if((*(x1ptr+N-2) - *(x1ptr+N-1)) == (*(x1ptr+N-3) - *(x1ptr+N-2))) tOrF = "TRUE";
	else tOrF = "FALSE";
	cout << ++i << ": " << graphname << "\t"
		<< " "
		<< *(x1ptr + N-1-1) << "\t"
		<< *(x1ptr + N-1-2) << "\t"
		<< *(x1ptr + N-1-3) << "\t"
		<< tOrF << "\t"
		<< *(x1ptr + N-1-1) - *(x1ptr + N-1-2) << "\t\t"
		<< *(x1ptr + N-1-2) - *(x1ptr + N-1-3) << "\t\t"
		<< (*(x1ptr + N-1-1) - *(x1ptr + N-1-2))-(*(x1ptr + N-1-2) - *(x1ptr + N-1-3))
		<< endl;
}

}
