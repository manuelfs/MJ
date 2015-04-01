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
bool PassAllBBaselineSelection()
{
  return (HT_>750 && MET_>250 && Nskinnyjet_>5); 
}

bool Pass2l_loose_1BBaselineSelection()
{
  return (HT_>650 && MET_>250 && NBtagCSVM_==1 && Nskinnyjet_>3); 
}

bool PassLowSJ1BBaselineSelection()
{
  return (HT_>750 && MET_>250 && NBtagCSVM_==1 && Nskinnyjet_>3); 
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

    if(Region=="SRincl" && PassBaselineSelection() &&  PassNLep(1)
        && HT > 750
        && MET > -1 
        && Nb > -1 
        && Njet > -1 
        && mT > -1
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

       if(Region=="1B4SJCRincl" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
       && HT > 750
        && MET > -1 
        && Nb > -1 
        && Njet > -1 
        && mT > -1
        && MJ > -1 
    )  passed = true;

       if(Region=="1B45SJ" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
	  && HT > 750
	  && MET > -1 
	  && Nb > -1 
	  && Njet > 3 && Njet<6 
	  && mT > -1
	  && MJ > -1 
	  )  passed = true;
       if(Region=="1B67SJ" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
	  && HT > 750
	  && MET > -1 
	  && Nb > -1 
	  && Njet > 5 && Njet<8 
	  && mT > -1
	  && MJ > -1 
	  )  passed = true;
       if(Region=="1B8SJ" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
	  && HT > 750
	  && MET > -1 
	  && Nb > -1 
	  && Njet > 7
	  && mT > -1
	  && MJ > -1 
	  )  passed = true;

     if(Region=="AllBCRincl" && PassAllBBaselineSelection() &&  PassNLep(1)
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

     if(Region=="1B2lCRincl_loose" && Pass2l_loose_1BBaselineSelection() &&  PassNLep(2)
       && HT > -1
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
