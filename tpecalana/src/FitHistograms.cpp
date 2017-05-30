// 
//  FitHistograms.cpp
//  tpecalana  
//              
//  Created by Adrian Irles 07/12/2016     
//  Contain FitHistograms functions, to be used by the AnalysisManager
#include "FitHistograms.h"

using namespace std;

FitHistograms::FitHistograms(){}

FitHistograms::~FitHistograms(){/* no op*/}

std::vector<Double_t> FitHistograms::FitPedestal(TH1F * pedestal_histo){


  // Fit to a gaussian

  double pedestal_mean =0;
  double pedestal_rms =0;
  double pedestal_error =0;

  // find first peak using the TSpectrum method
  TSpectrum *s = new TSpectrum(5,1);
  s->Search(pedestal_histo, 1);
  
  Double_t *PeakPosX;
  int NPeaks;
  
  NPeaks = s->GetNPeaks();
  PeakPosX = s->GetPositionX();

  std::cout<<" Npeaks = "<<NPeaks<<std::endl;  

  if (NPeaks>0){
    //rough first fit
    pedestal_histo->Fit("gaus","","", PeakPosX[0]-50, PeakPosX[0]+50);
    TF1* fit = pedestal_histo->GetFunction("gaus");
    if (fit != 0){
      pedestal_mean = fit->GetParameter(1);
      pedestal_rms = fit->GetParameter(2);
      pedestal_histo->Fit("gaus","","", pedestal_mean-3*pedestal_rms, pedestal_mean+3*pedestal_rms);
      TF1* fit2 = pedestal_histo->GetFunction("gaus");
      if (fit2 != 0){
	pedestal_mean = fit2->GetParameter(1);
	//for the error calculation, we assume that the input histograms has bin size = 1 ADC !!
	pedestal_error = fit2->GetParameter(2)/sqrt(pedestal_histo->Integral(pedestal_mean-3*pedestal_rms, pedestal_mean+3*pedestal_rms));
      } else {
	pedestal_mean = 0;
	pedestal_error = 0;
      }
    } else {
      pedestal_mean = 0;
      pedestal_error = 0;
    }
  }

  std::vector<Double_t> pedestal;
  pedestal.push_back(pedestal_mean);
  pedestal.push_back(pedestal_error);

  return pedestal;
}


std::vector<Double_t> FitHistograms::FitSignal(TH1F * pedestal_histo){


  std::vector<Double_t> pedestal;
  pedestal.push_back(-999); // MPV
  pedestal.push_back(-999); // Nentries
  pedestal.push_back(-999); // other parameters of the fit


  return pedestal;
}

