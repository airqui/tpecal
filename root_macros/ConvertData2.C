
void ConvertData2(TString filename, bool overwrite){
  gSystem->Load("/opt/calicoes/standard/RAW2ROOT_C");
  gSystem->Load("/opt/calicoes/standard/MeanRMS_FEV10_C");
  RAW2ROOT ss;
  ss.ReadFile(filename, overwrite, 10, 4);
  //MeanRMS_FEV10 MRMS;
  TString filename2 = filename;
  filename2 += ".root";
  MRMS=MeanRMS_FEV10(filename2);
  
  gSystem->Exit(0);
}

