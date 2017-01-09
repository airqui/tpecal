//
//  Channel.h
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#ifndef __cobana_project__Channel__
#define __cobana_project__Channel__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <TH1F.h>

class Channel {
    
public:
    Channel(unsigned);
    ~Channel();
    void init();
    //A method to acquire data 
    void acquireData(int, int, int, int, int, int);
    //Return the number of entries
    unsigned getNEntries();
    //Return the number of triggers
    unsigned getNTriggers();
    unsigned getNTriggers_planeEvents();
    unsigned getNTriggers_consBcid1();
    unsigned getNTriggers_consBcid5();
    unsigned getNTriggers_consBcid10();
    unsigned getNTriggers_negativeData();
    //Return the number of undefined entries
    unsigned getNUndefined();
    //Return the mean of that channel
    double getMean(std::string, int);
    //Return the rms of that channel
    double getRMS(std::string, int);


private:
    //The channelID
    unsigned _ichan;
    //histograms

    //data members that hold the sum of the measured values and their squares, for hits and noise
    int _valsumHigh;
    int _val2sumHigh;
    int _valsumLow;
    int _val2sumLow;
    //data members that hold the sum of the measured values and their squares, for only hits
    int _valsumHigh_1;
    int _val2sumHigh_1;
    int _valsumLow_1;
    int _val2sumLow_1;
    //data members that hold the sum of the measured values and their squares, for only noise
    int _valsumHigh_0;
    int _val2sumHigh_0;
    int _valsumLow_0;
    int _val2sumLow_0;
    //A data member that holds the number of entries for each buffer
    unsigned _numEntr;
    //A data member that holds the number of triggers for that channel
    unsigned _nTriggers;
    unsigned _nTriggers_planeEvents;
    unsigned _nTriggers_consBcid1;
    unsigned _nTriggers_consBcid5;
    unsigned _nTriggers_consBcid10;
    unsigned _nTriggers_negativeData;

    //A data memeber that holds the number of undefined entries (<0)
    unsigned _nUndefined;
    //Method to sum up the measured values and their squares
    void calculateSums(int, int, int, int);

};


#endif /* defined(__cobana_project__Channel__) */
