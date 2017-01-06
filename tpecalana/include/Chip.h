//
//  Chip.h
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#ifndef __cobana_project__Chip__
#define __cobana_project__Chip__

#include <iostream>
#include <stdio.h>
//#include <map>
#include <vector>
#include "Mapping.h"
#include "ChipBuffer.h"

class Chip {
    
public:
    Chip(unsigned);
    ~Chip();
    //initialisation
    void init(Mapping*);
    //A method to overwrite the bufferdpeth set in the constructor
    void setBufferDepth(unsigned);
    //A method to the number of channels of the ASIC "by hand" set in the
    void setNumberOfASICChannels(unsigned);
    //Acquire information that contain global information on the buffers of a chip (at the moment we receive an array which is ugly)
    void acquireChipBufferData(int*, int*, int*);
    //Acquire the data that belong to the channel of this chip (at the moment we receive an array which is ugly)
    void acquireChipChannelData(int*, int*, int*, int*, int*, int*);
    //Return the number of channels connected to the ASIC
    unsigned getNumberOfASICChannels();
    //Return the bufferdepth
    unsigned getBufferDepth();
    //Return a given ChipBuffer
    ChipBuffer getChipBuffer(unsigned);
    //A method to retrieve the x-position of a given channel
    double getXPadPosition(unsigned);
    //A method to retrieve the y-position of a given channel
    double getYPadPosition(unsigned);
    //A method to return the total number of triggers of this chip
    unsigned getTotalNumberOFTriggers();
    
private:
    //The ID of the chip this can be simple numbering or a hardware address, may need to handle both in the future
    unsigned _chipID;
    //The buffers depth;
    unsigned _bufferDepth;
    //The container that holds the channels
    std::vector<ChipBuffer> _chipBuffer;
    //The mapping object that contains information about the actual mapping of the ASIC
    Mapping* _aMappingPtr;
    //a map that holds entries for each buffer for each channel
    //typedef std::map<unsigned, std::vector<unsigned> > chipBufferMapT;
    //chipBufferMapT _chipBufferMap;
    unsigned _nChannels;

};



#endif /* defined(__cobana_project__Chip__) */
