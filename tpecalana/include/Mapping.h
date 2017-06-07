//
//  Mapping.hpp
//  tpecalana_project
//
//  Created by Roman Poeschl on 07/11/15.
//
//

#ifndef Mapping_hpp
#define Mapping_hpp
#define MYERR(msg) cerr << endl << "ERROR:" << __FILE__ << ":" << __LINE__ << ": " << msg << endl; exit(1)

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include "simple_file_parser.h"

using namespace std;

class Mapping {
    

public:
    Mapping();
    ~Mapping();
    void init(std::string);
    //Define a set of getter functions to retrieve the stored information
    //Number of chips
    unsigned getNumChips();
    //Number of channels of a Chip
    unsigned getNumberOfChannels(unsigned);
    //x-position of pad
    double getXPadPosition(unsigned, unsigned);
    //y-position of pad
    double getYPadPosition(unsigned, unsigned);
    std::vector<double> getChannelPosition(unsigned, unsigned);
private:
    //A method to create the Chip/Channel Mapping
    void createChipChannelMapping();
    std::string _aMapFileNameStr;
    //A method to acquire the information read of from the mapping file
    void handleMappingData(unsigned, double, double, unsigned, double, double);
    //A method to detect different connection patterns within the mapping
    void checkConnectionPattern();
    //Vector that stores the index of the connection pattern for each chip
    std::vector<unsigned> _connectionPatternIndexVec;
    //Simple map that stores the xy position of a chip (map makes sense since chip id occurs often current mapping file)
    typedef std::map<unsigned, std::pair<double,double> > ChipPositionMap_t;
    ChipPositionMap_t _chipPositionMap;
    //Vector of pairs that holds the xy position of a channel (pad) w.r.t. the reference of the chip
    //Each set will be stored in a vector for resusage if it repeats for other chips
    typedef std::vector<std::pair<double, double> > channelPositionVec_t;
    channelPositionVec_t _channelPositionVec;
    //Vector that holds the different sets of xy positions
    std::vector<channelPositionVec_t> _connectionPatternVec;
    //The position of in the vector defines the connectionPatternIndex as introduced above
    //In this methid we simply convert the arrays of doubles into arrays of ints and compare these as it is
    //dangerous and machine dependent to compare double values for equality
    bool compareArrays(channelPositionVec_t,channelPositionVec_t);
    //A method to verify the mapping
    void verifyMapping();
};



#endif /* Mapping_hpp */
