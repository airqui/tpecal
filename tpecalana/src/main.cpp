//
//  main.cpp
//  tpecalana
//
//  Created by Roman Poeschl on 28/7/15.
//  Copyright (c) 2015 CNRS. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <cmath>
#include <vector>
#include <map>
#include <TObject.h>
#include <TApplication.h>

#include "RunManager.h"
#include "ExperimentalSetup.h"
#include "AnaManager.h"
#include "ADCManager.h"
#include "global.h"


using namespace::std;
using namespace globalvariables;

int main (int argc, const char * argv[])
{

   char** dummyappl;
   int dummyint = 0;
   TApplication fooApp("fooApp",&dummyint,dummyappl);


    RunManager runmanager;    
    globalvariables::setGlobal_deepAnalysis(true);
    globalvariables::setGainAnalysis(1); //high =1, low =0
    globalvariables::setEnabledChipsNumber(1); //

    globalvariables::setAnalysisType("Pedestal"); //
    std::cout<<globalvariables::getAnalysisType()<<std::endl;

    globalvariables::pushScanValue(200);

    AnaManager anaManager;
    anaManager.init();

    ADCManager adcManager;
    adcManager.init();


    //Where are the data?
    std::string datadirStr="/home/irles/2016/testbench_nov2016/rawdata/20161201_145847/scurves";

    for (unsigned irun=0; irun < globalvariables::getScanVectorDoubles().size();irun++) {
        std::stringstream inputFileStr;
        //Set the ASU mappings for a given run
        std::vector<std::string> mapfilesStrvec;
        mapfilesStrvec.clear();
        mapfilesStrvec.push_back("/home/irles/2016/testbench_nov2016/mapping/tb-2015/fev10_chip_channel_x_y_mapping.txt");
        ExperimentalSetup::getInstance()->setRunSetup(mapfilesStrvec);

	std::cout<<"  ----------------------------------------- " <<std::endl;
	std::cout<<" New Trigger: "<< globalvariables::getScanVectorDoubles().at(irun) <<std::endl;

	for(int ifile =0; ifile<64; ifile+=8) {
	  std::cout<<" New set of measurements: file "<< ifile << " with trigger "<< globalvariables::getScanVectorDoubles().at(irun) <<std::endl;
	  inputFileStr.str("");
	  inputFileStr << datadirStr << "/"<<ifile<<"/scurve_trig" << globalvariables::getScanVectorDoubles().at(irun) << "_by_dif0.raw.root";
	  runmanager.registerDifFile(new TFile(inputFileStr.str().c_str()));
	}

	ExperimentalSetup::getInstance()->executeExperiment(runmanager.getDifFileVec(),10);
	if(globalvariables::getAnalysisType() == "Pedestal" ) {
		adcManager.acquireRunInformation(ExperimentalSetup::getInstance(),
						 TString::Format("trigger%i",int(globalvariables::getScanVectorDoubles().at(irun) ) ), true, true, "");
	} else anaManager.acquireRunInformation(	ExperimentalSetup::getInstance(),
			TString::Format("trigger%i",int(globalvariables::getScanVectorDoubles().at(irun) ) ) );

        ////reset experimental setup and run manager
	ExperimentalSetup::getInstance()->reset();
        runmanager.reset();
	
    }
    //
   // if(globalvariables::getAnalysisType() == "Pedestal" ) {
   //     adcManager.displayResults(true,true);
   //   } anaManager.displayResults();
    
    
    fooApp.Run();
    return 0;
}

//  myfile_pedestal.open ("Pedestal_"+file_sufix+gain+".tsv");
//  myfile_pedestal <<"#pedestal positions & errors for all buffers " << endl;
//  myfile_pedestal <<"#dif\chip\tchn\tpedbuffer0\tpederrorbuffer0\..\tpederrorbuffer14" << endl;
