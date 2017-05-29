//
//  ScanManager.h
//  tpecalana_project
//
//  Created by Roman Poeschl on 10/08/15.
//  A class to manage the analysis of the data acquired in experimentak setup to (separate Graphics from actual data processing)
//  It receives information in form of an ExperimentalSetupClass and stores information according to analysis needs
//

#ifndef __tpecalana_project__ScanManager__
#define __tpecalana_project__ScanManager__

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

class ScanManager {
    
public:
    ScanManager();
    ~ScanManager();
    //Initialise the manager
    void init();
    //Retrieve information
    void acquireRunInformation(ExperimentalSetup*,int);
    //A method called to display results of whatever nature
    void displayResults(TString,int);

private:

    TFile* f_scurve;
    ofstream fout_scurves;
    ofstream fout_scurves_chip_3sigma;
    ofstream fout_scurves_chip_5sigma;
    ofstream fout_scurves_chip_3sigma_firstzero;

    TCanvas * c_mean;
    TCanvas * c_sigma;

    //Number of runs analysed by the manager
    unsigned _nRuns;
    //A method for a simple channel by channel

    //-------------------------
    //Holdscan analysis
    //A map that holds the mean of the ADC distribution of all channels for all runs for all enabled chips
    typedef std::map<unsigned, std::vector<std::vector<Double_t> > > channelInfoComplDouble_t;
    channelInfoComplDouble_t _meanADCChipVecMapHigh;
    //analysis
    void holdscanAnalysis(ExperimentalSetup*);
    //The method that will effectively call the graphics part
    void holdscanAnalysisGraphics(TString);
    void holdscanAnalysisGraphicsPainter(channelInfoComplDouble_t::iterator, TString);
    
    //-------------------------
    //S-curve analysis
    //A map that holds the number of triggers of all channels for all runs for all enabled chips
    channelInfoComplDouble_t _ntrigVecMapHigh;
    std::map<unsigned, std::vector<Double_t> > _maxHithelpVec;
    std::map<unsigned, std::vector<Double_t> > _maxHitCounthelpVec;
    //analysis
    void sCurveAnalysis(ExperimentalSetup*,int);
    //The method that will effectively call the graphics part
    void sCurveAnalysisGraphics(TString,int);
    void sCurveAnalysisGraphicsPainter(channelInfoComplDouble_t::iterator, TString, int);
    
    //-------------------------
    //Plane Events analysis, per chip
    channelInfoComplDouble_t _ntrigChipVecMapHigh;
    channelInfoComplDouble_t _ntrigChipVecMapHigh_norm;
    //analysis
    void planeEventsAnalysis(ExperimentalSetup*);
    //... and here finally the job is done
    void planeEventsAnalysisGraphics(TString);
    void planeEventsAnalysisGraphicsPainter(channelInfoComplDouble_t::iterator, TFile*);

    //A helper method to fill graphs
    double* vectortoarray(std::vector<double> thevec, double* theArray );

};


#endif /* defined(__tpecalana_project__ScanManager__) */
