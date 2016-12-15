void RunMergeByN(TString dirname, bool overwrite, int Slice_Size, int Start, int Stop, int Step){
  	gSystem->Load("/opt/calicoes/standard/MergeByN_C");
	M=MergeByN();
	M.DoMerge(dirname,Slice_Size, Start, Stop, Step);  
 
  	gSystem->Exit(0);
}
