//
//  Dif.h
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#ifndef __cobana_project__Dif__
#define __cobana_project__Dif__

#include <stdio.h>
#include <vector>
#include "ASU.h"
#include "TFile.h"

class Dif {
    
public:
    Dif(unsigned);
    ~Dif();
    //Initialisation of class members after having received the input file
    void init(std::vector< std::pair<std::string, std::pair<unsigned,unsigned> > > );
    //The method in which it all happens
    void dataAnalysis(TFile*);
    //Return a given ASU
    //ASU getASU(unsigned);
    ASU getASU(unsigned);
    //return the difstat vector
    std::vector<std::pair<unsigned, unsigned> > getDifStatVec();

private:
    //The ID of the chips (might be a simple counter or a hardware id)
    unsigned _difID;
    //The number of ASUs served by this DIF
    unsigned _numASU;
    //The container that holds the ASUs
    std::vector<ASU> _asuVec;
    TFile* _theDifFile;
    //Create a vector of pairs
    typedef std::vector<std::pair<unsigned, unsigned> > difStatVec_t;
    difStatVec_t _difStatVec;
    //A method to store the information
    void storeAcquisitionInfo(unsigned, unsigned);
};



#endif /* defined(__cobana_project__Dif__) */
