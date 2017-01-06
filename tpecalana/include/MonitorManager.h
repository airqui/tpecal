/*
 * MonitorManager.h
 *
 *  Created on: 5 Jan 2017
 *      Author: irles
 */

#ifndef MONITORMANAGER_H_
#define MONITORMANAGER_H_

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

class MonitorManager {
public:
	virtual ~MonitorManager();
	MonitorManager();

    void init();
    //Retrieve information
    void acquireRunInformation(ExperimentalSetup*);
    //A method called to display results of whatever nature
    void displayResults(TString);

private:

    TFile* f_file;

    //A method for a simple channel by channel
    void simpleChannelAnalysis(ExperimentalSetup*);
    //The graphical representation
    void simpleChannelAnalysisGraphics(TString);

    //A map that holds the number of triggers of all channels for all buffers for all enabled chips
    typedef std::map<unsigned, std::vector<std::vector<unsigned> > > channelInfoComplUnsigned_t;
    channelInfoComplUnsigned_t _ntrigVecMapHigh;
    channelInfoComplUnsigned_t _ntrigVecMapHigh_overRunningBcid;
    channelInfoComplUnsigned_t _ntrigVecMapHigh_planeEvents;
    channelInfoComplUnsigned_t _ntrigVecMapHigh_negativeData;

    //A helper map that stores for each chip and each channel the maximum number of hits
    //for normalisation purposes
    std::map<unsigned, std::vector<unsigned> > _nHithelpVec;

    // maps with the mean (median) value of the buffers used to save triggered data
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMean;
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian;
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMean_overRunningBcid;
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian_overRunningBcid;
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMean_planeEvents;
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian_planeEvents;
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMean_negativeData;
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian_negativeData;


   // vector with the total number of triggers for each chip
    std::vector<unsigned>  _TrigChipVec;
    std::vector<unsigned>  _TrigChipVec_overRunningBcid;
    std::vector<unsigned>  _TrigChipVec_planeEvents;
    std::vector<unsigned>  _TrigChipVec_negativeData;

    //A simple map that holds the values for mean (median) of the overcounting rate for a selected channel
    std::map<unsigned,std::vector<Double_t> >  _overBcidCounterVecMapMean;
    std::map<unsigned,std::vector<Double_t> >  _overBcidCounterVecMapMedian;


    Double_t GetMean(std::vector<unsigned>);
    Double_t GetMedian(std::vector<unsigned>);
    Double_t GetMode(std::vector<unsigned>);


     double* vectortoarray(std::vector<double> thevec, double* theArray );
  //  PedestalManager pedestalmanager_;

};


#endif /* MONITORMANAGER_H_ */
