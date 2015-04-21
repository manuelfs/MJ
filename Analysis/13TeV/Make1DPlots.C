#include <iostream>
#include <iomanip> // for setw()

#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "TH1F.h"
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
#include "TGraphErrors.h"
#include "TRandom3.h"
#include "Configuration.h"

using namespace std;
//bool DoLog          = 1;
bool doData         = 1;
bool SignalScale    = 1;

//
// h1 cosmetics
//
void h1cosmetic(TH1F* &h1, char* title, int linecolor=kBlack, int linewidth=1, int fillcolor=0, TString var="")
{
    h1->SetLineColor(linecolor);
    h1->SetLineWidth(linewidth);
    h1->SetFillColor(fillcolor);
    h1->SetTitle(title);
    h1->SetXTitle(var);
    h1->SetStats(0);
    h1->SetMinimum(0.1);
}

//
// h2 cosmetics
//
void h2cosmetic(TH2F* &h2, char* title, TString Xvar="", TString Yvar="", TString Zvar="")
{
    h2->SetTitle(title);
    h2->SetXTitle(Xvar);
    h2->SetYTitle(Yvar);
    h2->SetZTitle(Yvar);
    h2->SetStats(0);
}


//
// Stacks
//
void Make1DPlots(TString HistName, char* Region,  bool DoLog=1,bool fluctuate=false,char* sys=(char*)"",int NMergeBins=1) 
{ 
  //gInterpreter->ExecuteMacro("~/macros/JaeStyle.C"); 
   
    char *var;
    if(HistName=="mindPhi_B_met")            	var=(char*)"min #Delta#phi(MET,closest B)";
    if(HistName=="MET")                 	var=(char*)"MET [GeV]";
    if(HistName=="nBSJ_perFJ")                  var=(char*)"N B-tags per FJ";
    if(HistName=="max_nBSJ_perEvent")           var=(char*)"Max N B-tags per FJ";
    if(HistName=="sumCSV_perFJ")                  var=(char*)"sum CSV per FJ";
    if(HistName=="max_sumCSV_perEvent")           var=(char*)"Max sum CSV per FJ";
    if(HistName=="mj_BFJ")                       var=(char*)"mJ, B-tagged FJ";
    if(HistName=="mj1_BFJ")                       var=(char*)"mJ, most massive B-tagged FJ";
    if(HistName=="sumMJ_BFJ")                     var=(char*)"MJ, B-tagged FJs";
    if(HistName=="max_nBSJ_perEvent")           var=(char*)"Max N B-tags per FJ";
    if(HistName=="METPhi")              	var=(char*)"#phi(MET)";
    if(HistName=="METx")              	    var=(char*)"METx [GeV]";
    if(HistName=="METy")              	    var=(char*)"METy [GeV]";
    if(HistName=="FatjetPt")            	var=(char*)"pT(Fatjet) [GeV]";                   
    if(HistName=="FatjetPt1")            	var=(char*)"pT(Fatjet1) [GeV]";                   
    if(HistName=="FatjetPt2")            	var=(char*)"pT(Fatjet2) [GeV]";                   
    if(HistName=="FatjetPt3")            	var=(char*)"pT(Fatjet3) [GeV]";                   
    if(HistName=="FatjetPt4")            	var=(char*)"pT(Fatjet4) [GeV]";                   
    if(HistName=="mj1")            	        var=(char*)"mj1 [GeV]";                   
    if(HistName=="mj2")            	        var=(char*)"mj2 [GeV]";                   
    if(HistName=="mj3")            	        var=(char*)"mj3 [GeV]";                   
    if(HistName=="mj4")            	        var=(char*)"mj4 [GeV]";                   
    if(HistName=="mj3overmj2")     	        var=(char*)"mj3/mj2 [GeV]";                   
    if(HistName=="mj2overmj1")     	        var=(char*)"mj2/mj1 [GeV]";                   
    if(HistName=="FatjetPhi1")            	var=(char*)"#phi(Fatjet1)";                   
    if(HistName=="FatjetPhi2")            	var=(char*)"#phi(Fatjet2)";                   
    if(HistName=="FatjetPhi3")            	var=(char*)"#phi(Fatjet3)";                   
    if(HistName=="FatjetPhi4")            	var=(char*)"#phi(Fatjet4)";                   
    if(HistName=="FatjetEta1")            	var=(char*)"#eta(Fatjet1)";                   
    if(HistName=="FatjetEta2")            	var=(char*)"#eta(Fatjet2)";                   
    if(HistName=="FatjetEta3")            	var=(char*)"#eta(Fatjet3)";                   
    if(HistName=="FatjetEta4")            	var=(char*)"#eta(Fatjet4)";                   
    if(HistName=="FatjetEta")            	var=(char*)"#eta(Fatjet)";                   
    if(HistName=="DPhi")                	var=(char*)"#Delta(MET, muon)";                  
    if(HistName=="dRFJ")                	var=(char*)"min #Delta R(FJ,FJ)";                  
    if(HistName=="dPhiFJ")                	var=(char*)"min #Delta #phi(FJ,FJ)";                  
    if(HistName=="dEtaFJ")                	var=(char*)"min #Delta #eta(FJ,FJ)";                  
    if(HistName=="HT")                  	var=(char*)"H_{T} [GeV]";    
    if(HistName=="toppT1")                  	var=(char*)"top 1 p_{T} [GeV]"; 
    if(HistName=="toppT2")                  	var=(char*)"top 2 p_{T} [GeV]"; 
    if(HistName=="toppT")                  	var=(char*)"top p_{T} [GeV]";
    if(HistName=="nISR")                  	var=(char*)"N ISR partons";
    if(HistName=="nISR_incl")                  	var=(char*)"N ISR partons"; 
    if(HistName=="toppT_incl")                	var=(char*)"top p_{T} [GeV]";
    if(HistName=="ttbarpT")                  	var=(char*)"ttbar system p_{T} [GeV]"; 
    if(HistName=="ttbarpT_incl")              	var=(char*)"ttbar system p_{T} [GeV]"; 
    if(HistName=="MJ")                  	var=(char*)"M_{J} [GeV]";
    if(HistName=="MJ_coarse")                  	var=(char*)"M_{J} [GeV]";
    if(HistName=="MJ_corr1")                  	var=(char*)"M_{J} [GeV]";  
    if(HistName=="MJ_corr2")                  	var=(char*)"M_{J} [GeV]";
    if(HistName=="MJ_corr1_coarse")            	var=(char*)"M_{J} [GeV]";  
    if(HistName=="MJ_corr2_coarse")            	var=(char*)"M_{J} [GeV]";  
    if(HistName=="mj")                  	var=(char*)"m_{j} [GeV]";                        
    if(HistName=="mT")                  	var=(char*)"m_{T} [GeV]";                        
    if(HistName=="muspT")               	var=(char*)"p_{T}(muon) [GeV]";                  
    if(HistName=="muspTminusMET")          	var=(char*)"(MET-p_{T}(muon))/p_{T}(muon)";                  
    if(HistName=="musEta")              	var=(char*)"#eta(muon)";                         
    if(HistName=="musPhi")              	var=(char*)"#phi(muon)";                         
    if(HistName=="elspT")               	var=(char*)"p_{T}(muon) [GeV]";                  
    if(HistName=="elspTminusMET")          	var=(char*)"(MET-p_{T}(muon))/p_{T}(muon)";                  
    if(HistName=="elsEta")              	var=(char*)"#eta(muon)";                         
    if(HistName=="elsPhi")              	var=(char*)"#phi(muon)";                         

    if(HistName=="Nfatjet")             	var=(char*)"N_{fatjet}";
    if(HistName=="Nskinnyjet")          	var=(char*)"N_{skinny}";
    if(HistName=="Ncsvm")          	        var=(char*)"N_{CSVM}";
    if(HistName=="WpT")                 	var=(char*)"p_{T}(W) [GeV]";

    TH1F *h1_DATA[7], *h1_T[7], *h1_TT_sl[7], *h1_TT_ll[7], *h1_TT_sys[7], *h1_TT[7], *h1_WJets[7], *h1_DY[7], *h1_MC[7]; 
    TH1F *h1_f1500_100[7], *h1_f1200_800[7];
    THStack *st[7];
    // bool corr1=false;
    // bool corr2=false;
    bool corr=false;
    bool coarse = false;
    TString c_region[2] = {"1BCRincl","1B4SJCRincl"};
    /*char* Regions[] = {"1BCRincl","1B4SJCRincl","SRincl"};
      int nregion = 3;
    char* Regions[] = {"1BCRincl","1B4SJCRincl","1B45SJ","1B67SJ","1B8SJ","SRincl"};
    int nregion =6;*/
    // char* Regions[] = {"SR0","SR1","baseline","1BCRincl","1B4SJCRincl","1B45SJ","1B67SJ","1B8SJ","SRincl"};
    // int nregion =2;
    //  char* Regions[] = {/*"SRstats1","SR0","SR1","baseline",*/"1BCRincl",/*"1B4SJCRincl","1B45SJ","1B67SJ","1B8SJ",*/"SRincl"};
    // int nregion =2;
 
    TString corr_region = "";
    if(HistName.Contains("coarse")){
      coarse=true;
      }

    for(int i=0;i<nregion;i++){
      if(HistName.Contains(Form("corr%i",i)))
	{ corr=true;
	  corr_region = Regions[i];
	  HistName = "MJ";
	}
    }
    /* if(HistName.Contains("corr1")){
      corr1=true;
      HistName="MJ";
    }
    if(HistName.Contains("corr2")){
      corr2=true;
      HistName="MJ";
      }*/
    if(corr && coarse){
	HistName+="_coarse";
    }
    TString syst = Form("%s",sys);
    if(syst=="") doData=false;
   
    
    TGraphErrors *MJ_SF[7];
    TGraphErrors *MJ_SF_non_pois[7];
    if(corr){
      TFile* SFFile;
      SFFile = TFile::Open(Form("Out/%s_v%i/HistFiles/%s%s_SF_%s_v%i.root",study.Data(),version,HistName.Data(),sys ,corr_region.Data(),version));
      // else SFFile = TFile::Open(Form("HistFiles/v%i/%s_SF_%s_v%i.root",version,HistName.Data() ,c_region[1].Data(),version));  
      for(int j=2;j<7;j++){
	MJ_SF[j] = (TGraphErrors*)SFFile->Get(Form("SF_%i",j));
	//MJ_SF_non_pois[j] = (TGraphErrors*)SFFile->Get(Form("SF_non_pois_%i",j));
      }
      SFFile->Close();
    }

    TFile* HistFile = TFile::Open(Form("Out/%s_v%i/HistFiles/Hist_%s_v%i.root",study.Data(),version,Region,version));
    //TCanvas *c = new TCanvas("c","c",1500,300);  
    //c->Divide(5,1);
    TCanvas *c = new TCanvas("c","c",1200,800);  
    c->Divide(3,2);
    for(int i=2; i<7; i++) 
    {

      if(HistName.Contains("incl")){
	if(i>3) break;
	h1_T[i]         = (TH1F*)HistFile->Get(Form("h1_T_%s_%s", Region,HistName.Data()));

        h1_TT_sl[i]     = (TH1F*)HistFile->Get(Form("h1_TT_sl_%s_%s", Region,HistName.Data()));

        h1_TT_ll[i]     = (TH1F*)HistFile->Get(Form("h1_TT_ll_%s_%s", Region,HistName.Data()));

	h1_TT_sys[i]     = (TH1F*)HistFile->Get(Form("h1_TT_sys_%s%s_%s", Region,sys,HistName.Data()));

        h1_WJets[i]     = (TH1F*)HistFile->Get(Form("h1_WJets_%s_%s", Region,HistName.Data()));

        h1_DY[i]        = (TH1F*)HistFile->Get(Form("h1_DY_%s_%s", Region,HistName.Data()));

        h1_f1500_100[i] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1500_100_%s_%s", Region,HistName.Data()));

        h1_f1200_800[i] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1200_800_%s_%s", Region,HistName.Data())); 

      }
      else{
      // h1_DATA[i]      = (TH1F*)HistFile->Get(Form("h1_DATA_%s_%ifatjet", HistName.Data(), i)); 
        h1_T[i]         = (TH1F*)HistFile->Get(Form("h1_T_%s_%s_%ifatjet", Region,HistName.Data(), i));
        h1_TT_sl[i]     = (TH1F*)HistFile->Get(Form("h1_TT_sl_%s_%s_%ifatjet",Region, HistName.Data(), i));
        h1_TT_ll[i]     = (TH1F*)HistFile->Get(Form("h1_TT_ll_%s_%s_%ifatjet", Region,HistName.Data(), i));
	h1_TT_sys[i]     = (TH1F*)HistFile->Get(Form("h1_TT_sys_%s%s_%s_%ifatjet", Region,sys,HistName.Data(), i));
        h1_WJets[i]     = (TH1F*)HistFile->Get(Form("h1_WJets_%s_%s_%ifatjet", Region,HistName.Data(), i));
        h1_DY[i]        = (TH1F*)HistFile->Get(Form("h1_DY_%s_%s_%ifatjet", Region,HistName.Data(), i)); 
        h1_f1500_100[i] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1500_100_%s_%s_%ifatjet", Region,HistName.Data(), i)); 
        h1_f1200_800[i] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1200_800_%s_%s_%ifatjet", Region,HistName.Data(), i)); 
      }


      
	if(!h1_T[i]) cout<<"ERROR 1DPlots Histfile Get T"<<endl;    
	if(!h1_TT_sl[i]) cout<<"ERROR 1DPlots Histfile Get TT sl"<<endl;  
	if(!h1_TT_ll[i]) cout<<"ERROR 1DPlots Histfile Get TT ll"<<endl;
	if(!h1_TT_sys[i]) cout<<"ERROR 1DPlots Histfile Get TT sys "<<endl;
    	if(!h1_WJets[i]) cout<<"ERROR 1DPlots Histfile Get Wjets"<<endl;     
	if(!h1_DY[i]) cout<<"ERROR 1DPlots Histfile Get DY"<<endl;
	if(!h1_f1500_100[i]) cout<<"ERROR 1DPlots Histfile Get noncomp"<<endl;
	if(!h1_f1200_800[i]) cout<<"ERROR 1DPlots Histfile Get compr"<<endl;
	
        // merge bins
        //h1_DATA[i]->Rebin(NMergeBins);
        h1_T[i]->Rebin(NMergeBins);
        h1_TT_sl[i]->Rebin(NMergeBins);
        h1_TT_ll[i]->Rebin(NMergeBins);
	h1_TT_sys[i]->Rebin(NMergeBins);
        h1_WJets[i]->Rebin(NMergeBins);
        h1_DY[i]->Rebin(NMergeBins);
        h1_f1500_100[i]->Rebin(NMergeBins);
        h1_f1200_800[i]->Rebin(NMergeBins);
        
	//put sys-varied into data path
	h1_DATA[i]=(TH1F*)h1_TT_sys[i]->Clone(Form("h1_sys_data_%s_%ifatjet", HistName.Data(), i));
	h1_DATA[i]->Add(h1_WJets[i]);
        h1_DATA[i]->Add(h1_T[i]);
        h1_DATA[i]->Add(h1_DY[i]);
	
	//restore poisson errors on data
	TRandom3 rand;
	rand.SetSeed(0);
	for(int b=0;b<((int)h1_DATA[i]->GetXaxis()->GetNbins()+1);b++){
	  if(fluctuate) h1_DATA[i]->SetBinContent(b,rand.Poisson(h1_DATA[i]->GetBinContent(b)));
	  h1_DATA[i]->SetBinError(b,pow(h1_DATA[i]->GetBinContent(b),0.5));

	}
	if(corr){
	  Int_t nSF = MJ_SF[i]->GetN();
	  for(int p=0;p<nSF;p++){
	    Double_t xx=-1; Double_t yy = -1; //Double_t ey=0;
	    MJ_SF[i]->GetPoint(p,xx,yy);
	    // cout<<"p  x  y:   "<<p<<" "<<xx<<" "<<yy<<" "<<endl;
	    if(yy>0.4 && xx>0.4){
	      // cout<<"Found bin  ,  content     new content  "<<h1_TT_sl[i]->FindBin(xx)<<"  "<<h1_TT_sl[i]->GetBinContent(xx)<<"  "<<yy*1_TT_sl[i]->GetBinContent(xx)<<endl;
	      int binx = h1_TT_sl[i]->FindBin(xx);
	      h1_TT_sl[i]->SetBinContent( binx,yy*(float)h1_TT_sl[i]->GetBinContent(binx));
	      //   h1_TT_sl[i]->SetBinError( binx, pow(pow(ey/yy,2)+pow(h1_TT_sl[i]->GetBinError(binx)/h1_TT_sl[i]->GetBinContent(binx),2),0.5));
	      h1_TT_ll[i]->SetBinContent( binx,yy*(float)h1_TT_ll[i]->GetBinContent(binx));
	      //h1_TT_ll[i]->SetBinError( binx, pow(pow(ey/yy,2)+pow(h1_TT_ll[i]->GetBinError(binx)/h1_TT_ll[i]->GetBinContent(binx),2),0.5));
	    }
	  }

	}
	
        h1_MC[i] = (TH1F*)h1_TT_sl[i]->Clone(Form("h1_MC_%s_%ifatjet", HistName.Data(), i));
        h1_MC[i]->Add(h1_TT_ll[i]);
        h1_MC[i]->Add(h1_WJets[i]);
        h1_MC[i]->Add(h1_T[i]);
        h1_MC[i]->Add(h1_DY[i]);

        h1_TT[i] = (TH1F*)h1_TT_ll[i]->Clone();
        h1_TT[i]->Add(h1_TT_sl[i]);

        h1cosmetic(h1_DATA[i],          Form("DATA %ifatjet", i),               kBlack, 1, 0,           var);
        h1cosmetic(h1_TT[i],            Form("TT %ifatjet", i),                 kBlack, 1, kYellow,     var);
        h1cosmetic(h1_TT_sl[i],         Form("TT(l) %ifatjet", i),              kBlack, 1, kYellow,     var);
        //h1cosmetic(h1_TT_ll[i],         Form("TT(ll) %ifatjet", i),             kBlack, 1, kYellow-6,   var);
        h1cosmetic(h1_TT_ll[i],         Form("TT(ll) %ifatjet", i),             kBlack, 1, kOrange-3,   var);
	h1cosmetic(h1_TT_sys[i],         Form("TT(sys) %ifatjet", i),             kBlack, 1, 0,   var);
        h1cosmetic(h1_T[i],             Form("t+tW %ifatjet", i),               kBlack, 1, kGreen+2,    var);
        h1cosmetic(h1_WJets[i],         Form("WJets %ifatjet", i),              kBlack, 1, kGray+1,     var);
        h1cosmetic(h1_DY[i],            Form("DYJets %ifatjet", i),             kBlack, 1, kAzure-9,    var);
        h1cosmetic(h1_f1500_100[i],     Form("T1tttt(1500,100) %ifatjet", i),   kRed,   1, 0,           var);
        h1cosmetic(h1_f1200_800[i],     Form("T1tttt(1200,800) %ifatjet", i),   kBlue,  1, 0,           var);

        bool DoLogOne = (DoLog && h1_MC[i]->Integral()>0);
	if(HistName.Contains("incl") && i==3) DoLogOne=false; 
        c->cd(i-1);
        if(DoLogOne) c->cd(i-1)->SetLogy(1);
        //c->cd(i-1)->SetLeftMargin(0.15);
        //c->cd(i-1)->SetRightMargin(0.07);
        //c->cd(i-1)->SetBottomMargin(0.15);
        //c->cd(i-1)->SetTopMargin(0.1);
        TString StackTitle;
	if( !HistName.Contains("incl")){ StackTitle = Form("%i fatjets", i);
        if(i==6) StackTitle = "All fatjets";
        if(i==5) StackTitle = "5+ fatjets";}
	else StackTitle = "";
        st[i] = new THStack( Form("Stack %ifatjet", i), StackTitle);
        st[i]->Add(h1_DY[i]);
        st[i]->Add(h1_WJets[i]);
        st[i]->Add(h1_T[i]);
//         st[i]->Add(h1_TT[i]);
         st[i]->Add(h1_TT_ll[i]);
         st[i]->Add(h1_TT_sl[i]);
	 st[i]->SetMaximum(max(h1_MC[i]->GetMaximum(), h1_f1500_100[i]->GetMaximum())*(DoLogOne?200:1.6));
        //st[i]->SetMinimum(h1_MC[i]->GetMinimum()*(DoLogOne?1:0));
        if(!HistName.Contains("incl")) st[i]->SetMinimum((DoLogOne?0.05:0));
	else st[i]->SetMinimum((DoLogOne?1:0));
        st[i]->Draw("HIST"); 

        ///* needs to be in JaeStyle : FIXME  
        //st[i]->GetYaxis()->SetLabelSize(0.07); 
        //st[i]->GetYaxis()->SetTitleOffset(1.1); 
        //st[i]->GetYaxis()->SetTitleSize(0.07); 
        st[i]->GetYaxis()->SetTitle("Events/bin"); 
        //st[i]->GetXaxis()->SetLabelSize(0.07); 
        st[i]->GetXaxis()->SetTitle(var); 
        //st[i]->GetXaxis()->SetTitleOffset(1.1); 
        //st[i]->GetXaxis()->SetTitleSize(0.07); 
        //*/
        h1_DATA[i]->SetLineColor(kBlack);
        h1_DATA[i]->SetMarkerColor(kBlack);
        h1_DATA[i]->SetMarkerSize(1);
        h1_DATA[i]->SetMarkerStyle(20);
        h1_DATA[i]->SetStats(0);
        if(doData) h1_DATA[i]->Draw("E SAME");

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
        if(doData) l1->AddEntry(h1_DATA[i],        "Toy Data",  "lp");
//        l1->AddEntry(h1_TT[i],          " TT",    "f");
//        l1->AddEntry(h1_TT_sl[i],        " ",    "");
        l1->AddEntry(h1_TT_sl[i],        " TT(l)",    "f");
        l1->AddEntry(h1_TT_ll[i],        " TT(ll)",    "f");
        l1->AddEntry(h1_T[i],           " t+tW",  "f");
        l1->AddEntry(h1_WJets[i],       " WJets", "f");
        l1->AddEntry(h1_DY[i],          " DY",    "f");
        //l1->AddEntry(h1_f1500_100[i],    Form("T1tttt[1500,100]x%i",SignalScale),     "l");
        //l1->AddEntry(h1_f1200_800[i],  Form("T1tttt[1200,800]x%i",SignalScale),   "l");
        l1->AddEntry(h1_f1500_100[i],   " T1tttt[1500,100]", "l");
        l1->AddEntry(h1_f1200_800[i],   " T1tttt[1200,800]", "l");
        l1->Draw();
        
        h1_f1500_100[i]->Scale(SignalScale);
        h1_f1200_800[i]->Scale(SignalScale);
        h1_f1500_100[i]->Draw("SAME HIST");
        h1_f1200_800[i]->Draw("SAME HIST");

        // CMS Labels 
        float textSize = 0.04;

        TLatex *TexEnergyLumi = new TLatex(0.9,0.92,Form("#sqrt{s}=13 TeV, L = %i fb^{-1}", 4));
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
	if(!doData) TexExt = new TLatex(0.85,0.7,LabelExt);
	else TexExt = new TLatex(0.85,0.65,LabelExt);
        TexExt->SetTextAlign (31);
        TexExt->SetNDC();
        TexExt->SetTextSize(textSize);
        TexExt->SetLineWidth(2);
        
        TexEnergyLumi->Draw("SAME");
        TexCMS->Draw("SAME");
        if(i!=6 && !HistName.Contains("incl")) TexExt->Draw("SAME");
    }

    // 
    if(HistName=="mj") HistName="JetMass";
    if(fluctuate) HistName = HistName+"_fluc";
    //if(corr) c->Print( Form("Figures/v%i/corr_%s_sys_CompareDataMC_%s_%s%s_v%i.pdf",version,corr_region.Data(),HistName.Data(), Region, DoLog?"_log":"",version) );
    if(corr) c->Print( Form("Out/%s_v%i/Figures/%s_%s_in_%s_corrected_by_%s%s_v%i.pdf",study.Data(),version,sys,HistName.Data(),Region, corr_region.Data(), DoLog?"_log":"",version) );
    //else if(corr2)c->Print( Form("Figures/v%i/corr_%s_sys_CompareDataMC_%s_%s%s_v%i.pdf",version, c_region[1].Data(),HistName.Data(), Region, DoLog?"_log":"",version) );
    else c->Print( Form("Out/%s_v%i/Figures/%s_%s_in_%s%s_v%i.pdf",study.Data(),version,sys,HistName.Data(), Region, DoLog?"_log":"",version) ); 
    
    // 
    HistFile->Close();
    delete c;
    cout<<"finished 1D"<<endl;

}
