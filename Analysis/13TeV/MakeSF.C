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

using namespace std;

void MakeSF( int version, char* Region, TString HistName)
{
  int NMergeBins=1;
  TFile* HistFile = TFile::Open(Form("Out/v%i/HistFiles/Hist_%s_v%i.root",version, Region, version));
   
 

  //TString HistName = "MJ";
  TH1F *h1_DATA[7], *h1_T[7], *h1_TT_sl[7], *h1_TT_ll[7], *h1_TT_sys[7], *h1_TT[7], *h1_WJets[7], *h1_DY[7]; 
  TH1F *h1_f1500_100[7], *h1_f1200_800[7];
  
  
  TGraphErrors *g1_SF[7];
  TCanvas *c = new TCanvas("c","c",1200,800); 
  c->Divide(3,2);
  for(int i=2; i<7; i++) {
    c->cd(i-1);
      h1_T[i]         = (TH1F*)HistFile->Get(Form("h1_T_%s_%ifatjet", HistName.Data(), i));
      h1_TT_sl[i]     = (TH1F*)HistFile->Get(Form("h1_TT_sl_%s_%ifatjet", HistName.Data(), i));
      h1_TT_ll[i]     = (TH1F*)HistFile->Get(Form("h1_TT_ll_%s_%ifatjet", HistName.Data(), i));
      h1_TT_sys[i]     = (TH1F*)HistFile->Get(Form("h1_TT_sys_%s_%ifatjet", HistName.Data(), i));
      h1_WJets[i]     = (TH1F*)HistFile->Get(Form("h1_WJets_%s_%ifatjet", HistName.Data(), i));
      h1_DY[i]        = (TH1F*)HistFile->Get(Form("h1_DY_%s_%ifatjet", HistName.Data(), i)); 
      h1_f1500_100[i] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1500_100_%s_%ifatjet", HistName.Data(), i)); 
      h1_f1200_800[i] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1200_800_%s_%ifatjet", HistName.Data(), i)); 
    

   //h1_DATA[i]->Rebin(NMergeBins);
        h1_T[i]->Rebin(NMergeBins);
        h1_TT_sl[i]->Rebin(NMergeBins);
        h1_TT_ll[i]->Rebin(NMergeBins);
	h1_TT_sys[i]->Rebin(NMergeBins);
        h1_WJets[i]->Rebin(NMergeBins);
        h1_DY[i]->Rebin(NMergeBins);
        h1_f1500_100[i]->Rebin(NMergeBins);
        h1_f1200_800[i]->Rebin(NMergeBins);
        
	h1_TT[i] = (TH1F*)h1_TT_ll[i]->Clone();
        h1_TT[i]->Add(h1_TT_sl[i]);
	
	//put sys-varied into data path
	h1_DATA[i]=(TH1F*)h1_TT_sys[i]->Clone(Form("h1_sys_data_%s_%ifatjet", HistName.Data(), i));
	h1_DATA[i]->Add(h1_WJets[i]);
        h1_DATA[i]->Add(h1_T[i]);
        h1_DATA[i]->Add(h1_DY[i]);
	
	//restore poisson errors on data
	for(int b=0;b<((int)h1_DATA[i]->GetXaxis()->GetNbins()+1);b++){
	  h1_DATA[i]->SetBinError(b,pow(h1_DATA[i]->GetBinContent(b),0.5));
	}


	h1_DATA[i]->Sumw2();
	h1_DATA[i]->Add(h1_WJets[i],-1);
        h1_DATA[i]->Add(h1_T[i],-1);
        h1_DATA[i]->Add(h1_DY[i],-1);
	
	Int_t nbins = h1_DATA[i]->GetXaxis()->GetNbins()+1;
	cout<<"nbins = "<<nbins<<endl;
	Double_t x[nbins];
	Double_t y[nbins];
	Double_t ex[nbins];
	Double_t ey[nbins];
	
	for(int b=0;b<(nbins+1);b++){
	  x[b] = h1_DATA[i]->GetBinCenter(b);
	  ex[b] = 0.5* h1_DATA[i]->GetBinWidth(b);
	  y[b] = -1; ey[b]=0;
	  
	  if(h1_TT[i]->GetBinContent(b)>1 && h1_DATA[i]->GetBinContent(b)>1){ y[b] = h1_DATA[i]->GetBinContent(b)/h1_TT[i]->GetBinContent(b);
	    ey[b] = y[b]*pow(pow(h1_TT[i]->GetBinError(b)/h1_TT[i]->GetBinContent(b),2)+pow(h1_DATA[i]->GetBinError(b)/h1_DATA[i]->GetBinContent(b),2),0.5);
	    if(b==(nbins-1) || b ==7) cout<<"SF NFJ = "<<i<<" x ex y ey  "<<x[b]<<" "<<ex[b]<<" "<<y[b]<<" "<<ey[b]<<endl; 
	  }
	  
	}
	
	g1_SF[i] = new TGraphErrors(nbins,x,y,ex,ey);
	g1_SF[i]->SetMarkerStyle(20);
	g1_SF[i]->SetMarkerColor(kBlue);
	g1_SF[i]->SetLineColor(kBlue);
	g1_SF[i]->SetMinimum(0);
	g1_SF[i]->SetMaximum(1.5);
       	g1_SF[i]->GetXaxis()->SetRangeUser(0,2000);
	TString StackTitle = Form("%i fatjets", i);
        if(i==6) StackTitle = "All fatjets";
        if(i==5) StackTitle = "5+ fatjets";
	g1_SF[i]->SetTitle(StackTitle);
	g1_SF[i]->SetName(Form("SF_%i",i));
	g1_SF[i]->GetXaxis()->SetTitle("MJ [GeV]");
	g1_SF[i]->GetYaxis()->SetTitle("Top pT SF");
	g1_SF[i]->Draw("APZ");



	
	 // CMS Labels 
        float textSize = 0.04;

        TLatex *TexEnergyLumi = new TLatex(0.9,0.92,Form("#sqrt{s}=13 TeV, L = %i fb^{-1}", 5));
        TexEnergyLumi->SetNDC();
        TexEnergyLumi->SetTextSize(textSize);
        TexEnergyLumi->SetTextAlign (31);
        TexEnergyLumi->SetLineWidth(2);

        TLatex *TexCMS = new TLatex(0.2,0.92,"CMS Preliminary");
        TexCMS->SetNDC();
        TexCMS->SetTextSize(textSize);
        TexCMS->SetLineWidth(2);
       
        // FIXME : need to add lepton flavor
        TString LabelExt = Form("N_{fatjet} = %i", i);
        if(i==5) LabelExt="N_{fatjet} >= 5";
	TLatex *TexExt;
	TexExt = new TLatex(0.85,0.8,LabelExt);
	
        TexExt->SetTextAlign (31);
        TexExt->SetNDC();
        TexExt->SetTextSize(textSize);
        TexExt->SetLineWidth(2);
        
        TexEnergyLumi->Draw("SAME");
        TexCMS->Draw("SAME");
	if(i!=6)TexExt->Draw("SAME");
  }
  c->Print( Form("Out/v%i/Figures/toy_SF_%s_%s_v%i.pdf",version, HistName.Data(), Region, version) ); 
    
    // 
  HistFile->Close();
  delete c; 
  //TString HistFileName = Form("%s_SF",HistName.Data());
  TString HistFileName = Form("Out/v%i/HistFiles/%s_SF_%s_v%i.root",version, HistName.Data(), Region, version);
  TFile *HistFile2 = new TFile(HistFileName, "RECREATE");
  gROOT->cd();
  HistFile2->cd();
  //cout<<"got here"<<endl;
  // write histograms
  for(int j =2;j<7;j++){
    //g1_SF[j]->SetDirectory(0);
    //cout<<j<<endl;
    g1_SF[j]->Write();
  }
  HistFile2->Close();
  cout<<"finished"<<endl;

}
