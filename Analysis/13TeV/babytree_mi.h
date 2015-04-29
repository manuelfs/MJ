#include <vector>
#include <string>
#include "TChain.h"

// bool status = true;
    int             event_;
    float           EventWeight_;
    float           EventWeightNeg_;
    float           Npu_;
    int             Npv_;
    int             Nfatjet_;
    int             Nskinnyjet_;
    int             NBtagCSVM_;
    float           MJ_;
    float           MET_;
    float           HT_;
    float           METPhi_;
    vector<float>   *mj_;
    vector<float>   *FatjetPt_;
    vector<float>   *FatjetEta_;
    vector<float>   *FatjetPhi_;
    vector<float>   *RA4MusPt_;
    vector<float>   *RA4MusPhi_;
    vector<float>   *RA4MusEta_;
    vector<float>   *RA4ElsPt_;
    vector<float>   *RA4ElsPhi_;
    vector<float>   *RA4ElsEta_;
    vector<float>   *JetPt_;
    vector<float>   *JetEta_;
    vector<float>   *JetPhi_;
    vector<float>   *JetCSV_;
    vector<float>   *RA4MusVetoPt_;
    vector<float>   *RA4ElsVetoPt_;
    vector<float>   *GenId_;
    vector<float>   *GenStatus_;
    vector<float>   *GenMId_;
    vector<float>   *GenGMId_;
vector<float>   *GenPt_;
vector<float>   *GenEta_;
vector<float>   *GenPhi_;
//vector<float>   *GenE_;

    bool        TrigMuon_;
    bool        TrigSingleMuon_;
    float        top1pT_;
    float        top1Phi_;
    float        top2pT_;
    float        top2Phi_;
    
    TBranch         *b_event;   //!
    TBranch         *b_EventWeight;   //!
    TBranch         *b_EventWeightNeg;   //!
    TBranch         *b_Npu;   //!
    TBranch         *b_Npv;   //!
    TBranch         *b_Nfatjet;   //!
    TBranch         *b_Nskinnyjet;   //!
    TBranch         *b_NBtagCSVM;   //!
    TBranch         *b_MJ;   //!
    TBranch         *b_MET;   //!
    TBranch         *b_HT;   //!
    TBranch         *b_METPhi;   //!
    TBranch         *b_mj;   //!
    TBranch         *b_FatjetPt;   //!
    TBranch         *b_FatjetEta;   //!
    TBranch         *b_FatjetPhi;   //!
    TBranch         *b_RA4MusPt;   //!
    TBranch         *b_RA4MusPhi;   //!
    TBranch         *b_RA4MusEta;   //!
    TBranch         *b_RA4ElsPt;   //!
    TBranch         *b_RA4ElsPhi;   //!
    TBranch         *b_RA4ElsEta;   //!
    TBranch         *b_JetPt;   //!
    TBranch         *b_JetEta;   //!
    TBranch         *b_JetPhi;   //!
    TBranch         *b_JetCSV;   //!
    TBranch         *b_RA4MusVetoPt;   //!
    TBranch         *b_RA4ElsVetoPt;   //!
    TBranch         *b_GenId;   //!
    TBranch         *b_GenStatus;   //!
 TBranch         *b_GenPt;   //!
 TBranch         *b_GenPhi;   //!
 TBranch         *b_GenEta;   //!
//TBranch         *b_GenE;   //!
    TBranch         *b_GenMId;   //!
    TBranch         *b_GenGMId;   //!
    TBranch     *b_TrigMuon; 
    TBranch     *b_TrigSingleMuon; 
    TBranch     *b_top1pT; 
    TBranch     *b_top1Phi; 
    TBranch     *b_top2pT; 
    TBranch     *b_top2Phi; 
    
void InitBaby(TChain *ch)
{
    mj_        = 0;
    FatjetPt_  = 0;
    FatjetEta_ = 0;
    FatjetPhi_ = 0;
    RA4MusPt_  = 0;
    RA4MusPhi_ = 0;
    RA4MusEta_ = 0;
    RA4ElsPt_  = 0;
    RA4ElsPhi_  = 0;
    RA4ElsEta_  = 0;
    JetPt_  = 0;
    JetEta_ = 0;
    JetPhi_ = 0;
    JetCSV_ = 0;
    RA4MusVetoPt_  = 0;
    RA4ElsVetoPt_  = 0;
    GenId_  = 0;
    GenPt_  = 0;
    GenPhi_  = 0;
    GenEta_  = 0;
    //GenE_  = 0;
    GenStatus_  = 0;
    GenMId_  = 0;
    GenGMId_  = 0;

    ch->SetBranchAddress("event",           &event_,         &b_event);
    ch->SetBranchAddress("EventWeight",     &EventWeight_,   &b_EventWeight);
    ch->SetBranchAddress("EventWeightNeg",     &EventWeightNeg_,   &b_EventWeightNeg);
    ch->SetBranchAddress("Npu",             &Npu_,           &b_Npu);
    ch->SetBranchAddress("Npv",             &Npv_,           &b_Npv);
    ch->SetBranchAddress("Nfatjet_pT30",    &Nfatjet_,       &b_Nfatjet);
    ch->SetBranchAddress("Nskinnyjet",      &Nskinnyjet_,    &b_Nskinnyjet);
    ch->SetBranchAddress("NBtagCSVM",       &NBtagCSVM_,     &b_NBtagCSVM);
    ch->SetBranchAddress("MJ_pT30",         &MJ_,            &b_MJ);
    ch->SetBranchAddress("MET",             &MET_,           &b_MET);
    ch->SetBranchAddress("HT",              &HT_,            &b_HT);
    ch->SetBranchAddress("METPhi",          &METPhi_,        &b_METPhi);
    ch->SetBranchAddress("mj_pT30",         &mj_,            &b_mj);
    ch->SetBranchAddress("FatjetPt_pT30",   &FatjetPt_,      &b_FatjetPt);
    ch->SetBranchAddress("FatjetEta_pT30",  &FatjetEta_,     &b_FatjetEta);
    ch->SetBranchAddress("FatjetPhi_pT30",  &FatjetPhi_,     &b_FatjetPhi);
    ch->SetBranchAddress("RA4MusPt_mi",        &RA4MusPt_,      &b_RA4MusPt);
    ch->SetBranchAddress("RA4MusPhi_mi",       &RA4MusPhi_,     &b_RA4MusPhi);
    ch->SetBranchAddress("RA4MusEta_mi",       &RA4MusEta_,     &b_RA4MusEta);
    ch->SetBranchAddress("RA4ElsPt_mi",        &RA4ElsPt_,      &b_RA4ElsPt);
    ch->SetBranchAddress("RA4ElsPhi_mi",       &RA4ElsPhi_,     &b_RA4ElsPhi);
    ch->SetBranchAddress("RA4ElsEta_mi",       &RA4ElsEta_,     &b_RA4ElsEta);
    ch->SetBranchAddress("JetPt_mi",           &JetPt_,         &b_JetPt);
    ch->SetBranchAddress("JetPhi_mi",          &JetPhi_,        &b_JetPhi);
    ch->SetBranchAddress("JetEta_mi",          &JetEta_,        &b_JetEta);
    ch->SetBranchAddress("JetCSV_mi",          &JetCSV_,        &b_JetCSV);
    ch->SetBranchAddress("RA4MusVetoPt_mi",    &RA4MusVetoPt_, &b_RA4MusVetoPt);
    ch->SetBranchAddress("RA4ElsVetoPt_mi",    &RA4ElsVetoPt_, &b_RA4ElsVetoPt);
    ch->SetBranchAddress("GenId",           &GenId_,         &b_GenId);
    ch->SetBranchAddress("GenPt",           &GenPt_,         &b_GenPt);
    ch->SetBranchAddress("GenPhi",           &GenPhi_,         &b_GenPhi);
    ch->SetBranchAddress("GenEta",           &GenEta_,         &b_GenEta);
    // ch->SetBranchAddress("GenE",           &GenE_,         &b_GenE);
    ch->SetBranchAddress("GenMId",          &GenMId_,        &b_GenMId);
    if(status)ch->SetBranchAddress("GenStatus",          &GenStatus_,        &b_GenStatus);
    ch->SetBranchAddress("GenGMId",         &GenGMId_,       &b_GenGMId);
    ch->SetBranchAddress("top1pT",        &top1pT_, &b_top1pT);
    ch->SetBranchAddress("top1Phi",       &top1Phi_, &b_top1Phi);
    ch->SetBranchAddress("top2pT",        &top2pT_, &b_top2pT);
    ch->SetBranchAddress("top2Phi",       &top2Phi_, &b_top2Phi);
    //ch->SetBranchAddress("TrigMuon",      &TrigMuon_, &b_TrigMuon);
    //ch->SetBranchAddress("TrigSingleMuon", &TrigSingleMuon_, &b_TrigSingleMuon);
}
