// Read fitResults5.dat
	// manipulate the data according to what plots are needed
// write new table in format suitable to easily create required plots

// content to be created:

/*
1. 3by5 matrix for a particular centrality, say 0:
	1.1 column with energy values, i.e., 5 rows
	1.2	corresponding to each energy, add the 
		values of (dET/dEta) and divide the sum by (Npart/2)
		This quotient goes into the corresponding row in the
		second column
	1.3 corresponding to each energy, add the 
		values of (dET/dEta) and divide the sum by the sum of
		dN/dEta
		This quotient goes into the corresponding row in the
		third column

*/
