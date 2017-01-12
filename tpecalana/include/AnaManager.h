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
#include <TLegend.h>
#include "ExperimentalSetup.h"

class AnaManager {
    
public:
    AnaManager();
    ~AnaManager();
    //Initialise the manager
    void init();
    //Retrieve information
    void acquireRunInformation(ExperimentalSetup*,int);
    //A method called to display results of whatever nature
    void displayResults(TString,int);

private:

    TFile* f_scurve;

    //Number of runs analysed by the manager
    unsigned _nRuns;
    //A method for a simple channel by channel

    //S-curve analysis
    //A map that holds the number of triggers of all channels for all runs for all enabled chips
    typedef std::map<unsigned, std::vector<std::vector<unsigned> > > channelInfoComplUnsigned_t;
    channelInfoComplUnsigned_t _ntrigVecMapHigh;
    std::map<unsigned, std::vector<unsigned> > _maxHithelpVec;
    //analysis
    void sCurveAnalysis(ExperimentalSetup*,int);
    //The method that will effectively call the graphics part
    void sCurveAnalysisGraphics(TString,int);
    void sCurveAnalysisGraphicsPainter(channelInfoComplUnsigned_t::iterator, TString,int);
    
    //Plane Events analysis, per chip
    typedef std::map<unsigned, std::vector<std::vector<Double_t> > > ChipInfoComplDouble_t;
    ChipInfoComplDouble_t _ntrigChipVecMapHigh;
    //analysis
    void planeEventsAnalysis(ExperimentalSetup*);
    //... and here finally the job is done
    void planeEventsAnalysisGraphics(TString);
    void planeEventsAnalysisGraphicsPainter(ChipInfoComplDouble_t::iterator, TFile*);

    //A helper method to fill graphs
    double* vectortoarray(std::vector<double> thevec, double* theArray );

};


#endif /* defined(__tpecalana_project__AnaManager__) */
