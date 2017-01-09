#include <TString.h>
//#include "RAW2ROOT.C"
void RunBatch_runs(TString path, bool overwrite){
  // compile/load Raw2root class before  ( .x RAW2ROOT.C+ in root shell)
  // .x RunBatch.C("/home/calice/data/SCurveAll_FEV10/",true)
  const int Start = 175;
  const int Step = 3;
  const int Stop = 214;

  const int PlanEventTh = 32;
  const int MinBcidInc = 10;

  gSystem->Load("RAW2ROOT_C");

  TString filename;
  RAW2ROOT *ss;
  
  for(int j = 0; j <64;j+=8){
    
    for(int i = 0;Start+Step*i<Stop+1;i++){
      filename = path;
      filename += "/";
      filename += j;
      filename += "/";
      filename += "scurve_trig";
      filename += Start+Step*i;
      filename += "_by_dif0.raw";
      cout << "File: " << filename << endl;
      ss = new RAW2ROOT();
      ss->ReadFile(filename.Data(), overwrite, PlanEventTh, MinBcidInc);
      delete ss;
    }
  }
  cout << endl;
  cout << "PlanEventTh: " << PlanEventTh << endl;
  cout << "MinBcidInc: " << MinBcidInc << endl; 
  
  
  
  gSystem->Exit(0);
}
