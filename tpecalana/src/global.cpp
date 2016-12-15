#include "global.h"

namespace globalvariables
{

  //------------------------------------------------------------------
  // for scan runs, (threshold scans, hold scans)
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
  // variable to decide if we doo deep analysis (saves long vectors to create histograms, requires x10 more memory and computation time
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

