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
#include "Mapping.h"

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

    // CHANNEL MONITORING
    //A method for a simple channel by channel
    void simpleChannelAnalysis(ExperimentalSetup*);
    //The graphical representation
    void simpleChannelAnalysisGraphics(TString);
    void simpleFilteredChannelAnalysisGraphics(TString);
    void simpleTaggedChannelAnalysisGraphics(TString);
    void simpleTaggedChannelAnalysisGraphics_2(TString);


    //CHANNEL FILTERED EVENTS ANALYSIS
    //A map that holds the number of triggers of all channels for all buffers for all enabled chips
    typedef std::map<unsigned, std::vector<std::vector<Double_t> > > channelInfoComplUnsigned_t;
    channelInfoComplUnsigned_t _ntrigVecMapHigh; 

    // maps with the mean (median) value of the buffers used to save triggered data
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian; // median of triggerd buffers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap; //npedestals per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap_buf0; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap_buf0; //npedestals per chip and channel

    std::map<unsigned,std::vector<Double_t> > _PedWidthChipChannelVecMap; //pedestal width per chip and channel (one buffer)
    std::map<unsigned,std::vector<Double_t> > _SignalWidthChipChannelVecMap; //signal width per chip and channel (one buffer)
    std::map<unsigned,std::vector<Double_t> > _PedMeanChipChannelVecMap; //pedestal mean per chip and channel (one buffer)
    std::map<unsigned,std::vector<Double_t> > _SignalMeanChipChannelVecMap; //signal mean per chip and channel (one dbuffer)

    //CHANNEL TAGGED EVENTS ANALYSIS
    channelInfoComplUnsigned_t _ntrigVecMapHigh_retrig; 
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian_retrig; // median of triggerd buffers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap_retrig; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap_retrig; //npedestals per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap_buf0_retrig; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap_buf0_retrig; //npedestals per chip and channel
    
    channelInfoComplUnsigned_t _ntrigVecMapHigh_negative; 
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian_negative; // median of triggerd buffers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap_negative; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap_negative; //npedestals per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap_buf0_negative; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap_buf0_negative; //npedestals per chip and channel

    channelInfoComplUnsigned_t _ntrigVecMapHigh_plane; 
    std::map<unsigned,std::vector<Double_t> >  _bufferVecMapMedian_plane; // median of triggerd buffers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap_plane; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap_plane; //npedestals per chip and channel
    std::map<unsigned,std::vector<unsigned> > _TrigChipChannelVecMap_buf0_plane; //ntriggers per chip and channel
    std::map<unsigned,std::vector<unsigned> > _PedChipChannelVecMap_buf0_plane; //npedestals per chip and channel
    
   // vector with the total number of triggers for each chip
    std::vector<unsigned>  _TrigChipVec;
    std::vector<unsigned>  _TrigChipVec_bcid1;
    std::vector<unsigned>  _TrigChipVec_bcid5;
    std::vector<unsigned>  _TrigChipVec_bcid10;
    std::vector<unsigned>  _TrigChipVec_planeEvents;
    std::vector<unsigned>  _TrigChipVec_negativeData;

    //vectors with the total number of sca that had triggers per chip
    std::vector<unsigned>  _TrigChipVec_sca; //vector of unsigned
    std::vector<unsigned>  _TrigChipVec_retrig_sca; //vector of unsigned
    std::vector<unsigned>  _TrigChipVec_planeEvents_sca; //vector of unsigned
    std::vector<unsigned>  _TrigChipVec_negativeData_sca; //vector of unsigned

    std::vector<double>  _RetriggerRatesChipVec;
    std::vector<double>  _PlaneRatesChipVec;
    std::vector<double>  _NegativeRatesChipVec;
	
    // CHIP MONITORING
    //A method for a simple channel by channel
    void simpleChipAnalysis(ExperimentalSetup*);
    //The graphical representation
    void simpleChipAnalysisGraphics(TString);

    std::map<unsigned,std::vector<Int_t> > _NhitsChipMap; //nhits rates per chip, bufer >0
    std::map<unsigned,std::vector<Int_t> > _NhitsChipMap_buf0; //nhits rates per chip, bufer== 0

    std::map<unsigned,std::vector<Int_t> > _BcidChipMap; //bcid per chip, bufer >0
    std::map<unsigned,std::vector<Int_t> > _BcidChipMap_buf0; //bcid per chip, bufer== 0

    std::map<unsigned,std::vector<Int_t> > _RetrigNhitsChipMap; //nhits rates per chip, bufer >0, badbcid>0 && badbcid<16
    std::map<unsigned,std::vector<Int_t> > _RetrigNhitsChipMap_buf0; //nhits rates per chip, bufer== 0, badbcid>0  && badbcid<16

    std::map<unsigned,std::vector<Int_t> > _RetrigBcidChipMap; //bcid per chip, bufer >0, badbcid>0  && badbcid<16
    std::map<unsigned,std::vector<Int_t> > _RetrigBcidChipMap_buf0; //bcid per chip, bufer== 0, badbcid>0 && badbcid<16
    
    std::map<unsigned,std::vector<Int_t> > _NegativeNhitsChipMap; //nhits rates per chip, bufer >0, badbcid>30, negative events
    std::map<unsigned,std::vector<Int_t> > _NegativeNhitsChipMap_buf0; //nhits rates per chip, bufer== 0, badbcid>30 , negative events

    std::map<unsigned,std::vector<Int_t> > _NegativeBcidChipMap; //bcid per chip, bufer >0, badbcid>30 , negative events
    std::map<unsigned,std::vector<Int_t> > _NegativeBcidChipMap_buf0; //bcid per chip, bufer== 0, badbcid>30, negative events

    std::map<unsigned,std::vector<Int_t> > _PlaneNhitsChipMap; //nhits rates per chip, bufer >0, badbcid>30, negative events
    std::map<unsigned,std::vector<Int_t> > _PlaneNhitsChipMap_buf0; //nhits rates per chip, bufer== 0, badbcid>30 , negative events

    std::map<unsigned,std::vector<Int_t> > _PlaneBcidChipMap; //bcid per chip, bufer >0, badbcid>30 , negative events
    std::map<unsigned,std::vector<Int_t> > _PlaneBcidChipMap_buf0; //bcid per chip, bufer== 0, badbcid>30, negative events
    
    // AUXILIARY FUNCTIONS
    Double_t GetMean(std::vector<Double_t>);
    Double_t GetMedian(std::vector<Double_t>);
    Double_t GetMode(std::vector<Double_t>);
     double* vectortoarray(std::vector<double> thevec, double* theArray );

};


#endif /* MONITORMANAGER_H_ */
