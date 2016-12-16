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

#include "boost/filesystem.hpp"   

using namespace::std;
using namespace globalvariables;
using namespace boost::filesystem; 


int main(int argc, char **argv) 
{

  char** dummyappl;
  int dummyint = 0;
  TApplication fooApp("fooApp",&dummyint,dummyappl);


  if(argc < 2)
    {
      std::cout << "Provide input folder (full path), output folder(relative) " << std::endl;
      return 1;
    }


      
    globalvariables::setGlobal_deepAnalysis(false);
    globalvariables::setGainAnalysis(1); //high =1, low =0
    globalvariables::setEnabledChipsNumber(16); //
      
    globalvariables::setAnalysisType("scurves"); //

    globalvariables::pushScanValue(175);
    globalvariables::pushScanValue(178);
    globalvariables::pushScanValue(181);
    globalvariables::pushScanValue(184);
    globalvariables::pushScanValue(187);
    globalvariables::pushScanValue(190);
    globalvariables::pushScanValue(193);
    globalvariables::pushScanValue(196);
    globalvariables::pushScanValue(199);
    globalvariables::pushScanValue(202);
    globalvariables::pushScanValue(205);
    globalvariables::pushScanValue(208);
    globalvariables::pushScanValue(211);
    globalvariables::pushScanValue(214);

    //Where are the data?
    std::string datadirStr=argv[1];//
    std::string datadirStr_output=datadirStr+"/"+argv[2]+"/";
    int step = atoi(argv[3]);
    int buffer = atoi(argv[4]);

    std::cout << "Directory where the data is: "<<datadirStr<<std::endl;

    if ( !exists( datadirStr ) ) {
      std::cout << " Data directory doesn't exist !!! STOP THE EXECUTABLE  "<<std::endl;
      return 0;
    }   
    if ( !exists( datadirStr_output ) ) {
      std::cout << " Data OUTPUT directory doesn't exist !!! STOP THE EXECUTABLE  "<<std::endl;
      std::cout << " Please, type and rerun: "<<std::endl;
      std::cout << "mkdir "<<datadirStr_output<<std::endl;
      return 0;
    }


    RunManager runmanager;    
    AnaManager anaManager;    anaManager.init();
    ADCManager adcManager;    adcManager.init();


    for (unsigned irun=0; irun < globalvariables::getScanVectorDoubles().size();irun++) {
        std::stringstream inputFileStr;
        //Set the ASU mappings for a given run
        std::vector<std::string> mapfilesStrvec;
        mapfilesStrvec.clear();
        mapfilesStrvec.push_back("/home/irles/2016/testbench_nov2016/mapping/tb-2015/fev10_chip_channel_x_y_mapping.txt");
        ExperimentalSetup::getInstance()->setRunSetup(mapfilesStrvec);

	std::cout<<"  ----------------------------------------- " <<std::endl;
	std::cout<<" New Trigger: "<< globalvariables::getScanVectorDoubles().at(irun) <<std::endl;

	for(int ifile =0; ifile<64; ifile+=step) {
	  std::cout<<" New set of measurements: file "<< ifile << " with trigger "<< globalvariables::getScanVectorDoubles().at(irun) <<std::endl;
	  inputFileStr.str("");
	  inputFileStr << datadirStr << "/"<<ifile<<"/scurve_trig" << globalvariables::getScanVectorDoubles().at(irun) << "_by_dif0.raw.root";
	  runmanager.registerDifFile(new TFile(inputFileStr.str().c_str()));
	}

	ExperimentalSetup::getInstance()->executeExperiment(runmanager.getDifFileVec(),10);

	TString output_path ;
	output_path =  TString(datadirStr_output) + TString::Format("/trigger%i",int(globalvariables::getScanVectorDoubles().at(irun) ) ) ;

	if(globalvariables::getAnalysisType() == "Pedestal" ) {
	  adcManager.acquireRunInformation(ExperimentalSetup::getInstance(), output_path , true, false, "");
	  adcManager.displayResults(output_path, true,false);
	}

	if(globalvariables::getAnalysisType() == "PedestalSignal" ) {
	  adcManager.acquireRunInformation(ExperimentalSetup::getInstance(), output_path , true, true, "");
	  adcManager.displayResults(output_path, true,true);
	}
	
	if(globalvariables::getAnalysisType() == "scurves" ) 
	  anaManager.acquireRunInformation(ExperimentalSetup::getInstance(), buffer);

        ////reset experimental setup and run manager
	ExperimentalSetup::getInstance()->reset();
        runmanager.reset();
	
    }

    TString scurvefile= TString(datadirStr_output)+TString::Format("/Scurves_buff%i_",buffer);
    if(globalvariables::getAnalysisType() == "scurves" )         anaManager.displayResults( scurvefile,buffer );
    
    //   fooApp.Run();
    return 1;
}


