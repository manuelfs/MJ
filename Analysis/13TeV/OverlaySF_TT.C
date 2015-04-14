#include <iostream>
#include <iomanip> // for setw()

#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TH2F.h"
#include "THStack.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDatime.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include "TInterpreter.h"
#include "TLatex.h"
#include "TMath.h"

void OverlaySF(int version, char* sys=(char*)"")
{

  char* Region[] = {"baseline","1BCRincl","1B4SJCRincl","1B45SJ","1B67SJ","1B8SJ","SRincl"};
  TString RegionNames[] = {"","6+ SJ","4+SJ","4 or 5 SJ","6 or 7 SJ","8+ SJ", "2B SR, 6+ SJ"};
  int colors[] = {0,2,3,4,5,6,1};
  int nregion =7;
  int start = 1;
  TFile* SFFile[nregion];
  TFile* HistFile[nregion];
  TGraphErrors *MJ_SF[7][nregion];
  TGraphErrors *MJ_SF_err[7][nregion];
  TString HistName="MJ_coarse";
  TString axistitle="blank";
  TString syst = Form("%s",sys);
  if(syst.Contains("toppT")) axistitle = "Top pT SF";
  if(syst.Contains("ISRpT")) axistitle = "ISR pT SF";
  if(syst.Contains("nISR")) axistitle = "nISR SF";
  for(int r=start;r<nregion;r++){
    //actually using MC error, not data error.
    SFFile[r] = TFile::Open(Form("Out/v%i/HistFiles/%s%s_SF_%s_v%i.root",version,HistName.Data(),sys ,Region[r],version));
    HistFile[r] = TFile::Open(Form("Out/v%i/HistFiles/Hist_%s_v%i.root",version, Region[r], version));
    for(int j=2;j<7;j++){
      MJ_SF_err[j][r] = (TGraphErrors*)SFFile[r]->Get(Form("SF_%i",j));
      // MJ_SF_err[j][r] = (TGraphErrors*)SFFile[r]->Get(Form("SF_mc_err_%i",j));
      if(!MJ_SF_err[j][r]) cout<<"Overlay error Get SF"<<endl;
       MJ_SF[j][r] = (TGraphErrors*)HistFile[r]->Get(Form("%s_%s%s_mcSF_%i","TT_sys",Region[r],sys,j));
       if(!MJ_SF[j][r]) cout<<"Overlay error Get SF err"<<endl;
    }
    SFFile[r]->Close();
    HistFile[r]->Close();
  }
  TCanvas *c = new TCanvas("c","c",1200,800); 
  c->Divide(3,2);

  for(int j=2;j<7;j++){
    c->cd(j-1);
    TLegend *l1 = new TLegend(0.23, 0.70, 0.87, 0.86);
        l1->SetNColumns(2);
        l1->SetBorderSize(0);
        l1->SetFillColor(0);
        l1->SetFillStyle(0);
        l1->SetTextFont(42);
        l1->SetTextAlign(12);
        l1->SetTextSize(0.034);
        l1->SetFillColor(kWhite);
        l1->SetLineColor(kWhite);
        l1->SetShadowColor(kWhite);
    for(int r=start; r<nregion;r++){

      MJ_SF[j][r]->SetMarkerStyle(20);
	MJ_SF[j][r]->SetMarkerColor(colors[r]);
	MJ_SF[j][r]->SetLineColor(colors[r]);
	MJ_SF[j][r]->SetFillColor(colors[r]);

	MJ_SF[j][r]->SetMinimum(0);
	MJ_SF[j][r]->SetMaximum(2.0);
       	MJ_SF[j][r]->GetXaxis()->SetRangeUser(0,2000);
	TString StackTitle = Form("%i fatjets", j);
        if(j==6) StackTitle = "All fatjets";
        if(j==5) StackTitle = "5+ fatjets";
	MJ_SF[j][r]->SetTitle(StackTitle);
	MJ_SF[j][r]->GetXaxis()->SetTitle("MJ [GeV]");
	MJ_SF[j][r]->GetYaxis()->SetTitle(axistitle);
	if(r==start) MJ_SF[j][r]->Draw("APZ");
	else MJ_SF[j][r]->Draw("PZ same");

	MJ_SF_err[j][r]->SetMinimum(0);
	MJ_SF_err[j][r]->SetMaximum(2.0);
	MJ_SF_err[j][r]->SetFillStyle(3002);
	MJ_SF_err[j][r]->SetFillColor(kBlack);
       	MJ_SF_err[j][r]->GetXaxis()->SetRangeUser(0,2000);
       	MJ_SF_err[j][r]->SetTitle(StackTitle);
	MJ_SF_err[j][r]->GetXaxis()->SetTitle("MJ [GeV]");
	MJ_SF_err[j][r]->GetYaxis()->SetTitle(axistitle);
	//	MJ_SF_err[j][r]->Draw("2Z same");

	
	l1->AddEntry(MJ_SF[j][r],RegionNames[r],  "f");
    }
    l1->Draw();
  }

 c->Print( Form("Out/v%i/Figures/Overlay_SF_%s%s_v%i.pdf",version, HistName.Data(),sys, version) ); 
  
}
