
//
// main 
// top pt branch
void DoAnalysis(bool OnlyDraw=false) 
{
  // Style
  //gROOT->ProcessLine(".L /Users/jaehyeok/macros/rootlogon.C");
  // Load macros
  int version = 4;
  bool OnlyEssential=true;
  gROOT->LoadMacro("MakeHists.C+");
  gROOT->LoadMacro("Make1DPlots.C+");
  gROOT->LoadMacro("MakeSF.C+");
  gROOT->LoadMacro("Make2DPlots.C+");
  gROOT->LoadMacro("MakeTables.C+");
  char* sysname[] = {"_toppT1","_toppT2","_ISRpT1","_ISRpT2","_ISRpartons"};
  int nsys=4;
  gSystem->Exec(Form("mkdir -p Out/v%i/Figures/",version));
  gSystem->Exec(Form("mkdir -p Out/v%i/HistFiles/",version));
  gSystem->Exec(Form("mkdir -p Out/v%i/Tables/",version));
  
  // ----------------------------------------
  //  Define chains  
  // ----------------------------------------
  TChain *ch_data         = new TChain("tree", "DATA");
  TChain *ch_ttbar_sl     = new TChain("tree", "TT_sl");
  TChain *ch_ttbar_ll     = new TChain("tree", "TT_ll");
  TChain *ch_ttbar_sys     = new TChain("tree", "TT_sys");
  TChain *ch_wjets        = new TChain("tree", "WJets");
  TChain *ch_dy           = new TChain("tree", "DY");
  TChain *ch_t            = new TChain("tree", "T");
  TChain *ch_f1500_100    = new TChain("tree", "T1tttt_f1500_100");
  TChain *ch_f1200_800    = new TChain("tree", "T1tttt_f1200_800");
  
  TString BabyDir = "/Users/heller/Jae/ntuples/HT750MET250/";
    //"/net/cms26/cms26r0/jaehyeok/baby/Fatjet/13TeV/Phys14NoSkim/";
    //"/net/cms26/cms26r0/jaehyeok/baby/Fatjet/13TeV/Phys14/HT750MET250/";
  //"/Users/jaehyeok/Research/Tools/fastjet-3.0.6/example/babies/13TeV/HT750MET250/";
    
  // Data
  //ch_data->Add(BabyDir+"baby_MuHad_*.root");                            
    
  // TT 
  ch_ttbar_sl->Add(BabyDir+"baby_TTJets*.root");
  ch_ttbar_ll->Add(BabyDir+"baby_TTJets*.root");
  ch_ttbar_sys->Add(BabyDir+"baby_TTJets*.root"); 
  // WJets 
  //ch_wjets->Add(BabyDir+"baby_WJetsToLNu*f1To*.root");
  ch_wjets->Add(BabyDir+"baby_WJetsToLNu*.root");
  // DY 
  //ch_dy->Add(BabyDir+"baby_DYJetsToLL*f1To*.root");
  ch_dy->Add(BabyDir+"baby_DYJetsToLL*.root");
  // Singla top 
  ch_t->Add(BabyDir+"baby_*channel*_f*.root");

  // Signal
  ch_f1500_100->Add(BabyDir+"baby_*mGl-1500_mLSP-100*.root");
  ch_f1200_800->Add(BabyDir+"baby_*mGl-1200_mLSP-800*.root");
    
    
  // ----------------------------------------
  //  Get number of entries 
  // ----------------------------------------
  cout << "data               : " << ch_data->GetEntries()        << endl;
  cout << "ttbarl             : " << ch_ttbar_sl->GetEntries()    << endl;
  cout << "ttbarll            : " << ch_ttbar_ll->GetEntries()    << endl;
  cout << "wjets              : " << ch_wjets->GetEntries()       << endl;
  cout << "dy                 : " << ch_dy->GetEntries()          << endl;
  cout << "Single top         : " << ch_t->GetEntries()           << endl;
  cout << "T1tttt(1500,100)   : " << ch_f1500_100->GetEntries()    << endl;
  cout << "T1tttt(1200,8000)  : " << ch_f1200_800->GetEntries()  << endl;
   

  //
  // Loop over SR and CR : make sure that these regions exist in "PassSelection.h"
  //
    
  char* Region[] = {"1BCRincl","1B4SJCRincl","1B45SJ","1B67SJ","1B8SJ","SRincl"};
  int nregion =6;
  int SRthres=4;
  //COPY TO MAKE1DPlots

  
  // char* Region[] = {"SR0", "SR1","SRincl","1BSR1","1BSR0","1BCR0","1B2lCR0","1BCRincl","1B2lCRincl","1B2lCRincl_loose","AllBCRincl"}; 
  //remember loose requires full babies
  for(int iregion=0; iregion<nregion; iregion++)
    {
      if(!OnlyDraw) 
        {
	  // ----------------------------------------
	  //  Fill histrograms 
	  // ----------------------------------------
	  MakeHists(version, ch_data,	    Region[iregion]); 
	  MakeHists(version, ch_ttbar_sl,  Region[iregion]); 
	  MakeHists(version, ch_ttbar_ll,  Region[iregion]);
	    for(int isys=0;isys<nsys;isys++){
	  MakeHists(version, ch_ttbar_sys, Region[iregion],sysname[isys]);

	    }
	  MakeHists(version, ch_wjets,     Region[iregion]); 
	  MakeHists(version, ch_dy,	    Region[iregion]); 
	  MakeHists(version, ch_t,         Region[iregion]); 
	  MakeHists(version, ch_f1500_100, Region[iregion]);  
	  MakeHists(version, ch_f1200_800, Region[iregion]); 

	  // ----------------------------------------
	  //  Make the final histogram file
	  // ----------------------------------------
	  cout << "[MJ Analysis] Merging result files" << endl;
	  gSystem->Exec(Form("rm Out/v%i/HistFiles/Hist_%s_v%i.root",version, Region[iregion],version));
	  gSystem->Exec(Form("hadd -f Out/v%i/HistFiles/Hist_%s_v%i.root Out/v%i/HistFiles/*_%s_v%i.root",version, Region[iregion],version, version, Region[iregion],version));
        }

      // ----------------------------------------
      //  Draw histograms 
      // ----------------------------------------
      for(int isys=0;isys<nsys;isys++){
	MakeSF(version, Region[iregion],"MJ",sysname[isys]);
	MakeSF(version, Region[iregion],"MJ_coarse",sysname[isys]);
      }
    }

  for(int isys=0;isys<nsys;isys++){
    for(int iregion=0; iregion<nregion; iregion++)
      {
	for(int bo=0; bo<2;bo++){
	  if(!OnlyDraw && !OnlyEssential){
	    Make1DPlots(version,"muspT",        Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version,"musPhi",       Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "musEta",       Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "elspT",        Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "elsPhi",       Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "elsEta",       Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "mT",           Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "mj",           Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "HT",           Region[iregion],bo,sysname[isys]);
	  }
	  Make1DPlots(version, "MJ",           Region[iregion],bo,sysname[isys]);
	  Make1DPlots(version, "MJ_coarse",           Region[iregion],bo,sysname[isys]);
	  if(iregion> SRthres){
	    for(int icorr=0;icorr<nregion-1;icorr++){
	      Make1DPlots(version, Form("MJ_coarse_corr%i",icorr), Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, Form("MJ_corr%i",icorr),  Region[iregion],bo,sysname[isys]);
	    }
	  }
	  TString temp = Form("%s",sysname[isys]);
	  if(temp.Contains("toppT")){
	    Make1DPlots(version, "toppT",       Region[iregion],bo,sysname[isys]);
	    if(bo) Make1DPlots(version, "toppT_incl",       Region[iregion],bo,sysname[isys]);}
	  if(temp.Contains("ISRpT")){
	    Make1DPlots(version, "ttbarpT",       Region[iregion],bo,sysname[isys]);
	    if(bo)Make1DPlots(version, "ttbarpT_incl",       Region[iregion],bo,sysname[isys]);}
	  //Make1DPlots(version, "toppT1",       Region[iregion],bo,sysname[isys]);
	  //Make1DPlots(version, "toppT2",       Region[iregion],bo,sysname[isys]);
	  if(!OnlyDraw && !OnlyEssential){
	    Make1DPlots(version, "Nfatjet",      Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "Nskinnyjet",   Region[iregion],bo,sysname[isys]);
	    Make1DPlots(version, "Ncsvm",        Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "MET",          Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "METPhi",       Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "WpT",          Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPt1"     ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPt2"     ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPt3"     ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPt4"     ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPhi1"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPhi2"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPhi3"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetPhi4"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetEta1"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetEta2"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetEta3"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "FatjetEta4"    ,Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "mj1",          Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "mj2",          Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "mj3",          Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "mj4",          Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "mj3overmj2",   Region[iregion],bo,sysname[isys]);
	      Make1DPlots(version, "mj2overmj1",   Region[iregion],bo,sysname[isys]);}
	  }
	// ----------------------------------------
	//  Make table of yields 
	// ---------------------------------------- 
      
	MakeTables(version, 0,   Region[iregion], false);
	MakeTables(version, 11,  Region[iregion], false);
	MakeTables(version, 13,  Region[iregion], false);
        
      }
  }//for(int iregion=0; iregion<2; iregion++)
  gSystem->Exec(Form("cp DoAnalysis.C Out/v%i/",version));
  gSystem->Exec(Form("cp MakeHists.C Out/v%i/",version));
 gSystem->Exec(Form("cp Make1DPlots.C Out/v%i/",version));
 gSystem->Exec(Form("cp Make2DPlots.C Out/v%i/",version));
 gSystem->Exec(Form("cp MakeSF.C Out/v%i/",version));
 gSystem->Exec(Form("cp MakeTables.C Out/v%i/",version));
 gSystem->Exec(Form("cp PassSelection.h Out/v%i/",version));
}
