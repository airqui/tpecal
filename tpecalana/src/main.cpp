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
#include <cmath>
#include <vector>
#include <map>

#include "TRint.h"
#include "TROOT.h"
#include "TStyle.h"
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>

#include "RunManager.h"
#include "ExperimentalSetup.h"
// analysis managers
#include "ScanManager.h"

#include "global.h"

#include "boost/filesystem.hpp"   

using namespace::std;
using namespace globalvariables;
using namespace boost::filesystem; 



void ScanAnalysis(TString dif, int step, int buffer, string datadirStr, string datadirStr_output ) {

  std::string path=datadirStr+"/";
  if(step >0 ) path+="0/";

  if ( !exists( path ) ) {
    path=datadirStr;
  } else {
    if( !exists( path ) ) {
      std::cout << " main:ScanAnalysis:-> I don't find the folder where the root files are. STOP THE EXECUTABLE  "<<std::endl;
      return;
    }
  }  
 
  boost::filesystem::path apk_path(path);
  boost::filesystem::directory_iterator end;

  int istringsize=0;
  for(int j=0; j<30; j++) {
    for (boost::filesystem::directory_iterator i(apk_path); i != end; ++i)   {
      const boost::filesystem::path cp = (*i);
      
      std::string mystring = cp.string();
      if(mystring.substr(mystring.length() - j) == string(dif+".raw.root") ){
	cout<<mystring<<" "<<j<<" "<<istringsize<<endl;
	istringsize=j;
	continue;
      }
    }
  }


  for (boost::filesystem::directory_iterator i(apk_path); i != end; ++i)   {
    const boost::filesystem::path cp = (*i);


    std::string mystring = cp.string();
    if(mystring.substr(mystring.length() - istringsize) == string(dif+".raw.root") ){
      string scanvalue_2;
      string scanvalue;
      cout<<mystring<<" "<<istringsize<<endl;

      if(globalvariables::getAnalysisType() == "scurves" ) {
	scanvalue =mystring.substr(mystring.find("_trig")+5, 3);
      }
      if(globalvariables::getAnalysisType() == "holdscan") {
	scanvalue =mystring.substr(mystring.find("_hold")+5, 3);
	scanvalue_2 =mystring.substr(mystring.find("_hold")+5, 2);
	cout<<scanvalue<<" " <<scanvalue_2<<	" " <<mystring.substr() <<endl;
      } 

      if( globalvariables::getAnalysisType()=="scurves"  && atoi(scanvalue.c_str()) > 0 ) globalvariables::pushScanValue(atof(scanvalue.c_str()));
      if( globalvariables::getAnalysisType()=="holdscan") {
	if( atoi(scanvalue.c_str()) > 75 && atoi(scanvalue.c_str()) % 10 == 0) globalvariables::pushScanValue(atof(scanvalue.c_str()));
      }
	
    }
  }

  for (unsigned irun=0; irun < globalvariables::getScanVectorDoubles().size();irun++) 
    std::cout<<" Main::NextScanValue: "<< globalvariables::getScanVectorDoubles().at(irun) <<std::endl;

  
  RunManager runmanager;    
  ScanManager scanManager;    scanManager.init();

  //Set the ASU mappings for a given run
  std::vector<std::string> mapfilesStrvec;
  mapfilesStrvec.clear();
  mapfilesStrvec.push_back("/home/calice/tpecal/mapping/tb-2015/fev10_chip_channel_x_y_mapping.txt");

  for (unsigned irun=0; irun < globalvariables::getScanVectorDoubles().size();irun++) {
    std::stringstream inputFileStr;
      
    std::cout<<"  ----------------------------------------- " <<std::endl;
    std::cout<<" Main::NextScanValue: "<< globalvariables::getScanVectorDoubles().at(irun) <<std::endl;

    ExperimentalSetup::getInstance()->setRunSetup(mapfilesStrvec);
      
    TString scanstring;
    if(globalvariables::getAnalysisType() == "scurves" ) 
      scanstring="scurve_trig";       
      
    if(globalvariables::getAnalysisType() == "holdscan") 
      scanstring="cosmicrun_DAC230_30min_spill2Hz_250ms_hold";//      scanstring="run_inj_150fC_hold";

    if(step> 0)  {
      int ifilemax=64;
      if(globalvariables::getAnalysisType() == "holdscan") ifilemax=1;

      for(int ifile =0; ifile<ifilemax; ifile+=step) {
	std::cout<<" New set of measurements: file "<< ifile << " with trigger "<< globalvariables::getScanVectorDoubles().at(irun) <<" " << step << " " << ifile<< std::endl;
	inputFileStr.str("");
	inputFileStr << datadirStr << "/"<<ifile<<"/"<<+scanstring << globalvariables::getScanVectorDoubles().at(irun) << "_"+dif<<".raw.root";
	runmanager.registerDifFile(new TFile(inputFileStr.str().c_str()));
      }
    } else {
      inputFileStr.str("");
      inputFileStr << datadirStr << "/"+scanstring << globalvariables::getScanVectorDoubles().at(irun) << "_"+dif<<".raw.root";
      std::cout<<" Main::ReadFile "<<inputFileStr.str().c_str()<<std::endl;
      runmanager.registerDifFile(new TFile(inputFileStr.str().c_str()));
    }


    ExperimentalSetup::getInstance()->executeExperiment(runmanager.getDifFileVec());
      
    ////reset experimental setup and run manager
    scanManager.acquireRunInformation(ExperimentalSetup::getInstance(), buffer);
    ExperimentalSetup::getInstance()->reset();
    runmanager.reset();
  }   

  TString scanfile;
  if(globalvariables::getAnalysisType() == "scurves" )  
    scanfile = TString(datadirStr_output)+TString::Format("/Scurves_PlaneEvThresh%i_buff%i_",globalvariables::getPlaneEventsThreshold(),buffer)+dif;

  if(globalvariables::getAnalysisType() == "holdscan" )  
    scanfile = TString(datadirStr_output)+"/Holdscan_"+dif;

  scanManager.displayResults( scanfile , buffer );
    

}





int main(int argc, char* argv[6])
{

  std::cout << " ----------------------------------------------- " << std::endl;
  std::cout << " *********************************************** " << std::endl;
  std::cout << " Technological Prototype Analysis Software " << std::endl;
  std::cout << "  " << std::endl;
  std::cout << " R. Poeschl & A. Irles " << std::endl;
  std::cout << " December 2016 " << std::endl;
  std::cout << "  " << std::endl;

  std::cout << " To run it: " << std::endl;
  std::cout << "  " << std::endl;
  std::cout << "./tpecalana for scan analysis (threshold scan, holdscan, etc) " << std::endl;
  std::cout << "    input_folder  == full path last subrun folder, containing converted root files. For No scan runs, this is the " << std::endl;
  std::cout << "    output_folder == full path to the ouptut folder, which should be in input_folder/output_folder. " << std::endl;
  std::cout << "    analysis_type == scurves, holdscan" << std::endl;
  std::cout << "    enabled_chips == Number of enabled chips per dif (default 16) " << std::endl;
  std::cout << "    step          == in case of scans, the number of channels in each subrun (default 8) " << std::endl;
  std::cout << "    buffer        == buffer to analyze (for scans) 0-14. If 15, all are analyzed together. (default 0)" << std::endl;
  std::cout << "    dif_id        == dif_id string (default dif_1_1_1)" << std::endl;
  std::cout << "  " << std::endl;
  std::cout << "  " << std::endl;
  
  std::cout << " " << std::endl;
  std::cout << "========    Current run: " << std::endl;
  std::cout<<"Number of arguments: "<<argc<<std::endl;
  for(int i=1; i<argc; i++) 
    std::cout<<  "    argument "<<i<<"="<<argv[i]<<std::endl;

  std::cout << " " << std::endl;
  if(argc < 4)    {
    std::cout << " Not enough arguments provided"<<std::endl;
    return 0;
  }

  // -------------------------------------------------------------
  // SET THE ARGUMENTS FOR THE ANALYSIS
  //Where are the data?
  std::string datadirStr=  string(argv[1]);//
  std::string datadirStr_output=  string(argv[2]);
  
  std::cout << "Directory/File where the data is: "<<datadirStr<<" "<< argv[1]<<std::endl;
  std::cout << "Directory/File where the output goes: "<<datadirStr_output<<std::endl;

  if ( !exists( datadirStr ) ) {
    std::cout << " Data directory doesn't exist !!! STOP THE EXECUTABLE  "<<std::endl;
    return 0;
  }   

  globalvariables::setAnalysisType(TString(argv[3]));

  if ( globalvariables::getAnalysisType()== "scurves" ||  globalvariables::getAnalysisType()== "holdscan"  ||  globalvariables::getAnalysisType()== "PlaneEventsScan" ) {
     if ( !exists( datadirStr ) ) {
       std::cout << " OUTPUT directory doesn't exist !!! Stop here "<<std::endl;
       return 0;
     }
  }

  if(argc > 4) globalvariables::setEnabledChipsNumber(atoi(argv[4])); //
  else globalvariables::setEnabledChipsNumber(0); //

  int step = 8;
  if(argc > 5) step = atoi(argv[5]);
  int buffer = 0;
  if(argc > 6) buffer = atoi(argv[6]);

  TString dif = "dif_1_1_1";
  if(argc > 7)     dif = TString(argv[7]);

  globalvariables::setMinBCIDThreshold(0); 


  // -------------------------------------------------------------


  // -------------------------------------------------------------
  // START THE ANALYSIS
  TApplication *fooApp = new TApplication("fooApp",&argc,argv);

  gROOT->SetStyle("Modern");
  gStyle->SetTitleH(0.08);

  gStyle->SetNdivisions(505,"Y");
  gStyle->SetLabelSize(0.07,"Y");
  gStyle->SetLabelOffset(0.005,"Y");
  gStyle->SetTitleSize(0.065,"Y");
  gStyle->SetTitleOffset(0.7,"Y");

  gStyle->SetNdivisions(510,"X");
  gStyle->SetLabelSize(0.065,"X");
  gStyle->SetLabelOffset(0.0,"X");
  gStyle->SetTitleSize(0.065,"X");
  gStyle->SetTitleOffset(0.75,"X");

  gStyle->SetTitleStyle(2001);
  
  if(globalvariables::getAnalysisType() == "scurves" || globalvariables::getAnalysisType() == "holdscan" || globalvariables::getAnalysisType() == "PlaneEventsScan" ) {
    globalvariables::setGainAnalysis(1); //high =1, low =0
    globalvariables::setPlaneEventsThreshold(64);
    globalvariables::setGlobal_deepAnalysis(false);
    globalvariables::setSkiroc(dif);
    ScanAnalysis(dif,step, buffer, datadirStr, datadirStr_output) ;
  }


  //fooApp->Run();
  return 0;

}
