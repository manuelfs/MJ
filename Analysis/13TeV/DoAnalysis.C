#include "Configuration.h"
//
// main 
// top pt branch
void DoAnalysis(bool OnlyDraw=false) 
{
  // Style
  //gROOT->ProcessLine(".L /Users/jaehyeok/macros/rootlogon.C");
  // Load macros

  // gROOT->LoadMacro("Definitions.h");
  gROOT->LoadMacro("MakeHists.C+");
  gROOT->LoadMacro("Make1DPlots.C+");
  gROOT->LoadMacro("MakeSF.C+");
  gROOT->LoadMacro("Make2DPlots.C+");
  gROOT->LoadMacro("MakeTables.C+");
  gROOT->LoadMacro("MakeTables_all.C+");
  gROOT->LoadMacro("OverlaySF_TT.C+");
  gROOT->LoadMacro("GetYields.C+");
  //char* sysname[] = {"_nISR",/*"_toppT1","_toppT2","_ISRpT1","_ISRpT2",*/"_ISRpT3","_ISRpT4"};
  //int nsys=3;
  // if(!status) nsys=1;
  gSystem->Exec(Form("mkdir -p Out/%s_v%i/Figures/",study.Data(),version));
  gSystem->Exec(Form("mkdir -p Out/%s_v%i/HistFiles/",study.Data(),version));
  gSystem->Exec(Form("mkdir -p Out/%s_v%i/Tables/",study.Data(),version));
  
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
  
  //TString BabyDir = "/Users/heller/Jae/ntuples/Phys14_13_Apr2015/";

    //"/Users/heller/Jae/ntuples/JetPt20_04April_NoSkim/";

  //"/Users/heller/Jae/ntuples/HT750MET250_JetPt20_04April/";
  //"/Users/heller/Jae/ntuples/HT750MET250_JetPt20/";
  //"/Users/heller/Jae/ntuples/HT750MET250/";
  //"/net/top/homes/rohan/MJ/Analysis/13TeV/Phys14/";
  //  "/net/cms26/cms26r0/jaehyeok/baby/Fatjet/13TeV/Phys14NoSkim/";
  // "/net/cms26/cms26r0/jaehyeok/baby/Fatjet/13TeV/";
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
    
  // char* Regions[] = {"1B_hi_mT","1B_lo_mT","1B_sl","1B_ll",/*"SRstats1","SR0","SR1","baseline",*/"1BCRincl",/*"1B4SJCRincl","1B45SJ","1B67SJ","1B8SJ",*/"SRincl"};
  //int nregion=1;
  // char* Regions[] = {"1B_hi_mT","1B_lo_mT","1B_sl","1B_ll","2B_hi_mT","2B_lo_mT","2B_sl","2B_ll",/*"SRstats1","SR0","SR1","baseline",*/"1BCRincl",/*"1B4SJCRincl","1B45SJ","1B67SJ","1B8SJ",*/"SRincl"};
  // int SRthres=8;
  //iRegion>SRthres
  
  //COPY TO MAKE1DPlots

  
  // char* Regions[] = {"SR0", "SR1","SRincl","1BSR1","1BSR0","1BCR0","1B2lCR0","1BCRincl","1B2lCRincl","1B2lCRincl_loose","AllBCRincl"}; 
  //remember loose requires full babies
  for(int iregion=0; iregion<nregion; iregion++)
    {
      if(!OnlyDraw) 
        {
	  // ----------------------------------------
	  //  Fill histrograms 
	  // ----------------------------------------
	  MakeHists(ch_data,	    Regions[iregion]); 
	  MakeHists(ch_ttbar_sl,  Regions[iregion]); 
	  MakeHists(ch_ttbar_ll,  Regions[iregion]);
	  for(int isys=0;isys<nsys;isys++){
	    MakeHists(ch_ttbar_sys, Regions[iregion],sysname[isys]);

	  }
	  MakeHists(ch_wjets,     Regions[iregion]); 
	  MakeHists(ch_dy,	    Regions[iregion]); 
	  MakeHists(ch_t,         Regions[iregion]); 
	  MakeHists(ch_f1500_100, Regions[iregion]);  
	  MakeHists(ch_f1200_800, Regions[iregion]); 

	  // ----------------------------------------
	  //  Make the final histogram file
	  // ----------------------------------------
	  cout << "[MJ Analysis] Merging result files" << endl;
	  gSystem->Exec(Form("rm Out/%s_v%i/HistFiles/Hist_%s_v%i.root",study.Data(),version, Regions[iregion],version));
	  gSystem->Exec(Form("hadd -f Out/%s_v%i/HistFiles/Hist_%s_v%i.root Out/%s_v%i/HistFiles/*_%s_v%i.root",study.Data(),version, Regions[iregion],version,study.Data(),version, Regions[iregion],version));
        }

      // ----------------------------------------
      //  Draw histograms 
      // ----------------------------------------
      for(int isys=0;isys<nsys;isys++){
	//	MakeSF(Regions[iregion],"MJ",sysname[isys]);
	MakeSF(Regions[iregion],"MJ_coarse",sysname[isys]);
	MakeSF(Regions[iregion],"MJ_coarse",sysname[isys],true);
      }
    }

  for(int isys=0;isys<nsys;isys++){
    for(int iregion=0; iregion<nregion; iregion++)
      {
	for(int bo=0; bo<2;bo++){
	  if(!OnlyDraw && !OnlyEssential){
	    Make1DPlots("muspT",        Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("musPhi",       Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("musEta",       Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("elspT",        Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("elsPhi",       Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("elsEta",       Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mT",           Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mj",           Regions[iregion],bo,false,sysname[isys]);
	   
	  }
	  //Make1DPlots("HT",           Regions[iregion],bo,false,sysname[isys]);
	  //Make1DPlots("MJ",           Regions[iregion],bo,false,sysname[isys]);
	  Make1DPlots("MJ_coarse",           Regions[iregion],bo,false,sysname[isys]);
	  Make1DPlots("MJ_coarse",           Regions[iregion],bo,false,sysname[isys]);
	  Make1DPlots("Nfatjet",      Regions[iregion],bo,false,sysname[isys]);
	  Make1DPlots("mj1",          Regions[iregion],bo,false,sysname[isys]);

	   
	  if(iregion> SRthres){
	    for(int icorr=0;icorr<=SRthres;icorr++){
	      Make1DPlots(Form("MJ_coarse_corr%i",icorr), Regions[iregion],bo,false,sysname[isys]);
	      // Make1DPlots(Form("MJ_corr%i",icorr),  Regions[iregion],bo,false,sysname[isys]);
	    }
	  }
	  TString temp = Form("%s",sysname[isys]);
	  if(temp.Contains("toppT")){
	    Make1DPlots("toppT",       Regions[iregion],bo,false,sysname[isys]);
	    if(bo) Make1DPlots("toppT_incl",       Regions[iregion],bo,false,sysname[isys]);}
	  if(temp.Contains("ISRpT")){
	    Make1DPlots("ttbarpT",       Regions[iregion],bo,false,sysname[isys]);
	    if(bo)Make1DPlots("ttbarpT_incl",       Regions[iregion],bo,false,sysname[isys]);}
	  if(temp.Contains("nISR")){
	    Make1DPlots("nISR",       Regions[iregion],bo,false,sysname[isys]);
	    if(bo)Make1DPlots("nISR_incl",       Regions[iregion],bo,false,sysname[isys]);}
	  //Make1DPlots("toppT1",       Regions[iregion],bo,false,sysname[isys]);
	  //Make1DPlots("toppT2",       Regions[iregion],bo,false,sysname[isys]);
	  if(!OnlyDraw && !OnlyEssential){
	    Make1DPlots("mj2",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mj3",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mindPhi_B_met",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("nBSJ_perFJ",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("max_nBSJ_perEvent",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("sumCSV_perFJ",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("max_sumCSV_perEvent",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mj_BFJ",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mj1_BFJ",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("sumMJ_BFJ",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("Nskinnyjet",   Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("Ncsvm",        Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("MET",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("METPhi",       Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("WpT",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPt1"     ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPt2"     ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPt3"     ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPt4"     ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPhi1"    ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPhi2"    ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPhi3"    ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetPhi4"    ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetEta1"    ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetEta2"    ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetEta3"    ,Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("FatjetEta4"    ,Regions[iregion],bo,false,sysname[isys]);
	      
	    Make1DPlots("mj4",          Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mj3overmj2",   Regions[iregion],bo,false,sysname[isys]);
	    Make1DPlots("mj2overmj1",   Regions[iregion],bo,false,sysname[isys]);}
	}
	// ----------------------------------------
	//  Make table of yields 
	// ---------------------------------------- 
      
	MakeTables(0,   Regions[iregion], false);
	MakeTables(11,  Regions[iregion], false);
	MakeTables(13,  Regions[iregion], false);
	if(Regions[iregion] == "baseline" && false){
	  for(int nMJ=0;nMJ<2;nMJ++){
	    for(int nMET=0;nMET<3;nMET++){
	      for(int nMT=0;nMT<2;nMT++){
	    
		for(int nb=0;nb<3;nb++){
		  for(int nsj=0;nsj<2;nsj++){
		    for(int nHT=0;nHT<2;nHT++){
		      cout<<Form("MJ bin%i, MET bin%i, MT bin%i, nb bin%i, Nsj bin%i, HT bin%i",nMJ,nMET,nMT,nb,nsj,nHT)<<endl;
		      MakeTables_all(0,   Regions[iregion],nMJ,nMET,nMT,nb,nsj,nHT, false);
		      MakeTables_all(15,   Regions[iregion],nMJ,nMET,nMT,nb,nsj,nHT, false);
		    }
		  }
		}
	    
	      }
	    }//nFJ, MJ, MET, MT, Nlep, NB
	  }
        }
      }
  }//for(int iregion=0; iregion<2; iregion++)
  /*OverlaySF_TT(1,sysname[0]);
  OverlaySF_TT(1,sysname[1]);
  OverlaySF_TT(1,sysname[2]);
  OverlaySF_TT(2,sysname[0]);
  OverlaySF_TT(2,sysname[1]);
  OverlaySF_TT(2,sysname[2]);*/
  // GetYields();
  gSystem->Exec(Form("cp DoAnalysis.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp Configuration.h Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp MakeHists.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp Make1DPlots.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp Make2DPlots.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp MakeSF.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp GetYields.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp MakeTables.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp PassSelection.h Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp OverlaySF.C Out/%s_v%i/",study.Data(),version));
  gSystem->Exec(Form("cp babytree_mi.h Out/%s_v%i/",study.Data(),version));
}
