//
//  Chip.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#include <iostream>
#include "Chip.h"

Chip::Chip(unsigned chipID){
  //Set the chip ID
  _chipID = chipID;
  _aMappingPtr=nullptr;
  //Set the buffer depth in a chip (has to come from an external source)
  _bufferDepth = 0;
  _nChannels = 0;
  _chipBuffer.clear();

}

Chip::~Chip() {/*no op */}

void Chip::init(Mapping* aMappingPtr) {
  _aMappingPtr=aMappingPtr;
  if (_aMappingPtr==nullptr) {
    std::cout << "Chip::init: WARNING: Did receive the NULL pointer" << std::endl;
    std::cout << "Maybe this is ok in case one doesn't have a proper mapping file" << std::endl;
    std::cout << "Please check!!!!!!" << std::endl;
  }
}

void Chip::setNumberOfASICChannels(unsigned nChannels) {
  //Needs to be done in case no proper inpt file is available
  _nChannels=nChannels;
}


void Chip::setBufferDepth(unsigned bufferDepth) {
  //to do: Need to implement warnings sanity checks and resizing if necessary 
  if(_aMappingPtr != nullptr) _nChannels=_aMappingPtr->getNumberOfChannels(_chipID);
  _bufferDepth = bufferDepth;
  _chipBuffer.clear();
  //Recreate the buffer of this chip
  for (unsigned ibuf=0; ibuf < _bufferDepth; ibuf++) {
    _chipBuffer.push_back(ChipBuffer (ibuf) );
    _chipBuffer.at(ibuf).setNumberOfASICChannels(_nChannels);
  }
}

void Chip::acquireChipChannelData(Int_t *aValNHitsArray, Int_t *aValBadBcidArray, Int_t *aValArrayHigh, int  *aValArrayLow, Int_t *aGainHitArrayHigh, Int_t *aGainHitArrayLow) {
  //It is assumed that we receive a pointer to the first entry in the first buffer positions of a chip

  unsigned counter(0);

  for (unsigned ibuf=0;ibuf<_bufferDepth;ibuf++) {
    //We fill now channels until the channel counter says stop (all these are terrible hacks)
    bool allChannelsFilled(false);
    while(!allChannelsFilled) {
      //To do: How to take best into account if no more buffers with valid entries (i.e. n.e. -999) do follow
      //How to verify this independent of the input source?
      //Sure further down we check for valid triggers but still always looping over -999 is a waste of CPU time
      //How to make a check quick and bullet proof
      // Also: how to do a proper nhit counter since the nhit in the ntuple makes no sense without any further filtering of the data
      // and I would like to keep all the "filtering" code in the Channel Class.

      Int_t nhits = *(aValNHitsArray+ibuf*64/_nChannels);
      Int_t badbcid = *(aValBadBcidArray+ibuf*64/_nChannels);
      Int_t highgain = *(aValArrayHigh+counter);
      Int_t lowgain = *(aValArrayLow+counter);
      Int_t highgain_hit = *(aGainHitArrayHigh+counter);
      Int_t lowgain_hit = *(aGainHitArrayLow+counter);


      allChannelsFilled = _chipBuffer.at(ibuf).setChannelVals(
							      nhits, badbcid,
							      highgain, lowgain,
							      highgain_hit, lowgain_hit) ;

      if( nhits==0 && highgain_hit==1 )
	std::cout<<"ALERT Chip "<<nhits<<" "<< highgain <<" "<<highgain_hit<<" "<<_nChannels<<" "<<counter<<" "<<ibuf<<" "<<ibuf*64/_nChannels<<std::endl;

      counter++;
      //A stop condition for debugging purposes to avoid endless loops and segfaults
      //      if(counter==100)   allChannelsFilled=false;
    }
  }
}


//method to read the buffer information (bcid, nhits, etc)
void Chip::acquireChipBufferData(Int_t *bcidArray, Int_t *corrected_bcidArray, Int_t *badbcidArray, Int_t *nhitsArray) {

	for (unsigned ibuf=0;ibuf<_bufferDepth;ibuf++) {
	  Int_t bcid= *(bcidArray+ibuf);
	  Int_t corrected_bcid= *(corrected_bcidArray+ibuf);
	  Int_t badbcid= *(badbcidArray+ibuf);
	  Int_t nhits = *(nhitsArray+ibuf);

	  _chipBuffer.at(ibuf).setChipVals(bcid, corrected_bcid, badbcid, nhits);
	}
}


unsigned Chip::getTotalNumberOFTriggers() {
  unsigned ntot(0);
  for (unsigned ibuf=0;ibuf<_bufferDepth;ibuf++) ntot+=_chipBuffer.at(ibuf).getTotalNumberOfTriggers();
  return ntot;
}


ChipBuffer Chip::getChipBuffer(unsigned ibuf) {
  return _chipBuffer.at(ibuf);
}

unsigned Chip::getNumberOfASICChannels() {
  return _nChannels;
}

unsigned Chip::getBufferDepth() {
  return _bufferDepth;
}

double Chip::getXPadPosition(unsigned ichan) {
  double xpad(0.0);

  if (_aMappingPtr!=nullptr) {
    xpad=_aMappingPtr->getXPadPosition(_chipID, ichan);
  } else {
    std::cout << "Chip::getXPadPosition: WARNING Mapping pointer is NULL Pointer!!!!" << std::endl;
    std::cout << "Will return 0" << std::endl;
  }
  return xpad;
}

double Chip::getYPadPosition(unsigned ichan) {
  double ypad(0.0);
    
  if (_aMappingPtr!=nullptr) {
    ypad=_aMappingPtr->getYPadPosition(_chipID, ichan);
  } else {
    std::cout << "Chip::getXPadPosition: WARNING Mapping pointer is NULL Pointer!!!!" << std::endl;
    std::cout << "Will return 0" << std::endl;
  }
  return ypad;
}

