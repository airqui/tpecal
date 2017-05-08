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
  
  // CHANNEL
  // ----------------------------------------
  _ntrigVecMapHigh.clear();
  _bufferVecMapMedian.clear();
  
  _TrigChipChannelVecMap.clear();
  _PedChipChannelVecMap.clear();
  _TrigChipChannelVecMap_buf0.clear();
  _PedChipChannelVecMap_buf0.clear();
  
  _ErrorsChipChannelVecMap.clear();
  
  _PedWidthChipChannelVecMap.clear();
  _SignalWidthChipChannelVecMap.clear();
  _PedMeanChipChannelVecMap.clear();
  _SignalMeanChipChannelVecMap.clear();
  
  _TrigChipVec.clear(); //vector of unsigned
  _TrigChipVec_bcid1.clear(); //vector of unsigned
  _TrigChipVec_bcid5.clear(); //vector of unsigned
  _TrigChipVec_bcid10.clear(); //vector of unsigned
  _TrigChipVec_planeEvents.clear(); //vector of unsigned
  _TrigChipVec_negativeData.clear(); //vector of unsigned


  // CHIP
  // ----------------------------------------
  _NhitsChipMap.clear();
  _NhitsChipMap_buf0.clear();

  _BcidChipMap.clear();
  _BcidChipMap_buf0.clear();
  //-
  _RetrigNhitsChipMap.clear();
  _RetrigNhitsChipMap_buf0.clear();

  _RetrigBcidChipMap.clear();
  _RetrigBcidChipMap_buf0.clear();
  //-
  _NegativeNhitsChipMap.clear();
  _NegativeNhitsChipMap_buf0.clear();

  _NegativeBcidChipMap.clear();
  _NegativeBcidChipMap_buf0.clear();

}



void MonitorManager::init(){


  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
    //scurves maps (triggers for threshold run and maximum of triggers for all threshold runs, per channel and chip)
    _ntrigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> >  () ));

    _bufferVecMapMedian.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));

    _TrigChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    
    _ErrorsChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    
    _TrigChipChannelVecMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    
    _PedWidthChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));
    _SignalWidthChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));
    _PedMeanChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));
    _SignalMeanChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));

  }

  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
    //chip maps
    _NhitsChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _NhitsChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));

    _BcidChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _BcidChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    //--
    _RetrigNhitsChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _RetrigNhitsChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));

    _RetrigBcidChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _RetrigBcidChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    //--
    _NegativeNhitsChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _NegativeNhitsChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));

    _NegativeBcidChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _NegativeBcidChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
  }

}

void MonitorManager::acquireRunInformation(ExperimentalSetup* aExpSetup, TString type){
  std::cout << "************** MonitorManager::AcquireRunInformation new file: " << std::endl;
  if(type == "channel" ) simpleChannelAnalysis(aExpSetup);
  if(type == "chip" ) simpleChipAnalysis(aExpSetup);

}

void MonitorManager::displayResults(TString file_prefix, TString type){

  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  if(type == "channel" ) simpleChannelAnalysisGraphics(file_prefix);
  if(type == "chip" ) simpleChipAnalysisGraphics(file_prefix);

}

void MonitorManager::simpleChannelAnalysis(ExperimentalSetup* aExpSetup) {

  //Loop over all enabled chips
  for (channelInfoComplUnsigned_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {
    std::cout << "------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNumberOfChannels());
    //total number of triggers per chip
    unsigned ntrigm(0);
    unsigned ntrigm_bcid1(0), ntrigm_bcid5(0), ntrigm_bcid10(0), ntrigm_planeevents(0), ntrigm_negativedata(0);

    for (unsigned ichan=0; ichan < numChans; ichan++) {

      unsigned ntrigm_chn_buf0(0);
      unsigned ntrigm_chn(0);
      unsigned npedm_chn_buf0(0);
      unsigned npedm_chn(0);

      unsigned nerrorsm_chn(0);

      unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
      // we are going to save the total number of hits per chip
      for (unsigned ibuf=0; ibuf < bufdepth; ibuf++) {

	if(ibuf == 0 ) {
	  //readout the pedestal and signal RMS
	  std::vector<double> width=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getChannelRMSVec(ichan, "High");
	  double signwidth=width.at(0);
	  double pedwidth=width.at(1);
	  _SignalWidthChipChannelVecMap.at((*mapiter).first).push_back(signwidth);
	  _PedWidthChipChannelVecMap.at((*mapiter).first).push_back(pedwidth);
	  
	  std::vector<double> mean=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getChannelMeanVec(ichan, "High");
	  double signmean=mean.at(0);
	  double pedmean=mean.at(1);
	  _SignalMeanChipChannelVecMap.at((*mapiter).first).push_back(signmean);
	  _PedMeanChipChannelVecMap.at((*mapiter).first).push_back(pedmean);
	}

	unsigned ntrigmtmp(0);
	unsigned npedmtmp(0);

	//Build up the vector for the individual channels (needs to be done only once)
	if((*mapiter).second.size() < ichan+1)   (*mapiter).second.push_back (std::vector<Double_t> () );
	 
	//reads out the trigger of each channel
	std::vector<unsigned> ntrigtmpVect = aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggersVec(ichan);

	//reads out the trigger of each channel
	ntrigmtmp=ntrigtmpVect.at(0);
	npedmtmp=ntrigtmpVect.at(1);
	ntrigm_bcid1+=ntrigtmpVect.at(2);
	ntrigm_bcid5+=ntrigtmpVect.at(3);
	ntrigm_bcid10+=ntrigtmpVect.at(4);
	ntrigm_planeevents+=ntrigtmpVect.at(5);
	ntrigm_negativedata+=ntrigtmpVect.at(6);

	//Add for each run the value in that channel
	//fills the triggers into a vector that is a part of a map of chips and channels
	for (unsigned itrig=0; itrig < ntrigmtmp; itrig++) (*mapiter).second.at(ichan).push_back(ibuf+0.25);

	//reads out the trigger of each channel
	ntrigm+=ntrigmtmp;
	if(ibuf==0) {
	  ntrigm_chn_buf0=ntrigmtmp;
	  npedm_chn_buf0=npedmtmp;
	  nerrorsm_chn=ntrigtmpVect.at(2);//+ntrigtmpVect.at(3)+ntrigtmpVect.at(4);//irles !! CHECK IT!!
	} else {
	  ntrigm_chn+=ntrigmtmp;
	  npedm_chn+=npedmtmp;
	  nerrorsm_chn+=ntrigtmpVect.at(2);//+ntrigtmpVect.at(3)+ntrigtmpVect.at(4);//irles !! CHECK IT!!
	}

      }

      //save the total number of triggers per channel
      _TrigChipChannelVecMap.at((*mapiter).first).push_back(ntrigm_chn);
      _PedChipChannelVecMap.at((*mapiter).first).push_back(npedm_chn);

      _ErrorsChipChannelVecMap.at((*mapiter).first).push_back(nerrorsm_chn);

      //save the total number of triggers per channel
      _TrigChipChannelVecMap_buf0.at((*mapiter).first).push_back(ntrigm_chn_buf0);
      _PedChipChannelVecMap_buf0.at((*mapiter).first).push_back(npedm_chn_buf0);


      // save the maps of mean/median SCA triggered per channel and chip
      std::vector<Double_t> tempvect = (*mapiter).second.at(ichan);
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



//
void MonitorManager::simpleChannelAnalysisGraphics(TString file_sufix) {


  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "SimpleChannelMonitoring";//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,1400,800);
  //Divide the canvas
  c_chips->Divide(4,3);

  //maps
  TH2F * bufferMedian = new TH2F("buffer_median","buffer_median",64,-0.5,63.5,16,-0.5,15.5);

  TH2F * ErrorsPerChannel = new TH2F("erros_per_channel","errors_per_channel",64,-0.5,63.5,16,-0.5,15.5);

  TH2F * TriggersPerChannel = new TH2F("triggers_per_channel","triggers_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * TriggersPerChannel_buf0 = new TH2F("triggers_per_channel_buf0","triggers_per_channel_buf0",64,-0.5,63.5,16,-0.5,15.5);

  TH2F * HitsPedestalPerChannel = new TH2F("ratio_hitpedestal_per_channel","ratio_hitpedestal_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * HitsPedestalPerChannel_buf0 = new TH2F("ratio_hitpedestal_per_channel_buf0","ratio_hitpedestal_per_channel_buf0",64,-0.5,63.5,16,-0.5,15.5);

  TH2F * PedWidthPerChannel = new TH2F("PedWidth_per_channel","PedWidth_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * SignalWidthPerChannel = new TH2F("SignalWidth_per_channel","SignalWidth_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * PedMeanPerChannel = new TH2F("PedMean_per_channel","PedMean_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * SignalMeanPerChannel = new TH2F("SignalMean_per_channel","SignalMean_per_channel",64,-0.5,63.5,16,-0.5,15.5);

  // histogram of total number of triggers
  TH1F * Triggers = new TH1F("Triggers","Triggers",16,-0.5,15.5);

  TH1F * Triggers_bcid1 = new TH1F("Triggers_bcid1","Triggers_bcid1",16,-0.5,15.5);
  TH1F * Triggers_bcid5 = new TH1F("Triggers_bcid5","Triggers_bcid5",16,-0.5,15.5);
  TH1F * Triggers_bcid10 = new TH1F("Triggers_bcid10","Triggers_bcid10",16,-0.5,15.5);
  TH1F * Triggers_planeEvents = new TH1F("Triggers_planeEvents","Triggers_planeEvents",16,-0.5,15.5);
  TH1F * Triggers_negativeData = new TH1F("Triggers_negativeData","Triggers_negativeData",16,-0.5,15.5);


  //Loop over all enabled chips
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
    // Fill chip histograms
    Triggers->Fill(ichip,_TrigChipVec.at(ichip));
    if(_TrigChipVec.at(ichip)>0) {
      Triggers_bcid1->Fill(ichip,float(_TrigChipVec_bcid1.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_bcid5->Fill(ichip,float(_TrigChipVec_bcid5.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_bcid10->Fill(ichip,float(_TrigChipVec_bcid10.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_planeEvents->Fill(ichip,float(_TrigChipVec_planeEvents.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_negativeData->Fill(ichip,float(_TrigChipVec_negativeData.at(ichip))/float(_TrigChipVec.at(ichip)));

      cout<<" chip "<< ichip<< " " <<_TrigChipVec_bcid1.at(ichip) << " " << _TrigChipVec.at(ichip)<< endl;
      
    } 
    

    //for(unsigned i=0; i<_bufferVecMapMedian.at(ichip).size(); i++ )  bufferMedian->Fill(i,double(ichip),_bufferVecMapMedian.at(ichip).at(i));

    // fill th2  (channel loop)
    for(int ichn=0; ichn<64; ichn++) {
      bufferMedian->Fill(ichn,double(ichip),_bufferVecMapMedian.at(ichip).at(ichn));

      float hitnoise=0;
      if(_PedChipChannelVecMap.at(ichip).at(ichn)>0 ) hitnoise = float(_TrigChipChannelVecMap.at(ichip).at(ichn))/float(_PedChipChannelVecMap.at(ichip).at(ichn));
      else if(_TrigChipChannelVecMap.at(ichip).at(ichn)>0 ) hitnoise=1;
      else hitnoise=0;
      HitsPedestalPerChannel->Fill(ichn,ichip,hitnoise);

      if(_PedChipChannelVecMap_buf0.at(ichip).at(ichn)>0 ) hitnoise = float(_TrigChipChannelVecMap_buf0.at(ichip).at(ichn))/float(_PedChipChannelVecMap_buf0.at(ichip).at(ichn));
      else if(_TrigChipChannelVecMap_buf0.at(ichip).at(ichn)>0 ) hitnoise=1;
      else hitnoise=0;
      HitsPedestalPerChannel_buf0->Fill(ichn,ichip,hitnoise);

      TriggersPerChannel->Fill(ichn,ichip,_TrigChipChannelVecMap.at(ichip).at(ichn));
      TriggersPerChannel_buf0->Fill(ichn,ichip,_TrigChipChannelVecMap_buf0.at(ichip).at(ichn));

      PedWidthPerChannel->Fill(ichn,ichip,_PedWidthChipChannelVecMap.at(ichip).at(ichn));
      SignalWidthPerChannel->Fill(ichn,ichip,_SignalWidthChipChannelVecMap.at(ichip).at(ichn));
      PedMeanPerChannel->Fill(ichn,ichip,_PedMeanChipChannelVecMap.at(ichip).at(ichn));
      SignalMeanPerChannel->Fill(ichn,ichip,_SignalMeanChipChannelVecMap.at(ichip).at(ichn));

      ErrorsPerChannel->Fill(ichn,ichip,float(_ErrorsChipChannelVecMap.at(ichip).at(ichn)));

    }

  }


  c_chips->cd(1);
  bufferMedian->SetTitle("median of SCA with triggered&filtered entries");
  bufferMedian->GetXaxis()->SetTitle("channel");
  bufferMedian->GetYaxis()->SetTitle("chip");
  bufferMedian->GetZaxis()->SetRangeUser(0,16);
  bufferMedian->Draw("colz");

  c_chips->cd(2);
  Triggers->SetTitle("NHits total (filtered)");
  Triggers->GetXaxis()->SetTitle("Chip");
  Triggers->GetYaxis()->SetTitle("Nhits");
  Triggers->GetYaxis()->SetRangeUser(0,Triggers->GetMaximum()*1.2);
  Triggers->Draw("h");

  c_chips->cd(3);
  Triggers_bcid1->SetTitle("NHits / Nhits (filtered)");
  Triggers_bcid1->GetXaxis()->SetTitle("Chip");
  Triggers_bcid1->GetYaxis()->SetTitle("Nhits");
  Triggers_bcid1->GetYaxis()->SetRangeUser(0,Triggers_bcid1->GetMaximum()*1.2);
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
  leg->AddEntry(Triggers_bcid10,"bcid[buf]-bcid[buf-1]<=15 (>5)","l");
  leg->AddEntry(Triggers_planeEvents,TString::Format("Nhits(chip)>%i",globalvariables::getPlaneEventsThreshold()),"l");
  leg->AddEntry(Triggers_negativeData,"Negative Data","l");
  leg->Draw();

  c_chips->cd(4);
  ErrorsPerChannel->SetTitle("Negative hits per channel, all SCA");
  ErrorsPerChannel->GetXaxis()->SetTitle("Channel");
  ErrorsPerChannel->GetYaxis()->SetTitle("Chip");
  ErrorsPerChannel->Draw("colz");

  c_chips->cd(5);
  TriggersPerChannel_buf0->SetTitle("NHits (filtered) per channel, SCA=0");
  TriggersPerChannel_buf0->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_buf0->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_buf0->Draw("colz");

  c_chips->cd(6);
  TriggersPerChannel->SetTitle("NHits (filtered) per channel, SCA>0");
  TriggersPerChannel->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel->Draw("colz");


  c_chips->cd(7);
  HitsPedestalPerChannel_buf0->SetTitle("NHits/Npedestals (filtered), SCA=0");
  HitsPedestalPerChannel_buf0->GetXaxis()->SetTitle("Chip");
  HitsPedestalPerChannel_buf0->GetYaxis()->SetTitle("Channel");
  HitsPedestalPerChannel_buf0->Draw("colz");

  c_chips->cd(8);
  HitsPedestalPerChannel->SetTitle("NHits/Npedestals (filtered), SCA>0");
  HitsPedestalPerChannel->GetXaxis()->SetTitle("Chip");
  HitsPedestalPerChannel->GetYaxis()->SetTitle("Channel");
  HitsPedestalPerChannel->Draw("colz");


  c_chips->cd(9);
  PedWidthPerChannel->SetTitle("PedestalWidth (filtered) per channel, SCA=0");
  PedWidthPerChannel->GetXaxis()->SetTitle("Channel");
  PedWidthPerChannel->GetYaxis()->SetTitle("Chip");
  PedWidthPerChannel->GetZaxis()->SetRangeUser(0,30);
  PedWidthPerChannel->Draw("colz");
  c_chips->cd(10);
  PedMeanPerChannel->SetTitle("PedestalMean (filtered) per channel, SCA=0");
  PedMeanPerChannel->GetXaxis()->SetTitle("Channel");
  PedMeanPerChannel->GetYaxis()->SetTitle("Chip");
  PedMeanPerChannel->GetZaxis()->SetRangeUser(0,500);
  PedMeanPerChannel->Draw("colz");

  c_chips->cd(11);
  SignalWidthPerChannel->SetTitle("SignallWidth (filtered) per channel, SCA=0");
  SignalWidthPerChannel->GetXaxis()->SetTitle("Channel");
  SignalWidthPerChannel->GetYaxis()->SetTitle("Chip");
  SignalWidthPerChannel->GetZaxis()->SetRangeUser(0,30);
  SignalWidthPerChannel->Draw("colz");
  c_chips->cd(12);
  SignalMeanPerChannel->SetTitle("SignallMean (filtered) per channel, SCA=0");
  SignalMeanPerChannel->GetXaxis()->SetTitle("Channel");
  SignalMeanPerChannel->GetYaxis()->SetTitle("Chip");
  SignalMeanPerChannel->GetZaxis()->SetRangeUser(0,500);
  SignalMeanPerChannel->Draw("colz");



  c_chips->Update();

  TString gain = globalvariables::getGainTStringAnalysis();
  TString planevent = globalvariables::getPlaneEventsThresholdTStringAnalysis();

  TString filerecreate = "RECREATE";
  TFile *f_scurve = TFile::Open(file_sufix+gain+planevent+"_MonitorChannel.root",filerecreate);
  TDirectory *dir = f_scurve->GetDirectory("histograms");
  if (!dir) dir = f_scurve->mkdir("histograms");

  f_scurve->cd();
  c_chips->Write();
  dir->cd();
  bufferMedian->Write();
  Triggers->Write();
  Triggers_bcid1->Write();
  Triggers_bcid5->Write();
  Triggers_bcid10->Write();
  Triggers_planeEvents->Write();
  Triggers_negativeData->Write();
  TriggersPerChannel_buf0->Write();
  TriggersPerChannel->Write();
  HitsPedestalPerChannel_buf0->Write();
  HitsPedestalPerChannel->Write();

  PedWidthPerChannel->Write();
  PedMeanPerChannel->Write();
  SignalWidthPerChannel->Write();
  SignalMeanPerChannel->Write();
  f_scurve->Close();


}


/// CHIP ANALYSIS
// ---------------------------------------------------------------

void MonitorManager::simpleChipAnalysis(ExperimentalSetup* aExpSetup) {

  //Loop over all enabled chips to fill the nhit rate for SCA=0 and SCA>0
  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _NhitsChipMap_buf0.begin();mapiter!=_NhitsChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> Nhitsvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNhitsRate();
    for(unsigned ibcid=0; ibcid<Nhitsvec.size(); ibcid++)
      (*mapiter).second.push_back(Nhitsvec.at(ibcid));
    //  if(Nhitsvec.size()==0) (*mapiter).second.push_back(0);
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _NhitsChipMap.find((*mapiter).first);

    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> Nhitsvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getNhitsRate();
      for(unsigned ibcid=0; ibcid<Nhitsvec2.size(); ibcid++)   (*helpMapIter).second.push_back(Nhitsvec2.at(ibcid));

    }
  }
  

  //-----------------------------------------------------------------------------
  // tagged as retrigger events, nhits analysis --> (if also negative ADC (<10) happens, then is considered as negative event, not retrigerred
  //Loop over all enabled chips to fill the nhit rate for SCA=0 and SCA>0

    //--------------
   // REtriggering Events
  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _RetrigNhitsChipMap_buf0.begin();mapiter!=_RetrigNhitsChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> RetrigNhitsvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getRetrigNhitsRate();
    for(unsigned ibcid=0; ibcid<RetrigNhitsvec.size(); ibcid++)
      (*mapiter).second.push_back(RetrigNhitsvec.at(ibcid));
    //  if(RetrigNhitsvec.size()==0) (*mapiter).second.push_back(0);
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _RetrigNhitsChipMap.find((*mapiter).first);

    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> RetrigNhitsvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getRetrigNhitsRate();
      for(unsigned ibcid=0; ibcid<RetrigNhitsvec2.size(); ibcid++)   (*helpMapIter).second.push_back(RetrigNhitsvec2.at(ibcid));

    }
  }
  
  //-----------------------------------------------------------------------------
  // tagged as Negative Events events, nhits analysis -->
  //Loop over all enabled chips to fill the nhit rate for SCA=0 and SCA>0
  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _NegativeNhitsChipMap_buf0.begin();mapiter!=_NegativeNhitsChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> NegativeNhitsvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNegativeNhitsRate();
    for(unsigned ibcid=0; ibcid<NegativeNhitsvec.size(); ibcid++)
      (*mapiter).second.push_back(NegativeNhitsvec.at(ibcid));
    //  if(NegativeNhitsvec.size()==0) (*mapiter).second.push_back(0);
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _NegativeNhitsChipMap.find((*mapiter).first);

    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> NegativeNhitsvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getNegativeNhitsRate();
      for(unsigned ibcid=0; ibcid<NegativeNhitsvec2.size(); ibcid++)   (*helpMapIter).second.push_back(NegativeNhitsvec2.at(ibcid));

    }
  }

  /// #########################################################   BCID 

  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _BcidChipMap_buf0.begin();mapiter!=_BcidChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> Bcidvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getBcidVec();
    for(unsigned ibcid=0; ibcid<Bcidvec.size(); ibcid++)
      (*mapiter).second.push_back(Bcidvec.at(ibcid));
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _BcidChipMap.find((*mapiter).first);

    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> Bcidvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getBcidVec();
      for(unsigned ibcid=0; ibcid<Bcidvec2.size(); ibcid++)   (*helpMapIter).second.push_back(Bcidvec2.at(ibcid));

    }
  }

  //--------------
   // REtriggering Events
  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _RetrigBcidChipMap_buf0.begin();mapiter!=_RetrigBcidChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> RetrigBcidvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getRetrigBcidVec();
    for(unsigned ibcid=0; ibcid<RetrigBcidvec.size(); ibcid++)
      (*mapiter).second.push_back(RetrigBcidvec.at(ibcid));
    if(RetrigBcidvec.size()==0) (*mapiter).second.push_back(-100);
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _RetrigBcidChipMap.find((*mapiter).first);

    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> RetrigBcidvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getRetrigBcidVec();
      for(unsigned ibcid=0; ibcid<RetrigBcidvec2.size(); ibcid++)   (*helpMapIter).second.push_back(RetrigBcidvec2.at(ibcid));

    }
  }

  //--------------
  // Negative Events
  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _NegativeBcidChipMap_buf0.begin();mapiter!=_NegativeBcidChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> NegativeBcidvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNegativeBcidVec();
    for(unsigned ibcid=0; ibcid<NegativeBcidvec.size(); ibcid++)
      (*mapiter).second.push_back(NegativeBcidvec.at(ibcid));
    if(NegativeBcidvec.size()==0) (*mapiter).second.push_back(-100);
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _NegativeBcidChipMap.find((*mapiter).first);

    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> NegativeBcidvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getNegativeBcidVec();
      for(unsigned ibcid=0; ibcid<NegativeBcidvec2.size(); ibcid++)   (*helpMapIter).second.push_back(NegativeBcidvec2.at(ibcid));

    }
  }

}

//
void MonitorManager::simpleChipAnalysisGraphics(TString file_sufix) {


  //Declare and open a Canvas
  TCanvas* c_chips = new TCanvas("SimpleChipMonitoring "+file_sufix,"SimpleChipMonitoring "+file_sufix,11,30,1800,800);
  //Divide the canvas
  c_chips->Divide(4,3);

  //maps
  TH2F * NHitsRate_buf0 = new TH2F("NHitsRate_buf0","NHitsRate_buf0",66,-0.5,65.5,16,-0.5,15.5);
  TH2F * NHitsRate = new TH2F("NHitsRate","NHitsRate",66,-0.5,65.5,16,-0.5,15.5);

  TH2F * BCID_buf0 = new TH2F("BCID_buf0","BCID_buf0",42,-50,4150,16,-0.5,15.5);
  TH2F * BCID = new TH2F("BCID","BCID",42,-50,4150,16,-0.5,15.5);

  TH2F * RetrigNHitsRate_buf0 = new TH2F("RetrigNHitsRate_buf0","RetrigNHitsRate_buf0",66,-0.5,65.5,16,-0.5,15.5);
  TH2F * RetrigNHitsRate = new TH2F("RetrigNHitsRate","RetrigNHitsRate",66,-0.5,65.5,16,-0.5,15.5);

  TH2F * RetrigBCID_buf0 = new TH2F("RetrigBCID_buf0","RetrigBCID_buf0",42,-50,4150,16,-0.5,15.5);
  TH2F * RetrigBCID = new TH2F("RetrigBCID","RetrigBCID",42,-50,4150,16,-0.5,15.5);
  
  TH2F * NegativeNHitsRate_buf0 = new TH2F("NegativeNHitsRate_buf0","NegativeNHitsRate_buf0",66,-0.5,65.5,16,-0.5,15.5);
  TH2F * NegativeNHitsRate = new TH2F("NegativeNHitsRate","NegativeNHitsRate",66,-0.5,65.5,16,-0.5,15.5);

  TH2F * NegativeBCID_buf0 = new TH2F("NegativeBCID_buf0","NegativeBCID_buf0",42,-50,4150,16,-0.5,15.5);
  TH2F * NegativeBCID = new TH2F("NegativeBCID","NegativeBCID",42,-50,4150,16,-0.5,15.5);

  //Loop over all enabled chips
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {

    std::cout << "MonitorManager::simpleChipAnalysisGraphics------------ New chip: " << ichip << " ---------------- " << std::endl;
    
    // Fill chip histograms
    for(unsigned ichn=0; ichn<_NhitsChipMap_buf0.at(ichip).size(); ichn++) 
      NHitsRate_buf0->Fill(ichn, ichip, _NhitsChipMap_buf0.at(ichip).at(ichn));

    for(unsigned ichn=0; ichn<_NhitsChipMap.at(ichip).size(); ichn++) 
      NHitsRate->Fill(ichn, ichip, _NhitsChipMap.at(ichip).at(ichn));
    
    for(unsigned ichn=0; ichn<_BcidChipMap_buf0.at(ichip).size(); ichn++)
      BCID_buf0->Fill(_BcidChipMap_buf0.at(ichip).at(ichn),ichip);
    
    for(unsigned ichn=0; ichn<_BcidChipMap.at(ichip).size(); ichn++)
      BCID->Fill(_BcidChipMap.at(ichip).at(ichn),ichip);

    // // Fill histograms with events tagged as bad ( 0<badbcid<16  )
    for(unsigned ichn=0; ichn<_RetrigNhitsChipMap_buf0.at(ichip).size(); ichn++) 
      RetrigNHitsRate_buf0->Fill(ichn, ichip, _RetrigNhitsChipMap_buf0.at(ichip).at(ichn));

    for(unsigned ichn=0; ichn<_RetrigNhitsChipMap.at(ichip).size(); ichn++) 
      RetrigNHitsRate->Fill(ichn, ichip, _RetrigNhitsChipMap.at(ichip).at(ichn));
    
    for(unsigned ichn=0; ichn<_RetrigBcidChipMap_buf0.at(ichip).size(); ichn++)
       RetrigBCID_buf0->Fill(_RetrigBcidChipMap_buf0.at(ichip).at(ichn),ichip);
    
    for(unsigned ichn=0; ichn<_RetrigBcidChipMap.at(ichip).size(); ichn++)
      RetrigBCID->Fill(_RetrigBcidChipMap.at(ichip).at(ichn),ichip);

   
    // // Fill histograms with events tagged as bad (badbcid > 32, negative events )
    for(unsigned ichn=0; ichn<_NegativeNhitsChipMap_buf0.at(ichip).size(); ichn++) 
      NegativeNHitsRate_buf0->Fill(ichn, ichip, _NegativeNhitsChipMap_buf0.at(ichip).at(ichn));

    for(unsigned ichn=0; ichn<_NegativeNhitsChipMap.at(ichip).size(); ichn++) 
      NegativeNHitsRate->Fill(ichn, ichip, _NegativeNhitsChipMap.at(ichip).at(ichn));
    

    for(unsigned ichn=0; ichn<_NegativeBcidChipMap_buf0.at(ichip).size(); ichn++)
       NegativeBCID_buf0->Fill(_NegativeBcidChipMap_buf0.at(ichip).at(ichn),ichip);
    
    for(unsigned ichn=0; ichn<_NegativeBcidChipMap.at(ichip).size(); ichn++)
      NegativeBCID->Fill(_NegativeBcidChipMap.at(ichip).at(ichn),ichip); 
  }


  NHitsRate_buf0->SetStats(kFALSE);
  NHitsRate->SetStats(kFALSE);

  RetrigNHitsRate_buf0->SetStats(kFALSE);
  RetrigNHitsRate->SetStats(kFALSE);

  NegativeNHitsRate_buf0->SetStats(kFALSE);
  NegativeNHitsRate->SetStats(kFALSE);
  
  c_chips->cd(1);
  NHitsRate_buf0->SetTitle("SCA=0");
  NHitsRate_buf0->GetXaxis()->SetTitle("NHits");
  NHitsRate_buf0->GetYaxis()->SetTitle("chip");
  NHitsRate_buf0->Draw("colz");
  NHitsRate_buf0->SetStats(kFALSE);
  c_chips->cd(2);
  NHitsRate->SetTitle("SCA>0");
  NHitsRate->GetXaxis()->SetTitle("NHits");
  NHitsRate->GetYaxis()->SetTitle("chip");
  NHitsRate->Draw("colz");

  c_chips->cd(3);
  BCID_buf0->SetTitle("SCA=0");
  BCID_buf0->GetXaxis()->SetTitle("BCID");
  BCID_buf0->GetYaxis()->SetTitle("chip");
  BCID_buf0->Draw("colz");

  c_chips->cd(4);
  BCID->SetTitle("SCA>0");
  BCID->GetXaxis()->SetTitle("BCID");
  BCID->GetYaxis()->SetTitle("chip");
  BCID->Draw("colz");

  c_chips->cd(5);
  RetrigNHitsRate_buf0->SetTitle("SCA=0, retriggered");
  RetrigNHitsRate_buf0->GetXaxis()->SetTitle("NHits");
  RetrigNHitsRate_buf0->GetYaxis()->SetTitle("chip");
  RetrigNHitsRate_buf0->Draw("colz");

  c_chips->cd(6);
  RetrigNHitsRate->SetTitle("SCA>0, retriggered");
  RetrigNHitsRate->GetXaxis()->SetTitle("NHits");
  RetrigNHitsRate->GetYaxis()->SetTitle("chip");
  RetrigNHitsRate->Draw("colz");

  c_chips->cd(7);
  RetrigBCID_buf0->SetTitle("SCA=0, retriggered");
  RetrigBCID_buf0->GetXaxis()->SetTitle("BCID");
  RetrigBCID_buf0->GetYaxis()->SetTitle("chip");
  RetrigBCID_buf0->Draw("colz");

  c_chips->cd(8);
  RetrigBCID->SetTitle("SCA>0, retriggered");
  RetrigBCID->GetXaxis()->SetTitle("BCID");
  RetrigBCID->GetYaxis()->SetTitle("chip");
  RetrigBCID->Draw("colz");

  c_chips->cd(9);
  NegativeNHitsRate_buf0->SetTitle("SCA=0, negative");
  NegativeNHitsRate_buf0->GetXaxis()->SetTitle("NHits");
  NegativeNHitsRate_buf0->GetYaxis()->SetTitle("chip");
  NegativeNHitsRate_buf0->Draw("colz");

  c_chips->cd(10);
  NegativeNHitsRate->SetTitle("SCA>0, negative");
  NegativeNHitsRate->GetXaxis()->SetTitle("NHits");
  NegativeNHitsRate->GetYaxis()->SetTitle("chip");
  NegativeNHitsRate->Draw("colz");

  c_chips->cd(11);
  NegativeBCID_buf0->SetTitle("SCA=0, negative");
  NegativeBCID_buf0->GetXaxis()->SetTitle("BCID");
  NegativeBCID_buf0->GetYaxis()->SetTitle("chip");
  NegativeBCID_buf0->Draw("colz");

  c_chips->cd(12);
  NegativeBCID->SetTitle("SCA>0, negative");
  NegativeBCID->GetXaxis()->SetTitle("BCID");
  NegativeBCID->GetYaxis()->SetTitle("chip");
  NegativeBCID->Draw("colz");

  c_chips->Update();

  TString gain = globalvariables::getGainTStringAnalysis();
  TString planevent = globalvariables::getPlaneEventsThresholdTStringAnalysis();

  TString filerecreate = "RECREATE";
  TFile *f_scurve = TFile::Open(file_sufix+gain+planevent+"_MonitorChip.root",filerecreate);
  TDirectory *dir = f_scurve->GetDirectory("histograms");
  if (!dir) dir = f_scurve->mkdir("histograms");

  f_scurve->cd();
  c_chips->Write();
  dir->cd();
  NHitsRate_buf0->Write();
  NHitsRate->Write();
  BCID_buf0->Write();
  BCID->Write();
  f_scurve->Close();


}

// -----------------------------------------------------------------------
/// AUXILIARY FUNCTIONS
double* MonitorManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}

Double_t MonitorManager::GetMean(std::vector<Double_t> vec){
  Double_t sum = 0;
  if(vec.size()>0) {
    for (unsigned k = 0; k<vec.size(); k++)	sum+=vec.at(k);
    Double_t average = sum/vec.size();
    return average;
  } else return 0;
}

Double_t MonitorManager::GetMedian(std::vector<Double_t> vec){

  Double_t median=0;
  if(vec.size()>0) {
    size_t size = vec.size();
    sort(vec.begin(), vec.end());
    if (size  % 2 == 0) median = (vec[size / 2 - 1] + vec[size / 2]) / 2;
    else  median = vec[size / 2];
  }
  return median;

}

Double_t MonitorManager::GetMode(std::vector<Double_t> vec){

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
