// 
bool PassNLep(unsigned int Nlep)
{
   
  /* && RA4MusVetoPt_->size()==0 
     && RA4ElsVetoPt_->size()==0 */
  int nveto=0;
  for(unsigned int i=0;i<RA4MusVetoPt_->size();i++){
    if(RA4MusVetoPt_->at(i)>10) nveto++;
  }
  for(unsigned int i=0;i<RA4ElsVetoPt_->size();i++){
    if(RA4ElsVetoPt_->at(i)>10) nveto++;
  }
  if( (RA4MusPt_->size()+RA4ElsPt_->size())==Nlep && nveto==0) return true;
  else return false;
       
}

// 
bool PassBaselineSelection()
{
  return true;//(HT_>750 && MET_>250 && NBtagCSVM_>1 && Nskinnyjet_>5); 
}

bool Pass1BBaselineSelection()
{
  return true;// (HT_>750 && MET_>250 && NBtagCSVM_==1 && Nskinnyjet_>5); 
}
bool PassAllBBaselineSelection()
{
  return true;//(HT_>750 && MET_>250 && Nskinnyjet_>5); 
}

bool Pass2l_loose_1BBaselineSelection()
{
  return true;// (HT_>650 && MET_>250 && NBtagCSVM_==1 && Nskinnyjet_>3); 
}

bool PassLowSJ1BBaselineSelection()
{
  return true;// (HT_>750 && MET_>250 && NBtagCSVM_==1 && Nskinnyjet_>3); 
}


// 
bool PassSelection(TString Region, 
                   float HT, float MET, int Nb, int Njet, float mT, float MJ, TString ChainName="" ,int Ngenlep =0)
{
    bool passed=false;


    if(Region=="lobbaseline" && (PassNLep(1) || PassNLep(2))
        && HT > 500 
        && MET > 200
        && Nb > 0 
        && Njet > 3
        && mT > -1
        && MJ > -1 
    )  passed = true;

      if(Region=="baseline" && PassNLep(1) 
        && HT > 500 
        && MET > 200
        && Nb > 1 
        && Njet > 5
        && mT > -1
        && MJ > -1 
    )  passed = true;
     
    if(Region=="SR0" && PassBaselineSelection() &&  PassNLep(1)
        && HT > 500 
        && MET > 400
        && Nb > 1 
        && Njet > 5
        && mT > 150 
        && MJ > 600 
    )  passed = true;
    
    if(Region=="SR1" && PassBaselineSelection() &&  PassNLep(1)
        && HT > 500
        && MET > 250 
        && Nb > 1 
        && Njet > 5
        && mT > 150 
        && MJ > 600 
    )  passed = true;

    if(Region=="SRstats" && PassBaselineSelection() &&  PassNLep(1)
        && HT > 500
        && MET > 250 
        && Nb > 1 
        && Njet > 5
        && mT > 150 
        && MJ > 400 
    )  passed = true;

 if(Region=="SRstats1" && PassBaselineSelection() &&  PassNLep(1)
        && HT > 500
        && MET > 400 
        && Nb > 1 
        && Njet > 5
        && mT > 150 
        && MJ > 400 
    )  passed = true;

    
    if(Region=="SRincl" && PassBaselineSelection() &&  PassNLep(1)
        && HT > 500
        && MET > 250 
        && Nb > 1 
        && Njet > 5 
        && mT > -1
        && MJ > -1 
    )  passed = true;

    if(Region=="1B_hi_mT"&& Pass1BBaselineSelection() &&  PassNLep(1)
       && HT > 500
        && MET > 250 
        && Nb ==1 
        && Njet > 5
        && mT > 150
        && MJ > -1 
    )  passed = true;

    if(Region=="1B_sl"&& Pass1BBaselineSelection() &&  PassNLep(1) && ChainName.Contains("TT") && Ngenlep == 1
       && HT > 500
        && MET > 250 
        && Nb ==1 
        && Njet > 5
        && mT > -1
        && MJ > -1 
    )  passed = true;

    if(Region=="1B_ll"&& Pass1BBaselineSelection() &&  PassNLep(1) && ChainName.Contains("TT") && Ngenlep == 2
       && HT > 500
        && MET > 250 
        && Nb ==1 
        && Njet > 5
        && mT > -1
        && MJ > -1 
    )  passed = true;
    
    if(Region=="1B_lo_mT" && Pass1BBaselineSelection() &&  PassNLep(1)
       && HT >500
        && MET > 250 
        && Nb >1 
        && Njet > 5 
        && mT < 150
        && MJ > -1 
    )  passed = true;

    if(Region=="2B_hi_mT" &&  PassNLep(1)
       && HT > 500
        && MET > 250 
        && Nb >1 
        && Njet > 5
        && mT > 150
        && MJ > -1 
    )  passed = true;

    if(Region=="2B_sl" &&  PassNLep(1) && ChainName.Contains("TT") && Ngenlep == 1
       && HT > 500
        && MET > 250 
        && Nb >1 
        && Njet > 5
        && mT > -1
        && MJ > -1 
    )  passed = true;

    if(Region=="2B_ll"&&  PassNLep(1) && ChainName.Contains("TT") && Ngenlep == 2
       && HT > 500
        && MET > 250 
        && Nb >1 
        && Njet > 5
        && mT > -1
        && MJ > -1 
    )  passed = true;
    
    if(Region=="2B_lo_mT"  &&  PassNLep(1)
       && HT >500
        && MET > 250 
        && Nb >1 
        && Njet > 5 
        && mT < 150
        && MJ > -1 
    )  passed = true;


    if(Region=="1B2lCR0" && Pass1BBaselineSelection() &&  PassNLep(2)
       && HT > 750
       && MET > 250 
       && Nb ==1 
       && Njet > 5 
       && mT > 150
       && MJ > -1 
    )  passed = true;

      if(Region=="1BCRincl" && Pass1BBaselineSelection() &&  PassNLep(1)
       && HT > 500
        && MET > 250 
        && Nb ==1
        && Njet > 5 
        && mT > -1
        && MJ > -1 
    )  passed = true;

       if(Region=="1B4SJCRincl" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
       && HT > 750
        && MET > 250 
        && Nb ==1 
        && Njet > 3
        && mT > -1
        && MJ > -1 
    )  passed = true;

       if(Region=="1B45SJ" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
	  && HT > 750
	  && MET > 250 
	  && Nb ==1 
	  && Njet > 3 && Njet<6 
	  && mT > -1
	  && MJ > -1 
	  )  passed = true;
       if(Region=="1B67SJ" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
	  && HT > 750
	  && MET > 250 
	  && Nb ==1
	  && Njet > 5 && Njet<8 
	  && mT > -1
	  && MJ > -1 
	  )  passed = true;
       if(Region=="1B8SJ" && PassLowSJ1BBaselineSelection() &&  PassNLep(1)
	  && HT > 750
	  && MET > 250 
	  && Nb ==1
	  && Njet > 7
	  && mT > -1
	  && MJ > -1 
	  )  passed = true;

     if(Region=="AllBCRincl" && PassAllBBaselineSelection() &&  PassNLep(1)
       && HT > 750
        && MET > 250 
        && Nb > -1 
        && Njet > 5 
        && mT > -1
        && MJ > -1 
    )  passed = true;

     if(Region=="1B2lCRincl" && Pass1BBaselineSelection() &&  PassNLep(2)
       && HT > 750
        && MET > 250 
        && Nb ==1 
        && Njet > 5
        && mT > -1
        && MJ > -1 
    )  passed = true;

     if(Region=="1B2lCRincl_loose" && Pass2l_loose_1BBaselineSelection() &&  PassNLep(2)
       && HT > -1
        && MET > 250 
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
