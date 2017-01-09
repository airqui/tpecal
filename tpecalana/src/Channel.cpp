//
//  Channel.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#include "Channel.h"
#include "ChipBuffer.h"
#include "global.h"
#include <cmath>

using namespace globalvariables;

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

  //reset the number of entries
  _numEntr=0;
  //reset the number of triggers
  _nTriggers=0;
  _nTriggers_planeEvents=0;
  _nTriggers_consBcid1=0;
  _nTriggers_consBcid5=0;
  _nTriggers_consBcid10=0;
  _nTriggers_negativeData=0;
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

void Channel::acquireData(int nhits, int badbcid, int valHigh, int valLow, int gainHitHigh, int gainHitLow) {

  int gain = globalvariables::getGainAnalysis();
  int thresh = globalvariables::getPlaneEventsThreshold();

  if(gain == 1) {
    //Count number of entries
    if( valHigh>0 ) _numEntr++;
    if( gainHitHigh == 1 ) {
      //Count number of triggers
      if (  badbcid == 1   &&  valHigh>50 && nhits < thresh  ) _nTriggers_consBcid1++;
      if (  (badbcid >1 && badbcid < 6)   && valHigh>50 && nhits < thresh  ) _nTriggers_consBcid5++;
      if (  (badbcid >5 && badbcid < 11)   &&  valHigh>50 && nhits < thresh  ) _nTriggers_consBcid10++;
      if (  ( badbcid == 0 || (badbcid >5 && badbcid<11) )   &&  valHigh>50 && nhits < thresh ) _nTriggers++;
      else {
	if (nhits > (thresh-1) ) _nTriggers_planeEvents++;
	if ((badbcid > 30 || valHigh < 50 ) ) _nTriggers_negativeData++;
      }
    }
    //Count number of undefined entries
    if (gainHitHigh < 0) _nUndefined++;

  } else if(gain == 0) {
    //Count number of entries
    if( valLow>0  ) _numEntr++;

    if( gainHitLow == 1 ) {
      //Count number of triggers
      if (  badbcid == 1   &&  valLow>50 && nhits < thresh  ) _nTriggers_consBcid1++;
      if (  (badbcid >1 && badbcid < 6)   && valLow>50 && nhits < thresh  ) _nTriggers_consBcid5++;
      if (  (badbcid >5 && badbcid < 11)   &&  valLow>50 && nhits < thresh  ) _nTriggers_consBcid10++;
      if (  ( badbcid == 0 || (badbcid >5 && badbcid<11) )  &&  valLow>50 && nhits < thresh ) _nTriggers++;
      else {
	if (nhits > (thresh-1) ) _nTriggers_planeEvents++;
	if ((badbcid > 30 || valLow < 50 ) ) _nTriggers_negativeData++;
      }
    }
    
    //Count number of undefined entries
    if (gainHitLow < 0) _nUndefined++;

  } else std::cout<<"ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;
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
unsigned Channel::getNTriggers_planeEvents() {return _nTriggers_planeEvents;}
unsigned Channel::getNTriggers_consBcid1() {return _nTriggers_consBcid1;}
unsigned Channel::getNTriggers_consBcid5() {return _nTriggers_consBcid5;}
unsigned Channel::getNTriggers_consBcid10() {return _nTriggers_consBcid10;}
unsigned Channel::getNTriggers_negativeData() {return _nTriggers_negativeData;}
unsigned Channel::getNUndefined() {return _nUndefined;}

