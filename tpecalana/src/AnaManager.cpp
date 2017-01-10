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
  _sigVecMapHigh.clear();
  _sigVecMapLow.clear();
  _ntrigVecMapHigh.clear();
  _nRuns=0;
  f_scurve=0;
}

AnaManager::~AnaManager(){/* no op*/}

void AnaManager::init(){
    

  _nRuns=0;
  //to be removed
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
    //sigVecMapHigh.insert(std::make_pair<UInt_t, std::vector<Double_t> >(enabledChips[ichip],  std::vector<Double_t> () ));
    //sigVecMapLow.insert(std::make_pair<UInt_t, std::vector<Double_t> >(enabledChips[ichip],  std::vector<Double_t> () ));
    _sigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));
    //scurves maps (triggers for threshold run and maximum of triggers for all threshold runs, per channel and chip)
    _ntrigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<unsigned> >  () ));
    _maxHithelpVec.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned> () ));
  }
  
  //Reset the graph with the global run information
  _tGraphVec.clear();

  TString kind_analisys = globalvariables::getAnalysisType();
  std::cout<<"Init the Analysis Manager for general analysis, with getAnalysisType()="<<globalvariables::getAnalysisType()<<std::endl;

}

void AnaManager::acquireRunInformation(ExperimentalSetup* aExpSetup, int buffer){
  std::cout << "************** AcquireRunInformation new file: " << _nRuns << "***********************" << std::endl;
  //simpleChannelAnalysis(aExpSetup);
  sCurveAnalysis(aExpSetup, buffer);
  //deeperDataAnalysis(aExpSetup);

  _nRuns++;
}

void AnaManager::displayResults(TString file_prefix, int buffer){

  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  // simpleChannelAnalysisGraphics();
  sCurveAnalysisGraphics(file_prefix,buffer);
  //deeperDataAnalysisGraphics();

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
      //unsigned nentrtmp(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(buffer).getChannelEntries(ichan));
      //  for(int ibuf=0; ibuf<1; ibuf++) 
      unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
      if(buffer < 15) {
	ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(buffer).getChannelTriggers(ichan);
      }   else for(unsigned ibuf=0; ibuf<bufdepth; ibuf++) ntrigmtmp+=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers(ichan);

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

void AnaManager::simpleChannelAnalysis(ExperimentalSetup* aExpSetup) {

  // ADRIAN --> commented while changing the getChannelMean function for the pedestal studies

  // //Information for high gain
  // for (std::map<int,vector<double> >::iterator mapiter = _sigVecMapHigh.begin();mapiter!=_sigVecMapHigh.end();mapiter++)
  // {
  //     std::cout << "Enabled chips " << (*mapiter).first << std::endl;
  //     double ampval = aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getChannelMean(1,"High");
  //     double pedvaltmp(0);
  //     for(unsigned ichan=0; ichan < 64;ichan++) {
  //         if(ichan!=1) pedvaltmp += aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getChannelMean(0, "High");
  //     }
  //     double pedval(pedvaltmp/63.0);
  //     std::cout << "pedval: " << pedval << std::endl;
  //     (*mapiter).second.push_back(ampval-pedval);
  // }
  // //Information for low gain
  // for (std::map<int,vector<double> >::iterator mapiter = _sigVecMapLow.begin();mapiter!=_sigVecMapLow.end();mapiter++)
  // {
  //     std::cout << "Enabled chips " << (*mapiter).first << std::endl;
  //     double ampval = aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getChannelMean(1,"Low");
  //     double pedvaltmp(0);
  //     for(unsigned ichan=0; ichan < 64;ichan++) {
  //         if(ichan!=1) pedvaltmp += aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getChannelMean(0, "Low");
  //     }
  //     double pedval(pedvaltmp/63.0);
  //     std::cout << "pedval: " << pedval << std::endl;
  //     (*mapiter).second.push_back(ampval-pedval);
  // }

}


void AnaManager::deeperDataAnalysis(ExperimentalSetup* aExpSetup){
    
  std::cout << "Entering deeper analysis" << std::endl;
  //Programme to be worked down
  //accumulated number of hits as function of aquisition number per dif
  //for each run in the analysis
  _tGraphVec.push_back( std::vector<TGraph*> ()  );
  for (unsigned idif = 0; idif < aExpSetup->getNumberOfDifs(); idif++) {
    //Obviously we need a local copy of the vector for a safe access, let's think about it later
    std::vector<std::pair<unsigned, unsigned> > helpVec = aExpSetup->getDif(idif).getDifStatVec();
    unsigned nentr = helpVec.size();
    //unsigned nentr = aExpSetup->getDif(idif).getDifStatVec().size();
    double vecarray[nentr];
    double valarray[nentr];
    unsigned ientr(0);
    //Loop over the vector holding the run statistics for this dif and fill the arrays
    //for (std::vector<std::pair<unsigned, unsigned> >::iterator statveciter = aExpSetup->getDif(idif).getDifStatVec().begin(); statveciter != aExpSetup->getDif(idif).getDifStatVec().end(); statveciter++) {
    for (std::vector<std::pair<unsigned, unsigned> >::iterator statveciter = helpVec.begin(); statveciter != helpVec.end(); statveciter++) {
      //std::cout << "nentr: " << nentr << ", ientr: " << ientr << std::endl;
      //std::cout << "first: " << (*statveciter).first << ", second: " << (*statveciter).second << std::endl;
      vecarray[ientr] = static_cast<double>((*statveciter).first);
      valarray[ientr] = static_cast<double>((*statveciter).second);
      ientr++;
            
    }
    _tGraphVec.back().push_back(new TGraph(nentr, vecarray, valarray));
  }
    
  //xy hit map per DIF and ASU coloumn 0
  //xy hit map per DIF and ASU integrated over all coloumns
    
  //signals and pedestals per chip
    
  //Number of coloumns per ASU per chip
    
  //What else????
    
  //number of plane events per chip
}



void AnaManager::simpleChannelAnalysisGraphics() {
    
  //    double att[8] = {5, 10, 20, 50, 100, 200, 500, 1000};
  double att[23] = {170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214};//
  //Open a canvas for high gain
  TCanvas* c_cob1high = new TCanvas("c_cob1high","c_cob1high",1000,800);
  c_cob1high->cd();
  //fill some graphs with the signals
  std::vector<TGraph*> graphVecHigh;
  unsigned icount(0);
  for (std::map<int,vector<double> >::iterator mapiter = _sigVecMapHigh.begin();mapiter!=_sigVecMapHigh.end();mapiter++) {
    double vecarrayhelp[ (*mapiter).second.size()];
    double* vecarray;
    vecarray = vectortoarray((*mapiter).second, &vecarrayhelp[0]);
    graphVecHigh.push_back(new TGraph(_nRuns, att, vecarray));
    graphVecHigh.back()->SetMarkerStyle(20+icount);
    graphVecHigh.back()->SetMarkerSize(1.2);
    if(icount==0)  graphVecHigh.back()->Draw("AP");
    else graphVecHigh.back()->Draw("PS");
    icount++;
  }
    
    
  //Open a canvas for low gain
  TCanvas* c_cob1low = new TCanvas("c_cob1low","c_cob1low",1000,800);
  c_cob1low->cd();
  //fill some graphs with the signals
  std::vector<TGraph*> graphVecLow;
  icount=0;
  for (std::map<int,vector<double> >::iterator mapiter = _sigVecMapLow.begin();mapiter!=_sigVecMapLow.end();mapiter++) {
    double vecarrayhelp[ (*mapiter).second.size()];
    double* vecarray;
    vecarray = vectortoarray((*mapiter).second, &vecarrayhelp[0]);
    graphVecLow.push_back(new TGraph(_nRuns, att, vecarray));
    graphVecLow.back()->SetMarkerStyle(20+icount);
    graphVecLow.back()->SetMarkerSize(1.2);
    if(icount==0)  graphVecLow.back()->Draw("AP");
    else graphVecLow.back()->Draw("PS");
    icount++;
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
  c_chips->Divide(2,2);
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
  std::vector<unsigned> chanlistVec;
  chanlistVec.clear();
    
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
    if(ref>2)  { 
      chanlistVec.push_back(ichan);
      // std::cout << "Channel: " << ichan << " hits: " << ref << std::endl;
    }
    //Loop over the entries with trigger in the given runs
    //Helper variable to count runs
    unsigned irun(0);
    for (std::vector<unsigned>::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
      // std::cout << "AnaManager::sCurveAnalysisGraphicsPainter: Run " << irun << ": " << (*runIter) << std::endl;
      //Fetch the value for that run
      double runval(static_cast<double>((*runIter)));
      //Fill the array with the relative counts for a given channel in a given run
      if(ref > 2) {
	valarray[irun] = runval;///ref;
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

void AnaManager::deeperDataAnalysisGraphics() {
  displayRunStat();
}

void AnaManager::displayRunStat() {

  //Display the accumulated events for a given run
    
  for (TGraphVec_t::iterator tgraphveciter = _tGraphVec.begin(); tgraphveciter != _tGraphVec.end(); tgraphveciter++) {
    //Loop over the individual difs
    unsigned idif(0);
    std::cout << "Size of grpah vec: " << (*tgraphveciter).size() << std::endl;
    for (std::vector<TGraph*>::iterator graphiter = (*tgraphveciter).begin(); graphiter != (*tgraphveciter).end(); graphiter++) {
      std::stringstream canvasNameStr;
      canvasNameStr << "DIF" << idif;
      TCanvas* c_dif = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,1000,800);
      c_dif->cd();
      (*graphiter)->SetMarkerStyle(20+idif);
      (*graphiter)->SetMarkerSize(1.2);
      (*graphiter)->Draw("AP");
      idif++;
    }        
  }
    
  /*
    graphRunStat.push_back(new TGraph(nentr, vecarray, valarray));
    graphRunStat.back()->SetMarkerStyle(20+idif);
    graphRunStat.back()->SetMarkerSize(1.2);
    graphRunStat.back()->Draw("P");
  */
}

double* AnaManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}

