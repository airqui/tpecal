//
//  ChipBuffer.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 29/07/15.
//
//

#include "ChipBuffer.h"
#include "global.h"

using namespace globalvariables;

ChipBuffer::ChipBuffer(unsigned bufferID){
  //set the buffer if
  _bufferID=bufferID;
  //set the number of channels has to come from an external source
  _numChans=0;
  //reset the channel count
  _channelCount=0;

  //nhits and bcid analysis
  _nhitsVec.clear();
  _nRetrighitsVec.clear();
  _nNegativehitsVec.clear();
  _nPlanehitsVec.clear();
  _chipBcidVec.clear();
  _chipRetrigBcidVec.clear();
  _chipNegativeBcidVec.clear();
  _chipPlaneBcidVec.clear();


}

ChipBuffer::~ChipBuffer(){/*no op*/}

void ChipBuffer::init(unsigned numchans){
  _numChans=numchans;
  //Create a vector holding the channels of this buffer
  for (unsigned ichan=0; ichan < _numChans; ichan++){
    _channelVec.push_back(Channel (ichan) );
  }

}


bool ChipBuffer::setChannelVals(Int_t nhits, Int_t badbcid,  Int_t correctedbcid, Int_t valHigh, Int_t valLow, Int_t gainHitHigh, Int_t gainHitLow) {

  //pass the measured value to the corresponding channel  

  if(globalvariables::getGlobal_deepAnalysis()==true) {
    if(gainHitHigh==0 && valHigh>0)  _channelPedHighHistoVec.at(_channelCount).push_back(valHigh);
    if(gainHitLow==0 && valLow>0)  _channelPedLowHistoVec.at(_channelCount).push_back(valLow);
    if(gainHitHigh==1 && valHigh>0)  _channelHighHistoVec.at(_channelCount).push_back(valHigh);
    if(gainHitLow==1 && valLow>0)  _channelLowHistoVec.at(_channelCount).push_back(valLow);
  }
  _channelVec.at(_channelCount).acquireData(nhits, badbcid, correctedbcid, valHigh, valLow, gainHitHigh, gainHitLow);

  //  if(nhits==0 && gainHitHigh==1) std::cout<<"ALERT ChipBuffer "<<nhits<<" "<< valHigh <<" "<<gainHitHigh<<std::endl;

  //increment the channel count
  _channelCount++;

  //signalise that all channels are filled for this buffer
  if(_channelCount==_numChans) {
    _channelCount=0; 
    return true;
  }
  //...else return false
  return false;
}


void ChipBuffer::setChipVals(Int_t bcid, Int_t corrected_bcid, Int_t badbcid, Int_t nhits) {

  Int_t thresh = globalvariables::getPlaneEventsThreshold();

  //pass the measured value to the corresponding channel

  if(badbcid>30 ) {
    if(_nNegativehitsVec.size() < (nhits+1) )
      for (int ichan=0; ichan < (nhits + 1); ichan++)
	_nNegativehitsVec.push_back(0);
    
    _nNegativehitsVec.at(nhits)++;
    _chipNegativeBcidVec.push_back(bcid);
    
  } else {
    if(nhits>0 && nhits<=thresh ) {
      if( badbcid == 0) {
	
	if(_nhitsVec.size() < (nhits+1) ) 
	  for (int ichan=0; ichan < (nhits + 1); ichan++)
	    _nhitsVec.push_back(0);
	
	_nhitsVec.at(nhits)++;
	_chipBcidVec.push_back(bcid);
      }
      
      if(badbcid>0 && badbcid<16 ) {
	if(_nRetrighitsVec.size() < (nhits+1) )
	  for (int ichan=0; ichan < (nhits + 1); ichan++)
	    _nRetrighitsVec.push_back(0);
	
	_nRetrighitsVec.at(nhits)++;
	_chipRetrigBcidVec.push_back(bcid);
	
      }
    } else {
      if(nhits>thresh && badbcid>-0.5) {
	if(_nPlanehitsVec.size() < (nhits+1) ) 
	  for (int ichan=0; ichan < (nhits + 1); ichan++)
	    _nPlanehitsVec.push_back(0);
	
	_nPlanehitsVec.at(nhits)++;
	_chipPlaneBcidVec.push_back(bcid);
      }
      
    }
  }


}
    
 

std::vector<Int_t> ChipBuffer::getRetrigBcidVec() {
  return _chipRetrigBcidVec;
}

std::vector<Int_t> ChipBuffer::getRetrigNhitsRate( ) {
  return _nRetrighitsVec;
}

std::vector<Int_t> ChipBuffer::getNegativeBcidVec() {
  return _chipNegativeBcidVec;
}

std::vector<Int_t> ChipBuffer::getNegativeNhitsRate( ) {
  return _nNegativehitsVec;
}

std::vector<Int_t> ChipBuffer::getPlaneBcidVec() {
  return _chipPlaneBcidVec;
}

std::vector<Int_t> ChipBuffer::getPlaneNhitsRate( ) {
  return _nPlanehitsVec;
}


std::vector<Int_t> ChipBuffer::getBcidVec() {
  return _chipBcidVec;
}

std::vector<Int_t> ChipBuffer::getNhitsRate( ) {
  return _nhitsVec;
}


void ChipBuffer::setNumberOfASICChannels(unsigned numChans) {
  _numChans=numChans;

  //Create a vector holding the channels of this buffer
  for (unsigned ichan=0; ichan < _numChans; ichan++) {
    _channelVec.push_back(Channel (ichan) );
    if(globalvariables::getGlobal_deepAnalysis()==true) {
      _channelPedHighHistoVec.push_back (std::vector<int> () );
      _channelPedLowHistoVec.push_back (std::vector<int> () );
      _channelHighHistoVec.push_back (std::vector<int> () );
      _channelLowHistoVec.push_back (std::vector<int> () );
    }
  }
}


unsigned ChipBuffer::getTotalNumberOfTriggers() {
  unsigned ntot(0);
  for (unsigned ichan=0; ichan < _numChans; ichan++) ntot+=_channelVec.at(ichan).getNTriggersVec().at(0);
  return ntot;
}

unsigned ChipBuffer::getNumberOfChannels() {return _numChans;}


std::vector<double> ChipBuffer::getChannelMeanVec(unsigned ichan, std::string modeStr) {
  return  _channelVec.at(ichan).getMeanVec(modeStr);
}

std::vector<double> ChipBuffer::getChannelRMSVec(unsigned ichan, std::string modeStr) {
  return  _channelVec.at(ichan).getRMSVec(modeStr);
}

unsigned ChipBuffer::getChannelEntries(unsigned ichan) {
  //std::cout << "example val: " <<  _channelVec.at(ichan).getMean() << std::endl;
  return  _channelVec.at(ichan).getNEntries();
}

std::vector<unsigned> ChipBuffer::getChannelTriggersVec(unsigned ichan) {
  return  _channelVec.at(ichan).getNTriggersVec();
}


std::vector<int> ChipBuffer::getChannelPedHisto(unsigned ichan, unsigned gainbit) {

  std::vector<int> temp;
  if(globalvariables::getGlobal_deepAnalysis()==true) {
    if(gainbit==1) return _channelPedHighHistoVec.at(ichan);
    if(gainbit==0) return _channelPedLowHistoVec.at(ichan);
    return temp;
  }
  return temp;

}

std::vector<int> ChipBuffer::getChannelChargeHisto(unsigned ichan, unsigned gainbit) {

  std::vector<int> temp;
  if(globalvariables::getGlobal_deepAnalysis()==true) {
    if(gainbit==1) return _channelHighHistoVec.at(ichan);
    if(gainbit==0) return _channelLowHistoVec.at(ichan);
    return temp;
  }
  return temp;

}
