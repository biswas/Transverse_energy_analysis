The Large Hadron Collider (LHC) at CERN and the Relativistic Heavy Ion Collider (RHIC) at the Brookhaven National Laboratory have the ability to collide heavy nuclei, such as those of gold and uranium, at nearly the speed of light, reaching temperatures of trillions of degrees Celcius. These laboratories have provided evidence of the formation of an exotic state of matter, called the quark-gluon plasma (QGP). It only exists for a brief amount of time after such collisions and instantly freezes out into a plethora of new particles, which carry the signatures we can use to deduct QGP properties. It reportedly behaves like an almost perfect quantum fluid with no resistance and exhibits other interesting properties.

One of the methods to probe the properties of this matter is by analyzing the conversion of the beam-direction energy at the time of collision into transverse energy after the collision. This analysis is generally done by using data from the calorimeters placed around the collision site. Under Dr. Christine Nattrass in the Relativistic Heavy Ion Physics (RHIP) group at the University of Tennessee (UT), I am developing C++ codes for a new method to perform the transverse energy analysis by using tracking detectors instead of the conventional calorimeters.

This method involves a manipulation of the transverse momentum spectra, utilizing the mathematical tools pertaining to heavy-ion collision physics, to calculate the transverse energy production. Due to the limited capabilities of the detectors, however, there is a lack of available data points corresponding to low and high momenta. I use the Boltzmann-Gibbs blast wave function to perform a non-linear regression on the available data points and extract the good-fit parameters and other statistics to extrapolate the given spectrum into the low- and high- momentum regions. CERN's ROOT, an object-oriented framework for large-scale data analysis, comes handy in this regard.

*********************************************************************************************
Tutorial sequence:

1. root BESDataToRootFile.cpp
2. source analyzeAllHistos.bash (~ 5 mins)
3. root BESLambdasToRootFile_TH1.cpp
4. root fitAllHistosInTFile.cpp (~ 5 mins)
5. root interpLaCentTGE.cpp
6. root interpLaCentTGE_y.cpp
7. root stackFinalPlots.C
8. root stackFinalPlots_y.C
*********************************************************************************************
Descriptionsof methods available in fitBESData5.h
*********************************************************************************************
__________________________________________
Description (incomplete) of contents in the repository:
-----------------------------------------
directory: oldVersions
	- created before learning git
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
	- ordinate is 1/(2pi*pT) * dN/dpT as opposed to dN/dpT
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
	- change the definition of variable stop to select histogram

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

11. fitResults5.dat
	- contains results from 7.

12. plotsFromResults3.cpp
	- this is an older version with limited functionality; find newer version below
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
	- old data; v2 in 31.
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
		4. histoName in if(histoName != "cent7_ka-_Au+Au_7.7")
			- (depending on the histoName) particleID and its dependent variables
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
	- ordinate is dN/dpT as opposed to 1/(2pi*pT) * dN/dpT

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
	- header file used by 29 or 34.
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
34. fitALICE2013Data_v2_transf.cpp

35. BES_strangeness_data.txt
	- BES data for strange particles with headers in the correct input format
	- data without bins and errors are only statistical
36. BES_lambdas.txt
	- subset of BES_strangeness_data.txt with only lambdas and antilambdas
37. BESLambdasToRootFile.cpp
	- creates BESLambdas.root (containing TGraphErrors objects) using BES_lambdas.txt
38. BESLambdas.root
	- contains TGraphErrors objects created from data in BES_lambdas.txt

39. chkTGEbins.C
	- intermediate macro to estimate the nature of the algorithm to bin unbinned lambdas
	- details at the beginning of the code file

40. BESLambdasToRootFile_TH1.cpp
	- modification of BESLambdasToRootFile.cpp
	- produces BESLambdas_optBins.root
	- uses algorithm in function "GetOptimalBinEdges(const vector<Double_t> &xV)"...
	- ... to get a vector of optimal bin edges using the available abscissas

41. BESLambdas_optBins.root
	- contains histograms corresponding to BES lambdas spectra

42. fitAllHistosInTFile.cpp
	- evolved from fitBESData5_1.cpp
	- details found at the beginning of the file
	- attempt to make the fitting process more generic
	- used to fit lambdas spectra (from BESLambdas_optBins.root)

43. fitResults_la.dat
	- result of using fitAllHistosInTFile.cpp on BESLambdas_optBins.root

44. interpLaCentTGE.cpp
	- interpolates lambdas ET to centrality bins not available for lambdas,
	- ... but present in the data for the rest of the particles
	- pseudorapidity coordinate
45. lambdasInterpWithErr.dat
	- result of interpolateLaCent.cpp

46. inerpLaCentTGE_y.cpp
	- same as interpLaCentTGE.cpp but for rapidity coordinate

47. lambdasInterpWithErr_y.dat
	- same as lambdasInterpWithErr.dat but for rapidity coordinate

48. finalPlots_TGE.cpp
	- uses results available in fitResults5.dat and fitResults_la.dat to
	- .. produce individual final plots and save them in crossCheckGraphs.root
	- pseudorapidity coordinate

49. finalPlots_TGE_y.cpp
	- same as finalPlots_TGE.cpp but for rapidity coordinate
	- results in crossCheckGraphs_y.root

50. crossCheckGraphs.root
	- contains individual final plots as produced by finalPlots_TGE.cpp

51. crossCheckGraphs_y.root
	- contains individual final plots as produced by finalPlots_TGE_y.cpp

52. stackFinalPlots.C
	- macro to create final stacked plots (thesis) in pseudorapidity coordinate

53. stackFinalPlots_y.C
	- macro to create final stacked plots (thesis) in rapidity coordinates

54. compare*.C
	- macros to compare results with those from other experiments



*******************************************************************************
*******************************************************************************
ToDos:
..................................................................................
- add error bars in the final plots: finalPlots_TGE.cpp
- to the above plots, add lambda spectra from STAR Preliminary data on BES strangeness
	1. clearify what "raw" means as opposed to "pT" in one of the directories, 
	 and what cut30 and cut0 mean
	 -> later
	 	

..................................................................................
- add errors to cross-check plots and see if they match better with:
https://arxiv.org/pdf/1509.06727.pdf#page=12
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

Current debugging note:
..................................................................................


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------



Past ToDos and debugging notes:
..................................................................................
	2. DONE --
	modify existing code to read data that is pointwise instead
	 of binned and turn it into TGraphErrors objects instead of TH1D objects
	 2.0 try to read data from all the input files in the directories within
	  a directory recursively -- that did not work, so trying 2.1 (see past 
	  debugging note for why it didn't work) 
	 2.1 copy data from different files into a sinlge file and convert it to
	  almost-original BES data format -- DONE: BES_lambdas.txt 
	 2.2 Turn data in almost-original BES format into TGraphErrors objects
	  in a ROOT TFile object -- DONE: BES_lambdas.root 
	  using BESLambdasToRootFile.cpp
	--> DONE 
	
	3. DONE --
	modify existing code to use the fitting method of the TGraphErrors class
	 instead of the TH1 class
	---> DONE (actually didn't need to use this method after all; constructed bins)
	 3.1 See if the methods (args) in the header need to be modified: DONE
		- yes, they need to me modified
		- spawned file fitSampleSpec_TH_TGE.cpp from fitSampleSpec.cpp
		- From fitBESData5.h spawned fitSpec.h
	 3.2 Add/modify methods in fitSpec.h
		3.2.1 Modify method to estimate integral from data points
		-> alternate solution to use: since the bins are not clear,
			just obtain fitting curves (3.2) for TGraphs mentioned in 3.2.1.1 
			and then use the Integral() method of the TF1 class to
			implement 3.2.1.2
		 - Inside the method getIntegralsAndErrorsFromData:
		 3.2.1.1 create TGraph objects corresponding to:
			3.2.1.1.1 y-values
			3.2.1.1.2 y-value + err
			3.2.1.1.3 y-value - err
		 3.2.1.2 find integrals corresponding to 3.2.1.1.*, then:
			- integral from data points = 3.2.1.1.1
			- integral err+ = 3.2.1.1.2 - 3.2.1.1.1
			- integral err- = 3.2.1.1.1 - 3.2.1.1.3
		3.2.2 Modify method to 
	 3.2 Modify fitSampleSpec.cpp and test on individual spectra
	 	-> file created: fitSampleSpec_TH_TGE.cpp
	 3.3 Modify fitBESData5_1. cpp 

	 
	4. DONE by interpolating -- 
	figure out the correct method to take care of the fact that the lambda 
	 spectra are available for slightly different centralities than the rest
	 of the spectra; specifically, lambda spectra have centralities 40-60 and
	 60-80 instead of 40-50, 50-60, 60-70, and 70-80; guess: combine the
	 relevant centralities in the rest of the data to match lambda
	 centralities; combine what quantities associated with the centralities
	 though? the spectra (in which case, how do the errors add up?) 
	 or the end results (in which case also, how do the errors add up)?
	 -> for quantities k1 and k2 that add up as k = k1 + k2 and have
	  associated uncertainties dk1 and dk2, the uncertainty in k is given
	  by dk = sqrt(dk1^2 + dk2^2)

..................................................................................
3. SCRATH THAT, JUST CONVERT THE GRAPHS TO HISTOGRAMS BECAUSE THAT'S WHAT THEY
ARE SUPPOSED TO BE:
	 
	3. SCRATCH THAT, LAMBDAS DON'T CONTRIBUTE THAT MUCH TO THE ET, SO THAT
	MUCH "ACCURACY" IS NOT NECESSARY 
	Convert the TGraphErrors objets to TH1 objects because that's what
		they are supposed to be
		- algorithm: except for i=n, where n is the number of points
		 in the graph, take the mid-point between the ith and the (i+1)th
		 x-values 

..................................................................................

- was trying to recursively read input files from a specified directory using
 gcc's "#include <experimental/filesystem>", but that pointed to errors
 in gcc's header

/usr/lib/gcc/x86_64-redhat-linux/6.4.1/../../../../include/c++/6.4.1/bits/locale_conv.h:33:2: error: 
      unterminated conditional directive
#if __cplusplus < 201103L
 ^
/usr/lib/gcc/x86_64-redhat-linux/6.4.1/../../../../include/c++/6.4.1/bits/locale_conv.h:30:2: error: 
      unterminated conditional directive
#ifndef _LOCALE_CONV_H
 ^
 it can probably be fixed by editing the headers by just terminating the
  conditional directives, but instead of doing  that, which might result in
  other problems and consume a lot of time in any case, I'll either just read
  the files one by one by manually customizing the input filename or copy the
  data in the files into one data file in the original BES data format
..................................................................................
- Need to fix the zeroes that appear in integral error calculation in the
following rows in the result:

5	7.7		pi-	3 STATUS=FAILED, CONVERGED with alt par
6	7.7		pi-	4 STATUS=FAILED, CONVERGED with alt par
7	7.7		pi-	5 STATUS=FAILED, CONVERGED with alt par
9	7.7		pi-	7 STATUS=FAILED,	OK with alt par
28	7.7		ka-	8 STATUS=CALL LIMIT, CONVERGED with alt par
37	7.7		ka+	8 STATUS=CALL LIMIT, CONVERGED with alt par
39	7.7		pba	1 STATUS=CALL LIMIT, NOT POS-DEF, but err fixed
41	7.7		pba	3 STATUS=CALL LIMIT, CONVERGED with alt par
42	7.7		pba	4 OK with alt par
44	7.7		pba	6 insufficient iter, but err fixed
45	7.7		pba	7 NOT POS-DEF, but err fixed
46	7.7		pba	8 with alt2 par STATUS=FAILED, but dETdyLErr: 0.000557218
52	7.7		pro	5 CONVERGED with alt par
62	11.5	pi-	6 STATUS=FAILED, CONVERGED with alt par
65	11.5	pi+	0 CONVERGED with alt par
72	11.5	pi+	7 CONVERGED with alt par
100	11.5	pba	8 CONVERGED in 2215 calls
151	19.6	pba	5 CONVERGED in 2006 calls
154	19.6	pba	8 CONVERGED in 2848 calls
163	19.6	pro	8 CONVERGED with alt par
201	27		pba	1 CONVERGED in 2764 calls
211	27		pro	2 CONVERGED in 2051 calls
259	39		pba	5 CONVERGED in 2023 calls

Strategy 1: analyze individual histograms using 22. fitSampleSpec.cpp
-> problem found: STATUS of minimizing chi-squared for above
histograms = either FAILED or CALL LIMIT, which means the
error matrix is probably problematic in these cases
Strategy 1.1: customize initial fit parameters
- works for some but not all (see comments at the end of each of the data rows above)
- worked for some after changing the maximum number of minimizer function calls to 10000 (default is 1000)
- works for all, now make corresponding changes
in the original fitter code
- some other spectra now giving similar errors.
-> cause: some fitted parameter in each case has unreasonable value/error
- trying to see if customizing parameters helps
27	pi+	6 fixed with alt par
27	ka-	2 "
27	ka+	3 "
27	pba	7 "
39	pi+	8 "
- !!!!!!!!!!Debugged!!!!!!!!!!!!!!!!!!

Strategy 1.2: use maximum likelihood estimate/cross-validation instead of minimum chi-square
- as elaborated in https://arxiv.org/pdf/1012.3754.pdf, chi-square minimization probably not a good way to fit non-linear models anyway!
- maximum likelihood method did not produce good fits - don't worry about it now, but go back to it later (out of curiosity)
- use chi-squre now since it seems to be working after fixes in Strategy 1.1. It's not good for non-linear models in general, but if it's producing good fits with reasonable values for parameters, use it anyway because the interpretation of the goodness of fit matters only as long as it does not imply unusual extrapolations of the transverse energy. One important way for its validation is comparison of results with past publications, and so far it looks good.
..................................................................................
1. Estimated values of dET/dEta lower than those found in publications
	- However, dET/dEta estimates fairly match those in transverse energy analysis note
Debugged: new estimates in agreement with publication
(39 GeV 0-5% central pi- dET/dETA 76.6687, so that times 5 is around 380,
a ballpark upper limit for total dET/dETA, which from publication, 
https://arxiv.org/pdf/1509.06727.pdf#page=25, is 303. Ballpark lower limit
is 76*3, which is around 230, so reasonable.)
..................................................................................
1. BGBW function not producing good fits to pi+ & pi- combined spectrum from:
	http://aliceinfo.cern.ch/ArtSubmission/node/501
	- good fits produced to spectra that are almost the same and available in SPECTRA_COMB_20120709.root
	- the two spectra maily differ in that the latter has some bins in the beginning and at the end with bin content zero, whereas the former does not have those bins altogether
Debug: see if limiting the range of fit to <3 produces similar fits
--> It does not
Hence, the only way the two spectra could have produced such different fits is that the errors are consistently smaller in the former
^ Right! scaled up the errors similarly, and now the fits are good
..................................................................................
-2. ALICE spectra match, but the errors and ETs don't match yet.
Debugged: ETs match within error bars, so they are considered consistent after consultation with Dr. Nattrass.
-1. Around line 102 in ALICE2013DataToRootFile_th1.cpp, trying to check if the j+1 in h->SetBinContent(j+1,binContent[j]) should actually be j.
Debugged: j+1 changed to j, j changed to j-1, loop stop condition changed to j<=binNum, and commented out binEdgesVec.push_back(0.0);
Need to make similar changes to all the codes from the beginning.
Do it with Nathan tomorrow (1/18/2018) so he gets to see what each of the codes does.
Done upto 15. -- DONE

0. pi- and pi+ spectra (ALICE2013Spec_v2_transformed.root) from ALICE 2013 data (31.) lower than those from SPECTRA_COMB_20120709.root as can be compared from 1/13/2018 screenshots (of plots) available in directory debugPlots -- FIXED (errors don't match yet)

..................................................................................
	- available BES data contains d^2N/(2pi*pt*dpt*dy)[(GeV/c)^-2] in pt bins
	- taken care of in fitBESData5.h: Double_t dNdpt_normalized	= 2 * 
		TMath::Pi() * pt * norm * dNdptOverpt+type*0.;
	- need to apply similar transformation in calculating integral from data points
..................................................................................


-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------
First Intro Meeting Notes

- transverse energy concepts and big picture
- TODO: email Ben link to master's thesis
- 
