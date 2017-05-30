/*
 * MonitorManager.cpp
 *
 *  Created on: 5 Jan 2017
 *      Author: irles
 */

#include <MonitorManager.h>
#include "global.h"
#include "TGaxis.h"
using namespace globalvariables;

MonitorManager::~MonitorManager() {
  // TODO Auto-generated destructor stub
}

MonitorManager::MonitorManager() {
  // TODO Auto-generated constructor stub
  f_file=0;
  
  // CHANNEL FILTERED EVENTS
  // ----------------------------------------
  _ntrigVecMapHigh.clear();
  _bufferVecMapMedian.clear();
  
  _TrigChipChannelVecMap.clear();
  _PedChipChannelVecMap.clear();
  _TrigChipChannelVecMap_buf0.clear();
  _PedChipChannelVecMap_buf0.clear();
  
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

  _TrigChipVec_sca.clear(); //vector of unsigned
  _TrigChipVec_retrig_sca.clear(); //vector of unsigned
  _TrigChipVec_planeEvents_sca.clear(); //vector of unsigned
  _TrigChipVec_negativeData_sca.clear(); //vector of unsigned

  // CHANNEL TAGGED EVETNS
  // ----------------------------------------
  _ntrigVecMapHigh_retrig.clear();
  _bufferVecMapMedian_retrig.clear();
  _TrigChipChannelVecMap_retrig.clear();
  _PedChipChannelVecMap_retrig.clear();
  _TrigChipChannelVecMap_buf0_retrig.clear();
  _PedChipChannelVecMap_buf0_retrig.clear();

  _ntrigVecMapHigh_negative.clear();    
  _bufferVecMapMedian_negative.clear();
  _TrigChipChannelVecMap_negative.clear();
  _PedChipChannelVecMap_negative.clear();
  _TrigChipChannelVecMap_buf0_negative.clear();
  _PedChipChannelVecMap_buf0_negative.clear();

  _ntrigVecMapHigh_plane.clear();    
  _bufferVecMapMedian_plane.clear();
  _TrigChipChannelVecMap_plane.clear();
  _PedChipChannelVecMap_plane.clear();
  _TrigChipChannelVecMap_buf0_plane.clear();
  _PedChipChannelVecMap_buf0_plane.clear(); 

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
  //-
  _PlaneNhitsChipMap.clear();
  _PlaneNhitsChipMap_buf0.clear();

  _PlaneBcidChipMap.clear();
  _PlaneBcidChipMap_buf0.clear();
}



void MonitorManager::init(){


  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {

    //channel FILTERED 
    _ntrigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> >  () ));

    _bufferVecMapMedian.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));

    _TrigChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
       
    _TrigChipChannelVecMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    
    _PedWidthChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));
    _SignalWidthChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));
    _PedMeanChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));
    _SignalMeanChipChannelVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Double_t>  () ));

    //channel TAGGED
    _ntrigVecMapHigh_retrig.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> >  () ));
    _bufferVecMapMedian_retrig.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));
    _TrigChipChannelVecMap_retrig.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_retrig.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _TrigChipChannelVecMap_buf0_retrig.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_buf0_retrig.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));

    _ntrigVecMapHigh_negative.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> >  () ));
    _bufferVecMapMedian_negative.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));
    _TrigChipChannelVecMap_negative.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_negative.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _TrigChipChannelVecMap_buf0_negative.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_buf0_negative.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    
    _ntrigVecMapHigh_plane.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<Double_t> >  () ));
    _bufferVecMapMedian_plane.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));
    _TrigChipChannelVecMap_plane.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_plane.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _TrigChipChannelVecMap_buf0_plane.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    _PedChipChannelVecMap_buf0_plane.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned>  () ));
    
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
    //--
    _PlaneNhitsChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _PlaneNhitsChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));

    _PlaneBcidChipMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
    _PlaneBcidChipMap_buf0.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<Int_t>  () ));
  }

}

void MonitorManager::acquireRunInformation(ExperimentalSetup* aExpSetup, TString type){
  std::cout << "************** MonitorManager::AcquireRunInformation new file: " << std::endl;
  if(type == "channel" ) simpleChannelAnalysis(aExpSetup);
  if(type == "chip" ) simpleChipAnalysis(aExpSetup);

}

void MonitorManager::displayResults(TString file_prefix, TString type){

  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  if(type == "channel" ) {
    simpleFilteredChannelAnalysisGraphics(file_prefix);
    simpleTaggedChannelAnalysisGraphics(file_prefix);
  }
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

    unsigned ntrigm_sca(0);
    unsigned ntrigm_retrig_sca(0), ntrigm_planeevents_sca(0), ntrigm_negativedata_sca(0);

    for (unsigned ibuf=0; ibuf < 15; ibuf++) {
 
      std::vector<int> temp_nhits=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getNhits();
      
      ntrigm_sca+=temp_nhits.at(0);
      ntrigm_retrig_sca+=temp_nhits.at(1);
      ntrigm_negativedata_sca+=temp_nhits.at(2);
      ntrigm_planeevents_sca+=temp_nhits.at(3);
    }
	
    for (unsigned ichan=0; ichan < numChans; ichan++) {

      unsigned ntrigm_chn_buf0(0);
      unsigned ntrigm_chn(0);
      unsigned npedm_chn_buf0(0);
      unsigned npedm_chn(0);

      unsigned ntrigm_chn_buf0_retrig(0);
      unsigned ntrigm_chn_retrig(0);

      unsigned ntrigm_chn_buf0_negative(0);
      unsigned ntrigm_chn_negative(0);

      unsigned ntrigm_chn_buf0_plane(0);
      unsigned ntrigm_chn_plane(0);
      // we are going to save the total number of hits per chip
      //  for (unsigned ibuf=0; ibuf < bufdepth; ibuf++) {
      
      unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
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

	unsigned ntrigmtmp(0),ntrigmtmp_retrig(0),ntrigmtmp_negative(0),ntrigmtmp_plane(0);
	unsigned npedmtmp(0);


	//Build up the vector for the individual channels (needs to be done only once)
	if((*mapiter).second.size() < ichan+1)   (*mapiter).second.push_back (std::vector<Double_t> () );

	if(_ntrigVecMapHigh_retrig.at((*mapiter).first).size() < ichan+1 )  _ntrigVecMapHigh_retrig.at((*mapiter).first).push_back (std::vector<Double_t> () );
	if(_ntrigVecMapHigh_negative.at((*mapiter).first).size() < ichan+1 )  _ntrigVecMapHigh_negative.at((*mapiter).first).push_back (std::vector<Double_t> () );
	if(_ntrigVecMapHigh_plane.at((*mapiter).first).size() < ichan+1 )  _ntrigVecMapHigh_plane.at((*mapiter).first).push_back (std::vector<Double_t> () );


	//reads out the trigger of each channel
	std::vector<unsigned> ntrigtmpVect = aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggersVec(ichan);

	//reads out the trigger of each channel
	ntrigmtmp=ntrigtmpVect.at(0);
	npedmtmp=ntrigtmpVect.at(1);
	
	ntrigmtmp_retrig=ntrigtmpVect.at(2)+ntrigtmpVect.at(3)+ntrigtmpVect.at(4);
	ntrigmtmp_negative=ntrigtmpVect.at(6);
	ntrigmtmp_plane=ntrigtmpVect.at(5);

	ntrigm_bcid1+=ntrigtmpVect.at(2);
	ntrigm_bcid5+=ntrigtmpVect.at(3);
	ntrigm_bcid10+=ntrigtmpVect.at(4);
	ntrigm_planeevents+=ntrigtmpVect.at(5);
	ntrigm_negativedata+=ntrigtmpVect.at(6);


	//Add for each run the value in that channel
	//fills the triggers into a vector that is a part of a map of chips and channels
	for (unsigned itrig=0; itrig < ntrigmtmp; itrig++) (*mapiter).second.at(ichan).push_back(ibuf+0.25);
	for (unsigned itrig=0; itrig < ntrigmtmp_retrig; itrig++) _ntrigVecMapHigh_retrig[(*mapiter).first].at(ichan).push_back(ibuf+0.25);
       	for (unsigned itrig=0; itrig < ntrigmtmp_negative; itrig++) _ntrigVecMapHigh_negative[(*mapiter).first].at(ichan).push_back(ibuf+0.25);
       	for (unsigned itrig=0; itrig < ntrigmtmp_plane; itrig++) _ntrigVecMapHigh_plane[(*mapiter).first].at(ichan).push_back(ibuf+0.25);



	//reads out the trigger of each channel
	ntrigm+=ntrigmtmp;
	if(ibuf==0) {
	  ntrigm_chn_buf0=ntrigmtmp;
	  npedm_chn_buf0=npedmtmp;
	  
	  ntrigm_chn_buf0_retrig=ntrigmtmp_retrig;
	  ntrigm_chn_buf0_negative=ntrigmtmp_negative;
	  ntrigm_chn_buf0_plane=ntrigmtmp_plane;
	  
	} else {
	  ntrigm_chn+=ntrigmtmp;
	  npedm_chn+=npedmtmp;

	  ntrigm_chn_retrig+=ntrigmtmp_retrig;
	  ntrigm_chn_negative+=ntrigmtmp_negative;
	  ntrigm_chn_plane+=ntrigmtmp_plane;
	}

      
      }
      //--------------------
      //FILTERED ANALYSIS
      //save the total number of triggers per channel
      _TrigChipChannelVecMap.at((*mapiter).first).push_back(ntrigm_chn);
      _PedChipChannelVecMap.at((*mapiter).first).push_back(npedm_chn);
      //save the total number of triggers per channel
      _TrigChipChannelVecMap_buf0.at((*mapiter).first).push_back(ntrigm_chn_buf0);
      _PedChipChannelVecMap_buf0.at((*mapiter).first).push_back(npedm_chn_buf0);
      // save the maps of mean/median SCA triggered per channel and chip
      std::vector<Double_t> tempvect = (*mapiter).second.at(ichan);
      double median = GetMedian(tempvect);
      _bufferVecMapMedian.at((*mapiter).first).push_back(median);
      
     
      //----------------
      //TAGGED ANALYSIS
      //retriggered
      _TrigChipChannelVecMap_retrig.at((*mapiter).first).push_back(ntrigm_chn_retrig);
      _TrigChipChannelVecMap_buf0_retrig.at((*mapiter).first).push_back(ntrigm_chn_buf0_retrig);
      _TrigChipChannelVecMap_negative.at((*mapiter).first).push_back(ntrigm_chn_negative);
      _TrigChipChannelVecMap_buf0_negative.at((*mapiter).first).push_back(ntrigm_chn_buf0_negative);
      _TrigChipChannelVecMap_plane.at((*mapiter).first).push_back(ntrigm_chn_plane);
      _TrigChipChannelVecMap_buf0_plane.at((*mapiter).first).push_back(ntrigm_chn_buf0_plane);

      std::vector<Double_t> tempvect_retrig =  _ntrigVecMapHigh_retrig[(*mapiter).first].at(ichan);
      double median_retrig = GetMedian(tempvect_retrig);
      _bufferVecMapMedian_retrig.at((*mapiter).first).push_back(median_retrig);
      
      std::vector<Double_t> tempvect_negative =  _ntrigVecMapHigh_negative[(*mapiter).first].at(ichan);
      double median_negative = GetMedian(tempvect_negative);
      _bufferVecMapMedian_negative.at((*mapiter).first).push_back(median_negative);

      std::vector<Double_t> tempvect_plane =  _ntrigVecMapHigh_plane[(*mapiter).first].at(ichan);
      double median_plane = GetMedian(tempvect_plane);
      _bufferVecMapMedian_plane.at((*mapiter).first).push_back(median_plane);    
      
    }

    // save the total number of triggers per chip;
    _TrigChipVec.push_back(ntrigm); 
    _TrigChipVec_bcid1.push_back(ntrigm_bcid1); 
    _TrigChipVec_bcid5.push_back(ntrigm_bcid5); 
    _TrigChipVec_bcid10.push_back(ntrigm_bcid10); 
    _TrigChipVec_planeEvents.push_back(ntrigm_planeevents); 
    _TrigChipVec_negativeData.push_back(ntrigm_negativedata);

    // save the total number of sca that had triggers
    _TrigChipVec_sca.push_back(ntrigm_sca);
    _TrigChipVec_retrig_sca.push_back(ntrigm_retrig_sca);
    _TrigChipVec_negativeData_sca.push_back(ntrigm_negativedata_sca);
    _TrigChipVec_planeEvents_sca.push_back(ntrigm_planeevents_sca);


    

  }
}




//
void MonitorManager::simpleFilteredChannelAnalysisGraphics(TString file_sufix) {


  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "SimpleChannelMonitoring";//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas("FilteredSimpleChannelMonitoring "+file_sufix,"FilteredSimpleChannelMonitoring "+file_sufix,11,30,1600,800);
  //Divide the canvas
  c_chips->Divide(4,3);

  //median of number os filled SCAs
  TH2F * bufferMedian = new TH2F("buffer_median","buffer_median",64,-0.5,63.5,16,-0.5,15.5);
  // histograms of total number of triggers (with and without filtering
  TH1F * Triggers = new TH1F("Triggers","Triggers",16,-0.5,15.5);

  TH1F * Triggers_bcid1 = new TH1F("Triggers_bcid1","Triggers_bcid1",16,-0.5,15.5);
  TH1F * Triggers_bcid5 = new TH1F("Triggers_bcid5","Triggers_bcid5",16,-0.5,15.5);
  TH1F * Triggers_bcid10 = new TH1F("Triggers_bcid10","Triggers_bcid10",16,-0.5,15.5);
  TH1F * Triggers_planeEvents = new TH1F("Triggers_planeEvents","Triggers_planeEvents",16,-0.5,15.5);
  TH1F * Triggers_negativeData = new TH1F("Triggers_negativeData","Triggers_negativeData",16,-0.5,15.5);



  TH1F * Triggers_sca = new TH1F("Triggers_sca","Triggers_sca",16,-0.5,15.5);
  TH1F * Triggers_retrig_sca = new TH1F("Triggers_retrig_sca","Triggers_retrig_sca",16,-0.5,15.5);
  TH1F * Triggers_planeEvents_sca = new TH1F("Triggers_planeEvents_sca","Triggers_planeEvents_sca",16,-0.5,15.5);
  TH1F * Triggers_negativeData_sca = new TH1F("Triggers_negativeData_sca","Triggers_negativeData_sca",16,-0.5,15.5);

  
  // maps with estimated pedestal widht, mip position and signal over noise
  TH2F * PedestalsPerChannel = new TH2F("pedestals_per_channel","pedestals_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * PedestalsPerChannel_buf0 = new TH2F("pedestals_per_channel_buf0","pedestals_per_channel_buf0",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * TriggersPerChannel = new TH2F("triggers_per_channel","triggers_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * TriggersPerChannel_buf0 = new TH2F("triggers_per_channel_buf0","triggers_per_channel_buf0",64,-0.5,63.5,16,-0.5,15.5);

  TH2F * PedMeanPerChannel = new TH2F("PedMean_per_channel","PedMean_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * SignalMeanPerChannel = new TH2F("SignalMean_per_channel","SignalMean_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * SignalWidthPerChannel = new TH2F("SignalWidth_per_channel","SignalWidth_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * SignaloverNoisePerChannel = new TH2F("SignaloverNoise_per_channel","SignaloverNoise_per_channel",64,-0.5,63.5,16,-0.5,15.5);

  //Loop over all enabled chips
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
    // Fill chip histograms
    Triggers->Fill(ichip,_TrigChipVec.at(ichip));
    // normalize number of errors to the 
    if(_TrigChipVec.at(ichip)>0) {
      Triggers_bcid1->Fill(ichip,float(_TrigChipVec_bcid1.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_bcid5->Fill(ichip,float(_TrigChipVec_bcid5.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_bcid10->Fill(ichip,float(_TrigChipVec_bcid10.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_planeEvents->Fill(ichip,float(_TrigChipVec_planeEvents.at(ichip))/float(_TrigChipVec.at(ichip)));
      Triggers_negativeData->Fill(ichip,float(_TrigChipVec_negativeData.at(ichip))/float(_TrigChipVec.at(ichip)));
    }

    if(_TrigChipVec_sca.at(ichip)>0) {
      Triggers_sca->Fill(ichip,float(_TrigChipVec_sca.at(ichip))/float(_TrigChipVec_sca.at(ichip)));
      Triggers_retrig_sca->Fill(ichip,float(_TrigChipVec_retrig_sca.at(ichip))/float(_TrigChipVec_sca.at(ichip)));
      Triggers_planeEvents_sca->Fill(ichip,float(_TrigChipVec_planeEvents_sca.at(ichip))/float(_TrigChipVec_sca.at(ichip)));
      Triggers_negativeData_sca->Fill(ichip,float(_TrigChipVec_negativeData_sca.at(ichip))/float(_TrigChipVec_sca.at(ichip)));
    }
   

    for(int ichn=0; ichn<64; ichn++) {
      bufferMedian->Fill(ichn,double(ichip),_bufferVecMapMedian.at(ichip).at(ichn));
      
      PedestalsPerChannel->Fill(ichn,ichip,_PedChipChannelVecMap.at(ichip).at(ichn));
      PedestalsPerChannel_buf0->Fill(ichn,ichip,_PedChipChannelVecMap_buf0.at(ichip).at(ichn));

      TriggersPerChannel->Fill(ichn,ichip,_TrigChipChannelVecMap.at(ichip).at(ichn));
      TriggersPerChannel_buf0->Fill(ichn,ichip,_TrigChipChannelVecMap_buf0.at(ichip).at(ichn));

      double pedmean = _PedMeanChipChannelVecMap.at(ichip).at(ichn);
      double sigmean = _SignalMeanChipChannelVecMap.at(ichip).at(ichn);
      double sigwidth = _SignalWidthChipChannelVecMap.at(ichip).at(ichn);
      double pedwidth = _PedWidthChipChannelVecMap.at(ichip).at(ichn);

      if( pedwidth> 0 )PedMeanPerChannel->Fill(ichn,ichip,pedmean);
      if( sigmean >0 ) {
	SignalMeanPerChannel->Fill(ichn,ichip,sigmean-pedmean);
	SignalWidthPerChannel->Fill(ichn,ichip,sigwidth);
	if( pedwidth> 0 )	SignaloverNoisePerChannel->Fill(ichn,ichip,(sigmean-pedmean)/pedwidth);
      }

    }

  }


  c_chips->cd(1);
  bufferMedian->SetStats(kFALSE);
  bufferMedian->SetTitle("median of SCA");
  bufferMedian->GetXaxis()->SetTitle("channel");
  bufferMedian->GetYaxis()->SetTitle("chip");
  bufferMedian->GetZaxis()->SetRangeUser(0,16);
  bufferMedian->Draw("colz");

  c_chips->cd(2);
  gPad->SetLogy();
  Triggers->SetTitle("NHits total");
  Triggers->GetXaxis()->SetTitle("Chip");
  Triggers->GetYaxis()->SetTitle("Nhits");
  Triggers->GetYaxis()->SetRangeUser(1,Triggers->GetMaximum()*10);
  Triggers->SetLineColor(1);
  Triggers->SetLineWidth(2);
  Triggers->Draw("histo");

  c_chips->cd(3);
  gPad->SetLogy();
  Triggers_bcid1->SetStats(kFALSE);
  Triggers_bcid5->SetStats(kFALSE);
  Triggers_bcid10->SetStats(kFALSE);
  Triggers_planeEvents->SetStats(kFALSE);
  Triggers_negativeData->SetStats(kFALSE);

  Triggers_bcid1->SetTitle("NHits / Nhits (ok)");
  Triggers_bcid1->GetXaxis()->SetTitle("Chip");
  //Triggers_bcid1->GetYaxis()->SetTitle("Nhits");
  Triggers_bcid1->GetYaxis()->SetRangeUser(0.0001,50000000);
  Triggers_bcid1->Draw("histo");
  
  Triggers_bcid5->SetLineColor(2);
  Triggers_bcid5->SetLineWidth(2);
  Triggers_bcid5->Draw("histosame");
  
  Triggers_bcid10->SetLineColor(3);
  Triggers_bcid10->SetLineWidth(3);
  Triggers_bcid10->Draw("histosame");
  
  Triggers_planeEvents->SetLineStyle(2);
  Triggers_planeEvents->SetLineWidth(4);
  Triggers_planeEvents->SetLineColor(1);
  Triggers_planeEvents->Draw("histosame");
  
  Triggers_negativeData->SetLineStyle(2);
  Triggers_negativeData->SetLineWidth(4);
  Triggers_negativeData->SetLineColor(2);
  Triggers_negativeData->Draw("histosame");
  
  TLegend *leg = new TLegend(0.1,0.6,0.9,0.9);
  leg->AddEntry(Triggers_bcid1,"bcid[buf]-bcid[buf-1]==1","l");
  leg->AddEntry(Triggers_bcid5,"bcid[buf]-bcid[buf-1]<=5 (>1)","l");
  leg->AddEntry(Triggers_bcid10,"bcid[buf]-bcid[buf-1]<=15 (>5)","l");
  leg->AddEntry(Triggers_planeEvents,TString::Format("Nhits(chip)>%i",globalvariables::getPlaneEventsThreshold()),"l");
  leg->AddEntry(Triggers_negativeData,"Negative Data","l");
  leg->Draw();

  c_chips->cd(4);

  // gPad->SetLogy();
  Triggers_sca->SetStats(kFALSE);
  Triggers_retrig_sca->SetStats(kFALSE);
  Triggers_planeEvents_sca->SetStats(kFALSE);
  Triggers_negativeData_sca->SetStats(kFALSE);

  Triggers_sca->SetTitle("NSCA with hits");
  Triggers_sca->GetXaxis()->SetTitle("Chip");
  Triggers_sca->SetLineColor(1);
  Triggers_sca->SetLineWidth(2);
  Triggers_sca->GetYaxis()->SetRangeUser(0,Triggers_sca->GetMaximum()*5);
  Triggers_sca->Draw("histo");

  Triggers_retrig_sca->Draw("histosame");
  
  Triggers_planeEvents_sca->SetLineStyle(2);
  Triggers_planeEvents_sca->SetLineWidth(4);
  Triggers_planeEvents_sca->SetLineColor(1);
  Triggers_planeEvents_sca->Draw("histosame");
  
  Triggers_negativeData_sca->SetLineStyle(2);
  Triggers_negativeData_sca->SetLineWidth(4);
  Triggers_negativeData_sca->SetLineColor(2);
  Triggers_negativeData_sca->Draw("histosame");
  
  // TLegend *leg2 = new TLegend(0.1,0.6,0.9,0.9);
  //leg2->AddEntry(Triggers_sca,"GOOD","l");
  //leg2->AddEntry(Triggers_retrig_sca,"retrig","l");
  //leg2->AddEntry(Triggers_planeEvents_sca,TString::Format("Nhits(chip)>%i",globalvariables::getPlaneEventsThreshold()),"l");
  //leg2->AddEntry(Triggers_negativeData_sca,"Negative Data","l");
  //leg2->Draw();

  
  c_chips->cd(5);
  PedestalsPerChannel_buf0->SetTitle("N-noise, SCA=0");
  PedestalsPerChannel_buf0->GetXaxis()->SetTitle("Channel");
  PedestalsPerChannel_buf0->GetYaxis()->SetTitle("Chip");
  PedestalsPerChannel_buf0->Draw("colz");

  c_chips->cd(6);
  PedestalsPerChannel->SetTitle("N-noise, SCA>0");
  PedestalsPerChannel->GetXaxis()->SetTitle("Channel");
  PedestalsPerChannel->GetYaxis()->SetTitle("Chip");
  PedestalsPerChannel->Draw("colz");

  c_chips->cd(7);
  TriggersPerChannel_buf0->SetTitle("N-hits, SCA=0");
  TriggersPerChannel_buf0->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_buf0->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_buf0->Draw("colz");

  c_chips->cd(8);
  TriggersPerChannel->SetTitle("N-hits, SCA>0");
  TriggersPerChannel->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel->Draw("colz");

  c_chips->cd(9);
  PedMeanPerChannel->SetStats(kFALSE);
  PedMeanPerChannel->SetTitle("Ped Mean, SCA=0");
  PedMeanPerChannel->GetXaxis()->SetTitle("Channel");
  PedMeanPerChannel->GetYaxis()->SetTitle("Chip");
  //PedMeanPerChannel->GetZaxis()->SetRangeUser(0,500);
  PedMeanPerChannel->Draw("colz");

  c_chips->cd(10);
  SignalMeanPerChannel->SetStats(kFALSE);
  SignalMeanPerChannel->SetTitle("Sig-Ped, SCA=0");
  SignalMeanPerChannel->GetXaxis()->SetTitle("Channel");
  SignalMeanPerChannel->GetYaxis()->SetTitle("Chip");
  //SignalMeanPerChannel->GetZaxis()->SetRangeUser(0,500);
  SignalMeanPerChannel->Draw("colz");
  
  c_chips->cd(11);
  SignalWidthPerChannel->SetStats(kFALSE);
  SignalWidthPerChannel->SetTitle("Sig Width, SCA=0");
  SignalWidthPerChannel->GetXaxis()->SetTitle("Channel");
  SignalWidthPerChannel->GetYaxis()->SetTitle("Chip");
  // SignalWidthPerChannel->GetZaxis()->SetRangeUser(0,30);
  SignalWidthPerChannel->Draw("colz");
   
  c_chips->cd(12);
  SignaloverNoisePerChannel->SetStats(kFALSE);
  SignaloverNoisePerChannel->SetTitle("S/N, SCA=0");
  SignaloverNoisePerChannel->GetXaxis()->SetTitle("Channel");
  SignaloverNoisePerChannel->GetYaxis()->SetTitle("Chip");
  SignaloverNoisePerChannel->GetZaxis()->SetRangeUser(0,50);
  SignaloverNoisePerChannel->Draw("colz");
   

  c_chips->Update();

  TString gain = globalvariables::getGainTStringAnalysis();
  TString planevent = globalvariables::getPlaneEventsThresholdTStringAnalysis();

  TString filerecreate ="RECREATE";
  TFile *f_scurve = TFile::Open(file_sufix+gain+planevent+"_MonitorChannelFiltered.root",filerecreate);
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
  PedestalsPerChannel_buf0->Write();
  PedestalsPerChannel->Write();

  PedMeanPerChannel->Write();
  SignalWidthPerChannel->Write();
  SignalMeanPerChannel->Write();
  SignaloverNoisePerChannel->Write();

  f_scurve->Close();


}


//
void MonitorManager::simpleTaggedChannelAnalysisGraphics(TString file_sufix) {


  //Declare and open a Canvas
  std::stringstream canvasNameStr;
  canvasNameStr << "SimpleChannelMonitoring";//the iterator gives the chip ID
  TCanvas* c_chips = new TCanvas("TaggedSimpleChannelMonitoring "+file_sufix,"TaggedSimpleChannelMonitoring "+file_sufix,11,30,1800,600);
  //Divide the canvas
  c_chips->Divide(3,4);


  //median of number os filled SCAs
  TH2F * bufferMedian = new TH2F("buffer_median","buffer_median",64,-0.5,63.5,16,-0.5,15.5);
  // histograms of total number of triggers (with and without filtering
  TH2F * TriggersPerChannel = new TH2F("triggers_per_channel","triggers_per_channel",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * TriggersPerChannel_buf0 = new TH2F("triggers_per_channel_buf0","triggers_per_channel_buf0",64,-0.5,63.5,16,-0.5,15.5);

  
  // retriggered ---------
  //median of number os filled SCAs
  TH2F * bufferMedian_retrig = new TH2F("buffer_median_retrig","buffer_median_retrig",64,-0.5,63.5,16,-0.5,15.5);
  // histograms of total number of triggers (with and without filtering
  TH2F * TriggersPerChannel_retrig = new TH2F("triggers_per_channel_retrig","triggers_per_channel_retrig",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * TriggersPerChannel_buf0_retrig = new TH2F("triggers_per_channel_buf0_retrig","triggers_per_channel_buf0_retrig",64,-0.5,63.5,16,-0.5,15.5);

  // negative ----------
  //median of number os filled SCAs
  TH2F * bufferMedian_negative = new TH2F("buffer_median_negative","buffer_median_negative",64,-0.5,63.5,16,-0.5,15.5);
  // histograms of total number of triggers (with and without filtering
  TH2F * TriggersPerChannel_negative = new TH2F("triggers_per_channel_negative","triggers_per_channel_negative",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * TriggersPerChannel_buf0_negative = new TH2F("triggers_per_channel_buf0_negative","triggers_per_channel_buf0_negative",64,-0.5,63.5,16,-0.5,15.5);

  // plane ----------
  //median of number os filled SCAs
  TH2F * bufferMedian_plane = new TH2F("buffer_median_plane","buffer_median_plane",64,-0.5,63.5,16,-0.5,15.5);
  // histograms of total number of triggers (with and without filtering
  TH2F * TriggersPerChannel_plane = new TH2F("triggers_per_channel_plane","triggers_per_channel_plane",64,-0.5,63.5,16,-0.5,15.5);
  TH2F * TriggersPerChannel_buf0_plane = new TH2F("triggers_per_channel_buf0_plane","triggers_per_channel_buf0_plane",64,-0.5,63.5,16,-0.5,15.5);

  //Loop over all enabled chips
  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
   

    for(int ichn=0; ichn<64; ichn++) {

      bufferMedian->Fill(ichn,double(ichip),_bufferVecMapMedian.at(ichip).at(ichn));
      TriggersPerChannel->Fill(ichn,ichip,_TrigChipChannelVecMap.at(ichip).at(ichn));
      TriggersPerChannel_buf0->Fill(ichn,ichip,_TrigChipChannelVecMap_buf0.at(ichip).at(ichn));
      
      bufferMedian_retrig->Fill(ichn,double(ichip),_bufferVecMapMedian_retrig.at(ichip).at(ichn));
      TriggersPerChannel_retrig->Fill(ichn,ichip,_TrigChipChannelVecMap_retrig.at(ichip).at(ichn));
      TriggersPerChannel_buf0_retrig->Fill(ichn,ichip,_TrigChipChannelVecMap_buf0_retrig.at(ichip).at(ichn));

      bufferMedian_negative->Fill(ichn,double(ichip),_bufferVecMapMedian_negative.at(ichip).at(ichn));
      TriggersPerChannel_negative->Fill(ichn,ichip,_TrigChipChannelVecMap_negative.at(ichip).at(ichn));
      TriggersPerChannel_buf0_negative->Fill(ichn,ichip,_TrigChipChannelVecMap_buf0_negative.at(ichip).at(ichn));
      
      bufferMedian_plane->Fill(ichn,double(ichip),_bufferVecMapMedian_plane.at(ichip).at(ichn));
      TriggersPerChannel_plane->Fill(ichn,ichip,_TrigChipChannelVecMap_plane.at(ichip).at(ichn));
      TriggersPerChannel_buf0_plane->Fill(ichn,ichip,_TrigChipChannelVecMap_buf0_plane.at(ichip).at(ichn));
    }

  }

  c_chips->cd(1);
  bufferMedian->SetStats(kFALSE);
  bufferMedian->SetTitle("median of SCA, GOOD");
  bufferMedian->GetXaxis()->SetTitle("channel");
  bufferMedian->GetYaxis()->SetTitle("chip");
  bufferMedian->GetZaxis()->SetRangeUser(0,16);
  bufferMedian->Draw("colz");

  c_chips->cd(2);
  TriggersPerChannel_buf0->SetTitle("N-hits, GOOD, SCA=0");
  TriggersPerChannel_buf0->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_buf0->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_buf0->Draw("colz");

  c_chips->cd(3);
  TriggersPerChannel->SetTitle("N-hits, GOOD, SCA>0");
  TriggersPerChannel->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel->Draw("colz");

  c_chips->cd(4);
  bufferMedian_retrig->SetStats(kFALSE);
  bufferMedian_retrig->SetTitle("median of SCA, RT");
  bufferMedian_retrig->GetXaxis()->SetTitle("channel");
  bufferMedian_retrig->GetYaxis()->SetTitle("chip");
  bufferMedian_retrig->GetZaxis()->SetRangeUser(0,16);
  bufferMedian_retrig->Draw("colz");

  c_chips->cd(5);
  TriggersPerChannel_buf0_retrig->SetTitle("N-hits, RT, SCA=0");
  TriggersPerChannel_buf0_retrig->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_buf0_retrig->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_buf0_retrig->Draw("colz");

  c_chips->cd(6);
  TriggersPerChannel_retrig->SetTitle("N-hits, RT, SCA>0");
  TriggersPerChannel_retrig->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_retrig->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_retrig->Draw("colz");


  c_chips->cd(7);
  bufferMedian_negative->SetStats(kFALSE);
  bufferMedian_negative->SetTitle("median of SCA, NEG");
  bufferMedian_negative->GetXaxis()->SetTitle("channel");
  bufferMedian_negative->GetYaxis()->SetTitle("chip");
  bufferMedian_negative->GetZaxis()->SetRangeUser(0,16);
  bufferMedian_negative->Draw("colz");

  c_chips->cd(8);
  TriggersPerChannel_buf0_negative->SetTitle("N-hits, NEG, SCA=0");
  TriggersPerChannel_buf0_negative->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_buf0_negative->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_buf0_negative->Draw("colz");

  c_chips->cd(9);
  TriggersPerChannel_negative->SetTitle("N-hits, NEG, SCA>0");
  TriggersPerChannel_negative->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_negative->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_negative->Draw("colz");

  c_chips->cd(10);
  bufferMedian_plane->SetStats(kFALSE);
  bufferMedian_plane->SetTitle("median of SCA, PLANE");
  bufferMedian_plane->GetXaxis()->SetTitle("channel");
  bufferMedian_plane->GetYaxis()->SetTitle("chip");
  bufferMedian_plane->GetZaxis()->SetRangeUser(0,16);
  bufferMedian_plane->Draw("colz");

  c_chips->cd(11);
  TriggersPerChannel_buf0_plane->SetTitle("N-hits, PLANE, SCA=0");
  TriggersPerChannel_buf0_plane->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_buf0_plane->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_buf0_plane->Draw("colz");

  c_chips->cd(12);
  TriggersPerChannel_plane->SetTitle("N-hits, PLANE, SCA>0");
  TriggersPerChannel_plane->GetXaxis()->SetTitle("Channel");
  TriggersPerChannel_plane->GetYaxis()->SetTitle("Chip");
  TriggersPerChannel_plane->Draw("colz");


  c_chips->Update();

  TString gain = globalvariables::getGainTStringAnalysis();
  TString planevent = globalvariables::getPlaneEventsThresholdTStringAnalysis();

  TString filerecreate ="RECREATE";
  TFile *f_scurve = TFile::Open(file_sufix+gain+planevent+"_MonitorChannelTagged.root",filerecreate);
  //TDirectory *dir = f_scurve->GetDirectory("histograms");
  //if (!dir) dir = f_scurve->mkdir("histograms");

  f_scurve->cd();
  c_chips->Write();
  //dir->cd();
  //bufferMedian_retrig->Write();
  //TriggersPerChannel_buf0_retrig->Write();
  //TriggersPerChannel_retrig->Write();
  //bufferMedian_negative->Write();
  //TriggersPerChannel_buf0_negative->Write();
  //TriggersPerChannel_negative->Write();

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
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _NhitsChipMap.find((*mapiter).first);
    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> Nhitsvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getNhitsRate();
      for(unsigned inhit=0; inhit<Nhitsvec2.size(); inhit++)   {
	if((*helpMapIter).second.size()<inhit+1) (*helpMapIter).second.push_back(0);
	(*helpMapIter).second.at(inhit)+=Nhitsvec2.at(inhit);
      }
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
      for(unsigned inhit=0; inhit<RetrigNhitsvec2.size(); inhit++)   {
	if((*helpMapIter).second.size()<inhit+1) (*helpMapIter).second.push_back(0);
	(*helpMapIter).second.at(inhit)+=RetrigNhitsvec2.at(inhit);
      }
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
      for(unsigned inhit=0; inhit<NegativeNhitsvec2.size(); inhit++)   {
	if((*helpMapIter).second.size()<inhit+1) (*helpMapIter).second.push_back(0);
	(*helpMapIter).second.at(inhit)+=NegativeNhitsvec2.at(inhit);
      }
    }

  }

  //-----------------------------------------------------------------------------
  // tagged as Plane Events events, nhits analysis -->
  //Loop over all enabled chips to fill the nhit rate for SCA=0 and SCA>0
  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _PlaneNhitsChipMap_buf0.begin();mapiter!=_PlaneNhitsChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> PlaneNhitsvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getPlaneNhitsRate();
    for(unsigned ibcid=0; ibcid<PlaneNhitsvec.size(); ibcid++)
      (*mapiter).second.push_back(PlaneNhitsvec.at(ibcid));
    //  if(PlaneNhitsvec.size()==0) (*mapiter).second.push_back(0);
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
    
    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _PlaneNhitsChipMap.find((*mapiter).first);
    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> PlaneNhitsvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getPlaneNhitsRate();
      for(unsigned inhit=0; inhit<PlaneNhitsvec2.size(); inhit++)   {
	if((*helpMapIter).second.size()<inhit+1) (*helpMapIter).second.push_back(0);
	(*helpMapIter).second.at(inhit)+=PlaneNhitsvec2.at(inhit);
      }
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

  //--------------
  // Plane Events
  //Loop over all enabled chips to fill the bcid mean and rms calculation rate for SCA=0 and SCA>0
  for (std::map<unsigned,std::vector<Int_t> >::iterator mapiter = _PlaneBcidChipMap_buf0.begin();mapiter!=_PlaneBcidChipMap_buf0.end();mapiter++) {
    std::cout << "MonitorManager::simpleChipAnalysis, BCID ------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;

    std::vector<Int_t> PlaneBcidvec=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getPlaneBcidVec();
    for(unsigned ibcid=0; ibcid<PlaneBcidvec.size(); ibcid++)
      (*mapiter).second.push_back(PlaneBcidvec.at(ibcid));
    if(PlaneBcidvec.size()==0) (*mapiter).second.push_back(-100);
    
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());

    std::map<unsigned,std::vector<Int_t> >::iterator helpMapIter = _PlaneBcidChipMap.find((*mapiter).first);

    for(unsigned ibuf =1; ibuf<bufdepth; ibuf++ ) {
      std::vector<Int_t> PlaneBcidvec2=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getPlaneBcidVec();
      for(unsigned ibcid=0; ibcid<PlaneBcidvec2.size(); ibcid++)   (*helpMapIter).second.push_back(PlaneBcidvec2.at(ibcid));

    }
  }

}

//
void MonitorManager::simpleChipAnalysisGraphics(TString file_sufix) {


  //Declare and open a Canvas
  TCanvas* c_chips = new TCanvas("SimpleChipMonitoring "+file_sufix,"SimpleChipMonitoring "+file_sufix,1000,30,1800,800);
  //Divide the canvas
  c_chips->Divide(4,4);

  //maps
  TH2F * NHitsRate_buf0 = new TH2F("NHitsRate_buf0","NHitsRate_buf0",65,0.5,64.5,16,-0.5,15.5);
  TH2F * NHitsRate = new TH2F("NHitsRate","NHitsRate",65,0.5,64.5,16,-0.5,15.5);

  TH2F * BCID_buf0 = new TH2F("BCID_buf0","BCID_buf0",42,-50,4150,16,-0.5,15.5);
  TH2F * BCID = new TH2F("BCID","BCID",42,-50,4150,16,-0.5,15.5);

  TH2F * RetrigNHitsRate_buf0 = new TH2F("RetrigNHitsRate_buf0","RetrigNHitsRate_buf0",65,0.5,64.5,16,-0.5,15.5);
  TH2F * RetrigNHitsRate = new TH2F("RetrigNHitsRate","RetrigNHitsRate",65,0.5,64.5,16,-0.5,15.5);

  TH2F * RetrigBCID_buf0 = new TH2F("RetrigBCID_buf0","RetrigBCID_buf0",42,-50,4150,16,-0.5,15.5);
  TH2F * RetrigBCID = new TH2F("RetrigBCID","RetrigBCID",42,-50,4150,16,-0.5,15.5);
  
  TH2F * NegativeNHitsRate_buf0 = new TH2F("NegativeNHitsRate_buf0","NegativeNHitsRate_buf0",65,0.5,64.5,16,-0.5,15.5);
  TH2F * NegativeNHitsRate = new TH2F("NegativeNHitsRate","NegativeNHitsRate",65,0.5,64.5,16,-0.5,15.5);

  TH2F * NegativeBCID_buf0 = new TH2F("NegativeBCID_buf0","NegativeBCID_buf0",42,-50,4150,16,-0.5,15.5);
  TH2F * NegativeBCID = new TH2F("NegativeBCID","NegativeBCID",42,-50,4150,16,-0.5,15.5);

  TH2F * PlaneNHitsRate_buf0 = new TH2F("PlaneNHitsRate_buf0","PlaneNHitsRate_buf0",65,0.5,64.5,16,-0.5,15.5);
  TH2F * PlaneNHitsRate = new TH2F("PlaneNHitsRate","PlaneNHitsRate",65,0.5,64.5,16,-0.5,15.5);

  TH2F * PlaneBCID_buf0 = new TH2F("PlaneBCID_buf0","PlaneBCID_buf0",42,-50,4150,16,-0.5,15.5);
  TH2F * PlaneBCID = new TH2F("PlaneBCID","PlaneBCID",42,-50,4150,16,-0.5,15.5);

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

    // // Fill histograms with events tagged as plane events
    for(unsigned ichn=0; ichn<_PlaneNhitsChipMap_buf0.at(ichip).size(); ichn++) 
      PlaneNHitsRate_buf0->Fill(ichn, ichip, _PlaneNhitsChipMap_buf0.at(ichip).at(ichn));

    for(unsigned ichn=0; ichn<_PlaneNhitsChipMap.at(ichip).size(); ichn++) 
      PlaneNHitsRate->Fill(ichn, ichip, _PlaneNhitsChipMap.at(ichip).at(ichn));
    

    for(unsigned ichn=0; ichn<_PlaneBcidChipMap_buf0.at(ichip).size(); ichn++)
       PlaneBCID_buf0->Fill(_PlaneBcidChipMap_buf0.at(ichip).at(ichn),ichip);
    
    for(unsigned ichn=0; ichn<_PlaneBcidChipMap.at(ichip).size(); ichn++)
      PlaneBCID->Fill(_PlaneBcidChipMap.at(ichip).at(ichn),ichip); 
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

  c_chips->cd(13);
  PlaneNHitsRate_buf0->SetTitle("SCA=0, plane events");
  PlaneNHitsRate_buf0->GetXaxis()->SetTitle("NHits");
  PlaneNHitsRate_buf0->GetYaxis()->SetTitle("chip");
  PlaneNHitsRate_buf0->Draw("colz");

  c_chips->cd(14);
  PlaneNHitsRate->SetTitle("SCA>0, plane events");
  PlaneNHitsRate->GetXaxis()->SetTitle("NHits");
  PlaneNHitsRate->GetYaxis()->SetTitle("chip");
  PlaneNHitsRate->Draw("colz");

  c_chips->cd(15);
  PlaneBCID_buf0->SetTitle("SCA=0, plane events");
  PlaneBCID_buf0->GetXaxis()->SetTitle("BCID");
  PlaneBCID_buf0->GetYaxis()->SetTitle("chip");
  PlaneBCID_buf0->Draw("colz");

  c_chips->cd(16);
  PlaneBCID->SetTitle("SCA>0, plane events");
  PlaneBCID->GetXaxis()->SetTitle("BCID");
  PlaneBCID->GetYaxis()->SetTitle("chip");
  PlaneBCID->Draw("colz");

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
