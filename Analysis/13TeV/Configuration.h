#include <vector>
#include <string>
#include <iostream>

#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDatime.h"
#include "TLegend.h"
#include "TMath.h"
#include "TGraphErrors.h"


int version = 29;
bool OnlyEssential=true;
TString BabyDir = "/Users/heller/Jae/ntuples/Phys14_13_Apr2015/";
bool status =true;

    //"/Users/heller/Jae/ntuples/JetPt20_04April_NoSkim/";

  //"/Users/heller/Jae/ntuples/HT750MET250_JetPt20_04April/";
  //"/Users/heller/Jae/ntuples/HT750MET250_JetPt20/";
  //"/Users/heller/Jae/ntuples/HT750MET250/";
  //"/net/top/homes/rohan/MJ/Analysis/13TeV/Phys14/";
  //  "/net/cms26/cms26r0/jaehyeok/baby/Fatjet/13TeV/Phys14NoSkim/";
  // "/net/cms26/cms26r0/jaehyeok/baby/Fatjet/13TeV/";
  //"/net/cms26/cms26r0/jaehyeok/baby/Fatjet/13TeV/Phys14/HT750MET250/";
  //"/Users/jaehyeok/Research/Tools/fastjet-3.0.6/example/babies/13TeV/HT750MET250/";


/*
TString study = "mT_extrapolation";



char* sysname[] = {"_nISR","_ISRpT3","_ISRpT4"};
int nsys=3;

int nregion=8;
char* Regions[] = {"1B_hi_mT","1B_lo_mT","1B_sl","1B_ll","2B_hi_mT","2B_lo_mT","2B_sl","2B_ll"};
//int SRthres=8;
*/
/*
TString study = "B_FJ";

char* sysname[] = {""};
int nsys=1;

int nregion=4;
char* Regions[] = {"SR0_lowmT","SR1_lowmT","SRstats1_lowmT","SRstats_lowmT"};
//"SRincl","SR0","SR1","SRstats1","SRstats"
int SRthres=5; */

TString study = "counts";

char* sysname[] = {""};
int nsys=1;

int nregion=1;
char* Regions[] = {"baseline_1B"};
int SRthres=2;




/*
TString study = "weird_event_dump";

char* sysname[] = {""};
int nsys=1;

int nregion=1;
char* Regions[] = {"nothing"};
int SRthres=1;
*/



