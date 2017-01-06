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
}

ChipBuffer::~ChipBuffer(){/*no op*/}

void ChipBuffer::init(unsigned numchans){
  _numChans=numchans;
  //Create a vector holding the channels of this buffer
  for (unsigned ichan=0; ichan < _numChans; ichan++){
    _channelVec.push_back(Channel (ichan) );
  }
}


bool ChipBuffer::setChannelVals(int nhits, int badbcid, int valHigh, int valLow, int gainHitHigh, int gainHitLow) {

  //pass the measured value to the corresponding channel  

  if(globalvariables::getGlobal_deepAnalysis()==true) {
    if(gainHitHigh==0 && valHigh>0)  _channelPedHighHistoVec.at(_channelCount).push_back(valHigh);
    if(gainHitLow==0 && valLow>0)  _channelPedLowHistoVec.at(_channelCount).push_back(valLow);
    if(gainHitHigh==1 && valHigh>0)  _channelHighHistoVec.at(_channelCount).push_back(valHigh);
    if(gainHitLow==1 && valLow>0)  _channelLowHistoVec.at(_channelCount).push_back(valLow);
  }
  _channelVec.at(_channelCount).acquireData(nhits, badbcid, valHigh, valLow, gainHitHigh, gainHitLow);

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
  for (unsigned ichan=0; ichan < _numChans; ichan++) ntot+=_channelVec.at(ichan).getNTriggers();
  return ntot;
}

unsigned ChipBuffer::getNumberOfChannels() {return _numChans;}

double ChipBuffer::getChannelMean(unsigned ichan, std::string modeStr, int trig) {
  //std::cout << "example val: " <<  _channelVec.at(ichan).getMean() << std::endl;
  return  _channelVec.at(ichan).getMean(modeStr, trig);
}

double ChipBuffer::getChannelRMS(unsigned ichan, std::string modeStr, int trig) {
  return  _channelVec.at(ichan).getRMS(modeStr, trig);
}

unsigned ChipBuffer::getChannelEntries(unsigned ichan) {
  //std::cout << "example val: " <<  _channelVec.at(ichan).getMean() << std::endl;
  return  _channelVec.at(ichan).getNEntries();
}

unsigned ChipBuffer::getChannelTriggers(unsigned ichan) {
  return  _channelVec.at(ichan).getNTriggers();
}

unsigned ChipBuffer::getChannelTriggers_planeEvents(unsigned ichan) {
  return  _channelVec.at(ichan).getNTriggers_planeEvents();
}

unsigned ChipBuffer::getChannelTriggers_overRunningBcid(unsigned ichan) {
  return  _channelVec.at(ichan).getNTriggers_overRunningBcid();
}

unsigned ChipBuffer::getChannelTriggers_negativeData(unsigned ichan) {
  return  _channelVec.at(ichan).getNTriggers_negativeData();
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
