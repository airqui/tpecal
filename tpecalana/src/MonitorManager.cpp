/*
 * MonitorManager.cpp
 *
 *  Created on: 5 Jan 2017
 *      Author: irles
 */

#include <MonitorManager.h>
#include "global.h"

using namespace globalvariables;

MonitorManager::~MonitorManager() {
	// TODO Auto-generated destructor stub
}

MonitorManager::MonitorManager() {
	// TODO Auto-generated constructor stub
	f_file=0;

	_ntrigVecMapHigh.clear();
	_nHithelpVec.clear();

  	_bufferVecMapMean.clear();
    _bufferVecMapMedian.clear();

    _overBcidCounterVecMapMean.clear();
    _overBcidCounterVecMapMedian.clear();

}



void MonitorManager::init(){

  for (unsigned ichip=0;ichip<  globalvariables::getEnabledChipsVec().size();ichip++) {
	  //scurves maps (triggers for threshold run and maximum of triggers for all threshold runs, per channel and chip)
	 _ntrigVecMapHigh.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<std::vector<unsigned> >  () ));
	 _nHithelpVec.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::vector<unsigned> () ));

	 _bufferVecMapMean.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));
	 _bufferVecMapMedian.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));

	 _overBcidCounterVecMapMean.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));
	 _overBcidCounterVecMapMedian.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip),  std::vector<Double_t> () ));

  }

}

void MonitorManager::acquireRunInformation(ExperimentalSetup* aExpSetup){
  std::cout << "************** MonitorManager::AcquireRunInformation new file: " << std::endl;
  simpleChannelAnalysis(aExpSetup);
}

void MonitorManager::displayResults(TString file_prefix){

  //Call the graphics part related to the simple graphics analysis (should be realised automatically if graphics is requires
  simpleChannelAnalysisGraphics(file_prefix);

}

void MonitorManager::simpleChannelAnalysis(ExperimentalSetup* aExpSetup) {

  //Loop over all enabled chips
  for (channelInfoComplUnsigned_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {
    std::cout << "------------ New chip: " << (*mapiter).first << " ---------------- " << std::endl;
    std::map<unsigned, std::vector<unsigned> >::iterator helpMapIter = _nHithelpVec.find((*mapiter).first);

    unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(0).getNumberOfChannels());
    for (unsigned ichan=0; ichan < numChans; ichan++) {

    	unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getBufferDepth());
   	    for (unsigned ibuf=0; ibuf < bufdepth; ibuf++) {
   	    	unsigned ntrigmtmp(0);
   	    	//Build up the vector for the individual channels (needs to be done only once)
   	    	if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back (std::vector<unsigned> () );
   	    	//reads out the trigger of each channel
   	    	ntrigmtmp=aExpSetup->getDif(0).getASU(0).getChip((*mapiter).first).getChipBuffer(ibuf).getChannelTriggers(ichan);
   	    	//Add for each run the value in that channel
   	    	//fills the triggers into a vector that is a part of a map of chips and channels
   	   	    for (unsigned itrig=0; itrig < ntrigmtmp; itrig++) (*mapiter).second.at(ichan).push_back(ibuf);

   	    	//Store the maximum nummber of triggers
   	    	if (helpMapIter != _nHithelpVec.end()) {
   	    		if ((*helpMapIter).second.size()<ichan+1 ) (*helpMapIter).second.push_back(ntrigmtmp);
   	    		else {
   	    			if (ntrigmtmp >  (*helpMapIter).second.at(ichan)) {
   	    				(*helpMapIter).second.at(ichan) = ntrigmtmp;
   	    			}
   	    		}
   	    	}
   	    }

   	    std::vector<unsigned> tempvect = (*mapiter).second.at(ichan);
   	    double mean = GetMean(tempvect);
   	    double median = GetMedian(tempvect);

   	   _bufferVecMapMean.at((*mapiter).first).push_back(mean);
   	   _bufferVecMapMedian.at((*mapiter).first).push_back(median);
    }
  }
}


/*void MonitorManager::deeperDataAnalysis(ExperimentalSetup* aExpSetup){

  std::cout << "Entering deeper analysis" << std::endl;
  //Programme to be worked down
  //accumulated number of hits as function of aquisition number per dif
  //for each run in the analysis
  _tGraphVec.push_back( std::vector<TGraph*> ()  );
  for (unsigned idif = 0; idif < aExpSetup->getNumberOfDifs(); idif++) {
    //Obviously we need a local copy of the vector for a safe access, let's think about it later
    std::vector<std::pair<unsigned, unsigned> > helpVec = aExpSetup->getDif(idif).getDifStatVec();
    unsigned nentr = helpVec.size();
    //unsigned nentr = aExpSetup->getDif(idif).getDifStatVec().size();
    double vecarray[nentr];
    double valarray[nentr];
    unsigned ientr(0);
    //Loop over the vector holding the run statistics for this dif and fill the arrays
    //for (std::vector<std::pair<unsigned, unsigned> >::iterator statveciter = aExpSetup->getDif(idif).getDifStatVec().begin(); statveciter != aExpSetup->getDif(idif).getDifStatVec().end(); statveciter++) {
    for (std::vector<std::pair<unsigned, unsigned> >::iterator statveciter = helpVec.begin(); statveciter != helpVec.end(); statveciter++) {
      //std::cout << "nentr: " << nentr << ", ientr: " << ientr << std::endl;
      //std::cout << "first: " << (*statveciter).first << ", second: " << (*statveciter).second << std::endl;
      vecarray[ientr] = static_cast<double>((*statveciter).first);
      valarray[ientr] = static_cast<double>((*statveciter).second);
      ientr++;

    }
    _tGraphVec.back().push_back(new TGraph(nentr, vecarray, valarray));
  }

  //xy hit map per DIF and ASU coloumn 0
  //xy hit map per DIF and ASU integrated over all coloumns

  //signals and pedestals per chip

  //Number of coloumns per ASU per chip

  //What else????

  //number of plane events per chip
}*/



/*void MonitorManager::simpleChannelAnalysisGraphics() {

  //    double att[8] = {5, 10, 20, 50, 100, 200, 500, 1000};
  double att[23] = {170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214};//
  //Open a canvas for high gain
  TCanvas* c_cob1high = new TCanvas("c_cob1high","c_cob1high",1000,800);
  c_cob1high->cd();
  //fill some graphs with the signals
  std::vector<TGraph*> graphVecHigh;
  unsigned icount(0);
  for (std::map<int,vector<double> >::iterator mapiter = _sigVecMapHigh.begin();mapiter!=_sigVecMapHigh.end();mapiter++) {
    double vecarrayhelp[ (*mapiter).second.size()];
    double* vecarray;
    vecarray = vectortoarray((*mapiter).second, &vecarrayhelp[0]);
    graphVecHigh.push_back(new TGraph(_nRuns, att, vecarray));
    graphVecHigh.back()->SetMarkerStyle(20+icount);
    graphVecHigh.back()->SetMarkerSize(1.2);
    if(icount==0)  graphVecHigh.back()->Draw("AP");
    else graphVecHigh.back()->Draw("PS");
    icount++;
  }


  //Open a canvas for low gain
  TCanvas* c_cob1low = new TCanvas("c_cob1low","c_cob1low",1000,800);
  c_cob1low->cd();
  //fill some graphs with the signals
  std::vector<TGraph*> graphVecLow;
  icount=0;
  for (std::map<int,vector<double> >::iterator mapiter = _sigVecMapLow.begin();mapiter!=_sigVecMapLow.end();mapiter++) {
    double vecarrayhelp[ (*mapiter).second.size()];
    double* vecarray;
    vecarray = vectortoarray((*mapiter).second, &vecarrayhelp[0]);
    graphVecLow.push_back(new TGraph(_nRuns, att, vecarray));
    graphVecLow.back()->SetMarkerStyle(20+icount);
    graphVecLow.back()->SetMarkerSize(1.2);
    if(icount==0)  graphVecLow.back()->Draw("AP");
    else graphVecLow.back()->Draw("PS");
    icount++;
  }
}*/


void MonitorManager::simpleChannelAnalysisGraphics(TString file_sufix) {


	//Declare and open a Canvas
	std::stringstream canvasNameStr;
	canvasNameStr << "Buffer_analysis";//the iterator gives the chip ID
	TCanvas* c_chips = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,1000,600);
	//Divide the canvas
	c_chips->Divide(2,1);

	//A vector of graphs
	TH2F * bufferMean = new TH2F("buffer_mean","buffer_mean",64,-0.5,63,16,-0.5,15.5);
	TH2F * bufferMedian = new TH2F("buffer_median","buffer_median",64,-0.5,63,16,-0.5,15.5);


  //Loop over all enabled chips
  for (channelInfoComplUnsigned_t::iterator mapiter = _ntrigVecMapHigh.begin();mapiter!=_ntrigVecMapHigh.end();mapiter++) {

  for(unsigned i=0; i<_bufferVecMapMean.at((*mapiter).first).size(); i++ ) {
	  	  bufferMean->Fill(i,double((*mapiter).first),_bufferVecMapMean.at((*mapiter).first).at(i));
	  	  bufferMedian->Fill(i,double((*mapiter).first),_bufferVecMapMedian.at((*mapiter).first).at(i));
  }

  }
  //bufferMean->Draw("colz");


  c_chips->cd(1);
  bufferMean->SetTitle("mean of buffer with triggered entries");
  bufferMean->GetXaxis()->SetTitle("channel");
  bufferMean->GetYaxis()->SetTitle("chip");
  bufferMean->Draw("colz");
  c_chips->cd(2);
  bufferMedian->SetTitle("median of buffer with triggered entries");
  bufferMedian->GetXaxis()->SetTitle("channel");
  bufferMedian->GetYaxis()->SetTitle("chip");
  bufferMedian->Draw("colz");

  c_chips->Update();

  TString gain = globalvariables::getGainTStringAnalysis();
  TFile *f_scurve = TFile::Open(file_sufix+gain+"_monitor.root", "RECREATE");
  TDirectory *dir = f_scurve->GetDirectory("buffer");
  if (!dir) dir = f_scurve->mkdir("buffer");

  f_scurve->cd();
  dir->cd();
  bufferMean->Write();
  bufferMedian->Write();
  f_scurve->Close();


}

/*void MonitorManager::deeperDataAnalysisGraphics() {
  displayRunStat();
}

void MonitorManager::displayRunStat() {

  //Display the accumulated events for a given run

  for (TGraphVec_t::iterator tgraphveciter = _tGraphVec.begin(); tgraphveciter != _tGraphVec.end(); tgraphveciter++) {
    //Loop over the individual difs
    unsigned idif(0);
    std::cout << "Size of grpah vec: " << (*tgraphveciter).size() << std::endl;
    for (std::vector<TGraph*>::iterator graphiter = (*tgraphveciter).begin(); graphiter != (*tgraphveciter).end(); graphiter++) {
      std::stringstream canvasNameStr;
      canvasNameStr << "DIF" << idif;
      TCanvas* c_dif = new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,1000,800);
      c_dif->cd();
      (*graphiter)->SetMarkerStyle(20+idif);
      (*graphiter)->SetMarkerSize(1.2);
      (*graphiter)->Draw("AP");
      idif++;
    }
  }


    graphRunStat.push_back(new TGraph(nentr, vecarray, valarray));
    graphRunStat.back()->SetMarkerStyle(20+idif);
    graphRunStat.back()->SetMarkerSize(1.2);
    graphRunStat.back()->Draw("P");

}*/

double* MonitorManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}

Double_t MonitorManager::GetMean(std::vector<unsigned> vec){
	Double_t sum = 0;
	if(vec.size()>0) {
		for (unsigned k = 0; k<vec.size(); k++)	sum+=vec.at(k);
		Double_t average = sum/vec.size();
		return average;
	} else return 0;
}

Double_t MonitorManager::GetMedian(std::vector<unsigned> vec){

	Double_t median=0;
	if(vec.size()>0) {
		size_t size = vec.size();
		sort(vec.begin(), vec.end());
		if (size  % 2 == 0) median = (vec[size / 2 - 1] + vec[size / 2]) / 2;
		else  median = vec[size / 2];
	}
	return median;

}

Double_t MonitorManager::GetMode(std::vector<unsigned> vec){

	if(vec.size()>0 ) {
    unsigned number = vec.at(0);
    unsigned mode = number;
    unsigned count = 1;
    unsigned countMode = 1;

    for (unsigned i=1; i<vec.size(); i++){
          if (vec.at(i) == number)  count++;// count occurrences of the current number
          else { // now this is a different number
        	  if (count > countMode) {
        		  countMode = count; // mode is the biggest ocurrences
                  mode = number;
        	  }
        	  count = 1; // reset count for the new number
              number = vec.at(i);
          }
    }
    return double(mode);
	} else return 0;

}
