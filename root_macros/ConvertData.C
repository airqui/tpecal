    

void ConvertData(TString filename, bool overwrite){
  gSystem->Load("RAW2ROOT_C");
  RAW2ROOT ss;
  ss.ReadFile(filename, overwrite, 5, 2);
  gSystem->Exit(0);
}






