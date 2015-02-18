// 
bool PassNLep(unsigned int Nlep)
{
    if( (RA4MusPt_->size()+RA4ElsPt_->size())==Nlep 
        && RA4MusVetoPt_->size()==0 
        && RA4ElsVetoPt_->size()==0 
      ) return true;
    else return false;
       
}

// 
bool PassBaselineSelection()
{
  return (HT_>750 && MET_>250 && NBtagCSVM_>1 && Nskinnyjet_>5); 
}

bool Pass1BBaselineSelection()
{
  return (HT_>750 && MET_>250 && NBtagCSVM_==1 && Nskinnyjet_>5); 
}
// 
bool PassSelection(TString Region, 
                   float HT, float MET, int Nb, int Njet, float mT, float MJ)
{
    bool passed=false;
    
    if(Region=="SR0" && PassBaselineSelection() &&  PassNLep(1)
        && HT > -1 
        && MET > 400
        && Nb > -1 
        && Njet > -1 
        && mT > 150 
        && MJ > 600 
    )  passed = true;
    
    if(Region=="SR1" && PassBaselineSelection() &&  PassNLep(1)
        && HT > 750
        && MET > -1 
        && Nb > -1 
        && Njet > -1 
        && mT < 150 
        && MJ > -1 
    )  passed = true;

    if(Region=="1BSR1"&& Pass1BBaselineSelection() &&  PassNLep(1)
       && HT > 750
        && MET > -1 
        && Nb > -1 
        && Njet > -1 
        && mT < 150
        && MJ > -1 
    )  passed = true;
   
    
    if(Region=="1BCR0" && Pass1BBaselineSelection() &&  PassNLep(1)
       && HT > 750
        && MET > -1 
        && Nb > -1 
        && Njet > -1 
        && mT > 150
        && MJ > -1 
    )  passed = true;

    if(Region=="1B2lCR0" && Pass1BBaselineSelection() &&  PassNLep(2)
       && HT > 750
       && MET > -1 
       && Nb > -1 
       && Njet > -1 
       && mT > 150
       && MJ > -1 
    )  passed = true;

      if(Region=="1BCRincl" && Pass1BBaselineSelection() &&  PassNLep(1)
       && HT > 750
        && MET > -1 
        && Nb > -1 
        && Njet > -1 
        && mT > -1
        && MJ > -1 
    )  passed = true;

     if(Region=="1B2lCRincl" && Pass1BBaselineSelection() &&  PassNLep(2)
       && HT > 750
        && MET > -1 
        && Nb > -1 
        && Njet > -1 
        && mT > -1
        && MJ > -1 
    )  passed = true;


    if(Region=="1BSR0" && Pass1BBaselineSelection() &&  PassNLep(1)
        && HT > -1 
        && MET > 400
        && Nb > -1 
        && Njet > -1 
        && mT > 150 
        && MJ > 600 
    )  passed = true;


    
    return passed;

}
