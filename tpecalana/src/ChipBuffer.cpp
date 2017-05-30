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
  _nhitsRateVec.clear();
  _nRetrighitsRateVec.clear();
  _nNegativehitsRateVec.clear();
  _nPlanehitsRateVec.clear();
  _chipBcidVec.clear();
  _chipRetrigBcidVec.clear();
  _chipNegativeBcidVec.clear();
  _chipPlaneBcidVec.clear();

  _nhits=0;
  _nRetrighits=0;
  _nNegativehits=0;
  _nPlanehits=0;

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
    if(_nNegativehitsRateVec.size() < (nhits+1) )
      for (int ichan=0; ichan < (nhits + 1); ichan++)
	_nNegativehitsRateVec.push_back(0);
    
    _nNegativehitsRateVec.at(nhits)++;
    _chipNegativeBcidVec.push_back(bcid);

    _nNegativehits++;
    
  } else {
    if(nhits>0 && nhits<=thresh ) {
      if( badbcid == 0) {
	
	if(_nhitsRateVec.size() < (nhits+1) ) 
	  for (int ichan=0; ichan < (nhits + 1); ichan++)
	    _nhitsRateVec.push_back(0);
	
	_nhitsRateVec.at(nhits)++;
	_chipBcidVec.push_back(bcid);
	_nhits++;

      }
      
      if(badbcid>0 && badbcid<16 ) {
	if(_nRetrighitsRateVec.size() < (nhits+1) )
	  for (int ichan=0; ichan < (nhits + 1); ichan++)
	    _nRetrighitsRateVec.push_back(0);
	
	_nRetrighitsRateVec.at(nhits)++;
	_chipRetrigBcidVec.push_back(bcid);
	_nRetrighits++;

      }
    } else {
      if(nhits>thresh && badbcid>-0.5) {
	if(_nPlanehitsRateVec.size() < (nhits+1) ) 
	  for (int ichan=0; ichan < (nhits + 1); ichan++)
	    _nPlanehitsRateVec.push_back(0);
	
	_nPlanehitsRateVec.at(nhits)++;
	_chipPlaneBcidVec.push_back(bcid);
	_nPlanehits++;
	
      }
      
    }
  }


}
    
 

std::vector<Int_t> ChipBuffer::getRetrigBcidVec() {
  return _chipRetrigBcidVec;
}

std::vector<Int_t> ChipBuffer::getRetrigNhitsRate( ) {
  return _nRetrighitsRateVec;
}

std::vector<Int_t> ChipBuffer::getNegativeBcidVec() {
  return _chipNegativeBcidVec;
}

std::vector<Int_t> ChipBuffer::getNegativeNhitsRate( ) {
  return _nNegativehitsRateVec;
}

std::vector<Int_t> ChipBuffer::getPlaneBcidVec() {
  return _chipPlaneBcidVec;
}

std::vector<Int_t> ChipBuffer::getPlaneNhitsRate( ) {
  return _nPlanehitsRateVec;
}


std::vector<Int_t> ChipBuffer::getBcidVec() {
  return _chipBcidVec;
}

std::vector<Int_t> ChipBuffer::getNhitsRate( ) {
  return _nhitsRateVec;
}

std::vector<Int_t> ChipBuffer::getNhits( ) {
  std::vector<Int_t> temp;
  temp.push_back(_nhits);
  temp.push_back(_nRetrighits);
  temp.push_back(_nNegativehits);
  temp.push_back(_nPlanehits);

  return temp;
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
