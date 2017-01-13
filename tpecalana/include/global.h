/*
 * global.h
 *
 *  Created on: 13 Dec 2016
 *      Author: irles
 */

// header for global variables used during the analysis
// can be more ellegant...
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "TString.h"

namespace globalvariables
{
  // for scan runs, (threshold scans, hold scans),
  // we need a method to fill a vector with the values set in any scan
  // and the type of scan done
  extern std::vector<Double_t> scanValVec; //vector for the parameter variated in the different acquisitions.
  extern TString ScanType; //vector for the parameter variated in the different acquisitions.
  void setScanType(TString);
  void pushScanValue(Double_t);
  TString getScanType();
  std::vector<Double_t> getScanVectorDoubles();


  // do or not a deep analysis, that requires saving a lot of vectors of integers (to be used for pedestal and charge histograms)
  extern bool global_bool_deepanalysis;
  bool getGlobal_deepAnalysis();
  void setGlobal_deepAnalysis(bool);

  // choose the gain to be used in the Analysis Managers (ScanManager, MonitorManager, ADCManager)
  extern int global_gain;
  int getGainAnalysis();
  TString getGainTStringAnalysis();
  void setGainAnalysis(int);

  // choose the Plane Event Threshold
  extern int planeEvent_th;
  int getPlaneEventsThreshold();
  TString getPlaneEventsThresholdTStringAnalysis( );
  void setPlaneEventsThreshold(int);

  // enabled chips
  // to be modified by a method that actually looks into a rundb file or an xml file and knows how many chips were
  // enabled
  extern std::vector<unsigned> enabledChipsVec;
  void setEnabledChipsNumber( unsigned );
  std::vector<unsigned> getEnabledChipsVec();

  //type of analysis, to be used in the AnaManager class
  extern TString type_analysis;
  void setAnalysisType(TString);
  TString getAnalysisType();

}
