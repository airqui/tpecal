//
//  ASU.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//   
//

#include "ASU.h"
#include "ExperimentalSetup.h"

ASU::ASU(unsigned asuID){
  //The ASU ID
  _asuID = asuID;
  //The number of chips carried by this ASU
  _numChips = 0;
  //reset the ChipVector
  _chipVec.clear();
  //reset the data container
  _theTree = NULL;
}

ASU::~ASU(){/* no op*/}

void ASU::init(std::string asicType, unsigned itype, unsigned idif) {
  //Remark the idif information is only needed in case no Mappingobject exists for this ASU.
  //In this case we have to consult the default description in the ExperimentalSetup
  //Set the asic type
  _asicType=asicType;
  Mapping* aMappingPtr=ExperimentalSetup::getInstance()->getMapping(itype);
  if (aMappingPtr != nullptr) {
    _numChips = aMappingPtr->getNumChips();
    //We'll make here a consisteny check on the number of channels for
    //each ASIC. In principle this belongs to a test to be carried
    //out for each chip but since we assume that a given ASU carries also
    //Get the nominal value expected for this chip from the ExperimentalSetup
    unsigned numChannelsNom(ExperimentalSetup::getInstance()->getNumberofASICChannels(asicType));
    //Create a vector with the number of Chips carried by this ASU
    for (unsigned ichip=0; ichip < _numChips; ichip++) {
      if ( aMappingPtr->getNumberOfChannels(ichip) == numChannelsNom ) {
	_chipVec.push_back(Chip (ichip) );
	_chipVec.at(ichip).init(aMappingPtr);
	_chipVec.at(ichip).setBufferDepth(ExperimentalSetup::getInstance()->getASICBufferDepth(asicType));
      } else {
	std::cout << "ASU::init: ERROR: Number of channels don't match on ASU " << _asuID <<  " for ASIC " << ichip <<std::endl;
	std::cout << "Expected/Nominal number of channels is: " << numChannelsNom << std::endl;
	std::cout << "Retrieved number of channels is: " << aMappingPtr->getNumberOfChannels(ichip) << std::endl;
      }//Consistency checl for number of channels
    }//loop over chips
  } else {
    std::cout << "ASU::init: WARNING: Retrieved NULL Pointer for the Mapping object for ASU " << _asuID << std::endl;
    std::cout << "Therefore we can't retrieve information from the input file" << std::endl;
    std::cout << "Maybe this is ok in case one doesn't have a proper mapping file" << std::endl;
    std::cout << "Will use defaults from the ExperimentalSetup instance!!!!!!" << std::endl;
    _numChips=ExperimentalSetup::getInstance()->getNumberofASICs(idif, _asuID);
    for (unsigned ichip=0; ichip < _numChips; ichip++) {
      _chipVec.push_back(Chip (ichip) );
      _chipVec.at(ichip).init(nullptr);
      _chipVec.at(ichip).setNumberOfASICChannels(ExperimentalSetup::getInstance()->getNumberofASICChannels(asicType));
      _chipVec.at(ichip).setBufferDepth(ExperimentalSetup::getInstance()->getASICBufferDepth(asicType));
    }
     
  }
    
}//method delimiter



void ASU::transferChipChannelData(int* chipid, int *aValNHitsArray, int *aValBadBcidArray, int *aValArrayHigh, int  *aValArrayLow, int *aGainHitArrayHigh, int *aGainHitArrayLow) {
    
  for (unsigned ichip=0;ichip<_numChips;ichip++) {
    unsigned istep(_chipVec.at(ichip).getNumberOfASICChannels()*_chipVec.at(ichip).getBufferDepth());
    unsigned istep_buf(_chipVec.at(ichip).getBufferDepth());

    if (chipid[ichip] > -1 ) {
      if (chipid[ichip] < static_cast<int>(_numChips)) {
	_chipVec.at(chipid[ichip]).acquireChipChannelData(
							  (aValNHitsArray+ichip*istep_buf),(aValBadBcidArray+ichip*istep_buf),
							  (aValArrayHigh+ichip*istep), (aValArrayLow+ichip*istep),
							  (aGainHitArrayHigh+ichip*istep), (aGainHitArrayLow+ichip*istep));
      } else std::cout << "Info: chipid > 15 encountered! " << "Chipid is: " << chipid[ichip] << std::endl;
    }
  }
}

void ASU::transferChipBufferData(int* chipid, int *bcid, int  *corrected_bcid, int *badbcid, int *nhits) {
    
  for (unsigned ichip=0;ichip<_numChips;ichip++) {
    unsigned istep(_chipVec.at(ichip).getBufferDepth());
    if (chipid[ichip] > -1 ) {
      if (chipid[ichip] < static_cast<int>(_numChips)) {
	_chipVec.at(chipid[ichip]).acquireChipBufferData(
							 (bcid+ichip*istep), (corrected_bcid+ichip*istep), (badbcid+ichip*istep), (nhits+ichip*istep)
							 );
      } else std::cout << "Info: chipid > 15 encountered! " << "Chipid is: " << chipid[ichip] << std::endl;
    }
  }

}


unsigned ASU::getTotalNumberOFTriggers() {
  unsigned ntot(0);
  for (unsigned ichip=0;ichip<_numChips;ichip++) ntot+=_chipVec.at(ichip).getTotalNumberOFTriggers();
  return ntot;
}


Chip ASU::getChip(unsigned ichip) {
  return _chipVec.at(ichip);
}

unsigned ASU::getNumberofASICs() {
  return _numChips;
}
