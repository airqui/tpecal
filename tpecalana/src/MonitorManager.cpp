/*
 * MonitorManager.cpp
 *
 *  Created on: 5 Jan 2017
 *      Author: irles
 */

#include <MonitorManager.h>
#include "global.h"

using namespace globalvariables;

MonitorManager::~MonitorManager() {
  // TODO Auto-generated destructor stub
}

MonitorManager::MonitorManager() {
  // TODO Auto-generated constructor stub
  f_file=0;

  _ntrigVecMapHigh.clear();
  _bufferVecMapMedian.clear();
  _TrigChipChanneVecMap.clear();
  _TrigChipVec.clear(); //vector of unsigned

  _TrigChipVec_bcid1.clear(); //vector of unsigned
  _TrigChipVec_bcid5.clear(); //vector of unsigned
  _TrigChipVec_bcid10.clear(); //vector of unsigned
  _TrigChipVec_planeEvents.clear(); //vector of unsigned
  _TrigChipVec_negativeData.clear(); //vector of unsigned

}



void MonitorManager::init(){

  _ntrigVecMapHigh.clear();
  _bufferVecMapMedian.clear();
  _TrigChipChanneVecMap.clear();
  _TrigChipVec.clear(); //vector of unsigned
  _TrigChipVec_bcid1.clear(); //vector of unsigned
  _TrigChipVec_bcid5.clear(); //vector of unsigned
  _TrigChipVec_bcid10.clear(); //vector of unsigned
  _TrigChipVec_planeEvents.clear(); //vector of unsigned
  _TrigChipVec_negativeData.clear(); //vector of unsigned


  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
    //scurves maps (triggers for threshold run and maximum of triggers for all threshold runs, per channel and chip)
    _ntrigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<unsigned> >  () ));
    _bufferVecMapMedian.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));
    _TrigChipChanneVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
  }

}

void MonitorManager::acquireRunInformation(ExperimentalSetup* aExpSetup, TString type){
  std::cout << "************** MonitorManager::AcquireRunInformation new file: " << std::endl;
  simpleChannelAnalysis(aExpSetup, type);
}

void MonitorManager::displayResults(TString file_prefix, TString type){

  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  simpleChannelAnalysisGraphics(file_prefix, type);

}

void MonitorManager::simpleChannelAnalysis(ExperimentalSetup* aExpSetup, TString type) {

  //Loop over all enabled chips
  for (channelInfoComplUnsigned_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {
    std::cout << "------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNumberOfChannels());
    //total number of triggers per chip
    unsigned ntrigm(0);
    unsigned ntrigm_bcid1(0), ntrigm_bcid5(0), ntrigm_bcid10(0), ntrigm_planeevents(0), ntrigm_negativedata(0);

    for (unsigned ichan=0; ichan < numChans; ichan++) {

      unsigned ntrigm_chn(0);

      unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
      // we are going to save the total number of hits per chip
      for (unsigned ibuf=0; ibuf < bufdepth; ibuf++) {
	unsigned ntrigmtmp(0);
	//Build up the vector for the individual channels (needs to be done only once)
	if((*mapiter).second.size() < ichan+1)   (*mapiter).second.push_back (std::vector<unsigned> () );
	 

	//reads out the trigger of each channel
	if(type == "goodevents") {
	  ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers(ichan);
	  ntrigm+=ntrigmtmp;
	  ntrigm_bcid1+=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_consBcid1(ichan);
	  ntrigm_bcid5+=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_consBcid5(ichan);
	  ntrigm_bcid10+=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_consBcid10(ichan);
	  ntrigm_planeevents+=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_planeEvents(ichan);
	  ntrigm_negativedata+=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_negativeData(ichan);
	} else if(type == "bcid1") ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_consBcid1(ichan);
	else if(type == "bcid5") ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_consBcid5(ichan);
	else if(type == "bcid10") ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_consBcid10(ichan);
	else if(type == "planevents") ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_planeEvents(ichan);
	else if(type == "negativedata") ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers_negativeData(ichan);
	else std::cout<<" ERROR  ###################         Wrong analysis type: " << type << " != goodevents or bcid1 or bcid5 or bcid10 or planevents or negativedata  ##################"<<std::endl;

	//Add for each run the value in that channel
	//fills the triggers into a vector that is a part of a map of chips and channels
	for (unsigned itrig=0; itrig < ntrigmtmp; itrig++) (*mapiter).second.at(ichan).push_back(ibuf+0.25);

	//reads out the trigger of each channel
	ntrigm+=ntrigmtmp;
	ntrigm_chn+=ntrigmtmp;
      }

      //save the total number of triggers per channel
      _TrigChipChanneVecMap.at((*mapiter).first).push_back(ntrigm_chn);

      // save the maps of mean/median buffer triggered per channel and chip
      std::vector<unsigned> tempvect = (*mapiter).second.at(ichan);
      double median = GetMedian(tempvect);
      _bufferVecMapMedian.at((*mapiter).first).push_back(median);
      
    }

    // save the total number of triggers per chip;
    _TrigChipVec.push_back(ntrigm); 
    _TrigChipVec_bcid1.push_back(ntrigm_bcid1); 
    _TrigChipVec_bcid5.push_back(ntrigm_bcid5); 
    _TrigChipVec_bcid10.push_back(ntrigm_bcid10); 
    _TrigChipVec_planeEvents.push_back(ntrigm_planeevents); 
    _TrigChipVec_negativeData.push_back(ntrigm_negativedata); 

  }
}




void MonitorManager::simpleChannelAnalysisGraphics(TString file_sufix,TString type) {


  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "SimpleMonitoring_"<<type;//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,800,600);
  //Divide the canvas
  if(type == "goodevents") c_chips->Divide(2,2);
  else c_chips->Divide(3,1);

  //maps
  TH2F * bufferMedian = new TH2F("buffer_median","buffer_median",64,-0.5,63,16,-0.5,15.5);
  TH2F * TriggersPerChannel = new TH2F("triggers_per_channel","triggers_per_channel",64,-0.5,63,16,-0.5,15.5);

  // histogram of total number of triggers
  TH1F * Triggers = new TH1F("Triggers","Triggers",16,-0.5,15.5);
  TH1F * Triggers_bcid1 = new TH1F("Triggers_bcid1","Triggers_bcid1",16,-0.5,15.5);
  TH1F * Triggers_bcid5 = new TH1F("Triggers_bcid5","Triggers_bcid5",16,-0.5,15.5);
  TH1F * Triggers_bcid10 = new TH1F("Triggers_bcid10","Triggers_bcid10",16,-0.5,15.5);
  TH1F * Triggers_planeEvents = new TH1F("Triggers_planeEvents","Triggers_planeEvents",16,-0.5,15.5);
  TH1F * Triggers_negativeData = new TH1F("Triggers_negativeData","Triggers_negativeData",16,-0.5,15.5);


  //Loop over all enabled chips
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {

    Triggers->Fill(ichip,_TrigChipVec.at(ichip));
    if(type == "goodevents") {
      Triggers_bcid1->Fill(ichip,float(_TrigChipVec_bcid1.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_bcid5->Fill(ichip,float(_TrigChipVec_bcid5.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_bcid10->Fill(ichip,float(_TrigChipVec_bcid10.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_planeEvents->Fill(ichip,float(_TrigChipVec_planeEvents.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_negativeData->Fill(ichip,float(_TrigChipVec_negativeData.at(ichip))/float(_TrigChipVec.at(ichip)));
    }
    for(unsigned i=0; i<_bufferVecMapMedian.at(ichip).size(); i++ )  bufferMedian->Fill(i,double(ichip),_bufferVecMapMedian.at(ichip).at(i));
    for(int ichn=0; ichn<64; ichn++) TriggersPerChannel->Fill(ichn,ichip,_TrigChipChanneVecMap.at(ichip).at(ichn));

  }


  c_chips->cd(1);
  bufferMedian->SetTitle("median of buffer with triggered entries");
  bufferMedian->GetXaxis()->SetTitle("channel");
  bufferMedian->GetYaxis()->SetTitle("chip");
  bufferMedian->Draw("colz");
  c_chips->cd(2);
  Triggers->SetTitle("NHits total");
  Triggers->GetXaxis()->SetTitle("Chip");
  Triggers->GetYaxis()->SetTitle("Nhits");
  Triggers->Draw("h");
  c_chips->cd(3);
  TriggersPerChannel->SetTitle("Ntriggers per channel");
  TriggersPerChannel->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel->Draw("colz");
  if(type == "goodevents") {
    c_chips->cd(4);
    Triggers_bcid1->SetTitle("NHits total");
    Triggers_bcid1->GetXaxis()->SetTitle("Chip");
    Triggers_bcid1->GetYaxis()->SetTitle("Nhits");
    Triggers_bcid1->Draw("h");

    Triggers_bcid5->SetLineColor(2);
    Triggers_bcid5->SetLineWidth(2);
    Triggers_bcid5->Draw("hsame");

    Triggers_bcid10->SetLineColor(3);
    Triggers_bcid10->SetLineWidth(3);
    Triggers_bcid10->Draw("hsame");

    Triggers_planeEvents->SetLineStyle(2);
    Triggers_planeEvents->SetLineWidth(4);
    Triggers_planeEvents->SetLineColor(1);
    Triggers_planeEvents->Draw("hsame");

    Triggers_negativeData->SetLineStyle(2);
    Triggers_negativeData->SetLineWidth(4);
    Triggers_negativeData->SetLineColor(2);
    Triggers_negativeData->Draw("hsame");

    TLegend *leg = new TLegend(0.6,0.7,0.9,0.9);
    leg->AddEntry(Triggers_bcid1,"bcid[buf]-bcid[buf-1]==1","l");
    leg->AddEntry(Triggers_bcid5,"bcid[buf]-bcid[buf-1]<=5 (>1)","l");
    leg->AddEntry(Triggers_bcid10,"bcid[buf]-bcid[buf-1]<=10 (>5)","l");
    leg->AddEntry(Triggers_planeEvents,TString::Format("Nhits(chip)>%i",globalvariables::getPlaneEventsThreshold()),"l");
    leg->AddEntry(Triggers_negativeData,"Negative Data","l");
    leg->Draw();
  }



  c_chips->Update();

  TString gain = globalvariables::getGainTStringAnalysis();
  TString planevent = globalvariables::getPlaneEventsThresholdTStringAnalysis();

  TString filerecreate = "RECREATE";
  if(type != "goodevents") filerecreate = "UPDATE";
  TFile *f_scurve = TFile::Open(file_sufix+gain+planevent+"_monitor.root",filerecreate);
  TDirectory *dir = f_scurve->GetDirectory(type);
  if (!dir) dir = f_scurve->mkdir(type);

  f_scurve->cd();
  c_chips->Write();
  dir->cd();
  bufferMedian->Write();
  Triggers->Write();
  TriggersPerChannel->Write();
  if(type == "goodevents") {
    Triggers_bcid1->Write();
    Triggers_bcid5->Write();
    Triggers_bcid10->Write();
    Triggers_planeEvents->Write();
    Triggers_negativeData->Write();
  }

  f_scurve->Close();


}


double* MonitorManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}

Double_t MonitorManager::GetMean(std::vector<unsigned> vec){
  Double_t sum = 0;
  if(vec.size()>0) {
    for (unsigned k = 0; k<vec.size(); k++)	sum+=vec.at(k);
    Double_t average = sum/vec.size();
    return average;
  } else return 0;
}

Double_t MonitorManager::GetMedian(std::vector<unsigned> vec){

  Double_t median=0;
  if(vec.size()>0) {
    size_t size = vec.size();
    sort(vec.begin(), vec.end());
    if (size  % 2 == 0) median = (vec[size / 2 - 1] + vec[size / 2]) / 2;
    else  median = vec[size / 2];
  }
  return median;

}

Double_t MonitorManager::GetMode(std::vector<unsigned> vec){

  if(vec.size()>0 ) {
    unsigned number = vec.at(0);
    unsigned mode = number;
    unsigned count = 1;
    unsigned countMode = 1;

    for (unsigned i=1; i<vec.size(); i++){
      if (vec.at(i) == number)  count++;// count occurrences of the current number
      else { // now this is a different number
	if (count > countMode) {
	  countMode = count; // mode is the biggest ocurrences
	  mode = number;
	}
	count = 1; // reset count for the new number
	number = vec.at(i);
      }
    }
    return double(mode);
  } else return 0;

}
