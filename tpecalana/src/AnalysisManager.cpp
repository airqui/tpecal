/*
 * AnalysisManager.cpp
 *
 *  Created on: 13 Dec 2016
 *      Author: irles
 */

#include "AnalysisManager.h"
#include "FitHistograms.h"
#include "global.h"

using namespace::std;
using namespace globalvariables;


AnalysisManager::AnalysisManager(){
  _pedestalVecMap.clear();
  _pedestalVecMapTH1.clear();
  _tGraphPedestalVec.clear();
  _signalVecMap.clear();
  _signalVecMapTH1.clear();
  _tGraphSignalVec.clear();
  _nRuns=0;
  f_signal_scan=0;
  f_signal=0;
}

AnalysisManager::~AnalysisManager(){/* no op*/}


void AnalysisManager::init(){

  std::cout<<"Init the Analysis Manager for ADC analysis, with getAnalysisType()="<<globalvariables::getAnalysisType()<<std::endl;

  for (unsigned ichip=0;ichip<globalvariables::getEnabledChipsVec().size();ichip++) {
    //ADC maps (a vector of pedestal/error pedestal for threshold run and maximum of triggers for all threshold runs, per channel, memory cell and chip)
    _pedestalVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::map<unsigned, std::vector<std::vector<std::vector<Double_t> > > > () ) );
    _pedestalVecMapTH1.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::map<unsigned, std::vector<std::vector<TH1I*> > > () ) ); // only used if deepanalysys==true

    _signalVecMap.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::map<unsigned, std::vector<std::vector<std::vector<Double_t> > > > () ) );
    _signalVecMapTH1.insert(std::make_pair(globalvariables::getEnabledChipsVec().at(ichip), std::map<unsigned, std::vector<std::vector<TH1I*> > > () ) ); // only used if deepanalysys==true

  }
}

void AnalysisManager::acquireRunInformation(ExperimentalSetup* aExpSetup, TString file_sufix, TString pedestal_file_name=""){
  std::cout << "************** AcquireRunInformation new file: " << _nRuns << "***********************" << std::endl;
  std::cout<<" ****Init the Analysis Manager for ADC analysis, with getAnalysisType()="<<globalvariables::getAnalysisType()<<" ****"<<std::endl;

  if(globalvariables::getAnalysisType() == "Pedestal" ) pedestalAnalysis(aExpSetup, file_sufix);
  if(globalvariables::getAnalysisType() == "PedestalMIP" ) signalAnalysis(aExpSetup, file_sufix);
  if(globalvariables::getAnalysisType() == "MIP" ) {
    pedestalFileExtractor(file_sufix);
    signalAnalysis(aExpSetup, file_sufix);
  }

  _nRuns++;
}

void AnalysisManager::displayResults(TString file_sufix){

  if(globalvariables::getAnalysisType() == "Pedestal" ) pedestalAnalysisGraphicsBasic(file_sufix);
  if(globalvariables::getAnalysisType() == "PedestalMIP" ||  globalvariables::getAnalysisType() == "MIP" ) signalAnalysisGraphicsBasic( file_sufix );

}

// pedestal analysis, read the info and do a fit to the pedestal histogram per channel, buffer, scan value and chip.
// if we want to do calibration, the function will be very similar, just chosing events with hit bit =1 and making a more complicated fit

void AnalysisManager::pedestalAnalysis(ExperimentalSetup* aExpSetup, TString file_sufix) {

  TString gain = globalvariables::getGainTStringAnalysis();
  TFile *f_pedestal_scan = TFile::Open(file_sufix+gain+"_Pedestal.root", "RECREATE");
  TDirectory *cdtof = f_pedestal_scan->GetDirectory("pedestal_histos");
  if (!cdtof) cdtof = f_pedestal_scan->mkdir("pedestal_histos");

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
	  TString gain = globalvariables::getGainTStringAnalysis();

	  f_pedestal_scan->cd();
	  cdtof->cd();

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
	  std::vector<Double_t> ped_double = fithistos.FitPedestal(pedhisto);
	  (*mapiter).second.at(ichan).push_back(ped_double);
	  delete pedhisto;

	} else {
	  // naive analysis with mean and rms
	  //reads out the Mean of each channel, for entries with hit bit 0
	  unsigned ped_val;
	  unsigned ped_error;
	  if( globalvariables::getGainAnalysis()==1) {
	    ped_val=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelMeanVec(ichan, "High").at(1);
	    ped_error=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelRMSVec(ichan, "High").at(1);
	  } else {
	    if( globalvariables::getGainAnalysis()==0) {
	      ped_val=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelMeanVec(ichan, "Low").at(1);
	      ped_error=aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelRMSVec(ichan, "Low").at(1);
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
  f_pedestal_scan->Close();
}



void AnalysisManager::pedestalFileExtractor(TString file_sufix) {

  TString gain = globalvariables::getGainTStringAnalysis();

  TFile *f_pedestal = new TFile(file_sufix+gain+"_Pedestal.root");
  std::cout<< "Opening Pedestal File: "<<file_sufix<<gain<<"_Pedestal.root"<<std::endl;

  for (bufferchannelInfoComplDouble_t::iterator mapiter_chip = _pedestalVecMap.begin(); mapiter_chip!=_pedestalVecMap.end();mapiter_chip++) {

    TH2F *pedestal_map = (TH2F*)f_pedestal->Get(TString::Format("pedestal_results/Pedestals_for_Chip%i",(*mapiter_chip).first));
    TH2F *pedestalUncert_map = (TH2F*)f_pedestal->Get(TString::Format("pedestal_results/PedestalsUncert_for_Chip%i",(*mapiter_chip).first));

    for (unsigned ibuf=0;ibuf<15;ibuf++)  ((*mapiter_chip).second).insert(std::make_pair(ibuf, std::vector<std::vector<std::vector<Double_t> > >  () ) );

    //    loop over each buffer column using a channelInfoComplDouble_t::iterator
    for (channelInfoComplDouble_t::iterator mapiter = (*mapiter_chip).second.begin();mapiter!=(*mapiter_chip).second.end();mapiter++) {
      std::cout << "New buffer, HIGH GAIN: " << (*mapiter).first << "" << std::endl;
      
      for (unsigned ichan=0; ichan < 64; ichan++) {
	//Build up the vector for the individual channels (needs to be done only once)
	if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back (std::vector<std::vector<Double_t> > () );
	
	std::vector<Double_t> ped_double;
	ped_double.push_back(pedestal_map->GetBinContent((*mapiter).first+1,ichan+1));
	ped_double.push_back(pedestalUncert_map->GetBinContent((*mapiter).first+1,ichan+1));
	std::cout<<(*mapiter).first+1 <<" "<< ichan+1<<" "<<pedestal_map->GetBinContent((*mapiter).first+1,ichan+1)<<std::endl;
	(*mapiter).second.at(ichan).push_back(ped_double);
      }
    }

  }


}

// signal analysis, read the info and do a LandauGauss fit to the signal histogram per channel, buffer, scan value and chip.

void AnalysisManager::signalAnalysis(ExperimentalSetup* aExpSetup, TString file_sufix) {

  TString gain = globalvariables::getGainTStringAnalysis();

  TFile *f_signal_scan = TFile::Open(file_sufix+gain+"_MIP.root", "UPDATE");
  TDirectory *cdtof = f_signal_scan->GetDirectory("signal_histos");
  if (!cdtof) cdtof = f_signal_scan->mkdir("signal_histos");



  //      Loop over all enabled chips
  for (bufferchannelInfoComplDouble_t::iterator mapiter_chip = _signalVecMap.begin(); mapiter_chip!=_signalVecMap.end();mapiter_chip++) {
    std::cout << "*************New chip, HIGH GAIN:" << (*mapiter_chip).first << "********************" << std::endl;

    // fill the iterator object for each chip with the right object
    unsigned bufdepth(aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getBufferDepth());
    for (unsigned ibuf=0;ibuf<bufdepth;ibuf++)
      ((*mapiter_chip).second).insert(std::make_pair(ibuf, std::vector<std::vector<std::vector<Double_t> > >  () ) );

    //    loop over each buffer column using a channelInfoComplDouble_t::iterator
    for (channelInfoComplDouble_t::iterator mapiter = (*mapiter_chip).second.begin();mapiter!=(*mapiter_chip).second.end();mapiter++) {
      std::cout << "New buffer, HIGH GAIN: " << (*mapiter).first << "" << std::endl;

      // loop over channels
      unsigned numChans(aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getNumberOfChannels());
      for (unsigned ichan=0; ichan < numChans; ichan++) {

	//Build up the vector for the individual channels (needs to be done only once)
	if((*mapiter).second.size() < ichan+1) (*mapiter).second.push_back(std::vector<std::vector<Double_t> > () );

	if( globalvariables::getGlobal_deepAnalysis()==true ) {

	  // FULL analysis with histograms for each channel, each memory cell, each chip, each asu, each dif
	  // too much memory and computing demand
	  // //save the histograms
	  TString gain = globalvariables::getGainTStringAnalysis();

	  f_signal_scan->cd();
	  cdtof->cd();
	  std::stringstream TitleNameStr;
	  TitleNameStr << "Chip"<<(*mapiter_chip).first<<"_Chn"<<ichan<<"_buf"<<(*mapiter).first<<gain;//the iterator gives the chip ID

	  std::vector<double> ped;
	  ped = _pedestalVecMap.at((*mapiter_chip).first).at((*mapiter).first).at(ichan).at((*mapiter).second.at(ichan).size());

	  std::vector<int> sig = aExpSetup->getDif(0).getASU(0).getChip((*mapiter_chip).first).getChipBuffer((*mapiter).first).getChannelChargeHisto(ichan, globalvariables::getGainAnalysis());
	  TH1F *sighisto = new TH1F(TitleNameStr.str().c_str(), TitleNameStr.str().c_str(), 4096,0.5,4096.5);
	  for(unsigned i=0; i< sig.size(); i++) sighisto->Fill(sig.at(i)-ped.at(0));
	  sighisto->SetName(TitleNameStr.str().c_str());
	  sighisto->SetTitle(TitleNameStr.str().c_str());
	  sighisto->Write();

	  // fit the histograms to gaussians, return mean and error (sqrt sigma / nentries)
	  FitHistograms fithistos;
	  std::vector<Double_t> sig_double = fithistos.FitSignal(sighisto);
	  (*mapiter).second.at(ichan).push_back(sig_double);
	  delete sighisto;

	} 
      }
    }
  }
  f_signal_scan->Close();

}


void AnalysisManager::signalAnalysisGraphicsBasic(TString file_sufix) {
  //
  // basic functionality for graphic analysis of the signal data
  // reads the different map objects and creates few th2f, for every chip, with MIP Value and error
  //
}

void AnalysisManager::pedestalAnalysisGraphicsBasic(TString file_sufix) {

  //
  // basic functionality for graphic analysis of the pedestal data
  // reads the different map objects and creates few th2f, for every chip, with pedestal mean, error etc values
  //

  TString gain = globalvariables::getGainTStringAnalysis();

  TFile *f_pedestal_scan = TFile::Open(file_sufix+gain+"_Pedestal.root", "UPDATE");
  TDirectory *cdtof = f_pedestal_scan->GetDirectory("pedestal_results");
  if (!cdtof) cdtof = f_pedestal_scan->mkdir("pedestal_results");

  //Loop over all enabled chips to create the graphs and fill them

  for (bufferchannelInfoComplDouble_t::iterator mapiter_chip = _pedestalVecMap.begin();mapiter_chip!=_pedestalVecMap.end();mapiter_chip++) {

    // Painter
    TString canvasNameStr =
      TString::Format("Pedestals_for_Chip%i",(*mapiter_chip).first);//the iterator gives the chip ID
    
    TString errorNameStr = 
      TString::Format("PedestalsUncert_for_Chip%i",(*mapiter_chip).first);//the iterator gives the chip ID
    TCanvas* c_chip= new TCanvas(canvasNameStr, canvasNameStr,11,30,1600,700);
    //Divide the canvas
    c_chip->Divide(2,1);
    
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
      std::cout << "AnalysisManager::pedestalAnalysisGraphicsPainter: SCA: " << (*mapiter).first << endl;
      for (std::vector<std::vector<std::vector<Double_t> > >::iterator chanVeciter=(*mapiter).second.begin(); chanVeciter!=(*mapiter).second.end(); chanVeciter++) {
	std::cout << "AnalysisManager::pedestalAnalysisGraphicsPainter: SCA: " << (*mapiter).first << " Nchan: " << ichan << std::endl;
	//Loop over the entries with pedestal in the given runs
	//in principle we assumme that all runs are of the same type (not scan runs)
	for (std::vector<std::vector<Double_t> >::iterator runIter = (*chanVeciter).begin(); runIter != (*chanVeciter).end(); runIter++) {
	  //Fetch the value for that run
	  if((*runIter).size()>1) {
	    std::cout << "AnalysisManager::pedestalAnalysisGraphicsBasic: SCA: " << (*mapiter).first << " Nchan: " << ichan << " pedestal: " << (*runIter).at(0) << "(" <<  (*runIter).at(1) <<")"<<std::endl;
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

    f_pedestal_scan->cd();
    cdtof->cd();
    //  unsigned ichn= (*chanVeciter).first;
    c_chip->cd(1);
    pedestal_map->SetTitle(canvasNameStr);
    pedestal_map->GetXaxis()->SetTitle("SCA");
    pedestal_map->GetYaxis()->SetTitle("channel");
    pedestal_map->Draw("colz");
    c_chip->cd(2);
    pedestalUnc_map->SetTitle(errorNameStr);
    pedestalUnc_map->GetXaxis()->SetTitle("SCA");
    pedestalUnc_map->GetYaxis()->SetTitle("channel");
    pedestalUnc_map->Draw("colz");
    //  c_chip->Update();


    // once than the plotting for one chip is finished, save the canvas
    c_chip->Write();
    pedestal_map->Write();
    pedestalUnc_map->Write();
  }
  
  f_pedestal_scan->Close();
  
}

// ########################################################################
// run by run analysis, normally not very interesting for pedestal/adc studies
// but will be very useful for debug run settings or to test new features of the daq or the chips
// i.e. stability of the pedestal/mip position for different running conditions

void AnalysisManager::pedestalAnalysisGraphics() {

  TString gain = globalvariables::getGainTStringAnalysis();
  TFile *f_pedestal = TFile::Open("Pedestal_"+gain+".root", "RECREATE");

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

void AnalysisManager::pedestalAnalysisGraphicsFill(bufferchannelInfoComplDouble_t::iterator aMapIter) {

  //Loop over all chips
  TString gain = globalvariables::getGainTStringAnalysis();
  std::cout << "AnalysisManager::pedestalAnalysisGraphicsPainter Chip: " << (*aMapIter).first << std::endl;
  unsigned bufdepth((*aMapIter).second.size());

  for (unsigned ibuf=0;ibuf<bufdepth;ibuf++) {
    _tGraphPedestalVec.insert(std::make_pair(ibuf, std::vector<TGraphErrors*>  () ) );
  }

  TFile *f_pedestal = TFile::Open("Pedestal_"+gain+".root", "UPDATE");


  for (channelInfoComplDouble_t::iterator mapiter = (*aMapIter).second.begin(); mapiter!=(*aMapIter).second.end(); mapiter++) {

    //Loop over all channels
    //Helper variable to count channels
    unsigned ichan(0);
    std::vector<unsigned> chanlistVec;
    chanlistVec.clear();
    std::cout << "AnalysisManager::pedestalAnalysisGraphicsPainter: SCA: " << (*mapiter).first << endl;
    for (std::vector<std::vector<std::vector<Double_t> > >::iterator chanVeciter=(*mapiter).second.begin(); chanVeciter!=(*mapiter).second.end(); chanVeciter++) {
      std::cout << "AnalysisManager::pedestalAnalysisGraphicsPainter: SCA: " << (*mapiter).first << " Nchan: " << ichan << std::endl;
      //At least as a sanity check we verify that the size of the vector for wach channel corresponds to the size of the
      //vector holding the thresholds as defined above
      if (globalvariables::getScanVectorDoubles().size() != (*chanVeciter).size()) {
    	std::cout << "WARNING --> AnalysisManager::pedestalAnalysisGraphicsPainter Warning: Size of vector with thresholds does not correspond to size of vector with readings for channe" << ichan << std::endl;
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
	  std::cout << "AnalysisManager::pedestalAnalysisGraphicsPainter: SCA: " << (*mapiter).first << " Nchan: " << ichan << " Run: " << irun << " pedestal: " << (*runIter).at(0) << "(" <<  (*runIter).at(1) <<")"<<std::endl;
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

double* AnalysisManager::vectortoarray(std::vector<double> thevec, double* theArray ) {
  for (unsigned i=0; i<thevec.size(); i++) theArray[i] = thevec[i];
  return &theArray[0];
}


