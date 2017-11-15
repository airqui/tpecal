//
//  ScanManager.cpp
//  tpecalana_project
//
//  Created by Adrian Irles on November/2017
//  Copyright (c) 2015 CNRS. All rights reserved.
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
  if(globalvariables::getAnalysisType()=="holdscan") holdscanAnalysis(aExpSetup);

  _nRuns++;

}

void ScanManager::displayResults(TString file_prefix, int buffer){
  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  if(globalvariables::getAnalysisType()=="scurves") sCurveAnalysisGraphics(file_prefix,buffer);
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
	  ntrigmtmp+=trigmtmpvec.at(0); //good hits
	  ntrigmtmp+=trigmtmpvec.at(2); //bcid+1 original hit
	  //ntrigmtmp+=trigmtmpvec.at(3); //empty trigger
	  ntrigmtmp+=trigmtmpvec.at(4); //retrigger hit
          ntrigmtmp+=trigmtmpvec.at(5); //plane events                                                                                                                                                    
          //ntrigmtmp+=trigmtmpvec.at(6); //negative hits
	}//if
      }//ibuf
      
      //Add for each run the value in that channel
      //fills the triggers into a vector that is a part of a map of chips and channels

      double trig_per_spill = 0;
      if( nSpills > 10 )trig_per_spill = ntrigmtmp/nSpills;
      (*mapiter).second.at(ichan).push_back(trig_per_spill);
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
  fout_scurves_chip_6sigma.open(file_sufix+"_6sigma.cmd",ios::out);
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
  fout_scurves_chip_6sigma.close();
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
    TF1 * f0;
    TF1 * f;

    int gaussianfit=0;

    if(buffer!=0) f0= fit.FitScurveGauss(graphScurve.back());
    else f0= fit.FitScurve(graphScurve.back());

    if(f0->GetParameter(0) > 1000. || f0->GetParameter(1) > 1000. || f0->GetParameter(2) > 1000. || f0->GetParameter(0) < 0.5 || f0->GetParameter(1) < 150. || f0->GetParameter(2) < 1. ) {
      f= fit.FitScurveGauss(graphScurve.back());
      gaussianfit=1;
    } else {
      if(buffer!=0) {
	f= fit.FitScurveGauss(graphScurve.back());
	gaussianfit=1;
      }
      else f= fit.FitScurve(graphScurve.back());
    } 

    float first_zero = fit.FirstZero(graphScurve.back());
    
    Int_t pointID = g_fitParScurve_1->GetN();

    g_fitParScurve_1->SetPoint(pointID,ichan , first_zero  );
    g_fitParErrScurve_1->SetPoint(pointID,ichan , 0 );
    hist_fitParScurve_1->Fill( first_zero);
    hist_fitParErrScurve_1->Fill(0);

    //  if(f->GetParameter(1) > 160 ) {
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
      
      hist_fitParScurve_2->Fill(float(f->GetParameter(1)));
      hist_fitParScurve_3->Fill(float(f->GetParameter(2)));
      
      hist_fitParErrScurve_2->Fill(float(f->GetParError(1)));
      hist_fitParErrScurve_3->Fill(float(f->GetParError(2)));
      
      hist_fitChisq->Fill(chi2ndf);
      fout_scurves<<int((*aMapIter).first) << " " <<ichan << " ";
      fout_scurves<<std::fixed << std::setprecision(3) << f->GetParameter(1) << " " << f->GetParameter(2) <<  " " << chi2ndf <<" "<<first_zero<< " " << gaussianfit <<std::endl;
	
 
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
  g_fitParScurve_2->GetYaxis()->SetRangeUser(150,250);
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

  //c_chips->Update();

  c_mean->cd((*aMapIter).first+1);
  c_mean->Update();
  g_fitParScurve_2->SetTitle("Scurve mean, per chip");
  g_fitParScurve_2->GetYaxis()->SetTitle("");
  g_fitParScurve_2->GetYaxis()->SetRangeUser(100,250);
  g_fitParScurve_2->GetXaxis()->SetTitle("DAC");
  g_fitParScurve_2->Draw("AL");

  c_sigma->cd((*aMapIter).first+1);
  c_sigma->Update();
  g_fitParScurve_3->SetTitle("Scurve width, per chip");
  g_fitParScurve_3->GetYaxis()->SetTitle("");
  g_fitParScurve_3->GetYaxis()->SetRangeUser(0,50);
  g_fitParScurve_3->GetXaxis()->SetTitle("DAC");
  g_fitParScurve_3->Draw("AL");

  int trigger = 225;
  TString skiroc=globalvariables::getSkiroc();
  if(hist_fitParScurve_2->GetEntries()>20) { 
    trigger = TMath::Max(225,int(hist_fitParScurve_2->GetMean() + 3 * hist_fitParScurve_3->GetMean()));
    fout_scurves_chip_3sigma<<"reconfigure(\""<<skiroc<<"_"<<(*aMapIter).first+1<<"\",\"set_gtrigger_skiroc\",str("<<trigger<<"))"<<endl;
    
    trigger = TMath::Max(225,int(hist_fitParScurve_2->GetMean() + 6 * hist_fitParScurve_3->GetMean()));
    fout_scurves_chip_6sigma<<"reconfigure(\""<<skiroc<<"_"<<(*aMapIter).first+1<<"\",\"set_gtrigger_skiroc\",str("<<trigger<<"))"<<endl;
  
    trigger = TMath::Max(225,int(TMath::Max(hist_fitParScurve_2->GetMean() + 3 * hist_fitParScurve_3->GetMean(), hist_fitParScurve_1->GetMean())));
    fout_scurves_chip_3sigma_firstzero<<"reconfigure(\""<<skiroc<<"_"<<(*aMapIter).first+1<<"\",\"set_gtrigger_skiroc\",str("<<trigger<<"))"<<endl;  
  }
    
    if((*aMapIter).first+1 == globalvariables::getEnabledChipsVec().size() ) {
      fout_scurves_chip_3sigma<<"quit"<<endl;
      fout_scurves_chip_6sigma<<"quit"<<endl;
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

  // c_sigma->Update();
  //c_mean->Update();

  if((*aMapIter).first == globalvariables::getEnabledChipsVec().size() - 1 ) {
    //c_sigma->Update();
    c_sigma->Write();
    //c_mean->Update();
    c_mean->Write();
    c_mean->Print(file_sufix+"_scurves_mean.png");
    c_sigma->Print(file_sufix+"_scurves_width.png");
  }
  
  f_scurve->Close();     
}




double* ScanManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}

