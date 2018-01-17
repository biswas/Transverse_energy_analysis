*********************************************************************************************
Descriptions of methods (updated 12/14/2017) available in fitBESData5.h
	- intentional variance (pertaining to transformations) in fitALICE2013Data_transformed.h
	- for details, see the beginning of fitALICE2013Data_transformed.h
*********************************************************************************************
__________________________________________
Description of contents in the repository:
-----------------------------------------
directory: oldVersions
	- contains codes and results from the past
	- not deleted because these codes contain 
		functionalities that may be relevant in the future
		
directory: finalPlots
	- contains subdirectories containing cross-check plots and new plots
	
directory: debugPlots
	- contains plots produced while debugging
	
directory: publication
	- contains (latex) documents under construction for publication

1. BESData_sorted.txt
	- data from the STAR Beam Energy Scan program
	- particle multiplicity** in transverse momentum bins
	- produced from Au+Au collisions
	- at 5 different collision energies: {7.7,11.5,19.6,27,39} Gev
	- data currently available for 6 different particles: 
		pi-, pi+, k-, k+, proton and antiproton
	- recorded at 9 different centralities:
		0-5% (0), 5-10% (1), 10-20% (2), 20-30% (3), ....., 70-80% (8)
	** per unit transverse momentum per unit pseudorapidity
	
2. BESDataToRootFile.cpp
	- organizes 1. into TH1 Objects in a .root file
	
3. BESData.root
	- result of 2.
	
4. fitBESData4_oneByOne_6Params.h
	- contains definitions of functions invoked by 5.

5. fitBESData4_oneByOne_6Params.cpp
	- older version of fitBESData5_1.cpp
	- has the convenient functionality to run said code on individual
		histograms instead of all at once

6. fitResults4_oneByOne.dat
	- result of 5.

7. analyzeAllHistos.bash
	- bash script that runs 8. and 9. in sequence
	- 8. and 9. needed to be separated due to memory issues
	- fits histograms in 3. with the Boltzmann-Gibbs Blast Wave function
	- uses good-fit parameters to estimate quantities used in final plots
	- outputs results into 11.
	- for quick reference to main items that may need to be customized for testing, ...
	- ... see 22. (fitSampleSpec.cpp)

8. fitBESData5_1.cpp
	- used for first 140 histograms
	
9. fitBESData5_2.cpp
	- used for the last 130 histograms
	
10. fitBESData5.h
	- contains definitions of functions used by 8. and 9.

11.  fitResults5.dat
	- contains results from 7.

12. plotsFromResults3.cpp
	- uses 11. to produce cross-check plots and new plots for publication
	- plots saved as .png files in subdirectories under directory finalPlots

13. fitResults5_python.csv
	- same as 11, but in csv format, which is suitable to use for analysis in Python

14. parallelCoordinates.py
	- produce parallel coordinates plot
	- to visualize high-dimensional data present in 13.
	- useful to see outliers and patterns in histogram analysis results
	- resulting plot parallelCoordPlot.png

15. parallelCoordPlot.png
	- result of 14.
	
16. ALICE2013Data.txt
	- combined pi- & pi+ spectrum (0-5% central) from ALICE 2013 data
	- as available in http://aliceinfo.cern.ch/ArtSubmission/node/544
	- layout formatted to match 1.
	- ordinate is 1/(2pi*pT) * dN/dpT as opposed to dN/dpT

17. ALICE2013DataToRootFile_th1.cpp
	- organizes 16. or 31. into a TH1D object in a .root file

18. ALICE2013Spec_th1d.root
	- result of 17. using 16.
	- ordinate is 1/(2pi*pT) * dN/dpT as opposed to dN/dpT

19. SPECTRA_COMB_20120709.root
	- ROOT file with spectra that result to table 2.2 in transverse energy analysis note
	- used to test reasonability of dET/dEta estimates produced by codes in this repository
	- ordinate is dN/dpT as opposed to 1/(2pi*pT) * dN/dpT

20. sampleResults.dat
	- buffer file containing results (to test on the fly) of .cpp codes since 16.

21. ETAnalysisNoteSpecResults.dat
	- like 20. but relevant only to spectra available in 19.

22. fitSampleSpec.cpp
	- Evolved from fitBESData5.cpp
	- Used to analyze any TH1D object within a TFile object by customizing:
		1. the name of the input TFile object myFile
		2. name of the output file (stream) datFile
		3. method to read TH1D object from Tfile depending on how it was created:
			3.1. h = (TH1D*)mikey->ReadObj();
					(only) in this case, string histoName = h->GetName(); makes sense
			3.2. h = (TH1D*)myFile->Get(Form("cent%i_proton_plus",0));
					this is the case applicable to SPECTRA_COMB_20120709.root
		4. (depending on the histoName) particleID and its dependent variables
		5. fit parameters: funcBGBW->SetParameters(mass,0.99,0.30,0.1,1000.,type);
		6. ylabel: ylabel = "#frac{d^{2}N}{dydp_{T}}";
		7. depending on the ylabel, possible transformation to be applied
			under the associated header file (eg. fitALICE2013Data.h) within functions:
			7.1. getIntegralsAndErrorsFromData(TH1D* hist, Double_t type, Double_t mass)
			7.2. getdNdpt(Double_t* pT, Double_t* params)

23. fitSpecFromETAnalasysNote.cpp
	- same as 22, i.e., fitSampleSpec.cpp, but customized specifically...
	- ... to use for SPECTRA_COMB_20120709.root

24. ALICE2013ToRootFile_trans.cpp
	- same as 17. but with a transformation
	- data y-value multiplied by 2pi*pt; pt = ptLow+0.5 (or 0.3) of binWidth
	- transformation applied in order to see if this gives a ...
	- ... spectrum comparable to the ones in 19., i.e., SPECTRA_COMB_20120709.root

25. ALICE2013Spec_transformed.root
	- result of 24, with pt = ptLow+0.5

26. ALICE2013Spec_transformed_0.3.root
	- result of 24, with pt = ptLow+0.3
	
27. fitALICE2013Data1.cpp
	- same as 22. but customized specifically to use for 18.
	
28. fitALICE2013Data.h
	- header file used by 27.

29. fitALICE2013Data1_transformed.cpp
	- same as 22. but customized specifically to use for 25. or 26.
	
30. fitALICE2013Data_transformed.h
	- header file used by 29.
	- accounts for the fact that a transformation had already been applied ...
	- ... in the process of producing the input ROOT file ...
	- ... and hence neglects the transformations in the relevant functions

31. ALICE2013Data_v2.txt
	- data available from http://hepdata.cedar.ac.uk/view/ins1222333
	- reference http://aliceinfo.cern.ch/ArtSubmission/node/501
	- replacement for 16. which had suboptimal data

32. ALICE2013Spec_v2_th1d.root
	- result of 17. using 31.
	- ordinate is 1/(2pi*pT) * dN/dpT [i.e., d^2N/(N_ev*2pi*pt*dpt*dy)]
		as opposed to dN/dpT

33. ALICE2013Spec_v2_transformed.root
	- result of 24. using 31.
	- ordinate is dN/dpT

*******************************************************************************
Current debugging note:

-1. Around line 102 in ALICE2013DataToRootFile_th1.cpp, trying to check if the j+1 in h->SetBinContent(j+1,binContent[j]) should actually be j.
Turns out binContent[j] needed to be changed to binContent[j-1] (even though that did not fix the discrepancy in the pion spectra)
Need to make similar changes to all the codes from the beginning.
Do it with Nathan tomorrow (1/18/2018) so he gets to see what each of the codes does.

0. pi- and pi+ spectra (ALICE2013Spec_v2_transformed.root) from ALICE 2013 data (31.) lower than those from SPECTRA_COMB_20120709.root as can be compared from 1/13/2018 screenshots (of plots) available in directory debugPlots

1. Estimated values of dET/dEta lower than those found in publications
	- However, dET/dEta estimates fairly match those in transverse energy analysis note
	
2. BGBW function not producing good fits to pi+ & pi- combined spectrum from:
	http://aliceinfo.cern.ch/ArtSubmission/node/544
-------------------------------------------------------------------------------

Past debugging notes:
..................................................................................
	- available BES data contains d^2N/(2pi*pt*dpt*dy)[(GeV/c)^-2] in pt bins
	- taken care of in fitBESData5.h: Double_t dNdpt_normalized	= 2 * 
		TMath::Pi() * pt * norm * dNdptOverpt+type*0.;
	- need to apply similar transformation in calculating integral from data points
..................................................................................
