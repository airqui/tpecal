//
//  ASU.h
//  (Comment R.P. 13/8/15: Functionality will become integrated into the DIF or (a new) Slab class in the future as the DIF may not be able to distinguish between different ASUs but sees only connected chips.)
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#ifndef __cobana_project__ASU__
#define __cobana_project__ASU__

#include <stdio.h>
#include <iostream>
#include "Chip.h"
#include "TTree.h"

//using namespace std;

class ASU {
    
public:
    ASU(unsigned);
    ~ASU();
    //On initialisation we pass the ASU type to the instance
    //This contains information about the ASICs implemented on the board (we can safely assume that only one
    //ASIC type is implemented on a given board)
    //and the connection type chosen for thisboard
    void init(std::string, unsigned, unsigned);
    //The next two methods extract information from the data container. For convenience the information is extracted
    //seperately for the global buffer and individual channel information
    //It has to be made sure that the information is kept parallel
    //The method to extract the information from the data container (global buffer information)
    void transferChipBufferData(Int_t*, Int_t*, Int_t*, Int_t*);
    //The method to extract the information from the data container (individual channels)
    void transferChipChannelData(Int_t*, Int_t*, Int_t*, Int_t*, Int_t*, Int_t*, Int_t*);
    //Return the number of ASICs
    unsigned getNumberofASICs();
    //Return a given Chip
    Chip getChip(unsigned);
    //Return the total number of triggers for this ASU (integrated over all chips, channels and buffers
    unsigned getTotalNumberOFTriggers();
    
private:
    //The ID of the ASU (may be a simple number or a hardware ID (may need to extend the class to deal with both
    unsigned _asuID;
    //THe number of Chips carried by this ASU
    unsigned _numChips;
    //The container that holds the ASUs
    std::vector<Chip> _chipVec;
    //The type of ASICs implemented on this ASU
    std::string _asicType;
    //The data member that holds he data container to be analysed
    TTree* _theTree;
};



#endif /* defined(__cobana_project__ASU__) */
