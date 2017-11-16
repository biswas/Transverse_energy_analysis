 1 // @(#)root/tmva $Id$
    2 /**********************************************************************************
    3  * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
    4  * Package   : TMVA                                                               *
    5  * Root Macro: TMVAClassification                                                 *
    6  *                                                                                *
    7  * This macro provides examples for the training and testing of the               *
    8  * TMVA classifiers.                                                              *
    9  *                                                                                *
   10  * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
   11  * and linearly correlated input variables.                                       *
   12  *                                                                                *
   13  * The methods to be used can be switched on and off by means of booleans, or     *
   14  * via the prompt command, for example:                                           *
   15  *                                                                                *
   16  *    root -l ./TMVAClassification.CUndefined control sequence \"                     *
   17  *                                                                                *
   18  * (note that the backslashes are mandatory)                                      *
   19  * If no method given, a default set of classifiers is used.                      *
   20  *                                                                                *
   21  * The output file "TMVA.root" can be analysed with the use of dedicated          *
   22  * macros (simply say: root -l <macro.C>), which can be conveniently              *
   23  * invoked through a GUI that will appear at the end of the run of this macro.    *
   24  * Launch the GUI via the command:                                                *
   25  *                                                                                *
   26  *    root -l ./TMVAGui.C                                                         *
   27  *                                                                                *
   28  * You can also compile and run the example with the following commands           *
   29  *                                                                                *
   30  *    make                                                                        *
   31  *    ./TMVAClassification <Methods>                                              *
   32  *                                                                                *
   33  * where: <Methods> = "method1 method2"                                           *
   34  *        are the TMVA classifier names                                           *
   35  *                                                                                *
   36  * example:                                                                       *
   37  *    ./TMVAClassification Fisher LikelihoodPCA BDT                               *
   38  *                                                                                *
   39  * If no method given, a default set is of classifiers is used                    *
   40  **********************************************************************************/
   41 
   42 #include <cstdlib>
   43 #include <iostream>
   44 #include <map>
   45 #include <string>
   46 
   47 #include "TChain.h"
   48 #include "TFile.h"
   49 #include "TTree.h"
   50 #include "TString.h"
   51 #include "TObjString.h"
   52 #include "TSystem.h"
   53 #include "TROOT.h"
   54 
   55 #include "TMVA/Factory.h"
   56 #include "TMVA/Tools.h"
   57 #include "TMVA/TMVAGui.h"
   58 
   59 int TMVAClassification( TString myMethodList = "" )
   60 {
   61    // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   62    // if you use your private .rootrc, or run from a different directory, please copy the
   63    // corresponding lines from .rootrc
   64 
   65    // methods to be processed can be given as an argument; use format:
   66    //
   67    // mylinux~> root -l TMVAClassification.CUndefined control sequence \"
   68    //
   69    // if you like to use a method via the plugin mechanism, we recommend using
   70    //
   71    // mylinux~> root -l TMVAClassification.CUndefined control sequence \"
   72    // (an example is given for using the BDT as plugin (see below),
   73    // but of course the real application is when you write your own
   74    // method based)
   75 
   76    //---------------------------------------------------------------
   77    // This loads the library
   78    TMVA::Tools::Instance();
   79 
   80    // Default MVA methods to be trained + tested
   81    std::map<std::string,int> Use;
   82 
   83    // --- Cut optimisation
   84    Use["Cuts"]            = 1;
   85    Use["CutsD"]           = 1;
   86    Use["CutsPCA"]         = 0;
   87    Use["CutsGA"]          = 0;
   88    Use["CutsSA"]          = 0;
   89    // 
   90    // --- 1-dimensional likelihood ("naive Bayes estimator")
   91    Use["Likelihood"]      = 1;
   92    Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   93    Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   94    Use["LikelihoodKDE"]   = 0;
   95    Use["LikelihoodMIX"]   = 0;
   96    //
   97    // --- Mutidimensional likelihood and Nearest-Neighbour methods
   98    Use["PDERS"]           = 1;
   99    Use["PDERSD"]          = 0;
  100    Use["PDERSPCA"]        = 0;
  101    Use["PDEFoam"]         = 1;
  102    Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
  103    Use["KNN"]             = 1; // k-nearest neighbour method
  104    //
  105    // --- Linear Discriminant Analysis
  106    Use["LD"]              = 1; // Linear Discriminant identical to Fisher
  107    Use["Fisher"]          = 0;
  108    Use["FisherG"]         = 0;
  109    Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
  110    Use["HMatrix"]         = 0;
  111    //
  112    // --- Function Discriminant analysis
  113    Use["FDA_GA"]          = 1; // minimisation of user-defined function using Genetics Algorithm
  114    Use["FDA_SA"]          = 0;
  115    Use["FDA_MC"]          = 0;
  116    Use["FDA_MT"]          = 0;
  117    Use["FDA_GAMT"]        = 0;
  118    Use["FDA_MCMT"]        = 0;
  119    //
  120    // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
  121    Use["MLP"]             = 0; // Recommended ANN
  122    Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
  123    Use["MLPBNN"]          = 1; // Recommended ANN with BFGS training method and bayesian regulator
  124    Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
  125    Use["TMlpANN"]         = 0; // ROOT's own ANN
  126    //
  127    // --- Support Vector Machine 
  128    Use["SVM"]             = 1;
  129    // 
  130    // --- Boosted Decision Trees
  131    Use["BDT"]             = 1; // uses Adaptive Boost
  132    Use["BDTG"]            = 0; // uses Gradient Boost
  133    Use["BDTB"]            = 0; // uses Bagging
  134    Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
  135    Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting 
  136    // 
  137    // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
  138    Use["RuleFit"]         = 1;
  139    // ---------------------------------------------------------------
  140 
  141    std::cout << std::endl;
  142    std::cout << "==> Start TMVAClassification" << std::endl;
  143 
  144    // Select methods (don't look at this code - not of interest)
  145    if (myMethodList != "") {
  146       for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
  147 
  148       std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
  149       for (UInt_t i=0; i<mlist.size(); i++) {
  150          std::string regMethod(mlist[i]);
  151 
  152          if (Use.find(regMethod) == Use.end()) {
  153             std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
  154             for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
  155             std::cout << std::endl;
  156             return 1;
  157          }
  158          Use[regMethod] = 1;
  159       }
  160    }
  161 
  162    // --------------------------------------------------------------------------------------------------
  163 
  164    // --- Here the preparation phase begins
  165 
  166    // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  167    TString outfileName( "TMVA.root" );
  168    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
  169 
  170    // Create the factory object. Later you can choose the methods
  171    // whose performance you'd like to investigate. The factory is 
  172    // the only TMVA object you have to interact with
  173    //
  174    // The first argument is the base of the name of all the
  175    // weightfiles in the directory weight/
  176    //
  177    // The second argument is the output file for the training results
  178    // All TMVA output can be suppressed by removing the "!" (not) in
  179    // front of the "Silent" argument in the option string
  180    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
  181                                                "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
  182 
  183    // If you wish to modify default settings
  184    // (please check "src/Config.h" to see all available global options)
  185    //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
  186    //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
  187 
  188    // Define the input variables that shall be used for the MVA training
  189    // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
  190    // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
  191    factory->AddVariable( "myvar1 := var1+var2", 'F' );
  192    factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );
  193    factory->AddVariable( "var3",                "Variable 3", "units", 'F' );
  194    factory->AddVariable( "var4",                "Variable 4", "units", 'F' );
  195 
  196    // You can add so-called "Spectator variables", which are not used in the MVA training,
  197    // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
  198    // input variables, the response values of all trained MVAs, and the spectator variables
  199    factory->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
  200    factory->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );
  201 
  202    // Read training and test data
  203    // (it is also possible to use ASCII format as input -> see TMVA Users Guide)
  204    TString fname = "./tmva_class_example.root";
  205    
  206    if (gSystem->AccessPathName( fname ))  // file does not exist in local directory
  207       gSystem->Exec("curl -O http://root.cern.ch/files/tmva_class_example.root");
  208    
  209    TFile *input = TFile::Open( fname );
  210    
  211    std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;
  212    
  213    // --- Register the training and test trees
  214 
  215    TTree *signal     = (TTree*)input->Get("TreeS");
  216    TTree *background = (TTree*)input->Get("TreeB");
  217    
  218    // global event weights per tree (see below for setting event-wise weights)
  219    Double_t signalWeight     = 1.0;
  220    Double_t backgroundWeight = 1.0;
  221    
  222    // You can add an arbitrary number of signal or background trees
  223    factory->AddSignalTree    ( signal,     signalWeight     );
  224    factory->AddBackgroundTree( background, backgroundWeight );
  225    
  226    // To give different trees for training and testing, do as follows:
  227    //    factory->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
  228    //    factory->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );
  229    
  230    // Use the following code instead of the above two or four lines to add signal and background
  231    // training and test events "by hand"
  232    // NOTE that in this case one should not give expressions (such as "var1+var2") in the input
  233    //      variable definition, but simply compute the expression before adding the event
  234    //
  235    //     // --- begin ----------------------------------------------------------
  236    //     std::vector<Double_t> vars( 4 ); // vector has size of number of input variables
  237    //     Float_t  treevars[4], weight;
  238    //     
  239    //     // Signal
  240    //     for (UInt_t ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
  241    //     for (UInt_t i=0; i<signal->GetEntries(); i++) {
  242    //        signal->GetEntry(i);
  243    //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
  244    //        // add training and test events; here: first half is training, second is testing
  245    //        // note that the weight can also be event-wise
  246    //        if (i < signal->GetEntries()/2.0) factory->AddSignalTrainingEvent( vars, signalWeight );
  247    //        else                              factory->AddSignalTestEvent    ( vars, signalWeight );
  248    //     }
  249    //   
  250    //     // Background (has event weights)
  251    //     background->SetBranchAddress( "weight", &weight );
  252    //     for (UInt_t ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
  253    //     for (UInt_t i=0; i<background->GetEntries(); i++) {
  254    //        background->GetEntry(i);
  255    //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
  256    //        // add training and test events; here: first half is training, second is testing
  257    //        // note that the weight can also be event-wise
  258    //        if (i < background->GetEntries()/2) factory->AddBackgroundTrainingEvent( vars, backgroundWeight*weight );
  259    //        else                                factory->AddBackgroundTestEvent    ( vars, backgroundWeight*weight );
  260    //     }
  261          // --- end ------------------------------------------------------------
  262    //
  263    // --- end of tree registration 
  264 
  265    // Set individual event weights (the variables must exist in the original TTree)
  266    //    for signal    : factory->SetSignalWeightExpression    ("weight1*weight2");
  267    //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
  268    factory->SetBackgroundWeightExpression( "weight" );
  269 
  270    // Apply additional cuts on the signal and background samples (can be different)
  271    TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
  272    TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";
  273 
  274    // Tell the factory how to use the training and testing events
  275    //
  276    // If no numbers of events are given, half of the events in the tree are used 
  277    // for training, and the other half for testing:
  278    //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
  279    // To also specify the number of testing events, use:
  280    //    factory->PrepareTrainingAndTestTree( mycut,
  281    //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
  282    factory->PrepareTrainingAndTestTree( mycuts, mycutb,
  283                                         "nTrain_Signal=1000:nTrain_Background=1000:SplitMode=Random:NormMode=NumEvents:!V" );
  284 
  285    // ---- Book MVA methods
  286    //
  287    // Please lookup the various method configuration options in the corresponding cxx files, eg:
  288    // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
  289    // it is possible to preset ranges in the option string in which the cut optimisation should be done:
  290    // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable
  291 
  292    // Cut optimisation
  293    if (Use["Cuts"])
  294       factory->BookMethod( TMVA::Types::kCuts, "Cuts",
  295                            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
  296 
  297    if (Use["CutsD"])
  298       factory->BookMethod( TMVA::Types::kCuts, "CutsD",
  299                            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );
  300 
  301    if (Use["CutsPCA"])
  302       factory->BookMethod( TMVA::Types::kCuts, "CutsPCA",
  303                            "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );
  304 
  305    if (Use["CutsGA"])
  306       factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
  307                            "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );
  308 
  309    if (Use["CutsSA"])
  310       factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
  311                            "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
  312 
  313    // Likelihood ("naive Bayes estimator")
  314    if (Use["Likelihood"])
  315       factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood",
  316                            "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );
  317 
  318    // Decorrelated likelihood
  319    if (Use["LikelihoodD"])
  320       factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD",
  321                            "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" );
  322 
  323    // PCA-transformed likelihood
  324    if (Use["LikelihoodPCA"])
  325       factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA",
  326                            "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 
  327 
  328    // Use a kernel density estimator to approximate the PDFs
  329    if (Use["LikelihoodKDE"])
  330       factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE",
  331                            "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 
  332 
  333    // Use a variable-dependent mix of splines and kernel density estimator
  334    if (Use["LikelihoodMIX"])
  335       factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX",
  336                            "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 
  337 
  338    // Test the multi-dimensional probability density estimator
  339    // here are the options strings for the MinMax and RMS methods, respectively:
  340    //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
  341    //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
  342    if (Use["PDERS"])
  343       factory->BookMethod( TMVA::Types::kPDERS, "PDERS",
  344                            "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );
  345 
  346    if (Use["PDERSD"])
  347       factory->BookMethod( TMVA::Types::kPDERS, "PDERSD",
  348                            "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );
  349 
  350    if (Use["PDERSPCA"])
  351       factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA",
  352                            "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );
  353 
  354    // Multi-dimensional likelihood estimator using self-adapting phase-space binning
  355    if (Use["PDEFoam"])
  356       factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam",
  357                            "!H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );
  358 
  359    if (Use["PDEFoamBoost"])
  360       factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoamBoost",
  361                            "!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );
  362 
  363    // K-Nearest Neighbour classifier (KNN)
  364    if (Use["KNN"])
  365       factory->BookMethod( TMVA::Types::kKNN, "KNN",
  366                            "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );
  367 
  368    // H-Matrix (chi2-squared) method
  369    if (Use["HMatrix"])
  370       factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V:VarTransform=None" );
  371 
  372    // Linear discriminant (same as Fisher discriminant)
  373    if (Use["LD"])
  374       factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );
  375 
  376    // Fisher discriminant (same as LD)
  377    if (Use["Fisher"])
  378       factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );
  379 
  380    // Fisher with Gauss-transformed input variables
  381    if (Use["FisherG"])
  382       factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );
  383 
  384    // Composite classifier: ensemble (tree) of boosted Fisher classifiers
  385    if (Use["BoostedFisher"])
  386       factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", 
  387                            "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );
  388 
  389    // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
  390    if (Use["FDA_MC"])
  391       factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
  392                            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );
  393 
  394    if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
  395       factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
  396                            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );
  397 
  398    if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
  399       factory->BookMethod( TMVA::Types::kFDA, "FDA_SA",
  400                            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
  401 
  402    if (Use["FDA_MT"])
  403       factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
  404                            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );
  405 
  406    if (Use["FDA_GAMT"])
  407       factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
  408                            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );
  409 
  410    if (Use["FDA_MCMT"])
  411       factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT",
  412                            "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );
  413 
  414    // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
  415    if (Use["MLP"])
  416       factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );
  417 
  418    if (Use["MLPBFGS"])
  419       factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );
  420 
  421    if (Use["MLPBNN"])
  422       factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators
  423 
  424    // CF(Clermont-Ferrand)ANN
  425    if (Use["CFMlpANN"])
  426       factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  
  427 
  428    // Tmlp(Root)ANN
  429    if (Use["TMlpANN"])
  430       factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...
  431 
  432    // Support Vector Machine
  433    if (Use["SVM"])
  434       factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
  435 
  436    // Boosted Decision Trees
  437    if (Use["BDTG"]) // Gradient Boost
  438       factory->BookMethod( TMVA::Types::kBDT, "BDTG",
  439                            "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );
  440 
  441    if (Use["BDT"])  // Adaptive Boost
  442       factory->BookMethod( TMVA::Types::kBDT, "BDT",
  443                            "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
  444 
  445    if (Use["BDTB"]) // Bagging
  446       factory->BookMethod( TMVA::Types::kBDT, "BDTB",
  447                            "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );
  448 
  449    if (Use["BDTD"]) // Decorrelation + Adaptive Boost
  450       factory->BookMethod( TMVA::Types::kBDT, "BDTD",
  451                            "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );
  452 
  453    if (Use["BDTF"])  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
  454       factory->BookMethod( TMVA::Types::kBDT, "BDTMitFisher",
  455                            "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );
  456 
  457    // RuleFit -- TMVA implementation of Friedman's method
  458    if (Use["RuleFit"])
  459       factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
  460                            "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );
  461 
  462    // For an example of the category classifier usage, see: TMVAClassificationCategory
  463 
  464    // --------------------------------------------------------------------------------------------------
  465 
  466    // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events
  467 
  468    // ---- STILL EXPERIMENTAL and only implemented for BDT's ! 
  469    // factory->OptimizeAllMethods("SigEffAt001","Scan");
  470    // factory->OptimizeAllMethods("ROCIntegral","FitGA");
  471 
  472    // --------------------------------------------------------------------------------------------------
  473 
  474    // ---- Now you can tell the factory to train, test, and evaluate the MVAs
  475 
  476    // Train MVAs using the set of training events
  477    factory->TrainAllMethods();
  478 
  479    // ---- Evaluate all MVAs using the set of test events
  480    factory->TestAllMethods();
  481 
  482    // ----- Evaluate and compare performance of all configured MVAs
  483    factory->EvaluateAllMethods();
  484 
  485    // --------------------------------------------------------------
  486 
  487    // Save the output
  488    outputFile->Close();
  489 
  490    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  491    std::cout << "==> TMVAClassification is done!" << std::endl;
  492 
  493    delete factory;
  494 
  495    // Launch the GUI for the root macros
  496    if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );
  497 
  498    return 0;
  499 }
  500 
  501 int main( int argc, char** argv )
  502 {
  503    // Select methods (don't look at this code - not of interest)
  504    TString methodList; 
  505    for (int i=1; i<argc; i++) {
  506       TString regMethod(argv[i]);
  507       if(regMethod=="-b" || regMethod=="--batch") continue;
  508       if (!methodList.IsNull()) methodList += TString(","); 
  509       methodList += regMethod;
  510    }
  511    return TMVAClassification(methodList); 
  512 }

Sub Main

End Sub

Sub Main

End Sub

Sub Main

End Sub
