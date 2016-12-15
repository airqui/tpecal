void RunFindNoisyN(TString filename, int iter, Double_t TThres, int DIFid){
  gSystem->Load("/opt/calicoes/standard/FindNoisyN_C");
  FindNoisyN N;
  N.GetDataStats(filename);
  N.ReadFile(filename,iter, DIFid);
  N.Analyse(filename,TThres, DIFid);
  N.WriteFile(filename, true, DIFid);
  gSystem->Exit(0);
}

