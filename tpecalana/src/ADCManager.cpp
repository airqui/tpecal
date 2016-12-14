/*
 * ADCManager.cpp
 *
 *  Created on: 13 Dec 2016
 *      Author: irles
 */

#include "ADCManager.h"
#include "FitHistograms.h"
#include "global.h"

using namespace::std;
using namespace globalvariables;


ADCManager::ADCManager(){
  _pedestalVecMap.clear();
  _pedestalVecMapTH1.clear();
  _tGraphPedestalVec.clear();
  _nRuns=0;
}

ADCManager::~ADCManager(){/* no op*/}


void ADCManager::init(){
  std::cout<<"init"<<std::endl;

  std::cout<<"Init the Analysis Manager for ADC analysis, with getAnalysisType()="<<globalvariables::getAnalysisType()<<std::endl;

  for (unsigned ichip=0;ichip<globalvariables::getEnabledChipsVec().size();ichip++) {
    //ADC maps (a vector of pedestal/error pedestal for threshold run and maximum of triggers for all threshold runs, per channel, memory cell and chip)
    _pedestalVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::map<unsigned, std::vector<std::vector<std::vector<Double_t> > > > () ) );
    _pedestalVecMapTH1.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::map<unsigned, std::vector<std::vector<TH1I*> > > () ) ); // only used if deepanalysys==true

  }
}

void ADCManager::acquireRunInformation(ExperimentalSetup* aExpSetup, TString file_sufix){
  std::cout << "************** AcquireRunInformation new file: " << _nRuns << "***********************" << std::endl;
  std::cout<<" ****Init the Analysis Manager for ADC analysis, with getAnalysisType()="<<globalvariables::getAnalysisType()<<" ****"<<std::endl;

  pedestalAnalysis(aExpSetup, file_sufix);
  // signalAnalysis(aExpSetup, file_sufix);
  _nRuns++;
}

void ADCManager::displayResults(){
  pedestalAnalysisGraphicsBasic();
  //  signalAnalysisGraphicsBasic();

}

// pedestal analysis, read the info and do a fit to the pedestal histogram per channel, buffer, scan value and chip.
// if we want to do calibration, the function will be very similar, just chosing events with hit bit =1 and making a more complicated fit

void ADCManager::pedestalAnalysis(ExperimentalSetup* aExpSetup, TString file_sufix) {
  TString gain = "";

  if( globalvariables::getGainAnalysis()==1) gain="_highGain";
  else if(globalvariables::getGainAnalysis()==0) gain="_lowGain";
  else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;
  f_pedestal_scan = TFile::Open("Pedestal_"+file_sufix+gain+".root", "RECREATE");

  //      Loop over all enabled chips
  for (bufferchannelInfoComplDouble_t::iterator mapiter_chip = _pedestalVecMap.begin(); mapiter_chip!=_pedestalVecMap.end();mapiter_chip++) {
    std::cout << "*************New chip, HIGH GAIN:" << (*mapiter_chip).first << "********************" << std::endl;
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getBufferDepth());
    // fill the iterator object for each chip with the right object
    for (unsigned ibuf=0;ibuf<bufdepth;ibuf++)
      ((*mapiter_chip).second).insert(std::make_pair(ibuf, std::vector<std::vector<std::vector<Double_t> > >  () ) );

    //    loop over each buffer column using a channelInfoComplDouble_t::iterator
    for (channelInfoComplDouble_t::iterator mapiter = (*mapiter_chip).second.begin();mapiter!=(*mapiter_chip).second.end();mapiter++) {
      std::cout << "New buffer, HIGH GAIN: " << (*mapiter).first << "" << std::endl;

      // loop over channels
      unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getNumberOfChannels());
      for (unsigned ichan=0; ichan < numChans; ichan++) {

	//Build up the vector for the individual channels (needs to be done only once)
	if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back (std::vector<std::vector<Double_t> > () );

	if( globalvariables::getGlobal_deepAnalysis()==true ) {

	  // FULL analysis with histograms for each channel, each memory cell, each chip, each asu, each dif
	  // too much memory and computing demand
	  // //save the histograms
	  TString gain = "";
	  if( globalvariables::getGainAnalysis()==1)   gain="_highGain";
	  else  if(globalvariables::getGainAnalysis()==0) gain="_lowGain";
	  else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;

	  f_pedestal_scan->cd();
	  std::stringstream TitleNameStr;
	  TitleNameStr << "Chip"<<(*mapiter_chip).first<<"_Chn"<<ichan<<"_buf"<<(*mapiter).first<<gain;//the iterator gives the chip ID

	  std::vector<int> ped = aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelPedHisto(ichan, globalvariables::getGainAnalysis());
	  TH1F *pedhisto = new TH1F(TitleNameStr.str().c_str(), TitleNameStr.str().c_str(), 4096,0.5,4096.5);
	  for(unsigned i=0; i< ped.size(); i++) pedhisto->Fill(ped.at(i));
	  pedhisto->SetName(TitleNameStr.str().c_str());
	  pedhisto->SetTitle(TitleNameStr.str().c_str());
	  pedhisto->Write();

	  // fit the histograms to gaussians, return mean and error (sqrt sigma / nentries)
	  FitHistograms fithistos;
	  std::vector<Double_t> ped_double = fithistos.FitPedestals(pedhisto);
	  (*mapiter).second.at(ichan).push_back(ped_double);
	  delete pedhisto;

	} else {
	  // naive analysis with mean and rms
	  //reads out the Mean of each channel, for entries with hit bit 0
	  unsigned ped_val;
	  unsigned ped_error;
	  if( globalvariables::getGainAnalysis()==1) {
	    ped_val=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelMean(ichan, "High", 0);
	    ped_error=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelRMS(ichan, "High", 0);
	  } else {
	    if( globalvariables::getGainAnalysis()==0) {
	      ped_val=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelMean(ichan, "Low", 0);
	      ped_error=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelRMS(ichan, "Low", 0);
	    } else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;

	    //Add for each run the value in that channel
	    //fills the pedestal values into a vector that is a part of a map of chips and channels
	    std::vector<Double_t> ped_double;
	    ped_double.push_back(ped_val);
	    ped_double.push_back(ped_error);
	    (*mapiter).second.at(ichan).push_back(ped_double);
	  }
	}

      }
    }
  }
}



// // pedestal analysis, read the info and do a fit to the pedestal histogram per channel, buffer, scan value and chip.
// // if we want to do calibration, the function will be very similar, just chosing events with hit bit =1 and making a more complicated fit

// void ADCManager::signalAnalysis(ExperimentalSetup* aExpSetup, TString file_sufix) {
//   TString gain = "";

//   if( globalvariables::getGainAnalysis()==1) gain="_highGain";
//   else if(globalvariables::getGainAnalysis()==0) gain="_lowGain";
//   else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;
//   f_pedestal_scan = TFile::Open("Pedestal_"+file_sufix+gain+".root", "RECREATE");

//   //      Loop over all enabled chips
//   for (bufferchannelInfoComplDouble_t::iterator mapiter_chip = _pedestalVecMap.begin(); mapiter_chip!=_pedestalVecMap.end();mapiter_chip++) {
//     std::cout << "*************New chip, HIGH GAIN:" << (*mapiter_chip).first << "********************" << std::endl;
//     unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getBufferDepth());
//     // fill the iterator object for each chip with the right object
//     for (unsigned ibuf=0;ibuf<bufdepth;ibuf++)
//       ((*mapiter_chip).second).insert(std::make_pair(ibuf, std::vector<std::vector<std::vector<Double_t> > >  () ) );

//     //    loop over each buffer column using a channelInfoComplDouble_t::iterator
//     for (channelInfoComplDouble_t::iterator mapiter = (*mapiter_chip).second.begin();mapiter!=(*mapiter_chip).second.end();mapiter++) {
//       std::cout << "New buffer, HIGH GAIN: " << (*mapiter).first << "" << std::endl;

//       // loop over channels
//       unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getNumberOfChannels());
//       for (unsigned ichan=0; ichan < numChans; ichan++) {

// 	//Build up the vector for the individual channels (needs to be done only once)
// 	if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back (std::vector<std::vector<Double_t> > () );

// 	if( globalvariables::getGlobal_deepAnalysis()==true ) {

// 	  // FULL analysis with histograms for each channel, each memory cell, each chip, each asu, each dif
// 	  // too much memory and computing demand
// 	  // //save the histograms
// 	  TString gain = "";
// 	  if( globalvariables::getGainAnalysis()==1)   gain="_highGain";
// 	  else  if(globalvariables::getGainAnalysis()==0) gain="_lowGain";
// 	  else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;

// 	  f_pedestal_scan->cd();
// 	  std::stringstream TitleNameStr;
// 	  TitleNameStr << "Chip"<<(*mapiter_chip).first<<"_Chn"<<ichan<<"_buf"<<(*mapiter).first<<gain;//the iterator gives the chip ID

// 	  std::vector<int> ped = aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelPedHisto(ichan, globalvariables::getGainAnalysis());
// 	  TH1F *pedhisto = new TH1F(TitleNameStr.str().c_str(), TitleNameStr.str().c_str(), 4096,0.5,4096.5);
// 	  for(unsigned i=0; i< ped.size(); i++) pedhisto->Fill(ped.at(i));
// 	  pedhisto->SetName(TitleNameStr.str().c_str());
// 	  pedhisto->SetTitle(TitleNameStr.str().c_str());
// 	  pedhisto->Write();

// 	  // fit the histograms to gaussians, return mean and error (sqrt sigma / nentries)
// 	  FitHistograms fithistos;
// 	  std::vector<Double_t> ped_double = fithistos.FitPedestals(pedhisto);
// 	  (*mapiter).second.at(ichan).push_back(ped_double);
// 	  delete pedhisto;

// 	} else {
// 	  // naive analysis with mean and rms
// 	  //reads out the Mean of each channel, for entries with hit bit 0
// 	  unsigned ped_val;
// 	  unsigned ped_error;
// 	  if( globalvariables::getGainAnalysis()==1) {
// 	    ped_val=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelMean(ichan, "High", 0);
// 	    ped_error=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelRMS(ichan, "High", 0);
// 	  } else {
// 	    if( globalvariables::getGainAnalysis()==0) {
// 	      ped_val=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelMean(ichan, "Low", 0);
// 	      ped_error=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelRMS(ichan, "Low", 0);
// 	    } else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;

// 	    //Add for each run the value in that channel
// 	    //fills the pedestal values into a vector that is a part of a map of chips and channels
// 	    std::vector<Double_t> ped_double;
// 	    ped_double.push_back(ped_val);
// 	    ped_double.push_back(ped_error);
// 	    (*mapiter).second.at(ichan).push_back(ped_double);
// 	  }
// 	}

//       }
//     }
//   }
// }


//
// basic functionality for graphic analysis of the pedestal data
// reads the different map objects and creates few th2f, for every chip, with pedestal mean, error etc values
//
void ADCManager::pedestalAnalysisGraphicsBasic() {

  TString gain = "";
  if( globalvariables::getGainAnalysis()==1) gain="_highGain";
  else if(globalvariables::getGainAnalysis()==0) gain="_lowGain";
  else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;
  f_pedestal = TFile::Open("Pedestal_"+gain+".root", "RECREATE");

  //Loop over all enabled chips to create the graphs and fill them
  for (bufferchannelInfoComplDouble_t::iterator mapiter_chip = _pedestalVecMap.begin();mapiter_chip!=_pedestalVecMap.end();mapiter_chip++) {

    // Painter
    TString canvasNameStr =
      TString::Format("Pedestals_for_Chip%i",(*mapiter_chip).first);//the iterator gives the chip ID
    
    TString errorNameStr = 
      TString::Format("PedestalsUncert_for_Chip%i",(*mapiter_chip).first);//the iterator gives the chip ID
    TCanvas* c_chip= new TCanvas(canvasNameStr, canvasNameStr,11,30,1600,700);
    //Divide the canvas
    c_chip->Divide(1,2);
    
    TH2F *pedestal_map = new TH2F(canvasNameStr,canvasNameStr,15,-0.5,14.5,64,-0.5,63.5);
    TH2F *pedestalUnc_map = new TH2F(errorNameStr,errorNameStr,15,-0.5,14.5,64,-0.5,63.5);

    // --------------------------------------------------------
    // fill the TH2Fs
    //Looping over all channels in the chip
    for (channelInfoComplDouble_t::iterator mapiter = (*mapiter_chip).second.begin(); mapiter!=(*mapiter_chip).second.end(); mapiter++) {
      //Loop over all channels
      //Helper variable to count channels
      unsigned ichan(0);
      std::vector<unsigned> chanlistVec;
      chanlistVec.clear();
      std::cout << "AnaManager::pedestalAnalysisGraphicsPainter: BufferNum: " << (*mapiter).first << endl;
      for (std::vector<std::vector<std::vector<Double_t> > >::iterator chanVeciter=(*mapiter).second.begin(); chanVeciter!=(*mapiter).second.end(); chanVeciter++) {
	std::cout << "AnaManager::pedestalAnalysisGraphicsPainter: BufferNum: " << (*mapiter).first << " Nchan: " << ichan << std::endl;
	//Loop over the entries with pedestal in the given runs
	//in principle we assumme that all runs are of the same type (not scan runs)
	for (std::vector<std::vector<Double_t> >::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
	  //Fetch the value for that run
	  if((*runIter).size()>1) {
	    std::cout << "AnaManager::pedestalAnalysisGraphicsBasic: BufferNum: " << (*mapiter).first << " Nchan: " << ichan << " pedestal: " << (*runIter).at(0) << "(" <<  (*runIter).at(1) <<")"<<std::endl;
	    double runval(static_cast<double>((*runIter).at(0)));
	    double erunval(static_cast<double>((*runIter).at(1)));
	    
	    pedestal_map->Fill((*mapiter).first,ichan,runval);
	    pedestalUnc_map->Fill((*mapiter).first,ichan,erunval);
	  }
	}
	ichan++;
      }
    }
    // -------------------------------------------------------

    //  unsigned ichn= (*chanVeciter).first;
    c_chip->cd(1);
   pedestal_map->SetTitle(canvasNameStr);
   pedestal_map->GetXaxis()->SetTitle("buffer");
   pedestal_map->GetYaxis()->SetTitle("channel");
   pedestal_map->Draw("colz");
   c_chip->cd(2);
   pedestalUnc_map->SetTitle(errorNameStr);
   pedestalUnc_map->GetXaxis()->SetTitle("buffer");
   pedestalUnc_map->GetYaxis()->SetTitle("channel");
   pedestalUnc_map->Draw("colz");

   f_pedestal->cd();
   // once than the plotting for one chip is finished, save the canvas
   canvasNameStr = TString::Format("Pedestals_for_Chip%i.png",(*mapiter_chip).first);//the iterator gives the chip ID
   c_chip->Print(canvasNameStr);
   c_chip->Write();
   pedestal_map->Write();
   pedestalUnc_map->Write();
  }
  
  f_pedestal->Close();
  
}

// ########################################################################
// run by run analysis, normally not very interesting for pedestal/adc studies
// but will be very useful for debug run settings or to test new features of the daq or the chips
// i.e. stability of the pedestal/mip position for different running conditions

void ADCManager::pedestalAnalysisGraphics() {

  TString gain = "";
  if( globalvariables::getGainAnalysis()==1) gain="_highGain";
  else if(globalvariables::getGainAnalysis()==0) gain="_lowGain";
  else std::cout<< "ERROR, you should define in which gain you do the analysis: globalvariables::setGainAnalysis() " <<std::endl;
  f_pedestal = TFile::Open("Pedestal_"+gain+".root", "RECREATE");

  //Loop over all enabled chips to create the graphs and fill them
  for (bufferchannelInfoComplDouble_t::iterator mapiter_chip = _pedestalVecMap.begin();mapiter_chip!=_pedestalVecMap.end();mapiter_chip++) {
    // fill the graphs using the _pedestalVecMap objects
    //(that contain pedestal + error for all channels, all buffers, all values of the scan variable and all chips in onw dif)
    pedestalAnalysisGraphicsFill(mapiter_chip);

    // Painter
    std::vector<TCanvas*> c_chips;
    for(unsigned ich=0; ich<64; ich++)  {
      //Declare and open a Canvas
      std::stringstream canvasNameStr;
      canvasNameStr << "Pedestals_for_Chip" << (*mapiter_chip).first<<"_Chn"<<ich;//the iterator gives the chip ID
      TCanvas* c_= new TCanvas(canvasNameStr.str().c_str(), canvasNameStr.str().c_str(),11,30,800,700);
      //Divide the canvas
      // c_chips->Divide(8,8);
      c_chips.push_back(c_);
    }

    for (TGraphPedestalVec_t::iterator graphBufferIter=_tGraphPedestalVec.begin(); graphBufferIter!=_tGraphPedestalVec.end(); graphBufferIter++) {

      unsigned buffer= (*graphBufferIter).first;
      unsigned numchans = (*graphBufferIter).second.size();

      unsigned ichn(0);
      for (std::vector<TGraphErrors*>::iterator chanVeciter=(*graphBufferIter).second.begin(); chanVeciter!=(*graphBufferIter).second.end(); chanVeciter++) {

      	//  unsigned ichn= (*chanVeciter).first;
  	c_chips.at(ichn)->cd();
  	if(buffer == 0) {
  	  (*chanVeciter)->Draw("ALP");
  	  (*chanVeciter)->GetXaxis()->SetTitle("threshold");
  	  (*chanVeciter)->GetYaxis()->SetTitle("Pedestal [ADC]");
  	  (*chanVeciter)->GetYaxis()->SetRangeUser(0,500);
  	  std::stringstream TitleNameStr;
  	  TitleNameStr << "Pedestals_for_Chn"<<ichn;//the iterator gives the chip ID
  	  (*chanVeciter)->SetTitle(TitleNameStr.str().c_str());
  	} else {
  	  (*chanVeciter)->Draw("LP");
  	}
      	(*chanVeciter)->SetMarkerStyle(20+buffer%5);
  	if(buffer<5) {
  	  (*chanVeciter)->SetLineColor(1);
  	  (*chanVeciter)->SetMarkerColor(1);
  	} else if(buffer<10) {
  	  (*chanVeciter)->SetLineColor(2);
  	  (*chanVeciter)->SetMarkerColor(2);
  	} else if(buffer<15) {
  	  (*chanVeciter)->SetLineColor(4);
  	  (*chanVeciter)->SetMarkerColor(4);
  	}

       	(*chanVeciter)->SetMarkerSize(1.2);
  	ichn++;
  	//}
      }
    }


    f_pedestal->cd();
    // once than the plotting for one chip is finished, save the canvas
    for(unsigned ich=0; ich<64; ich++)  {
      std::stringstream canvasNameStr;
      canvasNameStr << "Pedestals_for_Chip" << (*mapiter_chip).first<<"_Chn"<<ich<<".png";//the iterator gives the chip ID
      c_chips.at(ich)->Print(canvasNameStr.str().c_str());
      c_chips.at(ich)->Write();
    }
    f_pedestal->Close();

  }


}

void ADCManager::pedestalAnalysisGraphicsFill(bufferchannelInfoComplDouble_t::iterator aMapIter) {

  //Loop over all chips
  std::cout << "AnaManager::pedestalAnalysisGraphicsPainter Chip: " << (*aMapIter).first << std::endl;


  unsigned bufdepth((*aMapIter).second.size());

  for (unsigned ibuf=0;ibuf<bufdepth;ibuf++) {
    _tGraphPedestalVec.insert(std::make_pair(ibuf, std::vector<TGraphErrors*>  () ) );
  }

  for (channelInfoComplDouble_t::iterator mapiter = (*aMapIter).second.begin(); mapiter!=(*aMapIter).second.end(); mapiter++) {

    //Loop over all channels
    //Helper variable to count channels
    unsigned ichan(0);
    std::vector<unsigned> chanlistVec;
    chanlistVec.clear();
    std::cout << "AnaManager::pedestalAnalysisGraphicsPainter: BufferNum: " << (*mapiter).first << endl;
    for (std::vector<std::vector<std::vector<Double_t> > >::iterator chanVeciter=(*mapiter).second.begin(); chanVeciter!=(*mapiter).second.end(); chanVeciter++) {
      std::cout << "AnaManager::pedestalAnalysisGraphicsPainter: BufferNum: " << (*mapiter).first << " Nchan: " << ichan << std::endl;
      //At least as a sanity check we verify that the size of the vector for wach channel corresponds to the size of the
      //vector holding the thresholds as defined above
      if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
    	std::cout << "WARNING --> AnaManager::pedestalAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for channe" << ichan << std::endl;
    	std::cout << "WARNING --> Size of vector with thresholds is: " << globalvariables::getScanVectorDoubles().size() << std::endl;
    	std::cout << "WARNING --> Size of vector with readings is: " << (*chanVeciter).size()  << std::endl;
      }

      //Define an array that holds the pedestal mean and error for each run
      double valarray[ (*chanVeciter).size()];
      double evalarray[ (*chanVeciter).size()];

      //Loop over the entries with pedestal in the given runs
      //Helper variable to count runs
      unsigned irun(0);
      for (std::vector<std::vector<Double_t> >::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
    	//Fetch the value for that run
    	if((*runIter).size()>1) {
	  std::cout << "AnaManager::pedestalAnalysisGraphicsPainter: BufferNum: " << (*mapiter).first << " Nchan: " << ichan << " Run: " << irun << " pedestal: " << (*runIter).at(0) << "(" <<  (*runIter).at(1) <<")"<<std::endl;
	  double runval(static_cast<double>((*runIter).at(0)));
	  double erunval(static_cast<double>((*runIter).at(1)));
	  //Fill the array with the relative counts for a given channel in a given run
	  valarray[irun] = runval;
	  evalarray[irun] = erunval;
	}
    	irun++;
      }

      //Now define and fill a graph for each channel
      double vecarrayhelp[(*chanVeciter).size()];
      double* vecarray;
      vecarray = vectortoarray(globalvariables::getScanVectorDoubles(), &vecarrayhelp[0]);
      f_pedestal->cd();
      _tGraphPedestalVec.at((*mapiter).first).push_back(new TGraphErrors(globalvariables::getScanVectorDoubles().size(), vecarray, valarray, 0,  evalarray));
      ichan++;

    }
  }
}

double* ADCManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}


