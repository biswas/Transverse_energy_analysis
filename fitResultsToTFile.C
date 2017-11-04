TFile *fitResultsToTFile(Int_t get=0, Int_t print=1) {
	Float_t CollEn;	
	Char_t particle;
	Float_t mass;
	Float_t beta;
	Float_t betaErr;
	Float_t temp;
	Float_t tempErr;
	Float_t n;
	Float_t nErr;
	Float_t norm;
	Float_t normErr;
	Float_t dETdEta_data; //dETdEta_d
	Float_t dETdEta_d_err; //dETdEta_d_err
	Float_t dETdEtaLeft;
	Float_t dETdEtaLErr;
	Float_t dETdEtaRight;
	Float_t dETdEtaRErr;
	Float_t dETdEtaTotal; // dETdEtaTotal
	Float_t dETdEtaTErr;
	Float_t dETdy_d; //dETdy_d
	Float_t dETdy_d_err; //dETdy_d_err
	Float_t dETdyLeft;
	Float_t dETdyLErr;
	Float_t dETdyRight;
	Float_t dETdyRErr;
	Float_t dETdyTotal; // dETdyTotal
	Float_t dETdyTErr;
	Float_t dNdEta_d; //dNdEta_d
	Float_t dNdEta_d_err; //dNdEta_d_err
	Float_t dNdEtaLeft;
	Float_t NelErr;
	Float_t dNdEtaRight;
	Float_t NerErr;
	Float_t dNdEtaTotal; // dNdEtaTotal
	Float_t dNdEtaTErr;
	Float_t dNdy_d; //dNdy_d
	Float_t dNdy_d_err; //dNdy_d_err
	Float_t dNdyLeft;
	Float_t dNdyLErr;
	Float_t dNdyRight;
	Float_t dNdyRErr;
	Float_t dNdyTotal; // dNdyTotal
	Float_t dNdyTErr;

   //The input file cern.dat is a copy of the CERN staff data base
   //from 1988
   TString filename = "fitResults.root";
   TString dir = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
   dir.ReplaceAll("fitResultsToFile.C","");
   dir.ReplaceAll("/./","/");
   FILE *fp = fopen(Form("%sfitResults4.dat",dir.Data()),"r");

   TFile *hfile = 0;
   if (get) {
      // if the argument get =1 return the file "cernstaff.root"
      // if the file does not exist, it is created
      if (!gSystem->AccessPathName(dir+"fitResults.root",kFileExists)) {
         hfile = TFile::Open(dir+"fitResults.root"); //in $ROOTSYS/tutorials/tree
         if (hfile) return hfile;
      }
      //otherwise try $PWD/cernstaff.root
      if (!gSystem->AccessPathName("fitResults.root",kFileExists)) {
         hfile = TFile::Open("fitResults.root"); //in current dir
         if (hfile) return hfile;
      }
   }
   //no cernstaff.root file found. Must generate it !
   //generate cernstaff.root in $ROOTSYS/tutorials/tree if we have write access
   if (!gSystem->AccessPathName(dir,kWritePermission)) {
      filename = dir+"fitResults.root";
   } else if (!gSystem->AccessPathName(".",kWritePermission)) {
      //otherwise generate cernstaff.root in the current directory
   } else {
      printf("you must run the script in a directory with write access\n");
      return 0;
   }
}
