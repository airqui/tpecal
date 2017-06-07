//
//  Mapping.cpp
//  tpecalana_project
//  A class that will hold the mapping of the channel of the chips to the siwafer pads
//  including the psition of chip and channels on an ASU
//  Each type of ASU cretes a new instance
//  Created by Roman Poeschl on 07/11/15.
//
//

#include "Mapping.h"

Mapping::Mapping() {
    _aMapFileNameStr="";
    _connectionPatternIndexVec.clear();
    _chipPositionMap.clear();
    _connectionPatternVec.clear();
}

Mapping::~Mapping() {/*np op*/}

void Mapping::init(std::string aMapFileNameStr) {
    _aMapFileNameStr=aMapFileNameStr;
    createChipChannelMapping();
}

void Mapping::createChipChannelMapping() {
    simple_file_parser sfp;
    if (!sfp.open(_aMapFileNameStr)) {
        MYERR("cannot read file: '" << _aMapFileNameStr << "'");
    }
    // Describe additional single character delimiters
    // so that it will break out 'foo+bar' as 3 tokens
    // instead of 1.
    sfp.set_single_char_tokens(";+");
    // Read each line. Comments and blank lines are ignored.
    //Define a helper variable to recognize a new chip
    unsigned lastChip(0);
    //...and a bool to check whether we are at the beginning of the chip handling
    bool isfirstChip(true);
    while (sfp.get_next_line()) {
        /*Disable print out from parser as we know that it is correct
         // Print out the tokens for each line.
         cout << "Line " << sfp.get_line_number() << " has "
         << sfp.get_num_tokens() << " tokens" << endl;
         for(size_t i=0;i<sfp.get_num_tokens();i++) {
         cout << "   token["
         << setw(2) << setfill('0') << right << i
         << left << setfill(' ') << "] = "
         << "'" << sfp.get_token(i) << "'"
         << endl;
         }*/
        //Pass the information to a method that finally handles the mapping data
        //There may be a more clever vay
        //A valid line start with an unsigned int (the chip number)
        if(sfp.is_token_uint(0)) {
            //std::cout << "Mapping: Valid line found, pass it to the handler method!!!" << std::endl;
            //Account for cases in which the mapping file does not start with chip 0
            if (isfirstChip) {
                lastChip=sfp.get_token_uint(0);
                isfirstChip=false;
            }
            if (sfp.get_token_uint(0) != lastChip && !isfirstChip) {
                //std::cout << "Mapping::createChipChannelMapping: Checking now chip " << sfp.get_token_uint(0) << " for its mapping" << std::endl;
                checkConnectionPattern();
                lastChip=sfp.get_token_uint(0);
            }
            handleMappingData(sfp.get_token_uint(0), std::stod(sfp.get_token(1)), std::stod(sfp.get_token(2)),sfp.get_token_uint(3),std::stod(sfp.get_token(4)), std::stod(sfp.get_token(5)));
        }
    }
    //The check of the connection pattern for the last chip that has been handled
    checkConnectionPattern();
    //At the end of this method we should have the channel mapping, a list of connection patterns and the association of each chip to a connection pattern. We will validate thus in a dedicated method that will only be enabled for debugging purposes
    //verifyMapping();
}

void Mapping::handleMappingData(unsigned ichip, double xchip, double ychip, unsigned ichan, double xchan, double ychan) {

  /*
  
    std::cout << "***** This is the Mapping Handler *****" << std::endl;
    std::cout << "Chip number is: " << ichip << std::endl;
    std::cout << "xy position of chip [" << ichip << "] is: xchip=" << xchip <<", ychip="<<  ychip << std::endl;
    std::cout << "Channel number is: " << ichan << std::endl;
    std::cout << "xy position of channel [" << ichan << "] is: xchan=" << xchan <<", ychan="<<  ychan << std::endl;
    std::cout << "*************************************" << std::endl;
  */
    
    
    if(!_chipPositionMap.count(ichip)) {
        _chipPositionMap.insert(std::make_pair(ichip, std::make_pair(xchip, ychip)));
        //std::cout << "Mapping Handler: New Chip added, map size is now: " << _chipPositionMap.size() << std::endl;
    }
    //std::cout << "Channel number is: " << ichan << std::endl;
    if(_channelPositionVec.size()<ichan+1) _channelPositionVec.resize(ichan+1);
    //std::cout << "Size of channel position vector after resize is: " << _channelPositionVec.size() << std::endl;
    _channelPositionVec.at(ichan)=std::make_pair(xchan-xchip, ychan-ychip);
    //std::cout << "Size of channel position vector is: " << _channelPositionVec.size() << std::endl;
}

void Mapping::checkConnectionPattern() {
    bool isNewPattern(true);
    unsigned patternIndex(0);
    //We always store the first connection pattern that we encounter
    if(_connectionPatternVec.size()!=0) {
        //Now we have to check whether this pattern does exist already
        //Run a loop over the pattern vecto
        unsigned ipat(0);
        for (std::vector<channelPositionVec_t>::iterator conpatvec_iter=_connectionPatternVec.begin(); conpatvec_iter !=_connectionPatternVec.end(); conpatvec_iter++) {
            //Check whether the size of the already stored pattern vector has the same size as the one we would like to check
            //In general this may happen but for the Ecal prototype with SKIROC2 this should not happen so we issue an error message.
            if ((*conpatvec_iter).size() != _channelPositionVec.size()) {
                std::cout << "Error: Non equal number of channels for different chips" << std::endl;
                std::cout << "Size of existing vector is: " << (*conpatvec_iter).size() << std::endl;
                std::cout << "Size of tested vector is: " << _channelPositionVec.size() << std::endl;
                isNewPattern=false;
            } else {
                //Now we are ready to compare!!!
                if(compareArrays((*conpatvec_iter), _channelPositionVec)) {
                    isNewPattern=false;
                    patternIndex=ipat;
                }
            }//else of channel number consistency check
            ipat++;
        }//Loop over connection patterns
    }//Size of connection pattern vector
    //Update the information for the chip under study
    //The chip that we are testng is the last that has been added to the chip chippositionmap
    ChipPositionMap_t::reverse_iterator rit=_chipPositionMap.rbegin();
    //std::cout << "Mapping::CheckConnectionPattern: chip to test: " << (rit->first) << std::endl;
    if(_connectionPatternIndexVec.size()<(rit->first)+1 ) _connectionPatternIndexVec.resize((rit->first)+1);
    if(isNewPattern) {
        _connectionPatternVec.push_back(_channelPositionVec);
        //There is at least one pattern => the pattern vector should have at least one entry!!!!
        //_connectionPatternIndexVec.insert(_connectionPatternIndexVec.begin()+(rit->first),_connectionPatternVec.size()-1);
        _connectionPatternIndexVec.at(rit->first)=_connectionPatternVec.size()-1;
        //std::cout << "New pattern found!!!" << std::endl;
    
    } else _connectionPatternIndexVec.at(rit->first)=patternIndex;

    _channelPositionVec.clear();
}

bool Mapping::compareArrays(channelPositionVec_t firstVec, channelPositionVec_t secondVec) {
    bool isEqual(false);
    //Define two help vector in which the double values will be transformed into integer values
    //At the end this a transformation mm -> mum
    int mmtomum(1000);
    std::vector<std::pair<int, int> > firstIntVec, secondIntVec;
    firstIntVec.clear();
    secondIntVec.clear();
    //Transform first array
    for (channelPositionVec_t::iterator veciter = firstVec.begin(); veciter != firstVec.end(); veciter++) {
        //int ix= int((*veciter).first+mmtomum);
        //int iy= int((*veciter).second+mmtomum);
        firstIntVec.push_back(std::make_pair(int((*veciter).first+mmtomum),int((*veciter).second+mmtomum)));
    }
    //Transform second array
    for (channelPositionVec_t::iterator veciter = secondVec.begin(); veciter != secondVec.end(); veciter++) {
        //int ix= int((*veciter).first+mmtomum);
        //int iy= int((*veciter).second+mmtomum);
        secondIntVec.push_back(std::make_pair(int((*veciter).first+mmtomum),int((*veciter).second+mmtomum)));
    }
    //Compare the two integer vectors
    if (firstIntVec==secondIntVec) isEqual=true;
    return isEqual;
}

void Mapping::verifyMapping() {

    //Start with the psitions of the chips
    std::cout << "The ASU carries " << _chipPositionMap.size() << " ASICs" << std::endl;
    for (ChipPositionMap_t::iterator chippositer=_chipPositionMap.begin(); chippositer != _chipPositionMap.end(); chippositer++) std::cout << "ASIC [" << (*chippositer).first << "]:" << "xchip=" << (*chippositer).second.first << ", " << "ychip=" << (*chippositer).second.second << std::endl;
     
    
    std::cout << "The ASU features " << _connectionPatternVec.size() << " Connection patterns" << std::endl;
    unsigned ipat(0);
    for (std::vector<channelPositionVec_t>::iterator connpatveciter=_connectionPatternVec.begin() ;connpatveciter!=_connectionPatternVec.end(); connpatveciter++) {
        std::cout << "Listing of connection pattern " << ipat << std::endl;
        unsigned ichan(0);
        for (channelPositionVec_t::iterator posvec_iter=(*connpatveciter).begin();posvec_iter!=(*connpatveciter).end() ; posvec_iter++) {
            std::cout << "Channel[" << ichan << "]: xchan=" << (*posvec_iter).first << ", ychan=" << (*posvec_iter).second << std::endl;
            ichan++;
        }
        ipat++;
    }
    
    std::cout << "Listing of assignemnt between ASICs and index of connection pattern" << std::endl;
    unsigned ichip(0);
    for (std::vector<unsigned>::iterator index_iter=_connectionPatternIndexVec.begin(); index_iter!=_connectionPatternIndexVec.end(); index_iter++) {
        std::cout << "ASIC[" << ichip <<"]: Pattern index=" << (*index_iter) << std::endl;
        ichip++;
    }
    
}

unsigned Mapping::getNumChips() { return _chipPositionMap.size();}

unsigned Mapping::getNumberOfChannels(unsigned chipid) {
    unsigned nchan(0);
    if(chipid >_connectionPatternIndexVec.size()-1) {
        std::cout << "WARNING: ASIC " << chipid << "doesn't exist!!!"  << std::endl;
        std::cout << "Maximum number of ASIC is " << _connectionPatternIndexVec.size()-1 << std::endl;
        std::cout << "Will return 0" << std::endl;
    } else {
        //All is fine
        nchan = _connectionPatternVec.at(_connectionPatternIndexVec.at(chipid)).size();
    }
    return nchan;
}

double Mapping::getXPadPosition(unsigned chipid, unsigned chanid) {
    return _connectionPatternVec.at(_connectionPatternIndexVec.at(chipid)).at(chanid).first;
}

double Mapping::getYPadPosition(unsigned chipid, unsigned chanid) {
   return _connectionPatternVec.at(_connectionPatternIndexVec.at(chipid)).at(chanid).second;
}

std::vector<double> Mapping::getChannelPosition(unsigned chipid, unsigned chanid) {

  std::pair<double,double> xychip=_chipPositionMap.at(chipid);
  double xchip=xychip.first;
  double ychip=xychip.second;
  //  std::cout<<xchip<<" " <<ychip<<endl;
      
  std::vector<double> xy;
  xy.push_back(getXPadPosition(chipid,chanid)+xchip);
  xy.push_back(getYPadPosition(chipid,chanid)+ychip);
	       
  return xy;
}

