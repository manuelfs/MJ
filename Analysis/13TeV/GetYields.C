#include <iostream>
#include <fstream>
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

ofstream fout;

void GetYields(int version/*, char* Region=(char*)"baseline"*/){
  /* if(lepflav==0)  cout << "[MJ Table] Yields for Electron+Muon" << endl;
    if(lepflav==11) cout << "[MJ Table] Yields for Electron" << endl;
    if(lepflav==13) cout << "[MJ Table] Yields for Muon" << endl;
    if(lepflav==15) cout << "[MJ Table] Yields for Dilepton" << endl;
  */
  //int version =14;
    TString HistName="yields";
    char* Region = (char*)"baseline";
    TFile* HistFile = TFile::Open(Form("Out/v%i/HistFiles/Hist_%s_v%i.root", version,Region,version));
   
        
    TH1F *h1_DATA[7][2][2][2][3][3][2], *h1_T[7][2][2][2][3][3][2], *h1_TT_sl[7][2][2][2][3][3][2], *h1_TT_ll[7][2][2][2][3][3][2], *h1_WJets[7][2][2][2][3][3][2], *h1_DY[7][2][2][2][3][3][2], *h1_MC[7][2][2][2][3][3][2];
    TH1F *h1_f1500_100[7][2][2][2][3][3][2], *h1_f1200_800[7][2][2][2][3][3][2];
    for(int i=2; i<7; i++)
      {
	for(int nMJ=0;nMJ<2;nMJ++){
	  for(int nMET=0;nMET<2;nMET++){
	    for(int nMT=0;nMT<2;nMT++){	    
	      for(int nb=0;nb<3;nb++){
		for(int nsj=0;nsj<3;nsj++){
		  for(int nHT=0;nHT<2;nHT++){
		  //"h1_%s_yields_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", ChainName.Data(), i,nMJ,nMET,nMT,nb,nsj
		  h1_DATA[i][nMJ][nMET][nMT][nb][nsj][nHT]  = (TH1F*)HistFile->Get(Form("h1_DATA_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  // if(h1_DATA[i][nMJ][nMET][nMT][nb][nHT]) cout<<"got data nsj "<<nsj<<endl;
		  h1_T[i][nMJ][nMET][nMT][nb][nsj][nHT]  = (TH1F*)HistFile->Get(Form("h1_T_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  // if(h1_T[i][nMJ][nMET][nMT][nb][nHT]) cout<<"got T nsj "<<nsj<<endl;
		  h1_TT_sl[i][nMJ][nMET][nMT][nb][nsj][nHT] = (TH1F*)HistFile->Get(Form("h1_TT_sl_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  // if(h1_TT_sl[i][nMJ][nMET][nMT][nb][nsj][nHT]) cout<<"got TT sl nsj "<<nsj<<endl;
		  h1_TT_ll[i][nMJ][nMET][nMT][nb][nsj][nHT] = (TH1F*)HistFile->Get(Form("h1_TT_ll_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  // if(h1_TT_ll[i][nMJ][nMET][nMT][nb][nsj][nHT]) cout<<"got TT ll nsj "<<nsj<<endl;
		  h1_WJets[i][nMJ][nMET][nMT][nb][nsj][nHT] = (TH1F*)HistFile->Get(Form("h1_WJets_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  //if(h1_WJets[i][nMJ][nMET][nMT][nb][nsj][nHT]) cout<<"got WJets nsj "<<nsj<<endl;
		  h1_DY[i][nMJ][nMET][nMT][nb][nsj][nHT] = (TH1F*)HistFile->Get(Form("h1_DY_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  // if(h1_DY[i][nMJ][nMET][nMT][nb][nsj][nHT]) cout<<"got DY nsj "<<nsj<<endl;
		  h1_f1500_100[i][nMJ][nMET][nMT][nb][nsj][nHT] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1500_100_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  // if(h1_f1500_100[i][nMJ][nMET][nMT][nb][nsj][nHT]) cout<<"got f1500_100 nsj "<<nsj<<endl;
		  h1_f1200_800[i][nMJ][nMET][nMT][nb][nsj][nHT] = (TH1F*)HistFile->Get(Form("h1_T1tttt_f1200_800_%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));

		  
		  h1_MC[i][nMJ][nMET][nMT][nb][nsj][nHT] = (TH1F*)h1_TT_sl[i][nMJ][nMET][nMT][nb][nsj][nHT]->Clone(Form("h1_MC%s_%s_%ifatjet_%iMJ_%iMET_%iMT_%inb_%insj_%inHT", Region,HistName.Data(), i,nMJ,nMET,nMT,nb,nsj,nHT));
		  h1_MC[i][nMJ][nMET][nMT][nb][nsj][nHT]->Add(h1_TT_ll[i][nMJ][nMET][nMT][nb][nsj][nHT]);
		  h1_MC[i][nMJ][nMET][nMT][nb][nsj][nHT]->Add(h1_WJets[i][nMJ][nMET][nMT][nb][nsj][nHT]);
		  h1_MC[i][nMJ][nMET][nMT][nb][nsj][nHT]->Add(h1_T[i][nMJ][nMET][nMT][nb][nsj][nHT]);
		  h1_MC[i][nMJ][nMET][nMT][nb][nsj][nHT]->Add(h1_DY[i][nMJ][nMET][nMT][nb][nsj][nHT]);
		}
	      }
	    }
	  }
	  }
	}
      }
    TString name[] = {"4+ SJ","6+ SJ"};
    TString HTname[] = {"HT 500","HT 750"};
    int nsjbegin[] = {0,1};
    int nHTbegin[] = {0,1};
    for(int m=0;m<2;m++){
      for(int h=0;h<2;h++){
    TH1F* oneB[4],*twoB[4],*dilep[4];
    oneB[0]=(TH1F*) h1_MC[6][0][0][0][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB0");
    oneB[1]= (TH1F*)h1_MC[6][1][0][0][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB1");
    oneB[2]=(TH1F*) h1_MC[6][0][0][1][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB2");
    oneB[3]= (TH1F*) h1_MC[6][1][0][1][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB3");
    twoB[0]= (TH1F*)h1_MC[6][0][0][0][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB0");
    twoB[1]= (TH1F*)h1_MC[6][1][0][0][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB1");
    twoB[2]= (TH1F*)h1_MC[6][0][0][1][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB2");
    twoB[3]= (TH1F*)h1_MC[6][1][0][1][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB3");
    for(int nsj=nsjbegin[m];nsj<3;nsj++){
      for(int nMET=0;nMET<2;nMET++){
	for(int nHT=nHTbegin[h];nHT<2;nHT++){
	  if(!(nsj==nsjbegin[m]&&nMET==0&&nHT==nHTbegin[h])){
	    oneB[0]->Add( h1_MC[6][0][nMET][0][0][nsj][nHT]);
	    oneB[1]->Add( h1_MC[6][1][nMET][0][0][nsj][nHT]);
	    oneB[2]->Add( h1_MC[6][0][nMET][1][0][nsj][nHT]);
	    oneB[3]->Add( h1_MC[6][1][nMET][1][0][nsj][nHT]);
	  }
	  for(int nb=1;nb<3;nb++){
	    if(!(nsj==nsjbegin[m] && nMET==0 && nb==1 &&nHT==nHTbegin[h])){
	      twoB[0]->Add( h1_MC[6][0][nMET][0][nb][nsj][nHT]);
	      twoB[1]->Add( h1_MC[6][1][nMET][0][nb][nsj][nHT]);
	      twoB[2]->Add( h1_MC[6][0][nMET][1][nb][nsj][nHT]);
	      twoB[3]->Add( h1_MC[6][1][nMET][1][nb][nsj][nHT]);
	    }	
	  }
	}
      }
    }
    double error[2][4];
    for(int i=0;i<4;i++){
     oneB[i]->IntegralAndError(1,2,error[0][i]);
     twoB[i]->IntegralAndError(1,2,error[1][i]);
     /* cout<<Form("One B Square %i: %f +/- %f",1+i,oneB[i]->Integral(1,2),error[0][i])<<endl;
     cout<<Form("Two B Square %i: %f +/- %f",1+i,twoB[i]->Integral(1,2),error[1][i])<<endl;
     cout<<Form("Dilep Two B Square %i: %f +/- %f",1+i,twoB[i]->GetBinContent(3),twoB[i]->GetBinError(3))<<endl;
     cout<<Form("Dilep 1 B Square %i: %f +/- %f",1+i,oneB[i]->GetBinContent(3),oneB[i]->GetBinError(3))<<endl;*/
    }
    cout<<endl<<endl<<name[m]<<"  "<<HTname[h]<<endl;
    cout<<endl<<"NB = 1"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.1f    |    %.1f +/- %.1f  ",oneB[2]->Integral(1,2),error[0][2],oneB[3]->Integral(1,2),error[0][3])<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.1f   |    %.1f +/- %.1f  ",oneB[0]->Integral(1,2),error[0][0],oneB[1]->Integral(1,2),error[0][1])<<endl<<endl;

    cout<<"NB = 2+"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.1f    |    %.1f +/- %.1f  ",twoB[2]->Integral(1,2),error[1][2],twoB[3]->Integral(1,2),error[1][3])<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.1f   |    %.1f +/- %.1f  ",twoB[0]->Integral(1,2),error[1][0],twoB[1]->Integral(1,2),error[1][1])<<endl<<endl;

    cout<<"2-lep NB=2+"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.1f    |    %.1f +/- %.1f  ",twoB[2]->GetBinContent(3),twoB[2]->GetBinError(3),twoB[3]->GetBinContent(3),twoB[3]->GetBinError(3))<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.1f    |    %.1f +/- %.1f  ",twoB[0]->GetBinContent(3),twoB[0]->GetBinError(3),twoB[1]->GetBinContent(3),twoB[1]->GetBinError(3))<<endl<<endl;


    cout<<"2-lep NB=1"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.1f    |    %.1f +/- %.1f  ",oneB[2]->GetBinContent(3),oneB[2]->GetBinError(3),oneB[3]->GetBinContent(3),oneB[3]->GetBinError(3))<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.1f    |    %.1f +/- %.1f  ",oneB[0]->GetBinContent(3),oneB[0]->GetBinError(3),oneB[1]->GetBinContent(3),oneB[1]->GetBinError(3))<<endl<<endl;
    //cout<<Form("1B R1234 %f +/-%f   %f +/-%f   %f +/-%f  %f +/-%f  ",oneB[0]->Integral(1,2)



    TH1F* oneB_sig[4],*twoB_sig[4],*dilep_sig[4];
    oneB_sig[0]=(TH1F*) h1_f1500_100[6][0][0][0][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig0");
    oneB_sig[1]= (TH1F*)h1_f1500_100[6][1][0][0][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig1");
    oneB_sig[2]=(TH1F*) h1_f1500_100[6][0][0][1][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig2");
    oneB_sig[3]= (TH1F*) h1_f1500_100[6][1][0][1][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig3");
    twoB_sig[0]= (TH1F*)h1_f1500_100[6][0][0][0][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig0");
    twoB_sig[1]= (TH1F*)h1_f1500_100[6][1][0][0][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig1");
    twoB_sig[2]= (TH1F*)h1_f1500_100[6][0][0][1][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig2");
    twoB_sig[3]= (TH1F*)h1_f1500_100[6][1][0][1][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig3");
    for(int nsj=nsjbegin[m];nsj<3;nsj++){
      for(int nMET=0;nMET<2;nMET++){
	for(int nHT=nHTbegin[h];nHT<2;nHT++){
	if(!(nsj==nsjbegin[m]&&nMET==0&&nHT==nHTbegin[h])){
	  oneB_sig[0]->Add( h1_f1500_100[6][0][nMET][0][0][nsj][nHT]);
	  oneB_sig[1]->Add( h1_f1500_100[6][1][nMET][0][0][nsj][nHT]);
	  oneB_sig[2]->Add( h1_f1500_100[6][0][nMET][1][0][nsj][nHT]);
	  oneB_sig[3]->Add( h1_f1500_100[6][1][nMET][1][0][nsj][nHT]);
	}
	for(int nb=1;nb<3;nb++){
	  if(!(nsj==nsjbegin[m] && nMET==0 && nb==1 && nHT==nHTbegin[h])){
	    twoB_sig[0]->Add( h1_f1500_100[6][0][nMET][0][nb][nsj][nHT]);
	    twoB_sig[1]->Add( h1_f1500_100[6][1][nMET][0][nb][nsj][nHT]);
	    twoB_sig[2]->Add( h1_f1500_100[6][0][nMET][1][nb][nsj][nHT]);
	    twoB_sig[3]->Add( h1_f1500_100[6][1][nMET][1][nb][nsj][nHT]);
	  }	
	}
	}
      }
    }
    double error_sig[2][4];
    for(int i=0;i<4;i++){
     oneB_sig[i]->IntegralAndError(1,2,error_sig[0][i]);
     twoB_sig[i]->IntegralAndError(1,2,error_sig[1][i]);
     /* cout<<Form("One B_sig Square %i: %f +/- %f",1+i,oneB_sig[i]->Integral(1,2),error_sig[0][i])<<endl;
     cout<<Form("Two B_sig Square %i: %f +/- %f",1+i,twoB_sig[i]->Integral(1,2),error_sig[1][i])<<endl;
     cout<<Form("Dilep_Sig Two B_sig Square %i: %f +/- %f",1+i,twoB_sig[i]->GetB_siginContent(3),twoB_sig[i]->GetB_siginError_Sig(3))<<endl;
     cout<<Form("Dilep_Sig 1 B_sig Square %i: %f +/- %f",1+i,oneB_sig[i]->GetB_siginContent(3),oneB_sig[i]->GetB_siginError_Sig(3))<<endl;*/
    }
    cout<<"SIGNAL (1500,100)"<<endl;
    cout<<endl<<"NB = 1"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig[2]->Integral(1,2),error_sig[0][2],oneB_sig[3]->Integral(1,2),error_sig[0][3])<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig[0]->Integral(1,2),error_sig[0][0],oneB_sig[1]->Integral(1,2),error_sig[0][1])<<endl<<endl;

    cout<<"NB = 2+"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig[2]->Integral(1,2),error_sig[1][2],twoB_sig[3]->Integral(1,2),error_sig[1][3])<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig[0]->Integral(1,2),error_sig[1][0],twoB_sig[1]->Integral(1,2),error_sig[1][1])<<endl<<endl;

    cout<<"2-lep NB=2+"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig[2]->GetBinContent(3),twoB_sig[2]->GetBinError(3),twoB_sig[3]->GetBinContent(3),twoB_sig[3]->GetBinError(3))<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig[0]->GetBinContent(3),twoB_sig[0]->GetBinError(3),twoB_sig[1]->GetBinContent(3),twoB_sig[1]->GetBinError(3))<<endl<<endl;


    cout<<"2-lep NB=1"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig[2]->GetBinContent(3),oneB_sig[2]->GetBinError(3),oneB_sig[3]->GetBinContent(3),oneB_sig[3]->GetBinError(3))<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig[0]->GetBinContent(3),oneB_sig[0]->GetBinError(3),oneB_sig[1]->GetBinContent(3),oneB_sig[1]->GetBinError(3))<<endl<<endl;



    TH1F* oneB_sig_comp[4],*twoB_sig_comp[4],*dilep_sig_comp[4];
    oneB_sig_comp[0]=(TH1F*) h1_f1200_800[6][0][0][0][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig_comp0");
    oneB_sig_comp[1]= (TH1F*)h1_f1200_800[6][1][0][0][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig_comp1");
    oneB_sig_comp[2]=(TH1F*) h1_f1200_800[6][0][0][1][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig_comp2");
    oneB_sig_comp[3]= (TH1F*) h1_f1200_800[6][1][0][1][0][nsjbegin[m]][nHTbegin[h]]->Clone("oneB_sig_comp3");
    twoB_sig_comp[0]= (TH1F*)h1_f1200_800[6][0][0][0][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig_comp0");
    twoB_sig_comp[1]= (TH1F*)h1_f1200_800[6][1][0][0][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig_comp1");
    twoB_sig_comp[2]= (TH1F*)h1_f1200_800[6][0][0][1][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig_comp2");
    twoB_sig_comp[3]= (TH1F*)h1_f1200_800[6][1][0][1][1][nsjbegin[m]][nHTbegin[h]]->Clone("twoB_sig_comp3");
    for(int nsj=nsjbegin[m];nsj<3;nsj++){
      for(int nMET=0;nMET<2;nMET++){
	for(int nHT=nHTbegin[h];nHT<2;nHT++){
	if(!(nsj==nsjbegin[m]&&nMET==0&&nHT==nHTbegin[h])){
	  oneB_sig_comp[0]->Add( h1_f1200_800[6][0][nMET][0][0][nsj][nHT]);
	  oneB_sig_comp[1]->Add( h1_f1200_800[6][1][nMET][0][0][nsj][nHT]);
	  oneB_sig_comp[2]->Add( h1_f1200_800[6][0][nMET][1][0][nsj][nHT]);
	  oneB_sig_comp[3]->Add( h1_f1200_800[6][1][nMET][1][0][nsj][nHT]);
	}
	for(int nb=1;nb<3;nb++){
	  if(!(nsj==nsjbegin[m] && nMET==0 && nb==1 && nHT==nHTbegin[h])){
	    twoB_sig_comp[0]->Add( h1_f1200_800[6][0][nMET][0][nb][nsj][nHT]);
	    twoB_sig_comp[1]->Add( h1_f1200_800[6][1][nMET][0][nb][nsj][nHT]);
	    twoB_sig_comp[2]->Add( h1_f1200_800[6][0][nMET][1][nb][nsj][nHT]);
	    twoB_sig_comp[3]->Add( h1_f1200_800[6][1][nMET][1][nb][nsj][nHT]);
	  }	
	}
	}
      }
    }
    double error_sig_comp[2][4];
    for(int i=0;i<4;i++){
     oneB_sig_comp[i]->IntegralAndError(1,2,error_sig_comp[0][i]);
     twoB_sig_comp[i]->IntegralAndError(1,2,error_sig_comp[1][i]);
     /* cout<<Form("One B_sig_comp Square %i: %f +/- %f",1+i,oneB_sig_comp[i]->Integral(1,2),error_sig_comp[0][i])<<endl;
     cout<<Form("Two B_sig_comp Square %i: %f +/- %f",1+i,twoB_sig_comp[i]->Integral(1,2),error_sig_comp[1][i])<<endl;
     cout<<Form("Dilep_sig_comp Two B_sig_comp Square %i: %f +/- %f",1+i,twoB_sig_comp[i]->GetB_sig_compinContent(3),twoB_sig_comp[i]->GetB_sig_compinError_sig_comp(3))<<endl;
     cout<<Form("Dilep_sig_comp 1 B_sig_comp Square %i: %f +/- %f",1+i,oneB_sig_comp[i]->GetB_sig_compinContent(3),oneB_sig_comp[i]->GetB_sig_compinError_sig_comp(3))<<endl;*/
    }
    cout<<"SIGNAL (1200,800)"<<endl;
    cout<<endl<<"NB = 1"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig_comp[2]->Integral(1,2),error_sig_comp[0][2],oneB_sig_comp[3]->Integral(1,2),error_sig_comp[0][3])<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig_comp[0]->Integral(1,2),error_sig_comp[0][0],oneB_sig_comp[1]->Integral(1,2),error_sig_comp[0][1])<<endl<<endl;

    cout<<"NB = 2+"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig_comp[2]->Integral(1,2),error_sig_comp[1][2],twoB_sig_comp[3]->Integral(1,2),error_sig_comp[1][3])<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig_comp[0]->Integral(1,2),error_sig_comp[1][0],twoB_sig_comp[1]->Integral(1,2),error_sig_comp[1][1])<<endl<<endl;

    cout<<"2-lep NB=2+"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig_comp[2]->GetBinContent(3),twoB_sig_comp[2]->GetBinError(3),twoB_sig_comp[3]->GetBinContent(3),twoB_sig_comp[3]->GetBinError(3))<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",twoB_sig_comp[0]->GetBinContent(3),twoB_sig_comp[0]->GetBinError(3),twoB_sig_comp[1]->GetBinContent(3),twoB_sig_comp[1]->GetBinError(3))<<endl<<endl;


    cout<<"2-lep NB=1"<<endl;
    cout<<Form("Region 3 4:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig_comp[2]->GetBinContent(3),oneB_sig_comp[2]->GetBinError(3),oneB_sig_comp[3]->GetBinContent(3),oneB_sig_comp[3]->GetBinError(3))<<endl;
    cout<<Form("Region 1 2:     %.1f +/- %.2f    |    %.1f +/- %.2f  ",oneB_sig_comp[0]->GetBinContent(3),oneB_sig_comp[0]->GetBinError(3),oneB_sig_comp[1]->GetBinContent(3),oneB_sig_comp[1]->GetBinError(3))<<endl<<endl;





      }
    }
}
