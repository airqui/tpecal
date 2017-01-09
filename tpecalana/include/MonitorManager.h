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
#include <TLegend.h>
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
    void acquireRunInformation(ExperimentalSetup*, TString);
    //A method called to display results of whatever nature
    void displayResults(TString, TString);

private:

    TFile* f_file;

    //A method for a simple channel by channel
    void simpleChannelAnalysis(ExperimentalSetup*,TString);
    //The graphical representation
    void simpleChannelAnalysisGraphics(TString, TString);

    //A map that holds the number of triggers of all channels for all buffers for all enabled chips
    typedef std::map<unsigned, std::vector<std::vector<unsigned> > > channelInfoComplUnsigned_t;
    channelInfoComplUnsigned_t _ntrigVecMapHigh; 

    // maps with the mean (median) value of the buffers used to save triggered data
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian; // median of triggerd buffers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChanneVecMap; //ntriggers per chip and channel

   // vector with the total number of triggers for each chip
    std::vector<unsigned>  _TrigChipVec;
    std::vector<unsigned>  _TrigChipVec_bcid1;
    std::vector<unsigned>  _TrigChipVec_bcid5;
    std::vector<unsigned>  _TrigChipVec_bcid10;
    std::vector<unsigned>  _TrigChipVec_planeEvents;
    std::vector<unsigned>  _TrigChipVec_negativeData;


    Double_t GetMean(std::vector<unsigned>);
    Double_t GetMedian(std::vector<unsigned>);
    Double_t GetMode(std::vector<unsigned>);


     double* vectortoarray(std::vector<double> thevec, double* theArray );
  //  PedestalManager pedestalmanager_;

};


#endif /* MONITORMANAGER_H_ */
