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
#include "Configuration.h"
#include "babytree_mi.h"
#include "PassSelection.h"

using namespace std;
bool    DoLog           = 1;
int     FatjetpTthres   = 50;
int     mjthres         = 0;
//bool status = true;

//
//TH1F initialization
//
TH1F* InitTH1F(char* Name, char* Title, int Nbins, double XMin, double XMax)
{
  TH1F *h1 = new TH1F(Name, Title, Nbins, XMin, XMax);
  h1->Sumw2();
  return h1;
}

TH1F* InitTH1FVarBin(char* Name, char* Title, int Nbins, const Float_t* xbins)
{
  TH1F *h1 = new TH1F(Name, Title, Nbins, xbins);
  h1->Sumw2();
  return h1;
}
//
//TH2F initialization
//
TH2F* InitTH2F(char* Name, char* Title, int NXbins, double XMin, double XMax, int NYbins, double YMin, double YMax)
{
  TH2F *h2 = new TH2F(Name, Title, NXbins, XMin, XMax, NYbins, YMin, YMax);
  h2->Sumw2();
  return h2;
}

//
// Fill TH1F
//
void FillTH1F(TH1F* &h1, double var, double weight)
{
  if(var >= h1->GetXaxis()->GetBinUpEdge(h1->GetXaxis()->GetNbins())) 
    var=h1->GetXaxis()->GetBinUpEdge(h1->GetXaxis()->GetNbins())-0.00001;
  if(var < h1->GetXaxis()->GetBinLowEdge(1)) 
    var=h1->GetXaxis()->GetBinLowEdge(1)+0.00001;
  h1->Fill(var, weight);        
}


void FillTH1FAll(TH1F* h1[7], int NFJ, double var, double weight)
{
  FillTH1F(h1[NFJ], var,  weight);
  FillTH1F(h1[6], var,  weight);
}

//
// Fill TH2F
//
void FillTH2F(TH2F* &h2, double varX, double varY, double weight){
  if(varX >= h2->GetXaxis()->GetBinUpEdge(h2->GetXaxis()->GetNbins())) 
    varX=h2->GetXaxis()->GetBinUpEdge(h2->GetXaxis()->GetNbins())-0.00001;
  if(varY >= h2->GetYaxis()->GetBinUpEdge(h2->GetYaxis()->GetNbins())) 
    varY=h2->GetYaxis()->GetBinUpEdge(h2->GetYaxis()->GetNbins())-0.00001;
  if(varX < h2->GetXaxis()->GetBinLowEdge(1)) 
    varX=h2->GetXaxis()->GetBinLowEdge(1)+0.00001;
  if(varY < h2->GetYaxis()->GetBinLowEdge(1)) 
    varY=h2->GetYaxis()->GetBinLowEdge(1)+0.00001;
  h2->Fill(varX, varY, weight);        
}
void FillTH2FAll(TH2F* h2[7], int NFJ, double varX, double varY, double weight)
{
  FillTH2F(h2[NFJ], varX,  varY, weight);
  FillTH2F(h2[6], varX,  varY, weight);
}

//
// loading PU reweigting histogram
//
double nPUScaleFactor2012(TH1F* h1PU, float npu){
  // Get PU scale factor histogram
  double mynpu = TMath::Min(npu,(float)49.499);
  Int_t npuxbin = h1PU->GetXaxis()->FindBin(mynpu);
  return h1PU->GetBinContent(npuxbin);
}


void MCDump(){
  for(unsigned int igen=0; igen<GenId_->size(); igen++)
    {
      
      float p = GenPt_->at(igen) * TMath::CosH(GenEta_->at(igen));
      //float m = sqrt(pow(GenE_->at(igen),2)-pow(p,2));

      cout << "Printing contents of mc_doc..." << endl;
      // if (mc_doc_status->at(igen)==3||mc_doc_status->at(igen)==22||mc_doc_status->at(igen)==23) {  // hard scatter
      cout<<igen<<": ID "<<static_cast<int>(GenId_->at(igen))<<",   \tMom ID "<<static_cast<int>(GenMId_->at(igen))
 
	  <<",   \tpT "<<GenPt_->at(igen)
	  <<",   \teta "<<GenEta_->at(igen)
	  <<",   \tphi "<<GenPhi_->at(igen)
	//  <<",   \tmass "<<m
	  <<",   \tstatus "<<GenStatus_->at(igen)
	  <<", \tGMom ID "<<static_cast<int>(GenGMId_->at(igen)) <<endl;
    
    }
}

int GetnISR(){
  int nISR=0;
  for(unsigned int igen=0; igen<GenId_->size(); igen++)
    { 
      if(GenStatus_->at(igen)==23 && fabs(GenId_->at(igen))!=6 && fabs(GenMId_->at(igen))!=6 && fabs(GenGMId_->at(igen))!=6
	 && fabs(GenId_->at(igen))!=24 && fabs(GenMId_->at(igen))!=24 && fabs(GenGMId_->at(igen))!=24 ){
	nISR++;
      }
    }
  return nISR;
}


float GetMuonEff(float pt, float eta)
{ 
  float eff = 1; 
 
  // For SingleMu dataset
  if(pt<25) eff = 0;
  /* 
   
     if(pt>25 && pt<30)  
     { 
     if(TMath::Abs(eta)<0.8) eff = 0.9;
     if(TMath::Abs(eta)>0.8 && TMath::Abs(eta)<1.2) eff = 0.81;
     if(TMath::Abs(eta)>1.2 && TMath::Abs(eta)<2.1) eff = 0.78;
     }
   
     if(pt>30 && pt<35)  
     { 
     if(TMath::Abs(eta)<0.8) eff = 0.91;
     if(TMath::Abs(eta)>0.8 && TMath::Abs(eta)<1.2) eff = 0.83;
     if(TMath::Abs(eta)>1.2 && TMath::Abs(eta)<2.1) eff = 0.80;
     }
   
     if(pt>35 && pt<40)  
     { 
     if(TMath::Abs(eta)<0.8) eff = 0.92;
     if(TMath::Abs(eta)>0.8 && TMath::Abs(eta)<1.2) eff = 0.84;
     if(TMath::Abs(eta)>1.2 && TMath::Abs(eta)<2.1) eff = 0.80;
     }
   
     if(pt>40 && pt<50)  
     { 
     if(TMath::Abs(eta)<0.8) eff = 0.94;
     if(TMath::Abs(eta)>0.8 && TMath::Abs(eta)<1.2) eff = 0.85;
     if(TMath::Abs(eta)>1.2 && TMath::Abs(eta)<2.1) eff = 0.82;
     }
   
     if(pt>50)  
     { 
     if(TMath::Abs(eta)<0.8) eff = 0.94;
     if(TMath::Abs(eta)>0.8 && TMath::Abs(eta)<1.2) eff = 0.86;
     if(TMath::Abs(eta)>1.2 && TMath::Abs(eta)<2.1) eff = 0.82;
     }

     if(TMath::Abs(eta)>2.1) eff = 0; 

     eff = 0.88;
  */

  if(TMath::Abs(eta)<0.9) eff = 0.98;
  else eff = 0.84;
   
  return eff;
}

float getISRSF(float ISRpT)
{ 
  // ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SMST2ccMadgraph8TeV
  if( ISRpT>250) return 0.8;
  else if( ISRpT>150 ) return 0.9;
  else if( ISRpT>120 ) return 0.95;
  else return 1;
}

float getISR2SF(float ISRpT)
{ 
  // ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SMST2ccMadgraph8TeV
  if( ISRpT>400) return 1.3;
  //else if( ISRpT>350 ) return 1.15;
  else if( ISRpT>300 ) return 1.15;
  else if( ISRpT>200 ) return 1.0;
  else if( ISRpT>100 ) return 0.85;			   
  else return 0.7;
}

float getISR3SF(float ISRpT)
{ 
  // ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SMST2ccMadgraph8TeV
  if( ISRpT>500) return 0.6;
  //else if( ISRpT>350 ) return 1.15;
  else if( ISRpT>300 ) return 0.8;
  else if( ISRpT>240 ) return 0.9;
  else if( ISRpT>120) return 0.95;
  else return 1.0;
}

float getISR4SF(float ISRpT)
{ 
  // ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SMST2ccMadgraph8TeV
  if( ISRpT>500) return 0.6;
  else if( ISRpT>450 ) return 0.65;
  else if( ISRpT>400 ) return 0.7;
  else if( ISRpT>350 ) return 0.75;
  else if( ISRpT>300 ) return 0.8;
  else if( ISRpT>270 ) return 0.85;
  else if( ISRpT>240 ) return 0.9;
  else if( ISRpT>120) return 0.95;
  else return 1.0;
  }

float GetNISRSF(int nISR){
  if(nISR>3) {cout<<"ERROR: n ISR = "<<nISR<<endl; MCDump(); return 1.0;}
  else if(nISR==3) return 1.5;
  else if(nISR==2) return 1.25;
  else if(nISR==1) return 1.0;
  else return 0.75;
  
}

float getDPhi(float phi1, float phi2) 
{ 
  float absdphi = fabs(phi1-phi2);
  if(absdphi < TMath::Pi()) return absdphi;
  else return (2*TMath::Pi() - absdphi);
  // Ryan's : http://www.aidansean.com/cheatsheets/?p=105
  //return TMath::Abs(TMath::Abs(TMath::Abs(phi1 - phi2) - TMath::Pi())-TMath::Pi());  

}
float getDEta(float eta1, float eta2)
{
  return fabs(eta1-eta2);
}
float getDR(float dphi, float deta)
{
  return TMath::Sqrt(dphi*dphi+deta*deta);
}
float getDR(float eta1, float eta2, float phi1, float phi2)
{
  return getDR(getDPhi(phi1, phi2), eta1-eta2);
}


/*void EventDisplay(TString cate){
  bool truth = true;
  
  float start=0.62;
  float nextline = start;
  float increment = 0.04;
  float offset=0.02;
  int npert;
  int LSPcol[2] = {kCyan+1,kOrange-4};
  if(!sig) npert = 5;
  else npert=3;
  float xalign = 0.82;
  vector<TMarker> constituents, genpart;
  TEllipse *cone[FatjetPt_->size()]; 
  TH2F *h_fatjets = new TH2F("h_fatjets","h_fatjets", 230, -5.0, 5.0, 144, -3.141592, 3.141592);
  for(int ifj = 0; ifj< (int)FatjetPt_->size(); ifj++){
    h_fatjets->Fill(FatjetEta_->at(ifj),FatjetPhi_->at(ifj),mj->at(ifj));
  }

  TString cname = Form("Event_%i_",event);
  TCanvas *c = new TCanvas(cname,cname,1640,760);
  gPad->SetRightMargin(0.35);
  h_fatjets->SetZTitle("m_{j} [GeV]");
  h_fatjets->GetZaxis()->SetTitleSize(0.05);
  h_fatjets->GetZaxis()->SetTitleOffset(0.55);
  h_fatjets->GetYaxis()->SetTitleOffset(0.6);
  h_fatjets->Draw("colz");
  h_fatjets->GetZaxis()->SetRangeUser(0,1.1*h_fatjets->GetMaximum());
  h_fatjets->Draw("colz");
  gPad->Update();

   TPaletteAxis *palette = (TPaletteAxis*)h_fatjets->GetListOfFunctions()->FindObject("palette");
  for(int ifj = 0; ifj< (int)FatjetPt_->size(); ifj++){
    cone[ifj] = new TEllipse(FatjetEta_->at(ifj),FatjetPhi_->at(ifj), 1.2, 1.2);
    //  cone[ifj]->SetFillStyle(3003);
    cone[ifj]->SetFillStyle(0);
    Int_t binx,biny,binz;
    h_fatjets->GetBinXYZ(h_fatjets->FindBin(FatjetEta_->at(ifj),FatjetPhi_->at(ifj)),binx,biny,binz);
    //cout<<"bin x bin y "<<binx<<" "<<biny<<endl;
    //cout<<"content "<<h_fatjets->GetBinContent(binx,biny)<<endl;
   
    //cout<<palette<<endl;
    Int_t ci = palette->GetBinColor(binx,biny);
    if(ifj==0) ci = palette->GetBinColor(binx,biny);
    //cout<<"color"<<ci<<endl;
    cone[ifj]->SetFillColor(kGray);
    cone[ifj]->SetLineColor(ci);
  }


  myText(xalign+0.01,0.96,Form("H_{T} = %.0f GeV",0),1,0.04);
  myText(xalign+0.01,0.91,Form("M_{J} = %.0f GeV",0),1,0.04);
  myText(xalign+0.01,0.86,Form("#slash{E}_{T} = %.0f GeV",MET_),1,0.04);
  myText(xalign+0.01,0.81,Form("m_{T} = %.0f GeV",0),1,0.04);
  nextline=0.76;
  myText(xalign+0.01,nextline,Form("reco #mu p_{T} = %.0f GeV",RA4MusPt_->at(0),kBlack,0.04);
  // myText(xalign+0.07,nextline,Form("%.0f",mus_pt->at(RA4Muon.at(0))),kRed,0.04);
	 TMarker recomu = TMarker(RA4MusPt_->at(0), mus_phi->at(RA4Muon.at(0)), 27);
  recomu.SetMarkerSize(4);
  recomu.SetMarkerColor(kRed);
  genpart.push_back(recomu);
  if(truth) myText(xalign,0.68,"Gen  p_{T} [GeV]",1,0.04);
	  TMarker mumark = TMarker(xalign,nextline,27);
  mumark.SetNDC();
  mumark.SetX(xalign);
  mumark.SetY(nextline+0.01);
  mumark.SetMarkerSize(2);
  mumark.SetMarkerColor(kRed);
  genpart.push_back(mumark);


  TArrow line1 = TArrow();
  if(truth)line1.DrawLineNDC(xalign-0.02,start+0.035,xalign+0.14,start+0.035);
  TMarker met = TMarker(0,METPhi_, 27);
  met.SetMarkerSize(4);
  met.SetMarkerColor(kMagenta);
  genpart.push_back(met);
  TMarker metmark = TMarker(0.81,0.86,27);
  metmark.SetNDC();
  metmark.SetX(xalign);
  metmark.SetY(0.87);
  metmark.SetMarkerSize(2);
  metmark.SetMarkerColor(kMagenta);
  genpart.push_back(metmark);

  TMarker jetmark = TMarker(0.5,0.5,20);
  jetmark.SetNDC();
  jetmark.SetMarkerSize(1.2);
  jetmark.SetX(0.12);
  jetmark.SetY(0.2);
  genpart.push_back(jetmark);
  myText(0.13,0.19,"AK4 Jets",kBlack,0.04);
  TMarker bjetmark = TMarker(0.5,0.5,20);
  bjetmark.SetNDC();
  bjetmark.SetMarkerSize(1.2);
  bjetmark.SetMarkerColor(8);
  bjetmark.SetX(0.12);
  bjetmark.SetY(0.16);
  genpart.push_back(bjetmark);
  myText(0.13,0.15,"CSVM AK4",kBlack,0.04);
  myText(0.08,0.94,"Ring color indicates FJ mass",kBlack,0.04);

  // if(sig) nextline = start - (4*npert+2)*increment-0.02-offset;
  //else nextline = start - 2*npert*increment-offset;

	
  for(int ij=0;ij<(int)JetPt_->size();ij++){
    if(JetPt_->at(ij)>30){
      TMarker jet = TMarker(JetEta_->at(ij),JetPhi_->at(ij),20);
      jet.SetMarkerSize(1.2);
      if(JetCSV_->at(ij)>0.814) jet.SetMarkerColor(8);
      /*for(int ix=0;ix<(int)MediumBJet.size();ix++){
	if(ij==MediumBJet.at(ix)){
	  jet.SetMarkerColor(8);
	  break;
	}
	}
      constituents.push_back(jet);
    }
  }


  bool drawn=false;
	 for(int ifj = 0; ifj< (int)fjets.size(); ifj++){
    vector<fastjet::PseudoJet> cons = fjets[ifj].constituents();
    for(int ics = 0; ics<(int)cons.size();ics++){
      for(int ifj2=0; ifj2<(int)fjets.size(); ifj2++){
	if(ifj2==ifj) continue;
	TArrow first = TArrow(cons[ics].eta(),cons[ics].phi_std(), fjets[ifj].eta(),fjets[ifj].phi_std(),0.04,">");
	if(deltaR(cons[ics].eta(),cons[ics].phi_std(), fjets[ifj2].eta(),fjets[ifj2].phi_std()) < 1.25){
	  //cout<<"type 1 line: con eta phi FJ eta phi"<<cons[ics].eta()<<" "<<cons[ics].phi_std()<<" "<<fjets[ifj2].eta()<<" "<<fjets[ifj2].phi_std()<<endl;
	  first.DrawArrow(cons[ics].eta(),cons[ics].phi_std(), fjets[ifj].eta(),fjets[ifj].phi_std(),0.0045,"|>");
	  drawn=true;
	  break;
	}
	else if(deltaR(cons[ics].eta(),cons[ics].phi_std(), fjets[ifj].eta(),fjets[ifj].phi_std()) > 1.15){
	  //cout<<"type 2 line: con eta phi FJ eta phi"<<cons[ics].eta()<<" "<<cons[ics].phi_std()<<" "<<fjets[ifj].eta()<<" "<<fjets[ifj].phi_std()<<endl;
	  first.DrawArrow(cons[ics].eta(),cons[ics].phi_std(), fjets[ifj].eta(),fjets[ifj].phi_std(),0.0045,"|>");
	  drawn=true;
	  break;}
      }
    }
    cons.clear();
    }



	 }
}*/

//
// per process
//
void MakeHists(TChain *ch, char* Region, char* sys=(char*)"") 
{ 

  InitBaby(ch); 
  TString ChainName = Form("%s_%s%s",ch->GetTitle(),Region,sys);
  //vector<int> eventlist;
  cout << "[MJ Analysis] " << ChainName << endl;  
    
  //
  // Define histograms
  //
  TH1F *h1_yields[7];
   
  TH1F *h1_MJ[7], *h1_mT[7], *h1_Nskinnyjet[7], *h1_Ncsvm[7], 
    *h1_muspT[7], *h1_muspTminusMET[7], *h1_musEta[7], *h1_musPhi[7], 
    *h1_elspT[7], *h1_elspTminusMET[7], *h1_elsEta[7], *h1_elsPhi[7], 
    *h1_mj[7], *h1_FatjetPt[7], *h1_FatjetEta[7], 
    *h1_FatjetPt1[7],  *h1_FatjetPt2[7],  *h1_FatjetPt3[7],  *h1_FatjetPt4[7],
    *h1_mj1[7],  *h1_mj2[7],  *h1_mj3[7],  *h1_mj4[7], 
    *h1_mj3overmj2[7], *h1_mj2overmj1[7],
    *h1_FatjetPhi1[7], *h1_FatjetPhi2[7], *h1_FatjetPhi3[7], *h1_FatjetPhi4[7],
    *h1_FatjetEta1[7], *h1_FatjetEta2[7], *h1_FatjetEta3[7], *h1_FatjetEta4[7],
    *h1_dRFJ[7], *h1_dPhiFJ[7], *h1_dEtaFJ[7],
    *h1_HT[7], *h1_MET[7], *h1_METPhi[7], *h1_METx[7], *h1_METy[7], *h1_DPhi[7], *h1_Nfatjet[7], *h1_WpT[7];
  TH1F *h1_mindPhi_B_met[7];
  TH2F *h2_mj1vsmj2[7], *h2_mj2vsmj3[7], *h2_mj3vsmj4[7];
  TH2F *h2_HTMET[7], *h2_MJmT[7];
  TH2F *h2_HTmT[7], *h2_MJMET[7];
  TH2F *h2_HTMJ[7], *h2_METmT[7];
  TH1F *h1_toppT1[7], *h1_toppT2[7];
  TH1F *h1_toppT[7];
  TH1F *h1_nISR[7];
  TH1F *h1_ttbarpT[7];
  TH2F *h2_toppT[7];
  TH1F *h1_toppT1_incl, *h1_toppT2_incl;
  TH1F *h1_toppT_incl;
  TH1F *h1_nISR_incl;
  TH1F *h1_ttbarpT_incl;
  TH2F *h2_toppT_incl;
  TH1F * h1_yields_binned[7][2][3][2][3][3][2];
  TH1F *h1_nBSJ_perFJ[7];
  TH1F *h1_max_nBSJ_perEvent[7];
  TH1F *h1_sumCSV_perFJ[7];
  TH1F *h1_max_sumCSV_perEvent[7];
  TH1F *h1_mj_BFJ[7];
  TH1F *h1_MJ_BFJ[7];
  TH1F *h1_mj1_BFJ[7];
  TH1F *h1_mj2_BFJ[7];
  TH1F *h1_mj3_BFJ[7];
  
  //nFJ, MJ, MET, MT, NB, NSJ, HT
  //nsj 45,67,89
  TH1F *h1_MJ_coarse[7];
  Float_t xbins[] = {0,200,300,400,500,600,2000};
  const int nbin = 6;
  TH1F* h1_MJ_weights_by_bin[7][nbin];




    
    
  h1_toppT1_incl = InitTH1F( Form("h1_%s_toppT1_incl", ChainName.Data()), 
			     Form("h1_%s_toppT1_incl", ChainName.Data()), 
			     //20, 350, 1350);
			     30, 0, 1500);

  h1_toppT2_incl = InitTH1F( Form("h1_%s_toppT2_incl", ChainName.Data()), 
			     Form("h1_%s_toppT2_incl", ChainName.Data()), 
			     //20, 350, 1350);
			     30, 0, 1500);
    
  h1_toppT_incl = InitTH1F( Form("h1_%s_toppT_incl", ChainName.Data()), 
			    Form("h1_%s_toppT_incl", ChainName.Data()), 
			    //20, 350, 1350);
			    30, 0, 1500);
  h1_nISR_incl = InitTH1F( Form("h1_%s_nISR_incl", ChainName.Data()), 
			   Form("h1_%s_nISR_incl", ChainName.Data()), 
			   //20, 350, 1350);
			   6, -0.5, 5.5);
  h1_ttbarpT_incl = InitTH1F( Form("h1_%s_ttbarpT_incl", ChainName.Data()), 
			      Form("h1_%s_ttbarpT_incl", ChainName.Data()), 
			      //20, 350, 1350);
			      20, 0, 1600);
  h2_toppT_incl = InitTH2F( Form("h2_%s_toppT_incl", ChainName.Data()), 
			    Form("h2_%s_toppT_incl", ChainName.Data()), 
			    //20, 350, 1350);
			    30, 0, 1500,30, 0, 1500);

  for(int i=0; i<7; i++) 
    {
      for(int nMJ=0;nMJ<2;nMJ++){
	for(int nMET=0;nMET<3;nMET++){
	  for(int nMT=0;nMT<2;nMT++){
	    
	    for(int nb=0;nb<3;nb++){
	      for(int nsj=0;nsj<3;nsj++){
		for(int nHT=0;nHT<2;nHT++){
		  h1_yields_binned[i][nMJ][nMET][nMT][nb][nsj][nHT] = InitTH1F( Form("h1_%s_yields_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", ChainName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT), 
										Form("h1_%s_yields_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", ChainName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT), 
										3, 0, 3);
		}
	      }
	    }
	    
	  }
	}//nFJ, MJ, MET, MT, Nlep, NB
      }
      //
      // yields : for table                   
      //
      h1_yields[i] = InitTH1F( Form("h1_%s_yields_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_yields_%ifatjet", ChainName.Data(), i), 
			       2, 0, 2); // bin1 for e, bin2 for mu
      //
      // h1                    
      //
      h1_MJ[i] = InitTH1F( Form("h1_%s_MJ_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_MJ_%ifatjet", ChainName.Data(), i), 
			   20, 0, 2000);

      if(ChainName.Contains("TT_sys")){
	for(int j=0;j<nbin; j++){
	  h1_MJ_weights_by_bin[i][j] = InitTH1F( Form("h1_%s_MJ_weights_by_bin_%ifatjet_bin%i", ChainName.Data(), i, j), 
						 Form("h1_%s_MJ_weights_by_bin_%ifatjet_bin%i", ChainName.Data(), i, j), 
						 6,-3,3);
	}
      }
	
	
	
      h1_MJ_coarse[i] = InitTH1FVarBin( Form("h1_%s_MJ_coarse_%ifatjet", ChainName.Data(), i), 
					Form("h1_%s_MJ_%ifatjet", ChainName.Data(), i), 
					nbin,xbins);

      h1_nBSJ_perFJ[i]= InitTH1F( Form("h1_%s_nBSJ_perFJ_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_nBSJ_perFJ_%ifatjet", ChainName.Data(), i), 
			      5, -0.5, 4.5);
       h1_max_nBSJ_perEvent[i]= InitTH1F( Form("h1_%s_max_nBSJ_perEvent_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_max_nBSJ_perEvent_%ifatjet", ChainName.Data(), i), 
			      5, -0.5, 4.5);

        h1_sumCSV_perFJ[i]= InitTH1F( Form("h1_%s_sumCSV_perFJ_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_sumCSV_perFJ_%ifatjet", ChainName.Data(), i), 
			      10, 0, 5);
       h1_max_sumCSV_perEvent[i]= InitTH1F( Form("h1_%s_max_sumCSV_perEvent_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_max_sumCSV_perEvent_%ifatjet", ChainName.Data(), i), 
			     10, 0, 5);
       
       h1_mj_BFJ[i]=InitTH1F( Form("h1_%s_mj_BFJ_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_mj_BFJ_%ifatjet", ChainName.Data(), i), 
			   10, 0, 1000);

       h1_MJ_BFJ[i]=InitTH1F( Form("h1_%s_sumMJ_BFJ_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_sumMJ_BFJ_%ifatjet", ChainName.Data(), i), 
			      20, 0, 2000);
       
       h1_mj1_BFJ[i]=InitTH1F( Form("h1_%s_mj1_BFJ_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_mj1_BFJ_%ifatjet", ChainName.Data(), i), 
			   10, 0, 1000);
 h1_mj2_BFJ[i]=InitTH1F( Form("h1_%s_mj2_BFJ_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_mj2_BFJ_%ifatjet", ChainName.Data(), i), 
			   10, 0, 1000);

  h1_mj3_BFJ[i]=InitTH1F( Form("h1_%s_mj3_BFJ_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_mj3_BFJ_%ifatjet", ChainName.Data(), i), 
			   10, 0, 1000);


      
      h1_mindPhi_B_met[i] = InitTH1F( Form("h1_%s_mindPhi_B_met_%ifatjet", ChainName.Data(), i), 
				      Form("h1_%s_mindPhi_B_met_%ifatjet", ChainName.Data(), i), 
				      5, 0, 3.1416);
	
      h1_mj[i] = InitTH1F( Form("h1_%s_mj_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_mj_%ifatjet", ChainName.Data(), i), 
			   40, 0, 800);
      h1_Nfatjet[i] = InitTH1F( Form("h1_%s_Nfatjet_%ifatjet", ChainName.Data(), i), 
				Form("h1_%s_Nfatjet_%ifatjet", ChainName.Data(), i), 
				11, -0.5, 10.5);
      h1_Nskinnyjet[i] = InitTH1F( Form("h1_%s_Nskinnyjet_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_Nskinnyjet_%ifatjet", ChainName.Data(), i), 
				   16, -0.5, 15.5);
      h1_Ncsvm[i] = InitTH1F( Form("h1_%s_Ncsvm_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_Ncsvm_%ifatjet", ChainName.Data(), i), 
			      7, -0.5, 6.5);
      h1_mT[i] = InitTH1F( Form("h1_%s_mT_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_mT_%ifatjet", ChainName.Data(), i), 
			   //20, 0, 200);
			   16, 0, 800);
      h1_muspT[i] = InitTH1F( Form("h1_%s_muspT_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_muspT_%ifatjet", ChainName.Data(), i), 
			      20, 0, 200);
      h1_muspTminusMET[i] = InitTH1F( Form("h1_%s_muspTminusMET_%ifatjet", ChainName.Data(), i), 
				      Form("h1_%s_muspTminusMET_%ifatjet", ChainName.Data(), i), 
				      //20, 100, 400);
				      20, -2, 8);
      h1_musEta[i] = InitTH1F( Form("h1_%s_musEta_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_musEta_%ifatjet", ChainName.Data(), i), 
			       20, -2.5, 2.5);
      //100, -2.1, 2.1);
      h1_musPhi[i] = InitTH1F( Form("h1_%s_musPhi_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_musPhi_%ifatjet", ChainName.Data(), i), 
			       20, -TMath::Pi(), TMath::Pi());
      h1_elspT[i] = InitTH1F( Form("h1_%s_elspT_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_elspT_%ifatjet", ChainName.Data(), i), 
			      20, 0, 200);
      h1_elspTminusMET[i] = InitTH1F( Form("h1_%s_elspTminusMET_%ifatjet", ChainName.Data(), i), 
				      Form("h1_%s_elspTminusMET_%ifatjet", ChainName.Data(), i), 

				      //20, 100, 400);
				      20, -2, 8);
      h1_elsEta[i] = InitTH1F( Form("h1_%s_elsEta_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_elsEta_%ifatjet", ChainName.Data(), i), 
			       20, -2.5, 2.5);
      //100, -2.1, 2.1);
      h1_elsPhi[i] = InitTH1F( Form("h1_%s_elsPhi_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_elsPhi_%ifatjet", ChainName.Data(), i), 
			       20, -TMath::Pi(), TMath::Pi());
      h1_HT[i] = InitTH1F( Form("h1_%s_HT_%ifatjet", ChainName.Data(), i), 
			   Form("h1_%s_HT_%ifatjet", ChainName.Data(), i), 
			   //20, 350, 1350);
			   28, 500, 4000);

      h1_toppT1[i] = InitTH1F( Form("h1_%s_toppT1_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_toppT1_%ifatjet", ChainName.Data(), i), 
			       //20, 350, 1350);
			       30, 0, 1500);

      h1_toppT2[i] = InitTH1F( Form("h1_%s_toppT2_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_toppT2_%ifatjet", ChainName.Data(), i), 
			       //20, 350, 1350);
			       30, 0, 1500);
      h1_toppT[i] = InitTH1F( Form("h1_%s_toppT_%ifatjet", ChainName.Data(), i), 
			      Form("h1_%s_toppT_%ifatjet", ChainName.Data(), i), 
			      //20, 350, 1350);
			      30, 0, 1500);
      h1_nISR[i] = InitTH1F( Form("h1_%s_nISR_%ifatjet", ChainName.Data(), i), 
			     Form("h1_%s_nISR_%ifatjet", ChainName.Data(), i), 
			     //20, 350, 1350);
			     6,-0.5, 5.5);
      h1_ttbarpT[i] = InitTH1F( Form("h1_%s_ttbarpT_%ifatjet", ChainName.Data(), i), 
				Form("h1_%s_ttbarpT_%ifatjet", ChainName.Data(), i), 
				//20, 350, 1350);
				20, 0, 1600);
      h2_toppT[i] = InitTH2F( Form("h2_%s_toppT_%ifatjet", ChainName.Data(), i), 
			      Form("h2_%s_toppT_%ifatjet", ChainName.Data(), i), 
			      //20, 350, 1350);
			      30, 0, 1500,30, 0, 1500);
      
      
      h1_MET[i] = InitTH1F( Form("h1_%s_MET_%ifatjet", ChainName.Data(), i), 
			    Form("h1_%s_MET_%ifatjet", ChainName.Data(), i), 
			    //20, 0, 500);
			    18, 100, 1000);
      h1_METPhi[i] = InitTH1F( Form("h1_%s_METPhi_%ifatjet", ChainName.Data(), i), 
			       Form("h1_%s_METPhi_%ifatjet", ChainName.Data(), i), 
			       20, -TMath::Pi(), TMath::Pi());
      h1_METx[i] = InitTH1F( Form("h1_%s_METx_%ifatjet", ChainName.Data(), i), 
			     Form("h1_%s_METx_%ifatjet", ChainName.Data(), i), 
			     100, -300, 300);
      h1_METy[i] = InitTH1F( Form("h1_%s_METy_%ifatjet", ChainName.Data(), i), 
			     Form("h1_%s_METy_%ifatjet", ChainName.Data(), i), 
			     100, -300, 300);
      h1_DPhi[i] = InitTH1F( Form("h1_%s_DPhi_%ifatjet", ChainName.Data(), i), 
			     Form("h1_%s_DPhi_%ifatjet", ChainName.Data(), i), 
			     32, 0, TMath::Pi());
      h1_dRFJ[i] = InitTH1F( Form("h1_%s_dRFJ_%ifatjet", ChainName.Data(), i), 
			     Form("h1_%s_dRFJ_%ifatjet", ChainName.Data(), i), 
			     20, 1, 5);
      h1_dPhiFJ[i] = InitTH1F( Form("h1_%s_dPhiFJ_%ifatjet", ChainName.Data(), i), 
                               Form("h1_%s_dPhiFJ_%ifatjet", ChainName.Data(), i), 
                               32, 0, TMath::Pi());
      h1_dEtaFJ[i] = InitTH1F( Form("h1_%s_dEtaFJ_%ifatjet", ChainName.Data(), i), 
                               Form("h1_%s_dEtaFJ_%ifatjet", ChainName.Data(), i), 
                               20, 0, 4);
      h1_FatjetPt[i] = InitTH1F( Form("h1_%s_FatjetPt_%ifatjet", ChainName.Data(), i), 
				 Form("h1_%s_FatjetPt_%ifatjet", ChainName.Data(), i), 
				 20, 0, 800);
      h1_FatjetPt1[i] = InitTH1F( Form("h1_%s_FatjetPt1_%ifatjet", ChainName.Data(), i), 
				  Form("h1_%s_FatjetPt1_%ifatjet", ChainName.Data(), i), 
				  20, 0, 2000);
      h1_FatjetPt2[i] = InitTH1F( Form("h1_%s_FatjetPt2_%ifatjet", ChainName.Data(), i), 
				  Form("h1_%s_FatjetPt2_%ifatjet", ChainName.Data(), i), 
				  20, 0, 1200);
      h1_FatjetPt3[i] = InitTH1F( Form("h1_%s_FatjetPt3_%ifatjet", ChainName.Data(), i), 
				  Form("h1_%s_FatjetPt3_%ifatjet", ChainName.Data(), i), 
				  20, 0, 800);
      h1_FatjetPt4[i] = InitTH1F( Form("h1_%s_FatjetPt4_%ifatjet", ChainName.Data(), i), 
				  Form("h1_%s_FatjetPt4_%ifatjet", ChainName.Data(), i), 
				  20, 0, 500);
      h1_mj1[i] = InitTH1F( Form("h1_%s_mj1_%ifatjet", ChainName.Data(), i), 
			    Form("h1_%s_mj1_%ifatjet", ChainName.Data(), i), 
			    40, 0, 1000);
      h1_mj2[i] = InitTH1F( Form("h1_%s_mj2_%ifatjet", ChainName.Data(), i), 
			    Form("h1_%s_mj2_%ifatjet", ChainName.Data(), i), 
			    30, 0, 750);
      h1_mj3[i] = InitTH1F( Form("h1_%s_mj3_%ifatjet", ChainName.Data(), i), 
			    Form("h1_%s_mj3_%ifatjet", ChainName.Data(), i), 
			    20, 0, 500);
      h1_mj4[i] = InitTH1F( Form("h1_%s_mj4_%ifatjet", ChainName.Data(), i), 
			    Form("h1_%s_mj4_%ifatjet", ChainName.Data(), i), 
			    20, 0, 500);
      h1_mj3overmj2[i] = InitTH1F( Form("h1_%s_mj3overmj2_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_mj3overmj2_%ifatjet", ChainName.Data(), i), 
				   20, 0, 1);
      h1_mj2overmj1[i] = InitTH1F( Form("h1_%s_mj2overmj1_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_mj2overmj1_%ifatjet", ChainName.Data(), i), 
				   20, 0, 1);
      h1_FatjetPhi1[i] = InitTH1F( Form("h1_%s_FatjetPhi1_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetPhi1_%ifatjet", ChainName.Data(), i), 
				   32, -TMath::Pi(), TMath::Pi());
      h1_FatjetPhi2[i] = InitTH1F( Form("h1_%s_FatjetPhi2_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetPhi2_%ifatjet", ChainName.Data(), i), 
				   32, -TMath::Pi(), TMath::Pi());
      h1_FatjetPhi3[i] = InitTH1F( Form("h1_%s_FatjetPhi3_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetPhi3_%ifatjet", ChainName.Data(), i), 
				   32, -TMath::Pi(), TMath::Pi());
      h1_FatjetPhi4[i] = InitTH1F( Form("h1_%s_FatjetPhi4_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetPhi4_%ifatjet", ChainName.Data(), i), 
				   32, -TMath::Pi(), TMath::Pi());
      h1_FatjetEta1[i] = InitTH1F( Form("h1_%s_FatjetEta1_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetEta1_%ifatjet", ChainName.Data(), i), 
				   30, -3, 3);
      h1_FatjetEta2[i] = InitTH1F( Form("h1_%s_FatjetEta2_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetEta2_%ifatjet", ChainName.Data(), i), 
				   30, -3, 3);
      h1_FatjetEta3[i] = InitTH1F( Form("h1_%s_FatjetEta3_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetEta3_%ifatjet", ChainName.Data(), i), 
				   30, -3, 3);
      h1_FatjetEta4[i] = InitTH1F( Form("h1_%s_FatjetEta4_%ifatjet", ChainName.Data(), i), 
				   Form("h1_%s_FatjetEta4_%ifatjet", ChainName.Data(), i), 
				   30, -3, 3);
      h1_FatjetEta[i] = InitTH1F( Form("h1_%s_FatjetEta_%ifatjet", ChainName.Data(), i), 
				  Form("h1_%s_FatjetEta_%ifatjet", ChainName.Data(), i), 
				  20, -2.5, 2.5);
      h1_WpT[i] = InitTH1F( Form("h1_%s_WpT_%ifatjet", ChainName.Data(), i), 
			    Form("h1_%s_WpT_%ifatjet", ChainName.Data(), i), 
			    //20, 0, 500);
			    20, 0, 1000);
      //
      // h2                    
      //
      h2_HTMET[i]    =   InitTH2F(Form("h2_%s_HTMET_%ifatjet", ChainName.Data(), i),
				  Form("h2_%s_HTMET_%ifatjet", ChainName.Data(), i), 
				  25, 500, 1750, 10, 250, 750);
      h2_MJmT[i]    =   InitTH2F(Form("h2_%s_MJmT_%ifatjet", ChainName.Data(), i),
				 Form("h2_%s_MJmT_%ifatjet", ChainName.Data(), i), 
				 15, 0, 750, 10, 0, 500);
      h2_HTmT[i]    =   InitTH2F(Form("h2_%s_HTmT_%ifatjet", ChainName.Data(), i),
				 Form("h2_%s_HTmT_%ifatjet", ChainName.Data(), i), 
				 25, 500, 1750, 10, 0, 500);
      h2_MJMET[i]    =   InitTH2F(Form("h2_%s_MJMET_%ifatjet", ChainName.Data(), i),
				  Form("h2_%s_MJMET_%ifatjet", ChainName.Data(), i), 
				  15, 0, 750, 10, 250, 750);
      h2_HTMJ[i]    =   InitTH2F(Form("h2_%s_HTMJ_%ifatjet", ChainName.Data(), i),
				 Form("h2_%s_HTMJ_%ifatjet", ChainName.Data(), i), 
				 25, 500, 1750, 15, 0, 750);
      h2_METmT[i]    =   InitTH2F(Form("h2_%s_METmT_%ifatjet", ChainName.Data(), i),
				  Form("h2_%s_METmT_%ifatjet", ChainName.Data(), i), 
				  10, 250, 750, 10, 0, 500);
      h2_mj1vsmj2[i]    =   InitTH2F(Form("h2_%s_mj1vsmj2_%ifatjet", ChainName.Data(), i),
				     Form("h2_%s_mj1vsmj2_%ifatjet", ChainName.Data(), i), 
				     10, 0, 500, 10, 0, 500);
      h2_mj2vsmj3[i]    =   InitTH2F(Form("h2_%s_mj2vsmj3_%ifatjet", ChainName.Data(), i),
				     Form("h2_%s_mj2vsmj3_%ifatjet", ChainName.Data(), i), 
				     10, 0, 500, 10, 0, 500);
      h2_mj3vsmj4[i]    =   InitTH2F(Form("h2_%s_mj3vsmj4_%ifatjet", ChainName.Data(), i),
				     Form("h2_%s_mj4vsmj4_%ifatjet", ChainName.Data(), i), 
				     10, 0, 500, 10, 0, 500);
    }
    
  //
  //
  //
  int i_permille_old = 0; 
  TDatime DTStart;
  int StartDate = DTStart.GetDate();
  int StartTime = DTStart.GetTime();
  cout << "[MJ Analysis] Start time : " << (StartTime/10000)%100 << ":"
       << (StartTime/100)%100 << ":" << StartTime%100
       << endl;
  
  // 
  // TT composition 
  // 
  float Nll=0.;
  float Nlt=0.;
  float Ntt=0.;
  float Nltl=0.;
  float Nlth=0.;

  float N_pre_toppT=0;
  float N_post_toppT=0;
  //
  //
  //
  //InitTree(ch);
  Int_t nentries = (Int_t)ch->GetEntries();
  for(int i = 0; i<nentries; i++)
    {
      //bool flagged =false;
      ch->GetEntry(i);
      //	if(event_==21962862 || event_==121299115 || event_==64381257) flagged = true;

      // Progress indicator begin --------------------------------
      int i_permille = (int)floor(1000 * i / float(nentries));
      TDatime DTCurrent;
      int CurrentDate = DTCurrent.GetDate();
      int CurrentTime = DTCurrent.GetTime();
      int TimeLaps = (CurrentDate-StartDate)*1000000+(CurrentTime-StartTime);
      int TimeToRun = (int)((float)nentries/(float)i)*TimeLaps;
      if (i_permille != i_permille_old)
        {
	  // xterm magic from L. Vacavant and A. Cerri
	  if ( isatty(1) )
            {
	      printf("\015\033[32m Processed :: \033[1m\033[31m%4.1f %%"
		     "\033[0m\033[32m   Expected processing time :: \033[1m\033[31m%i:%i:%i \033[0m\015",
		     i_permille/10., (TimeToRun/10000)%100<60 ? (TimeToRun/10000)%100 : (TimeToRun/10000)%100-40,
		     (TimeToRun/100)%100<60 ? (TimeToRun/100)%100 : (TimeToRun/100)%100-40,
		     (TimeToRun%100)<60 ? (TimeToRun)%100 : (TimeToRun)%100-40 );
	      fflush(stdout);
            }
	  i_permille_old = i_permille;
        }
      // Progress indicator end ----------------------------------
    
      // separate TT_sl and TT_ll
      int Ngenlep=0;
      for(unsigned int igen=0; igen<GenId_->size(); igen++)
        { 
	  if( TMath::Abs(GenMId_->at(igen))!=24 ) continue;
	  if( TMath::Abs(GenId_->at(igen))!=11 && TMath::Abs(GenId_->at(igen))!=13 && TMath::Abs(GenId_->at(igen))!=15 ) continue;
	  Ngenlep++;
        }
      if(ChainName.Contains("TT_sl") && Ngenlep!=1) continue;  
      if(ChainName.Contains("TT_ll") && Ngenlep!=2) continue;  
      if(ChainName.Contains("TT_sys") && !(Ngenlep==2 || Ngenlep==1)) continue;  
      //calc HT
      float HT40 =0.0;
      int RA4NSJ=0;
      int RA4NB=0;
      float dphimin=10.;
      for(unsigned int a=0;a<JetPt_->size();a++){
	if(JetPt_->at(a)>=40. && TMath::Abs(JetEta_->at(a))<=2.5){ 
	  HT40+=JetPt_->at(a);
	  RA4NSJ++;
	  if(JetCSV_->at(a)>=0.814) {
	    RA4NB++;
	    if( getDPhi(JetPhi_->at(a), METPhi_)<dphimin) dphimin = getDPhi(JetPhi_->at(a), METPhi_);
	  }
	}
      }

      //REMOVE THIS
      // RA4NSJ = JetPt_->size();
      //if(HT40<=750 || RA4NB==0) continue;
	
      int nISR=0;
      if(status)EventWeight_ = EventWeight_*4000.;
      if(EventWeightNeg_<0) EventWeight_*= (-1.0);
      // 
      // weights 
      // 
      // Temp fixes for wrong event weights 
      // TTJets cross section  
      /*  if(ChainName.Contains("TT"))  
	  { 
	  EventWeight_ = EventWeight_*806.1/832.; // I used 832 pb while M used 806.1 pb.
	  }*/
      float ISRpT;
      float orig_weight=EventWeight_;
      N_pre_toppT+=EventWeight_;
      if(ChainName.Contains("TT"))
	{
	  if(status) nISR = GetnISR();
	  float ISRpx = top1pT_*TMath::Cos(top1Phi_) + top2pT_*TMath::Cos(top2Phi_);
	  float ISRpy = top1pT_*TMath::Sin(top1Phi_) + top2pT_*TMath::Sin(top2Phi_);
	  ISRpT = TMath::Sqrt(ISRpx*ISRpx+ISRpy*ISRpy);
	}
      if(ChainName.Contains("TT_sys"))
	{	  
	  //v1
	    
	  if(ChainName.Contains("toppT1")){
	    float t1pT,t2pT;
	    if(top1pT_>400) t1pT=400;
	    else t1pT=top1pT_;

	    if(top2pT_>400) t2pT=400;
	    else t2pT=top2pT_;
	    float weight_top1pT = TMath::Exp(0.159-0.00141*t1pT);
	    float weight_top2pT = TMath::Exp(0.159-0.00141*t2pT);
	    EventWeight_ = EventWeight_ * TMath::Sqrt(weight_top1pT*weight_top2pT);
	    EventWeight_ = EventWeight_ * 0.959;}

	  if(ChainName.Contains("toppT2")){
	    float t1pT,t2pT;
	    if(top1pT_>600) t1pT=600;
	    if(top1pT_<200) t1pT=200;
	    else t1pT=top1pT_;

	    if(top2pT_>600) t2pT=600;
	    if(top2pT_<200) t2pT=200;
	    else t2pT=top2pT_;
	    float weight_top1pT = TMath::Exp(-0.6662+0.00155*t1pT);
	    float weight_top2pT = TMath::Exp(-0.6662+0.00155*t2pT);
	    EventWeight_ = EventWeight_ * TMath::Sqrt(weight_top1pT*weight_top2pT);
	    // EventWeight_ = EventWeight_ * 0.959;
	  }
	    
	  if(ChainName.Contains("ISRpT1")){
	    EventWeight_ = EventWeight_ * getISRSF(ISRpT);}
	  // EventWeight = EventWeight * 1.013;
	  if(ChainName.Contains("ISRpT2")){
	    EventWeight_ = EventWeight_ * getISR2SF(ISRpT);}
	  if(ChainName.Contains("ISRpT3")){
	    EventWeight_ = EventWeight_ * getISR3SF(ISRpT);}

	  if(ChainName.Contains("ISRpT4")){
	    EventWeight_ = EventWeight_ * getISR4SF(ISRpT);}

	  if(ChainName.Contains("nISR")){
	    EventWeight_= EventWeight_ * GetNISRSF(nISR);
	  }
	}
	
      N_post_toppT+=EventWeight_;
      if(ChainName.Contains("TT")) {
	FillTH1F(h1_ttbarpT_incl, ISRpT, EventWeight_);
	FillTH1F(h1_toppT_incl, top1pT_, EventWeight_);
	FillTH1F(h1_nISR_incl, nISR, EventWeight_);
	FillTH1F(h1_toppT_incl, top2pT_, EventWeight_);
	FillTH2F(h2_toppT_incl, top1pT_, top2pT_, EventWeight_);
	FillTH1F(h1_toppT1_incl, top1pT_, EventWeight_);
	FillTH1F(h1_toppT2_incl, top2pT_, EventWeight_);
      }
      // Pileup 

      // 
      // single-lepton events selection
      //
      //if(flagged) cout<<"flagged event HT40 NSJ NB RA4mus RA4els RA4musveto RA4elsveto "<<event_<<"   "<<HT40<<"   "<<RA4NSJ<<"   "<<RA4NB<<"   "<<RA4MusPt_->size()<<"   "<<RA4ElsPt_->size()<<"   "<<RA4MusVetoPt_->size()<<"   "<<RA4ElsVetoPt_->size()<<endl;
      if(!(PassNLep(1) || PassNLep(2)))  continue; // need this upfront because of mT calculation

      // Nfatjet, MJ, mj sorting 
      int Nfatjet_thres = 0;
      double MJ_thres=0; 
      vector<double> mj_thres_sorted; 
      for(int ifj=0; ifj<(int)FatjetPt_->size(); ifj++)
        {   
	  if(FatjetPt_->at(ifj)<FatjetpTthres) continue; 
	  if(mj_->at(ifj)<mjthres) continue; 
	  Nfatjet_thres++;
	  MJ_thres = MJ_thres + mj_->at(ifj);   
	  mj_thres_sorted.push_back(mj_->at(ifj));
        }
      sort(mj_thres_sorted.begin(), mj_thres_sorted.end());
      reverse(mj_thres_sorted.begin(), mj_thres_sorted.end());

      //
      // Calculate variables 
      //
      double mT=0;
      double WpT=0;
      if(PassNLep(1)){
	if(RA4MusPt_->size()==1) {
	  mT  = TMath::Sqrt( 2*MET_*RA4MusPt_->at(0)*(1-TMath::Cos(METPhi_-RA4MusPhi_->at(0))) ); 
	  WpT = TMath::Sqrt(  
			    (RA4MusPt_->at(0)*TMath::Cos(RA4MusPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    *(RA4MusPt_->at(0)*TMath::Cos(RA4MusPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    +(RA4MusPt_->at(0)*TMath::Sin(RA4MusPhi_->at(0)) + MET_*TMath::Sin(METPhi_))
			    *(RA4MusPt_->at(0)*TMath::Sin(RA4MusPhi_->at(0)) + MET_*TMath::Sin(METPhi_))  ); 
	}
	if(RA4ElsPt_->size()==1) {
	  mT  = TMath::Sqrt( 2*MET_*RA4ElsPt_->at(0)*(1-TMath::Cos(METPhi_-RA4ElsPhi_->at(0))) ); 
	  WpT = TMath::Sqrt(  
			    (RA4ElsPt_->at(0)*TMath::Cos(RA4ElsPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    *(RA4ElsPt_->at(0)*TMath::Cos(RA4ElsPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    +(RA4ElsPt_->at(0)*TMath::Sin(RA4ElsPhi_->at(0)) + MET_*TMath::Sin(METPhi_))
			    *(RA4ElsPt_->at(0)*TMath::Sin(RA4ElsPhi_->at(0)) + MET_*TMath::Sin(METPhi_))  ); 
	}
      }

      else if(PassNLep(2)){
	if(RA4MusPt_->size()==2) {
	  mT  = TMath::Sqrt( 2*MET_*RA4MusPt_->at(0)*(1-TMath::Cos(METPhi_-RA4MusPhi_->at(0))) ); 
	  WpT = TMath::Sqrt(  
			    (RA4MusPt_->at(0)*TMath::Cos(RA4MusPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    *(RA4MusPt_->at(0)*TMath::Cos(RA4MusPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    +(RA4MusPt_->at(0)*TMath::Sin(RA4MusPhi_->at(0)) + MET_*TMath::Sin(METPhi_))
			    *(RA4MusPt_->at(0)*TMath::Sin(RA4MusPhi_->at(0)) + MET_*TMath::Sin(METPhi_))  ); 
	}
	if(RA4ElsPt_->size()==2) {
	  mT  = TMath::Sqrt( 2*MET_*RA4ElsPt_->at(0)*(1-TMath::Cos(METPhi_-RA4ElsPhi_->at(0))) ); 
	  WpT = TMath::Sqrt(  
			    (RA4ElsPt_->at(0)*TMath::Cos(RA4ElsPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    *(RA4ElsPt_->at(0)*TMath::Cos(RA4ElsPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			    +(RA4ElsPt_->at(0)*TMath::Sin(RA4ElsPhi_->at(0)) + MET_*TMath::Sin(METPhi_))
			    *(RA4ElsPt_->at(0)*TMath::Sin(RA4ElsPhi_->at(0)) + MET_*TMath::Sin(METPhi_))  ); 
	}
	  
	if(RA4MusPt_->size()==1 && RA4ElsPt_->size()==1 ) {
	  if(RA4MusPt_->at(0) > RA4ElsPt_->at(0)){ mT  = TMath::Sqrt( 2*MET_*RA4MusPt_->at(0)*(1-TMath::Cos(METPhi_-RA4MusPhi_->at(0))) ); 
	    WpT = TMath::Sqrt(  
			      (RA4MusPt_->at(0)*TMath::Cos(RA4MusPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			      *(RA4MusPt_->at(0)*TMath::Cos(RA4MusPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			      +(RA4MusPt_->at(0)*TMath::Sin(RA4MusPhi_->at(0)) + MET_*TMath::Sin(METPhi_))
			      *(RA4MusPt_->at(0)*TMath::Sin(RA4MusPhi_->at(0)) + MET_*TMath::Sin(METPhi_))  ); 
	  }
	  else{
	    mT  = TMath::Sqrt( 2*MET_*RA4ElsPt_->at(0)*(1-TMath::Cos(METPhi_-RA4ElsPhi_->at(0))) ); 
	    WpT = TMath::Sqrt(  
			      (RA4ElsPt_->at(0)*TMath::Cos(RA4ElsPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			      *(RA4ElsPt_->at(0)*TMath::Cos(RA4ElsPhi_->at(0)) + MET_*TMath::Cos(METPhi_))
			      +(RA4ElsPt_->at(0)*TMath::Sin(RA4ElsPhi_->at(0)) + MET_*TMath::Sin(METPhi_))
			      *(RA4ElsPt_->at(0)*TMath::Sin(RA4ElsPhi_->at(0)) + MET_*TMath::Sin(METPhi_))  ); 
	  }
	}

      }
      //
      // Apply selection   
      //
      // baseline selection
      // if( !PassBaselineSelection() ) continue; // now part of PassSelection
      int NFJbin = -1;
      if(Nfatjet_thres>4) NFJbin=5;
      else if(Nfatjet_thres>3) NFJbin=4;
      else if(Nfatjet_thres>2) NFJbin=3;
      else if(Nfatjet_thres>1) NFJbin=2;
      else if(Nfatjet_thres>0) NFJbin=1;
      else NFJbin=0;

      if(NFJbin==-1) 
        {   
	  cout << "[MJ Analysis] ERROR : NFJ is cannot be negative" << endl;
	  continue;
        }

      if(ChainName.Contains("baseline")){
	// cout<<"Region == baseline"<<endl;
	if( !PassSelection((TString)"lobbaseline", HT40, MET_, RA4NB, RA4NSJ, mT, MJ_thres)) continue;

	int bMJ = 0;
	if(MJ_thres>600) bMJ=1;
	int bMET = 0;
	if(MET_>250) bMET=1;
	if(MET_>400) bMET=2;
	int bMT=0;
	if(mT>150) bMT=1;
	int bnb=0;
	if(RA4NB==2) bnb=1;
	if(RA4NB>2) bnb=2;
	int bsj = 0;
	if(RA4NSJ>5) bsj=1;
	if(RA4NSJ>7) bsj=2;
	int nHT=0;
	if(HT40>750) nHT=1;

	float lepbin;
	if(PassNLep(1) && RA4ElsPt_->size()==1) lepbin = 0.5;
	if(PassNLep(1) && RA4MusPt_->size()==1) lepbin = 1.5;
	if(PassNLep(2)) lepbin=2.5;
	h1_yields_binned[NFJbin][bMJ][bMET][bMT][bnb][bsj][nHT]->Fill(lepbin,EventWeight_);
	h1_yields_binned[6][bMJ][bMET][bMT][bnb][bsj][nHT]->Fill(lepbin,EventWeight_);
      }
      if( !PassSelection(Region, HT40, MET_, RA4NB, RA4NSJ, mT, MJ_thres,ChainName,Ngenlep)) continue;
	
	
      //     
      // ttbar gen composition  
      //     
      int Ne=0;
      int Nm=0;
      int Nt=0;
      int Ntn=0; int Nen=0; int Nmn=0;
      for(unsigned int igen=0; igen<GenId_->size(); igen++)
	{ 
	  if( TMath::Abs(GenMId_->at(igen))==24 && TMath::Abs(GenId_->at(igen))==11 ) Ne++;
	  if( TMath::Abs(GenMId_->at(igen))==24 && TMath::Abs(GenId_->at(igen))==13 ) Nm++;
	  if( TMath::Abs(GenMId_->at(igen))==24 && TMath::Abs(GenId_->at(igen))==15 ) Nt++;
	  if( (TMath::Abs(GenGMId_->at(igen))==24||TMath::Abs(GenGMId_->at(igen))==15) && TMath::Abs(GenMId_->at(igen))==15 && TMath::Abs(GenId_->at(igen))==12 ) Nen++;
	  if( (TMath::Abs(GenGMId_->at(igen))==24||TMath::Abs(GenGMId_->at(igen))==15) && TMath::Abs(GenMId_->at(igen))==15 && TMath::Abs(GenId_->at(igen))==14 ) Nmn++;
	  if( (TMath::Abs(GenGMId_->at(igen))==24||TMath::Abs(GenGMId_->at(igen))==15) && TMath::Abs(GenMId_->at(igen))==15 && TMath::Abs(GenId_->at(igen))==16 ) Ntn++;
        }
      //cout << Ne << " " << Nm << " " << Nt << " " << Nen << " " << Nmn << " " << Ntn << endl;
      if( Ne==2 || (Ne==1 && Nm==1) || Nm==2 )                                    Nll  = Nll + EventWeight_; 
      if( (Ne==1 || Nm==1) && Nt==1 )                                             Nlt  = Nlt + EventWeight_; 
      if( Nt==2 )                                                                 Ntt  = Ntt + EventWeight_; 
      if( (Ne==1 || Nm==1) && Nt==1 && (Ntn==1 && (Nen==1 || Nmn==1)) )           Nltl = Nltl + EventWeight_; 
      if( (Ne==1 || Nm==1) && Nt==1 && (Ntn==1 && (Nen==0 && Nmn==0)) )           Nlth = Nlth + EventWeight_; 
	
      if(ChainName.Contains("TT")) {
	FillTH1FAll(h1_toppT1, NFJbin, top1pT_, EventWeight_);
	FillTH1FAll(h1_toppT2, NFJbin, top2pT_, EventWeight_);
	FillTH1FAll(h1_nISR, NFJbin, nISR, EventWeight_);
	FillTH1FAll(h1_ttbarpT, NFJbin, ISRpT, EventWeight_);
	FillTH1FAll(h1_toppT, NFJbin, top1pT_, EventWeight_);
	FillTH1FAll(h1_toppT, NFJbin, top2pT_, EventWeight_);
	FillTH2FAll(h2_toppT, NFJbin, top1pT_, top2pT_, EventWeight_);
      }
      //
      // Fill histogams 
      //

      // yields
      if(RA4ElsPt_->size()==1) FillTH1FAll(h1_yields, NFJbin, 0.5, EventWeight_);   
      if(RA4MusPt_->size()==1) FillTH1FAll(h1_yields, NFJbin, 1.5, EventWeight_);
      //if(NFJbin==5 && ChainName.Contains("TT_ll") ) eventlist.push_back(event_);
        
      // plots
      if(RA4MusPt_->size()==1) {
	FillTH1FAll(h1_muspT,   NFJbin,  RA4MusPt_->at(0),  EventWeight_); 
	FillTH1FAll(h1_musEta,  NFJbin, RA4MusEta_->at(0), EventWeight_); 
	FillTH1FAll(h1_musPhi,  NFJbin, RA4MusPhi_->at(0), EventWeight_); 
	if(getDPhi(RA4MusPhi_->at(0),METPhi_)<0.4) 
	  { 
	    FillTH1FAll(h1_muspTminusMET, NFJbin, (MET_-RA4MusPt_->at(0))/RA4MusPt_->at(0), EventWeight_);   
	  }
      }
      if(RA4ElsPt_->size()==1) {
	FillTH1FAll(h1_elspT, NFJbin, RA4ElsPt_->at(0), EventWeight_);   
	FillTH1FAll(h1_elsEta, NFJbin, RA4ElsEta_->at(0), EventWeight_); 
	FillTH1FAll(h1_elsPhi, NFJbin, RA4ElsPhi_->at(0), EventWeight_); 
	if(getDPhi(RA4ElsPhi_->at(0),METPhi_)<0.4) 
	  { 
	    FillTH1FAll(h1_elspTminusMET, NFJbin, (MET_-RA4ElsPt_->at(0))/RA4ElsPt_->at(0), EventWeight_);   
	  }
      }


	
	 
      FillTH1FAll(h1_mindPhi_B_met, NFJbin, dphimin,EventWeight_);
      
    

      int max_nBSJ = 0;
      float max_sumCSV=0.0;
      float m1_BFJ = -1;
      float m2_BFJ = -1;
      float m3_BFJ = -1;
      float MJ_BFJ=0;
      for(int ifj=0; ifj<(int)FatjetPt_->size(); ifj++)
	{   
	  if(FatjetPt_->at(ifj)<FatjetpTthres) continue; 
	  if(mj_->at(ifj)<mjthres) continue;
	  int nBSJ=0;
	  float sumCSV=0.0;
	  float fjeta = FatjetEta_->at(ifj);
	  float fjphi = FatjetPhi_->at(ifj);
	  for(unsigned int isj=0;isj<JetPt_->size();isj++){
	    if(JetPt_->at(isj)<30) continue;
	    if(getDR(fjeta, JetEta_->at(isj), fjphi, JetPhi_->at(isj)) < 1.2)
	      {
		float CSV = JetCSV_->at(isj);
		if(CSV>0) {sumCSV+=CSV;
		  if(CSV>0.814) nBSJ++;
		}
	      }
	  }
	  if(sumCSV>max_sumCSV) max_sumCSV = sumCSV;
	  if(nBSJ>max_nBSJ) max_nBSJ = nBSJ;
	  if(nBSJ>0){
	    FillTH1FAll(h1_mj_BFJ, NFJbin, mj_->at(ifj), EventWeight_);
	    MJ_BFJ+= mj_->at(ifj);

	    if(mj_->at(ifj) > m1_BFJ){ m3_BFJ=m2_BFJ; m2_BFJ = m1_BFJ; m1_BFJ = mj_->at(ifj); }
	    else if(mj_->at(ifj) > m2_BFJ){m3_BFJ=m2_BFJ; m2_BFJ = mj_->at(ifj);}
	    else if(mj_->at(ifj) > m3_BFJ) m3_BFJ=mj_->at(ifj);
	  }

	  FillTH1FAll(h1_nBSJ_perFJ, NFJbin, nBSJ, EventWeight_);
	  FillTH1FAll(h1_sumCSV_perFJ, NFJbin, sumCSV, EventWeight_);

	  
	}
      FillTH1FAll(h1_mj1_BFJ, NFJbin, m1_BFJ, EventWeight_);
      FillTH1FAll(h1_mj2_BFJ, NFJbin, m2_BFJ, EventWeight_);
      FillTH1FAll(h1_mj3_BFJ, NFJbin, m3_BFJ, EventWeight_);
      FillTH1FAll(h1_MJ_BFJ, NFJbin, MJ_BFJ, EventWeight_);
      FillTH1FAll(h1_max_nBSJ_perEvent, NFJbin, max_nBSJ, EventWeight_);
      FillTH1FAll(h1_max_sumCSV_perEvent, NFJbin, max_sumCSV, EventWeight_);
	  
      

      if(NFJbin>1) FillTH2FAll(h2_mj1vsmj2, NFJbin, mj_thres_sorted.at(0), mj_thres_sorted.at(1), EventWeight_);           
      if(NFJbin>2) FillTH2FAll(h2_mj2vsmj3, NFJbin, mj_thres_sorted.at(1), mj_thres_sorted.at(2), EventWeight_);           
      if(NFJbin>3) FillTH2FAll(h2_mj3vsmj4, NFJbin, mj_thres_sorted.at(2), mj_thres_sorted.at(3), EventWeight_);           
      FillTH2FAll(h2_HTMET, NFJbin, HT40, MET_, EventWeight_);         
      FillTH2FAll(h2_MJmT, NFJbin, MJ_thres, mT, EventWeight_);       
      FillTH2FAll(h2_HTmT, NFJbin, HT40, mT, EventWeight_);            
      FillTH2FAll(h2_MJMET, NFJbin, MJ_thres, MET_, EventWeight_);    
      FillTH2FAll(h2_HTMJ, NFJbin, HT40, MJ_thres,EventWeight_);       
      FillTH2FAll(h2_METmT, NFJbin, MET_, mT, EventWeight_);           
      FillTH1FAll(h1_mT,  NFJbin, mT, EventWeight_);                 
      FillTH1FAll(h1_WpT, NFJbin, WpT, EventWeight_);               
      FillTH1FAll(h1_HT, NFJbin, HT40, EventWeight_);   
      FillTH1FAll(h1_MJ, NFJbin, MJ_thres, EventWeight_);
      FillTH1FAll(h1_MJ_coarse, NFJbin, MJ_thres, EventWeight_);

      if(ChainName.Contains("TT_sys")){
	if(MJ_thres<0) cout<<"MJ IS LESS THAN ZERO"<<endl;
	int jbin = h1_MJ_coarse[NFJbin]->FindBin(MJ_thres)-1 ;
	if(jbin<0 || jbin >= nbin) cout<<"jbin out of range."<<endl;
	h1_MJ_weights_by_bin[NFJbin][jbin]->Fill(EventWeight_/orig_weight, orig_weight);
	h1_MJ_weights_by_bin[6][jbin]->Fill(EventWeight_/orig_weight, orig_weight);
      }



      FillTH1FAll(h1_MET, NFJbin, MET_, EventWeight_);              
      FillTH1FAll(h1_METPhi, NFJbin, METPhi_, EventWeight_);        
      FillTH1FAll(h1_METx, NFJbin, MET_*TMath::Cos(METPhi_), EventWeight_);           
      FillTH1FAll(h1_METy, NFJbin, MET_*TMath::Sin(METPhi_), EventWeight_);           
      if(RA4MusPt_->size()==1) FillTH1FAll(h1_DPhi, NFJbin, getDPhi(RA4MusPhi_->at(0),METPhi_), EventWeight_);   
      if(RA4ElsPt_->size()==1) FillTH1FAll(h1_DPhi, NFJbin, getDPhi(RA4ElsPhi_->at(0),METPhi_), EventWeight_);   
        
      if(Nfatjet_thres>0) 
        {
	  FillTH1FAll(h1_FatjetPt1,   NFJbin, FatjetPt_->at(0),       EventWeight_);   
	  FillTH1FAll(h1_mj1,         NFJbin, mj_thres_sorted.at(0),  EventWeight_);    
	  FillTH1FAll(h1_FatjetPhi1,  NFJbin, FatjetPhi_->at(0),      EventWeight_); 
	  FillTH1FAll(h1_FatjetEta1,  NFJbin, FatjetEta_->at(0),      EventWeight_); 
        }
      if(Nfatjet_thres>1) 
        {
	  FillTH1FAll(h1_FatjetPt2,   NFJbin, FatjetPt_->at(1),       EventWeight_);   
	  FillTH1FAll(h1_mj2,         NFJbin, mj_thres_sorted.at(1),  EventWeight_);    
	  FillTH1FAll(h1_mj2overmj1,  NFJbin, mj_thres_sorted.at(1)/mj_thres_sorted.at(0), EventWeight_);   
	  FillTH1FAll(h1_FatjetPhi2,  NFJbin, FatjetPhi_->at(1),      EventWeight_);  
	  FillTH1FAll(h1_FatjetEta2,  NFJbin, FatjetEta_->at(1),      EventWeight_);  
        }
      if(Nfatjet_thres>2) 
        {
	  FillTH1FAll(h1_FatjetPt3,   NFJbin, FatjetPt_->at(2),       EventWeight_);   
	  FillTH1FAll(h1_mj3,         NFJbin, mj_thres_sorted.at(2),  EventWeight_);    
	  FillTH1FAll(h1_mj3overmj2,  NFJbin, mj_thres_sorted.at(2)/mj_thres_sorted.at(1), EventWeight_);   
	  FillTH1FAll(h1_FatjetPhi3,  NFJbin, FatjetPhi_->at(2),      EventWeight_); 
	  FillTH1FAll(h1_FatjetEta3,  NFJbin, FatjetEta_->at(2),      EventWeight_); 
        }
      if(Nfatjet_thres>3) 
        {
	  FillTH1FAll(h1_FatjetPt4,   NFJbin, FatjetPt_->at(3),       EventWeight_);   
	  FillTH1FAll(h1_mj4,         NFJbin, mj_thres_sorted.at(3),  EventWeight_);    
	  FillTH1FAll(h1_FatjetPhi4,  NFJbin, FatjetPhi_->at(3),      EventWeight_); 
	  FillTH1FAll(h1_FatjetEta4,  NFJbin, FatjetEta_->at(3),      EventWeight_); 
        }
      if(Nfatjet_thres>4) 
        {
	  // add 5th jet to h1_FatjetPt4
	  FillTH1FAll(h1_FatjetPt4,   NFJbin, FatjetPt_->at(4),       EventWeight_);   
	  FillTH1FAll(h1_mj4,         NFJbin, mj_thres_sorted.at(4),  EventWeight_);    
	  FillTH1FAll(h1_FatjetPhi4,  NFJbin, FatjetPhi_->at(4),      EventWeight_); 
	  FillTH1FAll(h1_FatjetEta4,  NFJbin, FatjetEta_->at(4),      EventWeight_); 
        }
      if(Nfatjet_thres>5) 
        {
	  // add 6th jet to h1_FatjetPt4
	  FillTH1FAll(h1_FatjetPt4,   NFJbin, FatjetPt_->at(5),       EventWeight_);   
	  FillTH1FAll(h1_mj4,         NFJbin, mj_thres_sorted.at(5),  EventWeight_);    
	  FillTH1FAll(h1_FatjetPhi4,  NFJbin, FatjetPhi_->at(5),      EventWeight_); 
	  FillTH1FAll(h1_FatjetEta4,  NFJbin, FatjetEta_->at(5),      EventWeight_); 
        }

      for(int imj=0; imj<(int)mj_->size(); imj++) 
        { 
	  if(FatjetPt_->at(imj)<FatjetpTthres) continue; 
	  if(mj_->at(imj)<mjthres) continue; 
            
	  FillTH1FAll(h1_mj,          NFJbin, mj_->at(imj),           EventWeight_);                            
	  FillTH1FAll(h1_FatjetPt,    NFJbin, FatjetPt_->at(imj),     EventWeight_);                
	  FillTH1FAll(h1_FatjetEta,   NFJbin, FatjetEta_->at(imj),    EventWeight_);              
        }

      FillTH1FAll(h1_Nfatjet,     NFJbin, Nfatjet_thres,  EventWeight_);       
      FillTH1FAll(h1_Nskinnyjet,  NFJbin, RA4NSJ,    EventWeight_);      
      FillTH1FAll(h1_Ncsvm,       NFJbin, RA4NB,     EventWeight_);            
            
    } // for(int i = 0; i<nentries; i++)
    
  if(ChainName.Contains("TT_ll")) 
    { 
      cout << "---------------------------------" << endl;
      cout << "Nll  : " << Nll << endl;
      cout << "Nlt  : " << Nlt << endl;
      cout << "Ntt  : " << Ntt << endl;
      cout << "Nlth : " << Nlth << endl;
      cout << "Nltl : " << Nltl << endl;
      cout << "---------------------------------" << endl;
    }
    
  float ptscale;
  if(N_post_toppT>0.1) ptscale= N_pre_toppT/N_post_toppT;
  else ptscale=0;
  cout<<"TOP PT REWEIGHT pre: "<<N_pre_toppT<<"   post: "<<N_post_toppT<<"    Ratio: "<<ptscale<<endl;

  //make tgraph to output SF/unc
  TGraphErrors* g1_SF_mc[7];
  if(ChainName.Contains("TT_sys")){
    for(int ifj=0; ifj<7; ifj++){ 
      Double_t x[nbin],ex[nbin],y[nbin],ey[nbin];
      for(int i=0;i<nbin;i++){
	x[i] = h1_MJ_coarse[ifj]->GetBinCenter(i+1);
	ex[i] = 0.5*h1_MJ_coarse[ifj]->GetBinWidth(i+1);	
	y[i] = h1_MJ_weights_by_bin[ifj][i]->GetMean();
	ey[i] = h1_MJ_weights_by_bin[ifj][i]->GetMeanError();
      }
      g1_SF_mc[ifj] = new TGraphErrors(nbin,x,y,ex,ey);
      g1_SF_mc[ifj]->SetName(Form("%s_mcSF_%i",ChainName.Data(),ifj));
    }
  }
    
  TString HistFileName = ChainName;
  HistFileName = Form("Out/%s_v%i/HistFiles/%s_%s_v%i.root", study.Data(),version,HistFileName.Data(), Region,version);
  cout << "[MJ Analysis] Writing " << HistFileName << endl;
  TFile *HistFile = new TFile(HistFileName, "RECREATE");
  gROOT->cd();
  HistFile->cd();
  // write histograms
  h1_toppT1_incl->SetDirectory(0);                      h1_toppT1_incl->Write();
  h1_toppT2_incl->SetDirectory(0);                      h1_toppT2_incl->Write();
  h1_toppT_incl->SetDirectory(0);                      h1_toppT_incl->Write();
  h1_nISR_incl->SetDirectory(0);                      h1_nISR_incl->Write();
  h1_ttbarpT_incl->SetDirectory(0);                      h1_ttbarpT_incl->Write();
  for(int i=0; i<7; i++)  
    {
      if(ChainName.Contains("TT_sys")){ g1_SF_mc[i]->Write();}
      if(ChainName.Contains("baseline")){
	for(int nMJ=0;nMJ<2;nMJ++){
	  for(int nMET=0;nMET<3;nMET++){
	    for(int nMT=0;nMT<2;nMT++){
	    
	      for(int nb=0;nb<3;nb++){
		for(int nsj=0;nsj<3;nsj++){
		  for(int nHT=0; nHT<2; nHT++){		  
		    h1_yields_binned[i][nMJ][nMET][nMT][nb][nsj][nHT]->SetDirectory(0);
		    h1_yields_binned[i][nMJ][nMET][nMT][nb][nsj][nHT]->Write();
		  }
		}
	      }
	    }
	  }//nFJ, MJ, MET, MT, NB
	}
      }

      h1_nBSJ_perFJ[i]->SetDirectory(0);
      h1_max_nBSJ_perEvent[i]->SetDirectory(0);
      h1_sumCSV_perFJ[i]->SetDirectory(0);
      h1_max_sumCSV_perEvent[i]->SetDirectory(0);
      h1_nBSJ_perFJ[i]->Write();
      h1_max_nBSJ_perEvent[i]->Write();
      h1_sumCSV_perFJ[i]->Write();
      h1_max_sumCSV_perEvent[i]->Write();
      h1_mj_BFJ[i]->SetDirectory(0);
      h1_MJ_BFJ[i]->SetDirectory(0);
      h1_mj1_BFJ[i]->SetDirectory(0);
      h1_mj2_BFJ[i]->SetDirectory(0);
      h1_mj3_BFJ[i]->SetDirectory(0);
      h1_mj_BFJ[i]->Write();
      h1_MJ_BFJ[i]->Write();
      h1_mj1_BFJ[i]->Write();
      h1_mj2_BFJ[i]->Write();
      h1_mj3_BFJ[i]->Write();
      h1_mindPhi_B_met[i]->SetDirectory(0);                 h1_mindPhi_B_met[i]->Write(); 
      h1_yields[i]->SetDirectory(0);                      h1_yields[i]->Write();
      h1_MJ[i]->SetDirectory(0);                          h1_MJ[i]->Write();
      h1_MJ_coarse[i]->SetDirectory(0);                   h1_MJ_coarse[i]->Write();
      h1_HT[i]->SetDirectory(0);                          h1_HT[i]->Write();
      h1_toppT[i]->SetDirectory(0);                       h1_toppT[i]->Write();
      h1_nISR[i]->SetDirectory(0);                        h1_nISR[i]->Write();
      h1_ttbarpT[i]->SetDirectory(0);                     h1_ttbarpT[i]->Write();
      h1_toppT1[i]->SetDirectory(0);                      h1_toppT1[i]->Write();
      h1_toppT2[i]->SetDirectory(0);                      h1_toppT2[i]->Write();
      h1_MET[i]->SetDirectory(0);                         h1_MET[i]->Write();
      h1_METPhi[i]->SetDirectory(0);                      h1_METPhi[i]->Write();
      h1_METx[i]->SetDirectory(0);                        h1_METx[i]->Write();
      h1_METy[i]->SetDirectory(0);                        h1_METy[i]->Write();
      h1_FatjetPt[i]->SetDirectory(0);                    h1_FatjetPt[i]->Write();
      h1_FatjetPt1[i]->SetDirectory(0);                   h1_FatjetPt1[i]->Write();
      h1_FatjetPt2[i]->SetDirectory(0);                   h1_FatjetPt2[i]->Write();
      h1_FatjetPt3[i]->SetDirectory(0);                   h1_FatjetPt3[i]->Write();
      h1_FatjetPt4[i]->SetDirectory(0);                   h1_FatjetPt4[i]->Write();
      h1_mj1[i]->SetDirectory(0);                         h1_mj1[i]->Write();
      h1_mj2[i]->SetDirectory(0);                         h1_mj2[i]->Write();
      h1_mj3[i]->SetDirectory(0);                         h1_mj3[i]->Write();
      h1_mj4[i]->SetDirectory(0);                         h1_mj4[i]->Write();
      h1_mj3overmj2[i]->SetDirectory(0);                   h1_mj3overmj2[i]->Write();
      h1_mj2overmj1[i]->SetDirectory(0);                   h1_mj2overmj1[i]->Write();
      h1_FatjetPhi1[i]->SetDirectory(0);                   h1_FatjetPhi1[i]->Write();
      h1_FatjetPhi2[i]->SetDirectory(0);                   h1_FatjetPhi2[i]->Write();
      h1_FatjetPhi3[i]->SetDirectory(0);                   h1_FatjetPhi3[i]->Write();
      h1_FatjetPhi4[i]->SetDirectory(0);                   h1_FatjetPhi4[i]->Write();
      h1_FatjetEta1[i]->SetDirectory(0);                   h1_FatjetEta1[i]->Write();
      h1_FatjetEta2[i]->SetDirectory(0);                   h1_FatjetEta2[i]->Write();
      h1_FatjetEta3[i]->SetDirectory(0);                   h1_FatjetEta3[i]->Write();
      h1_FatjetEta4[i]->SetDirectory(0);                   h1_FatjetEta4[i]->Write();
      h1_dRFJ[i]->SetDirectory(0);                        h1_dRFJ[i]->Write();
      h1_dPhiFJ[i]->SetDirectory(0);                      h1_dPhiFJ[i]->Write();
      h1_dEtaFJ[i]->SetDirectory(0);                      h1_dEtaFJ[i]->Write();
      h2_HTMET[i]->SetDirectory(0);                       h2_HTMET[i]->Write();
      h2_MJmT[i]->SetDirectory(0);                        h2_MJmT[i]->Write();
      h2_HTmT[i]->SetDirectory(0);                        h2_HTmT[i]->Write();
      h2_MJMET[i]->SetDirectory(0);                       h2_MJMET[i]->Write();
      h2_HTMJ[i]->SetDirectory(0);                        h2_HTMJ[i]->Write();
      h2_METmT[i]->SetDirectory(0);                       h2_METmT[i]->Write();
      h1_FatjetEta[i]->SetDirectory(0);                   h1_FatjetEta[i]->Write();
      h1_DPhi[i]->SetDirectory(0);                        h1_DPhi[i]->Write();
      h1_mj[i]->SetDirectory(0);                          h1_mj[i]->Write();
      h1_mT[i]->SetDirectory(0);                          h1_mT[i]->Write();
      h1_WpT[i]->SetDirectory(0);                         h1_WpT[i]->Write();
      h1_muspT[i]->SetDirectory(0);                       h1_muspT[i]->Write();
      h1_muspTminusMET[i]->SetDirectory(0);               h1_muspTminusMET[i]->Write();
      h1_musEta[i]->SetDirectory(0);                      h1_musEta[i]->Write();
      h1_musPhi[i]->SetDirectory(0);                      h1_musPhi[i]->Write();
      h1_elspT[i]->SetDirectory(0);                       h1_elspT[i]->Write();
      h1_elspTminusMET[i]->SetDirectory(0);               h1_elspTminusMET[i]->Write();
      h1_elsEta[i]->SetDirectory(0);                      h1_elsEta[i]->Write();
      h1_elsPhi[i]->SetDirectory(0);                      h1_elsPhi[i]->Write();
      h1_Nfatjet[i]->SetDirectory(0);                     h1_Nfatjet[i]->Write();
      h1_Nskinnyjet[i]->SetDirectory(0);                  h1_Nskinnyjet[i]->Write();
      h1_Ncsvm[i]->SetDirectory(0);                       h1_Ncsvm[i]->Write();
      h2_mj1vsmj2[i]->SetDirectory(0);                    h2_mj1vsmj2[i]->Write();
      h2_mj2vsmj3[i]->SetDirectory(0);                    h2_mj2vsmj3[i]->Write();
      h2_mj3vsmj4[i]->SetDirectory(0);                    h2_mj3vsmj4[i]->Write();
    }
  HistFile->Close();
  /*for(unsigned int b =0;b<eventlist.size(); b++)
    {
    cout<<"5 FJ TT_ll event: "<<eventlist.at(b)<<endl;
    }*/
}
