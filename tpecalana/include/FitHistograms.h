// 
//  FitHistograms.h  
//  tpecalana  
//              
//  Created by Adrian Irles 07/12/2016     
//  Contain FitHistograms functions, to be used by the AnalysisManager


#include <stdio.h>
#include <sstream>
#include <vector>
#include <map>
#include <TH1.h>
#include <TF1.h>
#include <TSpectrum.h>

using namespace std;

#ifndef __tpecalana_project__FitHistograms__
#define __tpecalana_project__FitHistograms__

class FitHistograms {
    
public:
    FitHistograms();
    ~FitHistograms();
    std::vector<Double_t> FitPedestals(TH1F* histo);

private:

};


#endif /* defined(__tpecalana_project__FitHistograms__) */


