/*
 * ADCManager.h
 *
 *  Created on: 13 Dec 2016
 *      Author: irles
 */


#ifndef __tpecalana_project__ADCManager__
#define __tpecalana_project__ADCManager__

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

class ADCManager {

public:
    ADCManager();
    ~ADCManager();
    void init();

    //A map that holds the pedestal value of all channels for all runs for all enabled chips
    // translate of all variables
    //buffer, chip, scan value (i.e. trigger), channel, a vector of X doubles, normally 2: pedestal and pedestal error  (or mip, mip error)
    typedef std::map<unsigned, std::map<unsigned, std::vector<std::vector<std::vector<Double_t> > > > > bufferchannelInfoComplDouble_t;
    //chip, scan value (i.e. trigger), channel, a vector of two doubles: pedestal and pedestal error (or mip, mip error)
    typedef std::map<unsigned, std::vector<std::vector<std::vector<Double_t> > > > channelInfoComplDouble_t;
    //chip, buffer, a vector of pedestal histograms for each channel scan value (i.e. trigger), channel, a vector of two doubles: pedestal and pedestal error (or mip, mip error)
    typedef std::map<unsigned, std::map<unsigned, std::vector<std::vector<TH1I*> > > > bufferchannelInfoComplTH1_t;

    //A map of buffer and vector that holds the graphs of the pedestal values for each channel
    typedef std::map<unsigned, std::vector<TGraphErrors*> >  TGraphPedestalVec_t;

    void acquireRunInformation(ExperimentalSetup*, TString, bool, bool, TString);
    void pedestalAnalysis(ExperimentalSetup*, TString);
    void signalAnalysis(ExperimentalSetup*, TString, TString);

    void displayResults(TString, bool, bool);
    // basic analysis graphics: read the pedestal/signal info and make some fits for each channel/buffer
    void pedestalAnalysisGraphicsBasic(TString);
    void signalAnalysisGraphicsBasic();

    // more elaborated analysis graphics for scan runs (threshold scans, etc), only written for pedestal
    void pedestalAnalysisGraphics();
    void pedestalAnalysisGraphicsFill(bufferchannelInfoComplDouble_t::iterator);

private:
    //    TFile* f_pedestal_scan;
    //TFile* f_pedestal;

    TFile* f_signal_scan;
    TFile* f_signal;

    //Pedestal analysis, for scans (thresholds, etc)
    bufferchannelInfoComplDouble_t _pedestalVecMap; // Pedestal Mean
    bufferchannelInfoComplTH1_t _pedestalVecMapTH1;  // Pedestal Histograms
    TGraphPedestalVec_t _tGraphPedestalVec; // for scan analysis

    bufferchannelInfoComplDouble_t _signalVecMap; // Pedestal Mean
    bufferchannelInfoComplTH1_t _signalVecMapTH1;  // Pedestal Histograms
    TGraphPedestalVec_t _tGraphSignalVec; // for scan analysis


    //A helper method to fill graphs
    double* vectortoarray(std::vector<double> thevec, double* theArray );
    int _nRuns;
};
#endif
