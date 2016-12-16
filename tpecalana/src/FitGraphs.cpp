// 
//  FitGraphs.cpp
//  tpecalana  
//              
//  Created by Adrian Irles 16/12/2016     
//  Contain FitGraphs functions, to be used by the AnalysisManager
#include "FitGraphs.h"

using namespace std;

FitGraphs::FitGraphs(){}

FitGraphs::~FitGraphs(){/* no op*/}

//----> start
//Double_t erf( Double_t *x, Double_t *par){   //<===========
//  return par[0]*TMath::Erf( (x[0]-par[1])/par[2] ) ; //<===========
//}

TF1 *FitGraphs::FitScurve(TGraphErrors *gr)
{
   
  double par1=0, par2=0, par3=0;

  TF1 *fit1 = new TF1("fit1","[0]*TMath::Erfc((x-[1])/[2])",170,230); 
  TF1 *fit2 = new TF1("fit2","[0]*TMath::Erfc((x-[1])/[2])+[3]",170,230); 

   fit1->SetParameter(0,60); 
   fit1->SetParameter(1,190);
   fit1->SetParameter(2,2);
   if(fit1 !=0 ) {
     fit1->SetParameter(0,1); 
     fit1->SetParameter(1,190); 
     gr->Fit("fit1");
     if(fit2 != 0) {
       par1=fit1->GetParameter(0); 
       par2=fit1->GetParameter(1); 
       par3=fit1->GetParameter(2); 
       fit2->SetParameter(0,par1); 
       fit2->SetParameter(1,par2); 
       fit2->SetParameter(2,par3); 
       gr->Fit("fit2","EM");
     }
   }
   return fit2;
}




