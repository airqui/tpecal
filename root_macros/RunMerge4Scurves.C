void RunMerge4Scurves(string dirname, int start, int stop, int step)
{
	gSystem->Load("/opt/calicoes/standard/Merge4Scurves_C");
        M=Merge4Scurves();
        M.DoMerge(dirname,false,start,stop,step);
	gSystem->Exit(0);
}
