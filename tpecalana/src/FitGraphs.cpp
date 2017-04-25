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

TF1 *FitGraphs::FitScurveGauss(TGraphErrors *gr)
{
   
  double par1=0, par2=0, par3=0;
  int imax = TMath::LocMax(gr->GetN(),gr->GetY()); 
  Double_t xmax,ymax;
  gr->GetPoint(imax, xmax, ymax);

  if(imax< 2 || imax > gr->GetN()-2) return 0;
  Double_t xmax_m1,ymax_m1;
  Double_t xmax_p1,ymax_p1;
  gr->GetPoint(imax+1, xmax_p1, ymax_p1);
  gr->GetPoint(imax-1, xmax_m1, ymax_m1);
  if(ymax_p1 <0.1 || ymax_m1<0.1) return 0;
  

  int ilowedge = TMath::LocMin(gr->GetN(),gr->GetX()); 
  int ihighedge = TMath::LocMax(gr->GetN(),gr->GetX()); 
  Double_t xledge, yledge;
  Double_t xhedge, yhedge;
  gr->GetPoint(ilowedge, xledge, yledge);
  gr->GetPoint(ihighedge, xhedge, yhedge);

  TF1 *fit1 = new TF1("fit1","gaus",TMath::Max(xledge,xmax-35),TMath::Min(xhedge,xmax+35)); 
  // fit1->SetParameter(0,par1/2);
  fit1->SetParameter(1,par2);
  fit1->SetParameter(2,par3);

  TGraphErrors *gr1 =  (TGraphErrors*)gr->Clone();	

  if(fit1 !=0 ) {
    gr1->Fit("fit1","R");

    par1=fit1->GetParameter(0); 
    par2=fit1->GetParameter(1); 
    par3=fit1->GetParameter(2); 
    TF1 *fit2 = new TF1("fit2","gaus",TMath::Max(xledge,par2-3*par3),TMath::Min(xhedge,par2+3*par3));
    if(fit2 != 0) {
      fit2->SetParameter(0,par1); 
      fit2->SetParameter(1,par2); 
      fit2->SetParameter(2,par3); 
      gr1->Fit("fit2","EMR");
    }

    par1=fit2->GetParameter(0); 
    par2=fit2->GetParameter(1); 
    par3=fit2->GetParameter(2); 
    TF1 *fit3 = new TF1("fit3","gaus", TMath::Max(xledge,par2-2*par3),TMath::Min(xhedge,par2+2*par3) ); 
    if(fit3 != 0) {
      fit3->SetParameter(0,par1); 
      fit3->SetParameter(1,par2); 
      fit3->SetParameter(2,par3); 
      gr->Fit("fit3","EMR");
    }
    return fit3;
  }
  return 0;
}

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
      gr->Fit("fit2","EMR");
    }
  }
  return fit2;
}


/*TF1 *FitGraphs::FitScurve(TGraphErrors *gr)
{
   
  double par1=0, par2=0, par3=0;

  TF1 *fit1 = new TF1("fit1","[0]*TMath::Erfc((x-[1])/[2])",170,210); 

  if(fit1 !=0 ) {
    fit1->SetParameter(0,1); 
    fit1->SetParameter(1,190); 
    gr->Fit("fit1");
    par1=fit1->GetParameter(0);
    par2=fit1->GetParameter(1);
    par3=fit1->GetParameter(2);
    TF1 *fit2 = new TF1("fit2","[0]*TMath::Erfc((x-[1])/[2])",par2-4*par3,par2+4*par3);
    fit2->SetParameter(0,par1);
    fit2->SetParameter(1,par2);
    fit2->SetParameter(2,par3);
    if(fit2 != 0) {
      gr->Fit("fit2","EMR");
      par1=fit2->GetParameter(0);
      par2=fit2->GetParameter(1);
      par3=fit2->GetParameter(2);
      TF1 *fit3 = new TF1("fit3","[0]*TMath::Erfc((x-[1])/[2])",par2-2*par3,par2+2*par3);
      if(fit3!=0) {
	fit3->SetParameter(0,par1);
	fit3->SetParameter(1,par2);
	fit3->SetParameter(2,par3);
	gr->Fit("fit3","EMR");
	return fit3;
      }
      return fit2;
    }
    return fit1;
  }
  return 0;
  }*/




