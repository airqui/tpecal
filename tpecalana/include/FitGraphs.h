// 
//  FitGraphs.h  
//  tpecalana  
//              
//  Created by Adrian Irles 16/12/2016     
//  Contain FitGraphs functions, to be used by the AnalysisManager


#include <stdio.h>
#include <sstream>
#include <vector>
#include <map>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TMath.h>
#include <TSpectrum.h>

using namespace std;

#ifndef __tpecalana_project__FitGraphs__
#define __tpecalana_project__FitGraphs__

class FitGraphs {
    
public:
    FitGraphs();
    ~FitGraphs();
    // TF1* GetScurveTF1(TGraph* gr);
    TF1* FitScurve(TGraphErrors* gr);

private:
    //   Double_t erf( Double_t *x, Double_t *par);
    // void errorfunc();

};


#endif /* defined(__tpecalana_project__FitGraphs__) */


