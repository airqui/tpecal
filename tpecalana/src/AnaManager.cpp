//
//  AnaManager.cpp
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
#include "AnaManager.h"
#include "FitGraphs.h"
#include "global.h"

using namespace globalvariables;

AnaManager::AnaManager(){
  _ntrigVecMapHigh.clear();
  _ntrigChipVecMapHigh_norm.clear();
  _ntrigChipVecMapHigh.clear();
  _nRuns=0;
  f_scurve=0;
}

AnaManager::~AnaManager(){/* no op*/}

void AnaManager::init(){
   
  _nRuns=0;
  //to be removed
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size(); ichip++) {
    _ntrigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<unsigned> >  () ));
    _maxHithelpVec.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned> () ));
    _ntrigChipVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> > () ));
    _ntrigChipVecMapHigh_norm.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> > () ));

  }
  
  std::cout<<"Init the Analysis Manager for general analysis, with getAnalysisType()="<<globalvariables::getAnalysisType()<<std::endl;

}

void AnaManager::acquireRunInformation(ExperimentalSetup* aExpSetup, int buffer){
  std::cout << "************** AcquireRunInformation new file: " << _nRuns << "***********************" << std::endl;
  if(globalvariables::getAnalysisType()=="scurves") sCurveAnalysis(aExpSetup, buffer);
  if(globalvariables::getAnalysisType()=="PlaneEventsScan") planeEventsAnalysis(aExpSetup);

  _nRuns++;

}

void AnaManager::displayResults(TString file_prefix, int buffer){
  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  if(globalvariables::getAnalysisType()=="scurves") sCurveAnalysisGraphics(file_prefix,buffer);
  if(globalvariables::getAnalysisType()=="PlaneEventsScan") planeEventsAnalysisGraphics(file_prefix);

}


void AnaManager::planeEventsAnalysis(ExperimentalSetup* aExpSetup) {
    
  //Loop over all enabled chips
  for (ChipInfoComplDouble_t::iterator mapiter = _ntrigChipVecMapHigh_norm.begin();mapiter!=_ntrigChipVecMapHigh_norm.end();mapiter++) {
    std::cout << "*************New chip: " << (*mapiter).first << "********************" << std::endl;

    ChipInfoComplDouble_t::iterator helpMapIter = _ntrigChipVecMapHigh.find((*mapiter).first);

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
      std::cout << "AnaManager::planeEventsAnalysis - chip " << (*mapiter).first << 
	" TriggersBad/TriggersOkay: " << double(ntrigm_total)/double(ntrigm) <<" TriggersBad= "<<ntrigm_total<<" TriggersOK="<<ntrigm<< std::endl;  
  }
}


void AnaManager::sCurveAnalysis(ExperimentalSetup* aExpSetup, int buffer) {
    
  //Loop over all enabled chips
  for (channelInfoComplUnsigned_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {
    std::cout << "*************New chip: " << (*mapiter).first << "********************" << std::endl;
    std::map<unsigned, std::vector<unsigned> >::iterator helpMapIter = _maxHithelpVec.find((*mapiter).first);
    unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNumberOfChannels());

    for (unsigned ichan=0; ichan < numChans; ichan++) {
      //Build up the vector for the individual channels (needs to be done only once)
      if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back (std::vector<unsigned> () );
      //reads out the trigger of each channel
      unsigned ntrigmtmp(0);

      int bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
      std::vector<unsigned> trigmtmpvec;
      for(int ibuf=0; ibuf<bufdepth; ibuf++){
	if(ibuf==buffer || buffer > 14 ) {
	  trigmtmpvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggersVec(ichan);
	  ntrigmtmp+=trigmtmpvec.at(0);
	  trigmtmpvec.clear();
	}
      }
      

      //Add for each run the value in that channel
      //fills the triggers into a vector that is a part of a map of chips and channels
      (*mapiter).second.at(ichan).push_back(ntrigmtmp);
      if(ntrigmtmp>0) std::cout << "AnaManager::sCurveAnalysis - chip " << (*mapiter).first << " Channel: " << ichan << " Triggers: " << ntrigmtmp << std::endl;
      //Store the maximum nummber of triggers
      if (helpMapIter != _maxHithelpVec.end()) {
  	if ((*helpMapIter).second.size()<ichan+1 ) (*helpMapIter).second.push_back(ntrigmtmp);
  	else {
  	  if (ntrigmtmp >  (*helpMapIter).second.at(ichan)) {
  	    (*helpMapIter).second.at(ichan) = ntrigmtmp;
  	  }
                    
  	}
      }          
    }      
  }
}


void AnaManager::sCurveAnalysisGraphics(TString file_sufix, int buffer) {
    
  //Loop over all enabled chips
  for (channelInfoComplUnsigned_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {
    sCurveAnalysisGraphicsPainter(mapiter,file_sufix, buffer);
  }
}


void AnaManager::sCurveAnalysisGraphicsPainter(channelInfoComplUnsigned_t::iterator aMapIter, TString file_sufix, int buffer) {
    

  TString gain = globalvariables::getGainTStringAnalysis();
  TString filerecreate = "RECREATE";
  if((*aMapIter).first  > 0) filerecreate="UPDATE";
  TFile *f_scurve = TFile::Open(file_sufix+gain+".root", filerecreate);
  TDirectory *dir = f_scurve->GetDirectory(TString::Format("scurves_graphs_buffer%i",buffer));
  if (!dir) dir = f_scurve->mkdir(TString::Format("scurves_graphs_buffer%i",buffer));

  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "Chip" << (*aMapIter).first << "_buffer" << buffer;//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,1000,800);
  //Divide the canvas
  //Helper variable to allow to switch to another pad in the canvas
  unsigned ipad(0);
  //A vector of graphs
  std::vector<TGraphErrors*> graphScurve;//vector of graphs... extract the proper graph
    
  TH2F * fitParScurve_1 = new TH2F(TString::Format("Chip%i_fitParScurve_1",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_1",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);
  TH2F * fitParScurve_2 = new TH2F(TString::Format("Chip%i_fitParScurve_2",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_2",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);
  TH2F * fitParScurve_3 = new TH2F(TString::Format("Chip%i_fitParScurve_3",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_3",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);
  TH2F * fitParScurve_4 = new TH2F(TString::Format("Chip%i_fitParScurve_4",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_4",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);
 
  TH2F * fitParErrScurve_1 = new TH2F(TString::Format("Chip%i_fitParErrScurve_1",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_1",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);
  TH2F * fitParErrScurve_2 = new TH2F(TString::Format("Chip%i_fitParErrScurve_2",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_2",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);
  TH2F * fitParErrScurve_3 = new TH2F(TString::Format("Chip%i_fitParErrScurve_3",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_3",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);
  TH2F * fitParErrScurve_4 = new TH2F(TString::Format("Chip%i_fitParErrScurve_4",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_4",int((*aMapIter).first)),15,-0.5,14.5,64,-0.5,63.4);


  TH1F * hist_fitParScurve_1 = new TH1F(TString::Format("hist_Chip%i_fitParScurve_1",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_1",int((*aMapIter).first)),100,0,2);
  TH1F * hist_fitParScurve_2 = new TH1F(TString::Format("hist_Chip%i_fitParScurve_2",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_2",int((*aMapIter).first)),200,150,250);
  TH1F * hist_fitParScurve_3 = new TH1F(TString::Format("hist_Chip%i_fitParScurve_3",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_3",int((*aMapIter).first)),100,0,20);
  TH1F * hist_fitParScurve_4 = new TH1F(TString::Format("hist_Chip%i_fitParScurve_4",int((*aMapIter).first)),TString::Format("Chip%i_fitParScurve_4",int((*aMapIter).first)),100,-2,2);
 
  TH1F * hist_fitParErrScurve_1 = new TH1F(TString::Format("hist_Chip%i_fitParErrScurve_1",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_1",int((*aMapIter).first)),100,0,0.5);
  TH1F * hist_fitParErrScurve_2 = new TH1F(TString::Format("hist_Chip%i_fitParErrScurve_2",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_2",int((*aMapIter).first)),100,0,5);
  TH1F * hist_fitParErrScurve_3 = new TH1F(TString::Format("hist_Chip%i_fitParErrScurve_3",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_3",int((*aMapIter).first)),100,0,5);
  TH1F * hist_fitParErrScurve_4 = new TH1F(TString::Format("hist_Chip%i_fitParErrScurve_4",int((*aMapIter).first)),TString::Format("Chip%i_fitParErrScurve_4",int((*aMapIter).first)),200,0,1);

  //An iterator used to fetch the maximum number of counts
  std::map<unsigned, std::vector<unsigned> >::iterator helpMapIter = _maxHithelpVec.find((*aMapIter).first);
  //Loopover all chips
  std::cout << "AnaManager::sCurveAnalysisGraphicsPainter: Chip: " << (*aMapIter).first << std::endl;
  //Loop over all channels
  //Helper variable to count channels
  unsigned ichan(0);
    
  for (std::vector<std::vector<unsigned> >::iterator chanVeciter=(*aMapIter).second.begin(); chanVeciter!=(*aMapIter).second.end(); chanVeciter++) {
    std::cout << "AnaManager::sCurveAnalysisGraphicsPainter: Nchan: " << ichan << std::endl;
    //At least as a sanity check we verify that the size of the vector for wach channel corresponds to the size of the
    //vector holding the thresholds as defined above
    if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
      std::cout << "AnaManager::sCurveAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for channel" << ichan << std::endl;
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
    double ref(static_cast<double>((*helpMapIter).second.at(ichan)));

    //Loop over the entries with trigger in the given runs
    //Helper variable to count runs
    unsigned irun(0);
    for (std::vector<unsigned>::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
      // std::cout << "AnaManager::sCurveAnalysisGraphicsPainter: Run " << irun << ": " << (*runIter) << std::endl;
      //Fetch the value for that run
      double runval(static_cast<double>((*runIter)));
      //Fill the array with the relative counts for a given channel in a given run
      if(ref > 2) {
	valarray[irun] = runval/ref;
	evalarray[irun] = sqrt(runval)/ref;
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
    graphScurve.back()->SetMarkerSize(1.2);
    if (ichan%16 == 0) {
      c_chips->cd(ipad+1);
      ipad++;
      graphScurve.back()->SetName(TString::Format("Chip%i_chn%i",int((*aMapIter).first),int(ichan)));
      graphScurve.back()->Write();
      graphScurve.back()->GetXaxis()->SetTitle("Threshold");
      graphScurve.back()->GetYaxis()->SetTitle("Nhit/Nref");
      if(ref>10) graphScurve.back()->Draw("APSL");
    } else {
      graphScurve.back()->SetName(TString::Format("Chip%i_chn%i",int((*aMapIter).first),int(ichan)));
      graphScurve.back()->Write();
      if(ref > 10) graphScurve.back()->Draw("PSL");
    }
    FitGraphs fit;
    TF1 * f = fit.FitScurve(graphScurve.back());
    fitParScurve_1->Fill(buffer,ichan,f->GetParameter(0));
    fitParScurve_2->Fill(buffer,ichan,f->GetParameter(1));
    fitParScurve_3->Fill(buffer,ichan,f->GetParameter(2));
    fitParScurve_4->Fill(buffer,ichan,f->GetParameter(3));

    fitParErrScurve_1->Fill(buffer,ichan,f->GetParError(0));
    fitParErrScurve_2->Fill(buffer,ichan,f->GetParError(1));
    fitParErrScurve_3->Fill(buffer,ichan,f->GetParError(2));
    fitParErrScurve_4->Fill(buffer,ichan,f->GetParError(3));

    hist_fitParScurve_1->Fill(f->GetParameter(0));
    hist_fitParScurve_2->Fill(f->GetParameter(1));
    hist_fitParScurve_3->Fill(f->GetParameter(2));
    hist_fitParScurve_4->Fill(f->GetParameter(3));

    hist_fitParErrScurve_1->Fill(f->GetParError(0));
    hist_fitParErrScurve_2->Fill(f->GetParError(1));
    hist_fitParErrScurve_3->Fill(f->GetParError(2));
    hist_fitParErrScurve_4->Fill(f->GetParError(3));

    /*
    //For debugging purposes we maintain the possibility to display the values in a simple loop
    //Helper variable to count runs
    unsigned irun(0);
    for (std::vector<unsigned>::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
    std::cout << "AnaManager::sCurveAnalysisGraphicsPainter: Run " << irun << ": " << (*runIter) << std::endl;
    irun++;
    }
    */
    ichan++;
  }

  //Loop over all chips and display the maximum number of hits in a given channel
    
  if (helpMapIter != _maxHithelpVec.end()) {
    unsigned ichan(0);
    for (std::vector<unsigned>::iterator chanIter= (*helpMapIter).second.begin();chanIter!= (*helpMapIter).second.end() ; chanIter++) {
      std::cout << "AnaManager::sCurveAnalysisGraphicsPainter: maxcount Nchan: " << ichan << ", " << (*chanIter) << std::endl;
      ichan++;
    }
  }
 
  f_scurve->cd();
  c_chips->Write();
  fitParScurve_1->Write();
  fitParScurve_2->Write();
  fitParScurve_3->Write();
  fitParScurve_4->Write();
  
  fitParErrScurve_1->Write();
  fitParErrScurve_2->Write();
  fitParErrScurve_3->Write();
  fitParErrScurve_4->Write();

  hist_fitParScurve_1->Write();
  hist_fitParScurve_2->Write();
  hist_fitParScurve_3->Write();
  hist_fitParScurve_4->Write();
  
  hist_fitParErrScurve_1->Write();
  hist_fitParErrScurve_2->Write();
  hist_fitParErrScurve_3->Write();
  hist_fitParErrScurve_4->Write();
  f_scurve->Close();     
}



void AnaManager::planeEventsAnalysisGraphics(TString file_sufix) {
    
  TString gain = globalvariables::getGainTStringAnalysis();
  TString filerecreate = "RECREATE";
  TFile *f_planevent = TFile::Open(file_sufix+gain+".root", filerecreate);

  //Loop over all enabled chips
  for (ChipInfoComplDouble_t::iterator mapiter = _ntrigChipVecMapHigh.begin();mapiter!=_ntrigChipVecMapHigh.end();mapiter++) {
    planeEventsAnalysisGraphicsPainter(mapiter,f_planevent);
  }

  f_planevent->Close();   

}

void AnaManager::planeEventsAnalysisGraphicsPainter(ChipInfoComplDouble_t::iterator aMapIter, TFile* f_planevent) {
    
  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "Chip" << (*aMapIter).first ;//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,1200,600);
  c_chips->Divide(2,1);

  //Divide the canvas
  //A vector of graphs
  std::vector<TGraphErrors*> graphPlaneEvents;//vector of graphs... extract the proper graph
  std::vector<TGraphErrors*> graphPlaneEvents_norm;//vector of graphs... extract the proper graph                                                                                   
  ChipInfoComplDouble_t::iterator helpMapIter = _ntrigChipVecMapHigh_norm.find((*aMapIter).first);

  std::cout << "AnaManager::planeEventsAnalysisGraphicsPainter: Chip: " << (*aMapIter).first << std::endl;
  //Loop over all channels
  unsigned ierror(0);
    
  c_chips->cd(1);

  for (std::vector<std::vector<Double_t> >::iterator chanVeciter=(*aMapIter).second.begin(); chanVeciter!=(*aMapIter).second.end(); chanVeciter++) {
     if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
      std::cout << "AnaManager::planEventsAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for bad event selectionselection" << ierror << std::endl;
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
  leg->AddEntry(TString::Format("Chip%i_SetError2",int((*aMapIter).first)),"bcid[buf]-bcid[buf-1]<=10 (>5)","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError3",int((*aMapIter).first)),TString::Format("Nhits(chip)>%i",globalvariables::getPlaneEventsThreshold()),"lp");
  leg->AddEntry(TString::Format("Chip%i_SetError4",int((*aMapIter).first)),"Negative Data","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError5",int((*aMapIter).first)),"Total Bad Events","lp");
  leg->AddEntry(TString::Format("Chip%i_SetError6",int((*aMapIter).first)),"Ok","lp");
  leg->Draw();

  //c_chips->Update();


  //-------------------------------------------------------
  ierror=0;
  c_chips->cd(2);
    
  for (std::vector<std::vector<Double_t> >::iterator chanVeciter=(*helpMapIter).second.begin(); chanVeciter!=(*helpMapIter).second.end(); chanVeciter++) {
     if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
      std::cout << "AnaManager::planEventsAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for bad event selectionselection" << ierror << std::endl;
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
    graphPlaneEvents_norm.back()->GetYaxis()->SetTitle("N_Badhit/N_Ok");
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
  leg2->AddEntry(TString::Format("Chip%i_SetError6",int((*helpMapIter).first)),"Ok","lp");
  leg2->Draw();

  c_chips->Update();


  f_planevent->cd();
  c_chips->Write();


}






double* AnaManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}

