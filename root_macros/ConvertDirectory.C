/*
lancer root
.x run.cxx("nomDuDossier/")
*/

#include "TSystemDirectory.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLatex.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

// fonction qui fait la liste de tous les fichiers *by_dif0.raw d'un dossier et qui renvoie la liste sous filenames. Écrit par Floris Thiant (LLR).
vector<TString>* list_files(char *dirname, const char *ext="by_dif0.raw")
{
	TSystemDirectory dir(dirname, dirname);
	TList* files = dir.GetListOfFiles();
	vector<TString>* filenames = new vector<TString>();
	
	if (files)
	{
		TSystemFile *file;
		TString fname;
		TIter next(files);
		while ((file=(TSystemFile*)next()))
		{
			fname = file->GetName();
			if (!file->IsDirectory() && fname.EndsWith(ext))
			{
				filenames->push_back(dirname+fname);
			}
		}
	}
	return filenames;
}

void ConvertDirectory(string dirname)
{
	gSystem->Load("/opt/calicoes/standard/RAW2ROOT_C");
	vector<TString>* filenames = list_files(dirname.c_str());
	RAW2ROOT *ss;
	unsigned int nbCrb = filenames->size();
	
	for (int i = 0 ; i<nbCrb ; i++)
	{
		ss = new RAW2ROOT();
		ss->ReadFile((*filenames)[i], true, 10, 4);
		delete ss;
	}
	for (int i = 0 ; i<nbCrb ; i++)
	{
	        std::cout << (*filenames)[i] << std::endl;
	}
	gSystem->Exit(0);
}
