//
//  AnaManager.h
//  tpecalana_project
//
//  Created by Roman Poeschl on 10/08/15.
//  A class to manage the analysis of the data acquired in experimentak setup to (separate Graphics from actual data processing)
//  It receives information in form of an ExperimentalSetupClass and stores information according to analysis needs
//

#ifndef __tpecalana_project__AnaManager__
#define __tpecalana_project__AnaManager__

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TH1I.h>
#include <TString.h>
#include "ExperimentalSetup.h"

class AnaManager {
    
public:
    AnaManager();
    ~AnaManager();
    //Initialise the manager
    void init();
    //Retrieve information
    void acquireRunInformation(ExperimentalSetup*,int);
    //A method called to desplay results of whatever nature
    void displayResults(TString,int);
    //All graphics is realised in methods called by this method
    //void launchGraphicsPresentation();

    //boolean to decide if we do deep analysis 
    //(saving full histograms of pedestals and hit charge for each channel, 
    //each buffer, each chip, each asu and each dif 
    //---> this is a lot of histograms that take a lot of time to be created. 
    //So innefficient :-(
    bool deepanalysis=false;

private:

    TFile* f_charge;
    TFile* f_scurve;

    //Number of runs analysed by the manager
    unsigned _nRuns;
    //A method for a simple channel by channel
    void simpleChannelAnalysis(ExperimentalSetup*);
    //The grpahical representation
    void simpleChannelAnalysisGraphics();
    //A simple map that holds the values for the high gain for a selected channel
    std::map<int,vector<double> > _sigVecMapHigh;
    //A simple map that holds the values for the low gain for a selected channel
    std::map<int,vector<double> > _sigVecMapLow;

    //S-curve analysis
    void sCurveAnalysis(ExperimentalSetup*,int);
    //The method that will effectively call the graphics part
    void sCurveAnalysisGraphics(TString,int);
    //A map that holds the number of triggers of all channels for all runs for all enabled chips
    typedef std::map<unsigned, std::vector<std::vector<unsigned> > > channelInfoComplUnsigned_t;
    channelInfoComplUnsigned_t _ntrigVecMapHigh;

    //A helper map that stores for each chip and each channel the maximum number of hits
    //for normalisation purposes
    std::map<unsigned, std::vector<unsigned> > _maxHithelpVec;

    //... and here finally the job is done
    void sCurveAnalysisGraphicsPainter(channelInfoComplUnsigned_t::iterator, TString,int);

    //data quality analysis
    void deeperDataAnalysis(ExperimentalSetup*);
    //A vector that holds the graphs of the accumulated events for each run and each dif
    typedef std::vector< std::vector<TGraph*>   >  TGraphVec_t;
    TGraphVec_t _tGraphVec;
    //Graphical representation of data quality analysis
    void deeperDataAnalysisGraphics();
    //display run statistics
    void displayRunStat();
    //A helper method to fill graphs
    double* vectortoarray(std::vector<double> thevec, double* theArray );
  //  PedestalManager pedestalmanager_;

};


#endif /* defined(__tpecalana_project__AnaManager__) */
