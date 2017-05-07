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
  _nPedestals=0;
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


void Channel::acquireData(Int_t nhits, Int_t badbcid, Int_t correctedbcid, Int_t valHigh, Int_t valLow, Int_t gainHitHigh, Int_t gainHitLow) {

  Int_t gainAnalysis = globalvariables::getGainAnalysis();
  if(gainAnalysis == 1) acquireDataGain(nhits, badbcid, correctedbcid, valHigh, gainHitHigh);
  else if(gainAnalysis == 0) acquireDataGain(nhits, badbcid, correctedbcid, valLow, gainHitLow);
  else std::cout<<"ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;

}

void Channel::acquireDataGain(Int_t nhits, Int_t badbcid, Int_t correctedbcid, Int_t val, Int_t gainHit) {

  Int_t thresh = globalvariables::getPlaneEventsThreshold();
  Int_t bcid_thresh = globalvariables::getMinBCIDThreshold();

  if(correctedbcid > bcid_thresh && nhits>0){ 
    
    //Count number of entries
    if( val>0 ) _numEntr++;
    
    // Make filtering for pedestal and trigger counts
    // All selection stuff is hardcoded here...  BAD!!
    
    if (gainHit >-0.5  &&  val>10 && badbcid == 0 && nhits <= thresh){//&&  val>10 && badbcid <30 && nhits < thresh ) {
      calculateSums(val,gainHit);
      if (gainHit == 0 )   _nPedestals++;
      if (gainHit == 1 )    _nTriggers++;
    }
    
    // Clasify and count the events not included in the filtering, with consequtive bcid
    if( badbcid> 0 && gainHit == 1 &&  val>10 ) {
      if (  badbcid == 1   && nhits <= thresh ) _nTriggers_consBcid1++;
      else if (  (badbcid >1 && badbcid < 6)  && nhits <= thresh  ) _nTriggers_consBcid5++;
      else if( (badbcid >5 && badbcid < 16)  && nhits <= thresh   ) _nTriggers_consBcid10++;
      else if( (badbcid>30 || nhits > thresh) ) _nTriggers_planeEvents++;  
    }  
    
  
    if( gainHit == 1 &&  val > 0 && val < 10   )  _nTriggers_negativeData++;
    //Count number of undefined entries
    if (gainHit < 0 || gainHit>2) _nUndefined++;
 
  }
  
}



void Channel::calculateSums(int val, int gainHit) {

  Int_t gainAnalysis = globalvariables::getGainAnalysis();

  //high gain data
  if( gainAnalysis == 1 ) {
    // all hits and noise
    _valsumHigh += val;
    _val2sumHigh += val*val; 
    // only hits
    if(gainHit==1) {
      _valsumHigh_1 += val;
      _val2sumHigh_1 += val*val;
    } 
    // only noise
    if(gainHit==0) {
      _valsumHigh_0 += val;
      _val2sumHigh_0 += val*val;
    }
  }

  //low gain data analysis
  if(gainAnalysis == 0)  {
    // all hits and noise
    _valsumLow += val;
    _val2sumLow += val*val; 
    // only hits
    if(gainHit==1) {
      _valsumLow_1 += val;
      _val2sumLow_1 += val*val;
    } 
    // only noise
    if(gainHit==0) {
      _valsumLow_0 += val;
      _val2sumLow_0 += val*val;
    }
  }


}


unsigned Channel::getNEntries() {return _numEntr;}

std::vector<unsigned> Channel::getNTriggersVec() {
  std::vector<unsigned> _nTriggersVec;
  _nTriggersVec.push_back(_nTriggers);
  _nTriggersVec.push_back(_nPedestals);
  _nTriggersVec.push_back(_nTriggers_consBcid1);
  _nTriggersVec.push_back(_nTriggers_consBcid5);
  _nTriggersVec.push_back(_nTriggers_consBcid10);
  _nTriggersVec.push_back(_nTriggers_planeEvents);
  _nTriggersVec.push_back(_nTriggers_negativeData);
  _nTriggersVec.push_back(_nUndefined);
  return _nTriggersVec;
}

double Channel::getMean(std::string modeStr, int trigger = 1) {
  
  //noise signal
  if (trigger==0 && _nPedestals > 0) {
    if (modeStr=="High")return static_cast<double> (_valsumHigh_0) / static_cast<double>(_nPedestals);
    if (modeStr=="Low")return static_cast<double> (_valsumLow_0) / static_cast<double>(_nPedestals);
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
    
  
  if (trigger == 0 && _nPedestals > 0) {
    double meanVal = getMean(modeStr,trigger);
    double meanVal2 = meanVal*meanVal;
    double mean2Val(0.0);
    if(modeStr=="High") mean2Val =  static_cast<double> (_val2sumHigh_0) / static_cast<double>(_nPedestals);
    if(modeStr=="Low") mean2Val =  static_cast<double> (_val2sumHigh_0) / static_cast<double>(_nPedestals);
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

std::vector<double> Channel::getMeanVec(std::string modeStr) {
  double meansignal = getMean(modeStr, 1);
  double meanpedestal = getMean(modeStr, 0);

  std::vector<double> meanvec;
  meanvec.push_back(meansignal);
  meanvec.push_back(meanpedestal);

  return meanvec;
}

std::vector<double> Channel::getRMSVec(std::string modeStr) {
  double rmssignal = getRMS(modeStr, 1);
  double rmspedestal = getRMS(modeStr, 0);

  std::vector<double> rmsvec;
  rmsvec.push_back(rmssignal);
  rmsvec.push_back(rmspedestal);

  return rmsvec;
}

