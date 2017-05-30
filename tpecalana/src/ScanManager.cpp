//
//  ScanManager.cpp
//  tpecalana_project
//
//  Created by Roman Poeschl on 10/08/15.
//
//

#include <string>
#include <sstream>
#include "TF1.h"
#include "TGraph.h"
#include "TH1F.h"
#include "ScanManager.h"
#include "FitGraphs.h"
#include "global.h"

using namespace globalvariables;

ScanManager::ScanManager(){
  _ntrigVecMapHigh.clear();
  _ntrigChipVecMapHigh_norm.clear();
  _ntrigChipVecMapHigh.clear();
  _meanADCChipVecMapHigh.clear();

  _nRuns=0;
  f_scurve=0;
}

ScanManager::~ScanManager(){/* no op*/}

void ScanManager::init(){
   
  _nRuns=0;
  //to be removed
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size(); ichip++) {
    _ntrigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> >  () ));
    _maxHithelpVec.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t> () ));
    _maxHitCounthelpVec.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t> () ));

    _ntrigChipVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> > () ));
    _ntrigChipVecMapHigh_norm.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> > () ));

    _meanADCChipVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> > () ));


  }
  
  std::cout<<"Init the Analysis Manager for general analysis, with getAnalysisType()="<<globalvariables::getAnalysisType()<<std::endl;

}

void ScanManager::acquireRunInformation(ExperimentalSetup* aExpSetup, int buffer){
  std::cout << "************** AcquireRunInformation new file: " << _nRuns << "***********************" << std::endl;
  if(globalvariables::getAnalysisType()=="scurves") sCurveAnalysis(aExpSetup, buffer);
  if(globalvariables::getAnalysisType()=="PlaneEventsScan") planeEventsAnalysis(aExpSetup);
  if(globalvariables::getAnalysisType()=="holdscan") holdscanAnalysis(aExpSetup);

  _nRuns++;

}

void ScanManager::displayResults(TString file_prefix, int buffer){
  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  if(globalvariables::getAnalysisType()=="scurves") sCurveAnalysisGraphics(file_prefix,buffer);
  if(globalvariables::getAnalysisType()=="PlaneEventsScan") planeEventsAnalysisGraphics(file_prefix);
  if(globalvariables::getAnalysisType()=="holdscan") holdscanAnalysisGraphics(file_prefix);

}


void ScanManager::holdscanAnalysis(ExperimentalSetup* aExpSetup) {
    
  //Loop over all enabled chips
  for (channelInfoComplDouble_t::iterator mapiter = _meanADCChipVecMapHigh.begin();mapiter!=_meanADCChipVecMapHigh.end();mapiter++) {
    std::cout << "*************holdscanAnalysis:: New chip: " << (*mapiter).first << "********************" << std::endl;

    channelInfoComplDouble_t::iterator helpMapIter = _meanADCChipVecMapHigh.find((*mapiter).first);
    //Loop over all enabled chips

    unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNumberOfChannels());

    for (unsigned ichan=0; ichan < numChans; ichan++) {
      //Build up the vector for the individual channels (needs to be done only once)
      if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back (std::vector<Double_t> () );
      //reads out the trigger of each channel
      double meanADCmtmp(0);
      int nbuf=0;

      for(int ibuf=0; ibuf<15;ibuf++) {
	std::vector<double> meantmpvec;
	meantmpvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelMeanVec(ichan,"High");
	if(meantmpvec.at(0)>0) {
	  meanADCmtmp+=meantmpvec.at(0);
	  nbuf++;
	}
      }
      cout<<meanADCmtmp<<" "<<nbuf<< " ";
      //if(nbuf>0) cout<<meanADCmtmp/nbuf;
      //cout<<endl;
      
      if(nbuf>0) (*mapiter).second.at(ichan).push_back(meanADCmtmp/nbuf);//save the mean of signal per channel
      else (*mapiter).second.at(ichan).push_back(0);
    }

  }

}



void ScanManager::planeEventsAnalysis(ExperimentalSetup* aExpSetup) {
    
  //Loop over all enabled chips
  for (channelInfoComplDouble_t::iterator mapiter = _ntrigChipVecMapHigh_norm.begin();mapiter!=_ntrigChipVecMapHigh_norm.end();mapiter++) {
    std::cout << "*************New chip: " << (*mapiter).first << "********************" << std::endl;

    channelInfoComplDouble_t::iterator helpMapIter = _ntrigChipVecMapHigh.find((*mapiter).first);

    unsigned ntrigm(0);
    unsigned ntrigm_bcid1(0);
    unsigned ntrigm_bcid5(0);
    unsigned ntrigm_bcid10(0);
    unsigned ntrigm_planeevents(0);
    unsigned ntrigm_negativedata(0);
    unsigned ntrigm_total(0);

    unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNumberOfChannels());

    for (unsigned ichan=0; ichan < numChans; ichan++) {
      //Build up the vector for the individual channels (needs to be done only once)
      //reads out the trigger of each channel
      unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
      std::vector<unsigned> trigmtmpvec;
            
      for(unsigned ibuf=1; ibuf<bufdepth; ibuf++){
	trigmtmpvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggersVec(ichan);
	ntrigm+=trigmtmpvec.at(0);
	if(ibuf>0) {
	  ntrigm_bcid1+=trigmtmpvec.at(2);
	  ntrigm_bcid5+=trigmtmpvec.at(3);
	  ntrigm_bcid10+=trigmtmpvec.at(4);
	  ntrigm_planeevents+=trigmtmpvec.at(5);
	  ntrigm_negativedata+=trigmtmpvec.at(6);
	  ntrigm_total+=(trigmtmpvec.at(2)+trigmtmpvec.at(4)+trigmtmpvec.at(5)+trigmtmpvec.at(6));
	}
      }   
    }

    //Build up the vector for the different ratio counters
    for(unsigned isize=0; isize<7; isize++ ) {
      if( (*mapiter).second.size() < (isize + 1 )) (*mapiter).second.push_back (std::vector<Double_t> () );
      if( (*helpMapIter).second.size() < (isize + 1 )) (*helpMapIter).second.push_back (std::vector<Double_t> () );
    }

    //Add for each run the value of each ratio
    //fills the triggers into a vector that is a part of a map of chips and channels
    if(ntrigm>0){ 
      (*mapiter).second.at(6).push_back(double(ntrigm)/double(ntrigm));
      (*mapiter).second.at(0).push_back(double(ntrigm_bcid1)/double(ntrigm));
      (*mapiter).second.at(1).push_back(double(ntrigm_bcid5)/double(ntrigm));
      (*mapiter).second.at(2).push_back(double(ntrigm_bcid10)/double(ntrigm));
      (*mapiter).second.at(3).push_back(double(ntrigm_planeevents)/double(ntrigm));
      (*mapiter).second.at(4).push_back(double(ntrigm_negativedata)/double(ntrigm));
      (*mapiter).second.at(5).push_back(double(ntrigm_total)/double(ntrigm));
    } else {
      (*mapiter).second.at(0).push_back(0);
      (*mapiter).second.at(1).push_back(0);
      (*mapiter).second.at(2).push_back(0);
      (*mapiter).second.at(3).push_back(0);
      (*mapiter).second.at(4).push_back(0);
      (*mapiter).second.at(5).push_back(0);
      (*mapiter).second.at(6).push_back(0);
    }

    (*helpMapIter).second.at(6).push_back(double(ntrigm));
    (*helpMapIter).second.at(0).push_back(double(ntrigm_bcid1));
    (*helpMapIter).second.at(1).push_back(double(ntrigm_bcid5));
    (*helpMapIter).second.at(2).push_back(double(ntrigm_bcid10));
    (*helpMapIter).second.at(3).push_back(double(ntrigm_planeevents));
    (*helpMapIter).second.at(4).push_back(double(ntrigm_negativedata));
    (*helpMapIter).second.at(5).push_back(double(ntrigm_total));
    

    if(ntrigm>0) 
      std::cout << "ScanManager::planeEventsAnalysis - chip " << (*mapiter).first << 
	" TriggersBad/AllTriggers: " << double(ntrigm_total)/double(ntrigm) <<" TriggersBad= "<<ntrigm_total<<" All Triggers="<<ntrigm<< std::endl;  
  }
}

void ScanManager::sCurveAnalysis(ExperimentalSetup* aExpSetup, int buffer) {
    
  std::vector<std::pair<unsigned, unsigned> >acqnumber=aExpSetup->getDif(0).getDifStatVec();
  float nSpills= acqnumber.size();
  //for(int iac=0; iac<acqnumber.size();iac++) 
  //std::cout<< iac << " " << acqnumber.at(iac).first << " " << acqnumber.at(iac).second <<std::endl;

  //Loop over all enabled chips
  for (channelInfoComplDouble_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {
    //    std::cout << "*************New chip: " << (*mapiter).first << "********************" << std::endl;
    std::map<unsigned, std::vector<Double_t> >::iterator helpMapIter = _maxHithelpVec.find((*mapiter).first);
    std::map<unsigned, std::vector<Double_t> >::iterator helpMapIter2 = _maxHitCounthelpVec.find((*mapiter).first);

    unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNumberOfChannels());

    for (unsigned ichan=0; ichan < numChans; ichan++) {
      //Build up the vector for the individual channels (needs to be done only once)
      if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back (std::vector<Double_t> () );
      //reads out the trigger of each channel
      unsigned ntrigmtmp(0);

      int bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

      for(int ibuf=0; ibuf<bufdepth; ibuf++){
	if(ibuf==buffer || buffer == 15 ) {
	  std::vector<unsigned> trigmtmpvec;
	  trigmtmpvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggersVec(ichan);
	  //ntrigmtmp+=trigmtmpvec.at(0);
	  for(int itrig=0; itrig<trigmtmpvec.size() ; itrig++) {
	    if(itrig!=1 && itrig<5) ntrigmtmp+=trigmtmpvec.at(itrig); 
	    //save all triggers (bcid+1, bcid+x included) except plane and negative events
	  }
	}//if
      }//ibuf
      
      //Add for each run the value in that channel
      //fills the triggers into a vector that is a part of a map of chips and channels

      double trig_per_spill = 0;
      if( nSpills > 1 )trig_per_spill = ntrigmtmp/nSpills;
      (*mapiter).second.at(ichan).push_back(trig_per_spill);
      //      if(ntrigmtmp>0) std::cout << "ScanManager::sCurveAnalysis - chip " << (*mapiter).first << " Channel: " << ichan << " Triggers: " << ntrigmtmp << " " << nSpills << " " << ntrigmtmp/nSpills << std::endl;
      //Store the maximum nummber of triggers/spills
      if (helpMapIter != _maxHithelpVec.end()) {
  	if ((*helpMapIter).second.size()<ichan+1 ) (*helpMapIter).second.push_back(trig_per_spill);
  	else {
  	  if (ntrigmtmp/nSpills >  (*helpMapIter).second.at(ichan)) {
  	    (*helpMapIter).second.at(ichan) = trig_per_spill;
  	  }               
  	}
      }  
      //Store the maximum nummber of triggers
      if (helpMapIter2 != _maxHitCounthelpVec.end()) {
  	if ((*helpMapIter2).second.size()<ichan+1 ) (*helpMapIter2).second.push_back(ntrigmtmp);
  	else {
  	  if (ntrigmtmp >  (*helpMapIter2).second.at(ichan)) {
  	    (*helpMapIter2).second.at(ichan) = ntrigmtmp;
  	  }               
  	}
      } 
    }      
  }
}

void ScanManager::holdscanAnalysisGraphics(TString file_sufix) {

 
  //Loop over all enabled chips
  for (channelInfoComplDouble_t::iterator mapiter = _meanADCChipVecMapHigh.begin();mapiter!=_meanADCChipVecMapHigh.end();mapiter++) {
    holdscanAnalysisGraphicsPainter(mapiter,file_sufix);
  }

}


void ScanManager::holdscanAnalysisGraphicsPainter(channelInfoComplDouble_t::iterator aMapIter, TString file_sufix) {
    

  // TString gain = globalvariables::getGainTStringAnalysis();
  TString filerecreate = "RECREATE";
  if((*aMapIter).first  > 0) filerecreate="UPDATE";
  TFile *f_holdscan = TFile::Open(file_sufix+".root", filerecreate);
  TDirectory *dir = f_holdscan->GetDirectory(TString::Format("holdscan_graphs"));
  if (!dir) dir = f_holdscan->mkdir(TString::Format("holdscan_graphs"));

  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "Chip" << (*aMapIter).first ;
  std::stringstream canvasTitleStr;
  canvasTitleStr << "Chip" << (*aMapIter).first << "_" << TString::Format("Holdscans_PlaneEvThresh%i",globalvariables::getPlaneEventsThreshold());//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasTitleStr.str().c_str(),11,30,1200,200);
  c_chips->Divide(4,1);
  c_chips->SetTitle(canvasTitleStr.str().c_str());

  //Helper variabled to allow to switch to another pad in the canvas
  unsigned ipad(0);
  //A vector of graphs
  std::vector<TGraphErrors*> graphHoldscan;//vector of graphs... extract the proper graph
 
  //Loopover all chips
  std::cout << "ScanManager::holdscanAnalysisGraphicsPainter: Chip: " << (*aMapIter).first << std::endl;
  //Loop over all channels
  //Helper variable to count channels
  unsigned ichan(0);
    
  for (std::vector<std::vector<Double_t> >::iterator chanVeciter=(*aMapIter).second.begin(); chanVeciter!=(*aMapIter).second.end(); chanVeciter++) {
    std::cout << "ScanManager::holdscanAnalysisGraphicsPainter: Nchan: " << ichan << std::endl;
    //At least as a sanity check we verify that the size of the vector for each channel corresponds to the size of the
    //vector holding the hold values as defined above
    if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
      std::cout << "ScanManager::holdscanAnalysisGraphicsPainter Warning: Size of vector with hold values does not correspond to size of vector with readings for channel" << ichan << std::endl;
      std::cout << "Size of vector with hold values is: " << globalvariables::getScanVectorDoubles().size() << std::endl;
      std::cout << "Size of vector with readings is: " << (*chanVeciter).size()  << std::endl;
    }
    //Define an array that holds the entries for each run
    double valarray[ (*chanVeciter).size()];
    double evalarray[ (*chanVeciter).size()];
    double evalarray_x[ (*chanVeciter).size()];

    //Loop over the entries with trigger in the given runs
    //Helper variable to count runs
    unsigned irun(0);
    for (std::vector<Double_t>::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
      // std::cout << "ScanManager::holdscanAnalysisGraphicsPainter: Run " << irun << ": " << (*runIter) << std::endl;
      //Fetch the value for that run
      double runval(static_cast<double>((*runIter)));
      //Fill the array with the relative counts for a given channel in a given run
      if(runval >-1 && runval <99999) valarray[irun] = runval;
      else  valarray[irun] = 0;
      evalarray[irun] = 0;
      evalarray_x[irun] = 0;///ref;
      irun++;
    }

    f_holdscan->cd();
    dir->cd();
    //Now define and fill a graph for each channel
    double vecarrayhelp[(*chanVeciter).size()];
    double* vecarray;
    vecarray = vectortoarray(globalvariables::getScanVectorDoubles(), &vecarrayhelp[0]);
    graphHoldscan.push_back(new TGraphErrors(globalvariables::getScanVectorDoubles().size(), vecarray, valarray,  evalarray_x, evalarray));
    graphHoldscan.back()->SetMarkerStyle(20+ichan%16);
    //We have only 15 different marker symbols but in principle 16 channels to draw
    if (ichan%16==15) graphHoldscan.back()->SetMarkerStyle(5);
    graphHoldscan.back()->SetMarkerSize(0.8);
    if (ichan%16 == 0) {
      c_chips->cd(ipad+1);
      ipad++;
      graphHoldscan.back()->SetName(TString::Format("Chip%i_chn%i",int((*aMapIter).first),int(ichan)));
      graphHoldscan.back()->SetTitle(TString::Format("Chip%i_chn%i-%i",int((*aMapIter).first),int(ichan),int(ichan)+15));
      graphHoldscan.back()->Write();
      graphHoldscan.back()->GetXaxis()->SetTitle("Hold Value");
      graphHoldscan.back()->GetYaxis()->SetTitle("Nhit/Nref");
      graphHoldscan.back()->Draw("APSL");
    } else {
      graphHoldscan.back()->SetName(TString::Format("Chip%i_chn%i",int((*aMapIter).first),int(ichan)));
      graphHoldscan.back()->Write();
      graphHoldscan.back()->Draw("PSL");
    }
 
    ichan++;
  }

  c_chips->Update(); 
  f_holdscan->Close();     
}

// -------------------------------------------------------

void ScanManager::sCurveAnalysisGraphics(TString file_sufix, int buffer) {

  fout_scurves_chip_3sigma.open(file_sufix+"_3sigma.cmd",ios::out);
  fout_scurves_chip_5sigma.open(file_sufix+"_5sigma.cmd",ios::out);
  fout_scurves_chip_3sigma_firstzero.open(file_sufix+"_3sigma_firstzero.cmd",ios::out);

  fout_scurves.open(file_sufix+".log",ios::out);

  fout_scurves<<"#Scurve analysis summary" <<std::endl;
  fout_scurves<<"#"<<file_sufix <<std::endl;
  fout_scurves<<"#chip channel threshold width chi2/NDF first_zero" <<std::endl;

  c_mean = new TCanvas("Threshold Mean","Threshold Mean",800,800);
  c_mean->Divide(4,4);
  c_sigma = new TCanvas("Threshold Sigma","Threshold sigma",800,800);
  c_sigma->Divide(4,4);

  //Loop over all enabled chips
  for (channelInfoComplDouble_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {
    sCurveAnalysisGraphicsPainter(mapiter,file_sufix, buffer);
  }

  fout_scurves.close();
  fout_scurves_chip_3sigma.close();
  fout_scurves_chip_5sigma.close();
  fout_scurves_chip_3sigma_firstzero.close();


}


void ScanManager::sCurveAnalysisGraphicsPainter(channelInfoComplDouble_t::iterator aMapIter, TString file_sufix, int buffer) {
    

  // TString gain = globalvariables::getGainTStringAnalysis();
  TString filerecreate = "RECREATE";
  if((*aMapIter).first  > 0) filerecreate="UPDATE";
  TFile *f_scurve = TFile::Open(file_sufix+".root", filerecreate);
  TDirectory *dir = f_scurve->GetDirectory(TString::Format("scurves_graphs_buffer%i",buffer));
  if (!dir) dir = f_scurve->mkdir(TString::Format("scurves_graphs_buffer%i",buffer));

  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "Chip" << (*aMapIter).first ;
  std::stringstream canvasTitleStr;
  canvasTitleStr << "Chip" << (*aMapIter).first << "_" << TString::Format("Scurves_PlaneEvThresh%i",globalvariables::getPlaneEventsThreshold());//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasTitleStr.str().c_str(),11,30,1600,800);
  c_chips->Divide(4,3);
  c_chips->SetTitle(canvasTitleStr.str().c_str());

  //Helper variabled to allow to switch to another pad in the canvas
  unsigned ipad(0);
  //A vector of graphs
  std::vector<TGraphErrors*> graphScurve;//vector of graphs... extract the proper graph
 
  //-----
  TGraph * g_fitParScurve_1 = new TGraph(64);
  TGraph * g_fitParScurve_2 = new TGraph(64);
  TGraph * g_fitParScurve_3 = new TGraph(64);
 
  TGraph * g_fitParErrScurve_1 = new TGraph(64);
  TGraph * g_fitParErrScurve_2 = new TGraph(64);
  TGraph * g_fitParErrScurve_3 = new TGraph(64);

  TGraph * g_fitChisq = new TGraph(64);
  //-----
  TH1F * hist_fitParScurve_1 = new TH1F(TString::Format("hist_Chip%i_Scurve_firstzero",int((*aMapIter).first)),TString::Format("hist_Chip%i_Scurve_firstzero",int((*aMapIter).first)),200,100.5,300.5);
  TH1F * hist_fitParScurve_2 = new TH1F(TString::Format("hist_Chip%i_Scurve_mean",int((*aMapIter).first)),TString::Format("hist_Chip%i_Scurve_mean",int((*aMapIter).first)),400,125,250);
  TH1F * hist_fitParScurve_3 = new TH1F(TString::Format("hist_Chip%i_Scurve_sigma",int((*aMapIter).first)),TString::Format("hist_Chip%i_Scurve_sigma",int((*aMapIter).first)),500,0.05,50.05);
 
  TH1F * hist_fitParErrScurve_1 = new TH1F(TString::Format("hist_Chip%i_Scurve_Errnorm",int((*aMapIter).first)),TString::Format("hist_Chip%i_Scurve_Errnorm",int((*aMapIter).first)),100,0,0.5);
  TH1F * hist_fitParErrScurve_2 = new TH1F(TString::Format("hist_Chip%i_Scurve_Errmean",int((*aMapIter).first)),TString::Format("hist_Chip%i_Scurve_Errmean",int((*aMapIter).first)),100,0,5);
  TH1F * hist_fitParErrScurve_3 = new TH1F(TString::Format("hist_Chip%i_Scurve_Errsigma",int((*aMapIter).first)),TString::Format("hist_Chip%i_Scurve_Errsigma",int((*aMapIter).first)),100,0,5);
 
  TH1F * hist_fitChisq = new TH1F(TString::Format("hist_Chip%i_fitChisq_NDF",int((*aMapIter).first)),TString::Format("hist_Chip%i_fitChisq_NDF",int((*aMapIter).first)),100,-0.05,9.95);

  //An iterator used to fetch the maximum number of counts/spill and counts
  std::map<unsigned, std::vector<Double_t> >::iterator helpMapIter = _maxHithelpVec.find((*aMapIter).first);
  std::map<unsigned, std::vector<Double_t> >::iterator helpMapIter2 = _maxHitCounthelpVec.find((*aMapIter).first);

  //Loopover all chips
  std::cout << "ScanManager::sCurveAnalysisGraphicsPainter: Chip: " << (*aMapIter).first << std::endl;
  //Loop over all channels
  //Helper variable to count channels
  unsigned ichan(0);
    
  for (std::vector<std::vector<Double_t> >::iterator chanVeciter=(*aMapIter).second.begin(); chanVeciter!=(*aMapIter).second.end(); chanVeciter++) {
    std::cout << "ScanManager::sCurveAnalysisGraphicsPainter: Nchan: " << ichan << std::endl;
    //At least as a sanity check we verify that the size of the vector for each channel corresponds to the size of the
    //vector holding the thresholds as defined above
    if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
      std::cout << "ScanManager::sCurveAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for channel" << ichan << std::endl;
      std::cout << "Size of vector with thresholds is: " << globalvariables::getScanVectorDoubles().size() << std::endl;
      std::cout << "Size of vector with readings is: " << (*chanVeciter).size()  << std::endl;
    }
    //If the maximum is >2 we assume that this channel was enabled and we process it further
    //if ((*helpMapIter).second.at(ichan) > 0) {
    //Define an array that holds the entries for each run
    double valarray[ (*chanVeciter).size()];
    double evalarray[ (*chanVeciter).size()];
    double evalarray_x[ (*chanVeciter).size()];
    //A local variable that holds the reference values
    double ref(static_cast<double>((*helpMapIter).second.at(ichan))); //max triggers / num spills (acq)
    double ref_count(static_cast<double>((*helpMapIter2).second.at(ichan))); // max triggers

    //Loop over the entries with trigger in the given runs
    //Helper variable to count runs
    unsigned irun(0);
    for (std::vector<Double_t>::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
      // std::cout << "ScanManager::sCurveAnalysisGraphicsPainter: Run " << irun << ": " << (*runIter) << std::endl;
      //Fetch the value for that run
      double runval(static_cast<double>((*runIter)));
      //Fill the array with the relative counts for a given channel in a given run
      if(ref_count > 5 && ref>0) {
	valarray[irun] = runval/ref;
	evalarray[irun] = sqrt(runval)/sqrt(ref_count);
      }   else {
	valarray[irun] = 0;
	evalarray[irun] = 0;
      }
      evalarray_x[irun] = 0;///ref;

      //	std::cout << "Relative counts " << runval/ref << std::endl;
      irun++;
    }

    f_scurve->cd();
    dir->cd();
    //Now define and fill a graph for each channel
    double vecarrayhelp[(*chanVeciter).size()];
    double* vecarray;
    vecarray = vectortoarray(globalvariables::getScanVectorDoubles(), &vecarrayhelp[0]);
    graphScurve.push_back(new TGraphErrors(globalvariables::getScanVectorDoubles().size(), vecarray, valarray,  evalarray_x, evalarray));
    graphScurve.back()->SetMarkerStyle(20+ichan%16);
    //We have only 15 different marker symbols but in principle 16 channels to draw
    if (ichan%16==15) graphScurve.back()->SetMarkerStyle(5);
    graphScurve.back()->SetMarkerSize(0.8);
    if (ichan%16 == 0) {
      c_chips->cd(ipad+1);
      ipad++;
      graphScurve.back()->SetName(TString::Format("Chip%i_chn%i",int((*aMapIter).first),int(ichan)));
      graphScurve.back()->SetTitle(TString::Format("Chip%i_chn%i-%i",int((*aMapIter).first),int(ichan),int(ichan)+16));
      graphScurve.back()->Write();
      graphScurve.back()->GetXaxis()->SetTitle("Threshold");
      graphScurve.back()->GetYaxis()->SetTitle("Nhit/Nref");
      graphScurve.back()->Draw("APSL");
    } else {
      graphScurve.back()->SetName(TString::Format("Chip%i_chn%i",int((*aMapIter).first),int(ichan)));
      graphScurve.back()->Write();
      if(ref_count > 5) graphScurve.back()->Draw("PSL");
    }
    FitGraphs fit;
    TF1 * f;
    if(buffer!=0) f= fit.FitScurveGauss(graphScurve.back());
    else f= fit.FitScurve(graphScurve.back());

    float first_zero = fit.FirstZero(graphScurve.back());
    
    Int_t pointID = g_fitParScurve_1->GetN();

    g_fitParScurve_1->SetPoint(pointID,ichan , first_zero  );
    g_fitParErrScurve_1->SetPoint(pointID,ichan , 0 );
    hist_fitParScurve_1->Fill( first_zero);
    hist_fitParErrScurve_1->Fill(0);

    if(f->GetParameter(1) > 150 ) {
      //  g_fitParScurve_1->SetPoint(pointID,ichan , f->GetParameter(0) );
      g_fitParScurve_2->SetPoint(pointID,ichan , f->GetParameter(1) );
      g_fitParScurve_3->SetPoint(pointID,ichan , f->GetParameter(2) );
      
      //   g_fitParErrScurve_1->SetPoint(pointID,ichan , f->GetParError(0) );
      g_fitParErrScurve_2->SetPoint(pointID,ichan , f->GetParError(1) );
      g_fitParErrScurve_3->SetPoint(pointID,ichan , f->GetParError(2) );
      
      pointID = g_fitChisq->GetN();

      float chi2ndf = 0;
      if(f->GetNDF()!=0) chi2ndf = f->GetChisquare()/f->GetNDF();
      g_fitChisq->SetPoint(pointID,ichan , chi2ndf);
      
      
      // hist_fitParScurve_1->Fill(float(f->GetParameter(0)));
      hist_fitParScurve_2->Fill(float(f->GetParameter(1)));
      hist_fitParScurve_3->Fill(float(f->GetParameter(2)));
      
      // hist_fitParErrScurve_1->Fill(float(f->GetParError(0)));
      hist_fitParErrScurve_2->Fill(float(f->GetParError(1)));
      hist_fitParErrScurve_3->Fill(float(f->GetParError(2)));
      
      hist_fitChisq->Fill(chi2ndf);
      fout_scurves<<int((*aMapIter).first) << " " <<ichan << " ";
      fout_scurves<<std::fixed << std::setprecision(3) << f->GetParameter(1) << " " << f->GetParameter(2) <<  " " << chi2ndf <<" "<<first_zero<<std::endl;
	
    } else {
      
      fout_scurves<<int((*aMapIter).first) << " " <<ichan << " ";
      fout_scurves<<fixed << setprecision(3) << 0 << " " << 0 <<  " " << 0 <<" " <<first_zero<<std::endl;
      
    }

    ichan++;
  }



  c_chips->cd(5);
  g_fitParScurve_1->SetTitle(TString::Format("Chip%i_Scurve_firstzero",int((*aMapIter).first)));
  g_fitParScurve_1->SetName(TString::Format("Chip%i_Scurve_firstzero",int((*aMapIter).first)));
  g_fitParScurve_1->GetYaxis()->SetTitle("");
  g_fitParScurve_1->GetXaxis()->SetTitle("channel");
  g_fitParScurve_1->Draw("AL");

  c_chips->cd(6);
  g_fitParScurve_2->SetTitle(TString::Format("Chip%i_Scurve_mean",int((*aMapIter).first)));
  g_fitParScurve_2->SetName(TString::Format("Chip%i_Scurve_mean",int((*aMapIter).first)));
  g_fitParScurve_2->GetYaxis()->SetTitle("DAC");
  g_fitParScurve_2->GetYaxis()->SetRangeUser(170,220);
  g_fitParScurve_2->GetXaxis()->SetTitle("channel");
  g_fitParScurve_2->Draw("AL");

  c_chips->cd(7);
  g_fitParScurve_3->SetTitle(TString::Format("Chip%i_Scurve_sigma",int((*aMapIter).first)));
  g_fitParScurve_3->SetName(TString::Format("Chip%i_Scurve_sigma",int((*aMapIter).first)));  
  g_fitParScurve_3->GetYaxis()->SetTitle("DAC");
  g_fitParScurve_3->GetXaxis()->SetTitle("channel");
  g_fitParScurve_3->Draw("AL");

  c_chips->cd(8);
  g_fitChisq->SetTitle(TString::Format("Chip%i_Scurve_chisq",int((*aMapIter).first)));
  g_fitChisq->SetName(TString::Format("Chip%i_Scurve_chisq",int((*aMapIter).first)));
  g_fitChisq->GetXaxis()->SetTitle("channel");
  g_fitChisq->GetYaxis()->SetTitle("#Chi^{2}/NDF");
  g_fitChisq->GetYaxis()->SetRangeUser(0,5);
  g_fitChisq->Draw("AL");


  c_chips->Update();

  c_chips->cd(9);
  hist_fitParScurve_1->SetTitle(TString::Format("hist_Chip%i_Scurve_firstzero",int((*aMapIter).first)));
  hist_fitParScurve_1->GetYaxis()->SetTitle("");
  hist_fitParScurve_1->GetXaxis()->SetTitle("channel");
  hist_fitParScurve_1->Draw("L");
  //g_fitParScurve_1->Draw("AL");

  c_chips->cd(10);
  hist_fitParScurve_2->SetTitle(TString::Format("hist_Chip%i_Scurve_mean",int((*aMapIter).first)));
  hist_fitParScurve_2->GetYaxis()->SetTitle("");
  hist_fitParScurve_2->GetXaxis()->SetTitle("DAC");
  hist_fitParScurve_2->Draw("L");

  c_chips->cd(11);
  hist_fitParScurve_3->SetTitle(TString::Format("hist_Chip%i_Scurve_sigma",int((*aMapIter).first)));
  hist_fitParScurve_3->GetXaxis()->SetTitle("DAC");
  hist_fitParScurve_3->Draw("L");

  c_chips->cd(12);
  hist_fitChisq->SetTitle(TString::Format("hist_Chip%i_Scurve_chisq",int((*aMapIter).first)));
  hist_fitChisq->GetXaxis()->SetTitle("#Chi^{2}/NDF");
  hist_fitChisq->Draw("L");

  c_chips->Update();

  c_mean->cd((*aMapIter).first+1);
  hist_fitParScurve_2->SetTitle("Thresholds per chip");
  hist_fitParScurve_2->GetYaxis()->SetTitle("");
  hist_fitParScurve_2->GetXaxis()->SetTitle("DAC");
  hist_fitParScurve_2->Draw("L");
  //c_mean->Update();

  c_sigma->cd((*aMapIter).first+1);
  hist_fitParScurve_3->SetTitle("Sigma/Thresholds per chip");
  hist_fitParScurve_3->GetYaxis()->SetTitle("");
  hist_fitParScurve_3->GetXaxis()->SetTitle("DAC");
  hist_fitParScurve_3->Draw("L");
  //c_sigma->Update();

  int trigger = 225;
  if(hist_fitParScurve_2->GetEntries()>20) 
    trigger = TMath::Max(225,int(hist_fitParScurve_2->GetMean() + 3 * hist_fitParScurve_3->GetMean()));
  fout_scurves_chip_3sigma<<"reconfigure(\"skiroc_1_1_1_1_"<<(*aMapIter).first+1<<"\",\"set_gtrigger_skiroc\",str("<<trigger<<"))"<<endl;
    
  if(hist_fitParScurve_2->GetEntries()>20) 
    trigger = TMath::Max(225,int(hist_fitParScurve_2->GetMean() + 5 * hist_fitParScurve_3->GetMean()));
  fout_scurves_chip_5sigma<<"reconfigure(\"skiroc_1_1_1_1_"<<(*aMapIter).first+1<<"\",\"set_gtrigger_skiroc\",str("<<trigger<<"))"<<endl;
  
  if(hist_fitParScurve_2->GetEntries()>20) 
    trigger = TMath::Max(225, int(TMath::Max(hist_fitParScurve_2->GetMean() + 3 * hist_fitParScurve_3->GetMean(), hist_fitParScurve_1->GetMean())));
  fout_scurves_chip_3sigma_firstzero<<"reconfigure(\"skiroc_1_1_1_1_"<<(*aMapIter).first+1<<"\",\"set_gtrigger_skiroc\",str("<<trigger<<"))"<<endl;  
  
    if((*aMapIter).first+1 == globalvariables::getEnabledChipsVec().size() ) {
      fout_scurves_chip_3sigma<<"quit"<<endl;
      fout_scurves_chip_5sigma<<"quit"<<endl;
      fout_scurves_chip_3sigma_firstzero<<"quit"<<endl;
    }

  //Loop over all chips and display the maximum number of hits in a given channel
    
  if (helpMapIter != _maxHithelpVec.end()) {
    unsigned ichan(0);
    for (std::vector<Double_t>::iterator chanIter= (*helpMapIter).second.begin();chanIter!= (*helpMapIter).second.end() ; chanIter++) {
      std::cout << "ScanManager::sCurveAnalysisGraphicsPainter: maxcount Nchan: " << ichan << ", " << (*chanIter) << std::endl;
      ichan++;
    }
  }
 
  f_scurve->cd();
  c_chips->Write();
  g_fitParScurve_1->Write();
  g_fitParScurve_2->Write();
  g_fitParScurve_3->Write();
  // g_fitParErrScurve_1->Write();
  // g_fitParErrScurve_2->Write();
  // g_fitParErrScurve_3->Write();
  g_fitChisq->Write();

  hist_fitParScurve_1->Write();
  hist_fitParScurve_2->Write();
  hist_fitParScurve_3->Write();
  // hist_fitParErrScurve_1->Write();
  // hist_fitParErrScurve_2->Write();
  // hist_fitParErrScurve_3->Write();
  hist_fitChisq->Write();

  c_sigma->Update();
  c_mean->Update();

  if((*aMapIter).first == globalvariables::getEnabledChipsVec().size() - 1 ) {
    //c_sigma->Update();
    c_sigma->Write();
    //c_mean->Update();
    c_mean->Write();
  }
  
  f_scurve->Close();     
}



void ScanManager::planeEventsAnalysisGraphics(TString file_sufix) {
    
  TString gain = globalvariables::getGainTStringAnalysis();
  TString filerecreate = "RECREATE";
  TFile *f_planevent = TFile::Open(file_sufix+gain+".root", filerecreate);

  //Loop over all enabled chips
  for (channelInfoComplDouble_t::iterator mapiter = _ntrigChipVecMapHigh.begin();mapiter!=_ntrigChipVecMapHigh.end();mapiter++) {
    planeEventsAnalysisGraphicsPainter(mapiter,f_planevent);
  }

  f_planevent->Close();   

}

void ScanManager::planeEventsAnalysisGraphicsPainter(channelInfoComplDouble_t::iterator aMapIter, TFile* f_planevent) {
    
  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "Chip" << (*aMapIter).first ;//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,1200,600);
  c_chips->Divide(2,1);

  //Divide the canvas
  //A vector of graphs
  std::vector<TGraphErrors*> graphPlaneEvents;//vector of graphs... extract the proper graph
  std::vector<TGraphErrors*> graphPlaneEvents_norm;//vector of graphs... extract the proper graph                                                                                   
  channelInfoComplDouble_t::iterator helpMapIter = _ntrigChipVecMapHigh_norm.find((*aMapIter).first);

  std::cout << "ScanManager::planeEventsAnalysisGraphicsPainter: Chip: " << (*aMapIter).first << std::endl;
  //Loop over all channels
  unsigned ierror(0);
    
  c_chips->cd(1);

  for (std::vector<std::vector<Double_t> >::iterator chanVeciter=(*aMapIter).second.begin(); chanVeciter!=(*aMapIter).second.end(); chanVeciter++) {
    if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
      std::cout << "ScanManager::planEventsAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for bad event selectionselection" << ierror << std::endl;
      std::cout << "Size of vector with thresholds is: " << globalvariables::getScanVectorDoubles().size() << std::endl;
      std::cout << "Size of vector with readings is: " << (*chanVeciter).size()  << std::endl;
    }
    //If the maximum is >2 we assume that this channel was enabled and we process it further
    //Define an array that holds the entries for each run
    double valarray[ (*chanVeciter).size()];
    double evalarray[ (*chanVeciter).size()];
    double evalarray_x[ (*chanVeciter).size()];

    //Loop over the entries with trigger in the given runs
    //Helper variable to count runs
    unsigned irun(0);
    for (std::vector<Double_t>::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
      //Fetch the value for that run
      double runval=(static_cast<double>((*runIter)));
      //Fill the array with the relative counts for a given channel in a given run
      valarray[irun] = runval;
      evalarray[irun] = 0;
      evalarray_x[irun] = 0;///ref;
      irun++;
    }

    //Now define and fill a graph for each channel
    double vecarrayhelp[(*chanVeciter).size()];
    double* vecarray;
    vecarray = vectortoarray(globalvariables::getScanVectorDoubles(), &vecarrayhelp[0]);
    graphPlaneEvents.push_back(new TGraphErrors(globalvariables::getScanVectorDoubles().size(), vecarray, valarray,  evalarray_x, evalarray));
    graphPlaneEvents.back()->SetMarkerStyle(20+ierror);
    graphPlaneEvents.back()->SetMarkerColor(ierror+1);
    graphPlaneEvents.back()->SetLineColor(ierror+1);
    graphPlaneEvents.back()->SetMarkerSize(1.2);
    graphPlaneEvents.back()->SetName(TString::Format("Chip%i_SetError%i",int((*aMapIter).first),int(ierror)));
    graphPlaneEvents.back()->SetTitle(TString::Format("Chip%i, buffer>0",int((*aMapIter).first)));
    graphPlaneEvents.back()->Write();
    graphPlaneEvents.back()->GetXaxis()->SetTitle("Threshold");
    graphPlaneEvents.back()->GetYaxis()->SetTitle("N_hit");
    if(ierror==0) graphPlaneEvents.back()->Draw("APL");
    else graphPlaneEvents.back()->Draw("PL");
    ierror++;
  }
  TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->AddEntry(TString::Format("Chip%i_SetError0",int((*aMapIter).first)),"bcid[buf]-bcid[buf-1]==1","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError1",int((*aMapIter).first)),"bcid[buf]-bcid[buf-1]<=5 (>1)","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError2",int((*aMapIter).first)),"bcid[buf]-bcid[buf-1]<=15 (>5)","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError3",int((*aMapIter).first)),TString::Format("Nhits(chip)>%i",globalvariables::getPlaneEventsThreshold()),"lp");
  leg->AddEntry(TString::Format("Chip%i_SetError4",int((*aMapIter).first)),"Negative Data","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError5",int((*aMapIter).first)),"Total Bad Events","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError6",int((*aMapIter).first)),"All","lp");
  leg->Draw();

  //c_chips->Update();


  //-------------------------------------------------------
  ierror=0;
  c_chips->cd(2);
    
  for (std::vector<std::vector<Double_t> >::iterator chanVeciter=(*helpMapIter).second.begin(); chanVeciter!=(*helpMapIter).second.end(); chanVeciter++) {
    if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
      std::cout << "ScanManager::planEventsAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for bad event selectionselection" << ierror << std::endl;
      std::cout << "Size of vector with thresholds is: " << globalvariables::getScanVectorDoubles().size() << std::endl;
      std::cout << "Size of vector with readings is: " << (*chanVeciter).size()  << std::endl;
    }
    //If the maximum is >2 we assume that this channel was enabled and we process it further
    //Define an array that holds the entries for each run
    double valarray[ (*chanVeciter).size()];
    double evalarray[ (*chanVeciter).size()];
    double evalarray_x[ (*chanVeciter).size()];

    //Loop over the entries with trigger in the given runs
    //Helper variable to count runs
    unsigned irun(0);
    for (std::vector<Double_t>::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
      //Fetch the value for that run
      double runval(static_cast<double>((*runIter)));
      //Fill the array with the relative counts for a given channel in a given run
      valarray[irun] = runval;
      evalarray[irun] = 0;
      evalarray_x[irun] = 0;///ref;
      irun++;
    }

    //Now define and fill a graph for each channel
    double vecarrayhelp[(*chanVeciter).size()];
    double* vecarray;
    vecarray = vectortoarray(globalvariables::getScanVectorDoubles(), &vecarrayhelp[0]);
    graphPlaneEvents_norm.push_back(new TGraphErrors(globalvariables::getScanVectorDoubles().size(), vecarray, valarray,  evalarray_x, evalarray));
    graphPlaneEvents_norm.back()->SetMarkerStyle(20+ierror);
    graphPlaneEvents_norm.back()->SetMarkerColor(ierror+1);
    graphPlaneEvents_norm.back()->SetLineColor(ierror+1);
    graphPlaneEvents_norm.back()->SetMarkerSize(1.2);
    graphPlaneEvents_norm.back()->SetName(TString::Format("Chip%i_SetError%i",int((*helpMapIter).first),int(ierror)));
    graphPlaneEvents_norm.back()->SetTitle(TString::Format("Chip%i, buffer>0",int((*helpMapIter).first)));
    graphPlaneEvents_norm.back()->Write();
    graphPlaneEvents_norm.back()->GetXaxis()->SetTitle("Threshold");
    graphPlaneEvents_norm.back()->GetYaxis()->SetTitle("N_Badhit/N_All");
    if(ierror==0) graphPlaneEvents_norm.back()->Draw("APL");
    else graphPlaneEvents_norm.back()->Draw("PL");
    ierror++;
  }
  //  c_chips->Update();

  // c_chips->cd(0);
  TLegend *leg2 = new TLegend(0.6,0.7,0.9,0.9);
  leg2->AddEntry(TString::Format("Chip%i_SetError0",int((*helpMapIter).first)),"bcid[buf]-bcid[buf-1]==1","lp");
  leg2->AddEntry(TString::Format("Chip%i_SetError1",int((*helpMapIter).first)),"bcid[buf]-bcid[buf-1]<=5 (>1)","lp");
  leg2->AddEntry(TString::Format("Chip%i_SetError2",int((*helpMapIter).first)),"bcid[buf]-bcid[buf-1]<=10 (>5)","lp");
  leg2->AddEntry(TString::Format("Chip%i_SetError3",int((*helpMapIter).first)),TString::Format("Nhits(chip)>%i",globalvariables::getPlaneEventsThreshold()),"lp");
  leg2->AddEntry(TString::Format("Chip%i_SetError4",int((*helpMapIter).first)),"Negative Data","lp");
  leg2->AddEntry(TString::Format("Chip%i_SetError5",int((*helpMapIter).first)),"Total Bad Events","lp");
  leg2->AddEntry(TString::Format("Chip%i_SetError6",int((*helpMapIter).first)),"All","lp");
  leg2->Draw();

  c_chips->Update();


  f_planevent->cd();
  c_chips->Write();


}






double* ScanManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}

