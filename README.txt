Description of contents in the repository:

-1. directory: oldVersions
	- contains codes and results from the past
	- not deleted because these codes contain 
		functionalities that may be relevant in the future
		
0. directory: finalPlots
	- contains subdirectories containing cross-check plots and new plots

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


*******************************************************************************
Current debugging note:
	- available BES data contains d^2N/(2pi*pt*dpt*dy)[(GeV/c)^-2] in pt bins
	- taken care of in fitBESData5.h: Double_t dNdpt_normalized			= 2 * TMath::Pi() * pt * norm * dNdptOverpt+type*0.;
	- need to apply similar transformation in calculating integral from data points
