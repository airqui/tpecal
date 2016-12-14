//
//  Channel.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#include "Channel.h"
#include <cmath>
#include "ChipBuffer.h"

Channel::Channel(unsigned ichan){
    //Set the channel ID
    _ichan = ichan;
    //reset the sum of the vals qnd their squares
    //-- for all entries
    _valsumHigh=0;
    _val2sumHigh=0;
    _valsumLow=0;
    _val2sumLow=0;
    //-- hit bit == 1 entries
    _valsumHigh_1=0;
    _val2sumHigh_1=0;
    _valsumLow_1=0;
    _val2sumLow_1=0;
    //-- hit bit == 0 entries
    _valsumHigh_0=0;
    _val2sumHigh_0=0;
    _valsumLow_0=0;
    _val2sumLow_0=0;

    //  if(_pedHighTH1 == NULL) 
    // delete _pedHighTH1 ;
    // _pedHighTH1 = new TH1F("_pedHighTH1","_pedHighTH1",4096,0.5,4096.5);
    // _pedLowTH1 = new TH1F("_pedLowTH1","_pedLowTH1",4096,0.5,4096.5);
    // _HighTH1 = new TH1F("_HighTH1","_HighTH1",4096,0.5,4096.5);
    // _LowTH1 = new TH1F("_LowTH1","_LowTH1",4096,0.5,4096.5);

    //reset the number of entries
    _numEntr=0;
    //reset the number of triggers
    _nTriggers=0;
    //reset the number of undefined entries )can happen for buffers > 1 in case of 'clean' fonctioning i.e. no retriggering bcid+1 etc.)
    _nUndefined=0;
}


Channel::~Channel(){/*no op*/}

void Channel::init(){/*no op*/}


double Channel::getMean(std::string modeStr, int trigger = 1) {
  
  //noise signal
  if (trigger==0 && (_numEntr-_nTriggers) > 0) {
    if (modeStr=="High")return static_cast<double> (_valsumHigh_0) / static_cast<double>((_numEntr-_nTriggers));
    if (modeStr=="Low")return static_cast<double> (_valsumLow_0) / static_cast<double>((_numEntr-_nTriggers));
  }
  
  //hit bit == 1
  if (trigger==1 && _nTriggers > 0) {
    if (modeStr=="High")return static_cast<double> (_valsumHigh_1) / static_cast<double>(_nTriggers);
    if (modeStr=="Low")return static_cast<double> (_valsumLow_1) / static_cast<double>(_nTriggers);
  }
  
  // all hits and noise
  if (trigger==-1 && _numEntr > 0) {
    if (modeStr=="High")return static_cast<double> (_valsumHigh) / static_cast<double>(_numEntr);
    if (modeStr=="Low")return static_cast<double> (_valsumLow) / static_cast<double>(_numEntr);
  }

  if (trigger!=-1 && trigger!=0 && trigger !=1) std::cout<< "ERROR Wrong setup of the hit bit choice "<<std::endl;
    return 0.0;
}

double Channel::getRMS(std::string modeStr, int trigger = 1 ) {
    
  
  if (trigger == 0 && (_numEntr-_nTriggers) > 0) {
    double meanVal = getMean(modeStr,trigger);
    double meanVal2 = meanVal*meanVal;
    double mean2Val(0.0);
    if(modeStr=="High") mean2Val =  static_cast<double> (_val2sumHigh_0) / static_cast<double>(_numEntr);
    if(modeStr=="Low") mean2Val =  static_cast<double> (_val2sumHigh_0) / static_cast<double>(_numEntr);
    return sqrt(mean2Val - meanVal2  );
  }
  
  if (trigger == 1 && _nTriggers > 0) {
    double meanVal = getMean(modeStr,trigger);
    double meanVal2 = meanVal*meanVal;
    double mean2Val(0.0);
    if(modeStr=="High") mean2Val =  static_cast<double> (_val2sumHigh_1) / static_cast<double>(_nTriggers);
    if(modeStr=="Low") mean2Val =  static_cast<double> (_val2sumHigh_1) / static_cast<double>(_nTriggers);
    return sqrt(mean2Val - meanVal2  );
  }
  
  if (trigger == -1 && _numEntr > 0) {
    double meanVal = getMean(modeStr,trigger);
    double meanVal2 = meanVal*meanVal;
    double mean2Val(0.0);
    if(modeStr=="High") mean2Val =  static_cast<double> (_val2sumHigh) / static_cast<double>(_numEntr);
    if(modeStr=="Low") mean2Val =  static_cast<double> (_val2sumHigh) / static_cast<double>(_numEntr);
    return sqrt(mean2Val - meanVal2  );
  }
    
    return 0.0;
}

void Channel::acquireData(int valHigh, int valLow, int gainHitHigh, int gainHitLow) {


  //Count number of entries
  if( valHigh>0 ) _numEntr++;
  //Count number of triggers
  if (gainHitHigh == 1 && valHigh>10) _nTriggers++;
  //Count number of undefined entries
  if (gainHitHigh < 0) _nUndefined++;
  //std::cout << "example val: " <<  val << std::endl;
  calculateSums(valHigh, valLow, gainHitHigh, gainHitLow);
}


void Channel::calculateSums(int valHigh, int valLow, int gainHitHigh, int gainHitLow) {

    // all hits and noise
    _valsumHigh += valHigh;
    _valsumLow += valLow;
    _val2sumHigh += valHigh*valHigh;
    _val2sumLow += valLow*valLow;

     // only hits
    if(gainHitHigh==1) {
      _valsumHigh_1 += valHigh;
      _val2sumHigh_1 += valHigh*valHigh;
    }
    if(gainHitLow==1) {
      _valsumLow_1 += valLow;
      _val2sumLow_1 += valLow*valLow;
    }

    // only noise
    if(gainHitHigh==0) {
      _valsumHigh_0 += valHigh;
      _val2sumHigh_0 += valHigh*valHigh;
    }
    if(gainHitLow==0) {
      _valsumLow_0 += valLow;
      _val2sumLow_0 += valLow*valLow;
    }

}


unsigned Channel::getNEntries() {return _numEntr;}
unsigned Channel::getNTriggers() {return _nTriggers;}
unsigned Channel::getNUndefined() {return _nUndefined;}

