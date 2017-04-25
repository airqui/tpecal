#include "global.h"

namespace globalvariables
{

  //------------------------------------------------------------------
  // values set for scan runs, (threshold scans, hold scans)
  std::vector<Double_t> scanValVec;
  TString ScanType = "";
  void setScanType(TString type) {
    ScanType = type;
  }
  TString getScanType() { return ScanType; }
  void pushScanValue(Double_t value) {
    scanValVec.push_back(value);
  }
  std::vector<Double_t> getScanVectorDoubles() {
    std::sort (scanValVec.begin(), scanValVec.end()) ;
    return scanValVec;
  }

  //------------------------------------------------------------------
  // variable to decide if we do deep analysis (saves long vectors to create histograms, requires x10 more memory and computation time
  bool global_bool_deepanalysis = false;

  bool getGlobal_deepAnalysis()
  {
    return global_bool_deepanalysis;
  }

  void setGlobal_deepAnalysis( bool deepanalysis)
  {
    global_bool_deepanalysis = deepanalysis;
  }

  //------------------------------------------------------------------
  // choose the gain to be used
  int global_gain = -1;
  int getGainAnalysis()
  {
    return global_gain;
  }

  TString getGainTStringAnalysis( ) {
    TString gains="";
    if( global_gain==1 ) gains="_highGain";
    else if(global_gain==0) gains="_lowGain";
    else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;
    return gains;
  }

  void setGainAnalysis(int gain )
  {
    if(gain ==1 || gain ==0 ) {
      global_gain = gain;
      if(gain ==1) std::cout<<  " Do HIGH GAIN analysis: gain="<< gain <<std::endl;
      if(gain ==0) std::cout<<  " Do LOW GAIN analysis: gain="<< gain <<std::endl;
    } else { 
      std::cout<<  " WRONG GAIN CHOICE for the analysis, gain="<< gain<<std::endl;
      std::cout<<  " please, choose gain=0 (low gain) or gain+1(high gain) "<<std::endl;
      std::cout<<  " in the meanwhile FORCED TO DO HIGH GAIN ANALYSIS with gain=1" <<std::endl;
      global_gain = 1;
    }
  }

 //------------------------------------------------------------------
  // Plane Event Threshold
  int planeEvent_th = -1;
  int getPlaneEventsThreshold()
  {
    if( planeEvent_th > 1 ) {
      return planeEvent_th;
    } else { 
      std::cout<<  " WRONG THRESHOLD CHOICE for the analysis, threshold="<< planeEvent_th<<std::endl;
      std::cout<<  " automatically set to 32 " <<std::endl;
      planeEvent_th = 32;
    }
    return planeEvent_th;
  }

  TString getPlaneEventsThresholdTStringAnalysis( ) {
    TString gains="";
    gains=TString::Format("_plEventThres%i",planeEvent_th);
    return gains;
  }


  void setPlaneEventsThreshold(int threshold )
  {
    if(threshold > 1 ) {
      planeEvent_th = threshold;
      std::cout<<  " Plane Event Threshold set to "<< threshold <<std::endl;
    } else { 
      std::cout<<  " WRONG THRESHOLD CHOICE for the analysis, threshold="<< threshold <<std::endl;
      std::cout<<  " automatically set to 32 " <<std::endl;
      planeEvent_th = 32;
    }
  }


 //------------------------------------------------------------------
  // Set Min BCID for selection
  int minBcid_th = 0;
  int getMinBCIDThreshold()
  {
    if( minBcid_th > 0 ) {
      return minBcid_th;
    } else { 
      //      std::cout<<  " WRONG MIN BCID THRESHOLD CHOICE for the analysis, threshold="<< minBcid_th<<std::endl;
      //std::cout<<  " automatically set to 0 " <<std::endl;
      minBcid_th = 0;
    }
    return minBcid_th;
  }

  void setMinBCIDThreshold(int threshold )
  {
    if(threshold > 0 ) {
      minBcid_th = threshold;
      std::cout<<  " Min BCID Threshold set to "<< threshold <<std::endl;
    } else { 
      //std::cout<<  " WRONG Min BCID THRESHOLD CHOICE for the analysis, threshold="<< threshold <<std::endl;
      //std::cout<<  " automatically set to 0 " <<std::endl;
      minBcid_th = 0;
    }
  }


  //------------------------------------------------------------------
  // enabled chips
  // to be modified by a method that actually looks into a rundb file or an xml file and knows how many chips were
  // enabled
  std::vector<unsigned> enabledChipsVec;
  void setEnabledChipsNumber( unsigned nchips) {
    enabledChipsVec.clear();
    for(unsigned i=0; i<nchips; i++) enabledChipsVec.push_back(i);
  }

  std::vector<unsigned> getEnabledChipsVec() {
    return enabledChipsVec;
  }

  //------------------------------------------------------------------
  // type of analysis
  TString type_analysis;
  void setAnalysisType(TString type) {
    type_analysis = type;
  }
  TString getAnalysisType() { return type_analysis; }


}

