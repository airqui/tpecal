//MRR, RC

#include <iostream>
#include <fstream>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "InfoChip.C"
////#include "TImage.h"


/*
Update library if needed :  .x FindNoisyN.C+
Call with :  
  FindNoisyN N;
  N.GetDataStats(filename);
  N.ReadFile(filename,iter, DIFid);
  N.Analyse(filename,0, DIFid);
  N.WriteFile(filename,true, DIFid);

Use RunFindNoisyN.C to call this from python
  os.system("/usr/local/root/bin/root -l -q /opt/calicoes/standard/RunFindNoisyN.C\(\\\"%s/%s_by_dif0.raw.root\\\"\)"%(datadir,runname))

Outputs :
  Text file (harcoded path) including calicoes commands
  Root file with statistics

*/


class FindNoisyN {

public:

 FindNoisyN(){  __debug=true; __iter=-999; NOISE_THRESHOLD = 0.01; OUTPUT_CommandFile ="/home/calice/data/AnalyseCalib.cmd";
     h_imgmasked = new TH2D("Masked_channels","Masked_Channels",NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
     h_imgmasked->Reset();
	 h_asumasked = new TH2I("Map_Masked_channels","Map_Masked_channels",32,-89.5,90-0.5,32,-89.5,90-0.5);
     h_asumasked->Reset();
	 info = new InfoChip();
		Nentries = 1;
    if (__debug) std::cout << "FindNoisyN  ---  Welcome  ---" << std::endl;

 };
 
~FindNoisyN(){
     delete h_imgmasked;
    if (__debug) std::cout << "FindNoisyN  ---  Goodby   ---" << std::endl;
};

void   GetDataStats(TString infilename); 
void   Analyse(TString infilename, Double_t TThres, int DIFid=0); 
void   ReadFile(TString inputFileName,  int iter=0, int DIFid=0);
void   WriteFile(TString inputFileName, bool overwrite=false, int DIFid=0);


protected:


private:


    bool __debug;
    Double_t NOISE_THRESHOLD;
    TString OUTPUT_CommandFile;
    int __iter;

  enum {
    NCHANNELS=64,
    NCOL=15,
    NCHIP=16
  };


	int Nentries;

    Double_t CumulatednumCol[NCHIP];
    Double_t CumulatednumCol_filtred[NCHIP];
    Double_t CumulatedHits[NCHIP];
    Double_t CumulatedHits_filtred[NCHIP];
	Double_t Stat_filtred[NCHIP];
    
    TGraph *g_CumulatednumCol;
    TGraph *g_CumulatednumCol_filtred;
    TGraph *g_CumulatedHits;
    TGraph *g_CumulatedHits_filtred;
    
    TH2D *h_imgmasked;
    TH2D *h_imgmasked_read;
    TH2D *h_imghit;
    TH2D *h_imghit_filtred;
	TH2I *h_asumasked;
    TFile* fout;
	InfoChip * info;


};//class


//******************************************************************************************************************
void   FindNoisyN::GetDataStats(TString infilename) {
    if (__iter<0) __iter=0; //real iter number not yet initialized
    TString name,title;

    //open pre-precessed data
    TFile *fin = new TFile(infilename,"read");
    TTree *fev10 = (TTree*)fin->Get("fev10"); //rawdata
    Nentries = fev10->GetEntries();
    //event data structures
    int charge_high[NCHIP][NCOL][NCHANNELS];
    int gain_hit_high[NCHIP][NCOL][NCHANNELS];
    int bcid[NCHIP][NCOL];
    int badbcid[NCHIP][NCOL];
    int numCol[NCHIP];


  
    //get data from file
    fev10->SetBranchAddress("gain_hit_high",gain_hit_high );
    fev10->SetBranchAddress("charge_hiGain",charge_high );
    fev10->SetBranchAddress("bcid", bcid);
    fev10->SetBranchAddress("badbcid", badbcid);
    fev10->SetBranchAddress("nColumns", numCol);

    if (__debug) std::cout << "FindNoisy::GetDataStats : step 1,  iter " << __iter << std::endl;



      //for each column,2 plot mean and rms, per chip and per channel
      for(int chip = 0;chip<NCHIP;chip++){
        CumulatednumCol[chip] = 0;
        CumulatednumCol_filtred[chip] = 0;
        CumulatedHits[chip] = 0;
        CumulatedHits_filtred[chip] = 0;
        /*for(int col = 0;col<NCOL;col++){
            //name  = Form("BCID_chip_%d_col_%d",chip+1,col);
            //title = Form("BCID, Chip %d column %d ",chip+1, col);
            //if (col==0) {h_BCIDdiff_chip_col[chip][col] = new TH1D(name,title,100,0,4096);}//bins,min,max
            //if (col>0) {h_BCIDdiff_chip_col[chip][col] = new TH1D(name,title,100,0,100);}//bins,min,max

          for(int chn = 0;chn<NCHANNELS;chn++){
            //create histograms per chip per column and per channel
            //name  = Form("chargeHigh_chip_%d_chn_%d_col_%d",chip+1,chn,col);
            //title = Form("chargeHigh, Chip %d channel %d column %d ",chip+1, chn, col);
            //h_HG_chip_col_chn[chip][col][chn] = new TH1D(name,title,100,100,500);//bins,min,max
            //name  = Form("chargeHigh_filt_chip_%d_chn_%d_col_%d",chip+1,chn,col);
            //title = Form("chargeHigh, Filtred Chip %d channel %d column %d ",chip+1, chn, col);
            //h_HG_chip_col_chn_filtred[chip][col][chn] = new TH1D(name,title,100,100,500);//bins,min,max
          }
        }*/
     }

 

        //for(int col = 0;col<NCOL;col++){
            //name  = Form("rms_per_col_all_chips_col_%d",col);
            //title = Form("rms_per_col_all_chips_col_%d",col);
            //h_rms_per_col_allchips[col] = new TH1D(name,title,100,0,25);//bins,min,max
            name  = "hit_all_chips";
            title = "hit_all_chips";    
	    h_imghit = new TH2D(name,title,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
            name  = "hit_filtered_all_chips";
            title =  "hit_filtered_all_chips";   
	    h_imghit_filtred = new TH2D(name,title,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);

	//}



   //fill histo with all entries
    for(unsigned entry = 0 ;entry<Nentries;entry++){
      fev10->GetEntry(entry);
      for(int chip = 0;chip<NCHIP;chip++){
        if (numCol[chip]>0) {CumulatednumCol[chip] += (Double_t) numCol[chip];
        for(int col = 0;col<numCol[chip];col++){
			if (badbcid[chip][col]<1) {CumulatednumCol_filtred[chip] += 1.0;}
          	for(int chn = 0;chn<NCHANNELS;chn++){
	    		if (gain_hit_high[chip][col][chn]%2==1) {
	       			h_imghit->Fill((Double_t) chn,(Double_t) chip,1);CumulatedHits[chip]+=1;
	       			if (badbcid[chip][col]<1) {h_imghit_filtred->Fill((Double_t) chn,(Double_t) chip,1);CumulatedHits_filtred[chip]+=1;}
	    		}
          	}
        }
      }//if numcol>0
      }//for chip

    }//for entry

    Double_t c [NCHIP];
    for (int i=0;i<NCHIP;i++) c[i]=i;

    g_CumulatednumCol = new TGraph(NCHIP,c,CumulatednumCol);
    g_CumulatednumCol_filtred = new TGraph(NCHIP,c,CumulatednumCol_filtred);

    g_CumulatedHits = new TGraph(NCHIP,c,CumulatedHits);
    g_CumulatedHits_filtred = new TGraph(NCHIP,c,CumulatedHits_filtred);

    // histo of rms for each col every channel & chip
    //for(int col = 0;col<NCOL;col++){ 
      //for(int chip = 0;chip<NCHIP;chip++){
          //for(int chn = 0;chn<NCHANNELS;chn++){
          //h_rms_per_col_allchips[col]->Fill(h_HG_chip_col_chn[chip][col][chn]->GetRMS());
        //}
      //}
   // }



}//GetEventStats


//******************************************************************************************************************
void   FindNoisyN::Analyse(TString infilename, Double_t TThres, int DIFid) {
    if (__iter<0) return;
    if (__debug) std::cout << "FindNoisy::Analyse : step 0,  iter " << __iter << std::endl;

    //GetDataStats(infilename );  

  NOISE_THRESHOLD = TThres;

    Double_t tmp;
    unsigned acc = 0 ;
           
    if (__debug) std::cout << "FindNoisy::Analyse : step 2, Noise Threshold : "<< NOISE_THRESHOLD << std::endl;
    if (__debug) 
    {

      for (int chip=0; chip<NCHIP; chip++) {
         for (int chn=0; chn<NCHANNELS; chn++) {//bins start at index 1.
	   tmp = h_imgmasked->GetBinContent(chn+1,chip+1);
	   if (tmp  >0 ) {acc++;}
	  }
      }
      std::cout << "FindNoisy::Analyse : start with " << acc << " masked" << std::endl;
    }


// Diagnostics, 
     acc = 0 ;
    ofstream log;
    bool NeedReconfigure = false;
    //log.open("/home/calice/data/AnalyseCalib.cmd"); //TODO : take name from "infilename"
    log.open(OUTPUT_CommandFile); //TODO : take name from "infilename"
    //Float_t rms_thres = h_rms_per_col_allchips[0]->GetMean()+h_rms_per_col_allchips[0]->GetRMS();
    //std::cout << "   RMS threshold: " << rms_thres << std::endl;
    //log << "#!/usr/bin/python2"<< std::endl;
    //log << "import sys, time"  << std::endl;
    //log << "from pycaldaq import *" << std::endl;

	//check that stat is enough
    for(int chip = 0;chip<NCHIP;chip++){			   
	  //Stat_filtred[chip] = NCOL*Nentries*NOISE_THRESHOLD;
	  Stat_filtred[chip] = CumulatednumCol_filtred[chip]*NOISE_THRESHOLD;
	  if (__debug && Stat_filtred[chip] < 2) std::cout << "FindNoisy::Analyse : not enough statistics for chip " << chip <<  std::endl;
    }

      for(int chip = 0;chip<NCHIP;chip++){               
        	  for(int chn = 0;chn<NCHANNELS;chn++){
			  //if (HG_rms[chip][0][chn]>rms_thres) {
			  //if (h_imghit->GetBinContent(chn+1,chip+1)> CumulatednumCol[chip]/50) {
			  
			  //150519 tmp = h_imghit_filtred->GetBinContent(chn+1,chip+1)/Stat_filtred[chip];
			  tmp = h_imghit_filtred->GetBinContent(chn+1,chip+1)/(NCOL*Nentries*0.75); //0.75 : expected ratio of filtred data at low trig rate

			  ////tmp = h_imghit->GetBinContent(chn+1,chip+1);
			  //Require enough statistics for applying mask
			  /////if ((CumulatednumCol_filtred[chip] > 5/sqrt(NOISE_THRESHOLD)) && (tmp > CumulatednumCol[chip]*NOISE_THRESHOLD)) {
			  /////if ((tmp > CumulatednumCol_filtred[chip]*NOISE_THRESHOLD) && (tmp > 5)) {
			  /////if ((CumulatednumCol_filtred[chip] > 1/NOISE_THRESHOLD) && (tmp > 1+CumulatednumCol_filtred[chip]*NOISE_THRESHOLD)) {

			  //150519 if ((tmp > (1+(CumulatednumCol_filtred[chip]*NOISE_THRESHOLD)))) {
			  if (tmp > NOISE_THRESHOLD) {
	        		 //h_imgmasked->Fill((Double_t) chn,(Double_t) chip,1); acc++;
	    		 h_imgmasked->SetBinContent(chn+1, chip+1,(Double_t) __iter+1); acc++;
	    		 //log << "reconfigure(\"skiroc_1_1_1_" << chip+1 << "\",\"disallow_trig_chans_skiroc\",\"" << chn << "\")" << std::endl; //old calicoes versions
	    		 log << "reconfigure(\"skiroc_1_1_" << DIFid+1 << "_1_" << chip+1 << "\",\"disallow_trig_chans_skiroc\",\"" << chn << "\")" << std::endl;
	    		 //log << "reconfigure(\"skiroc_1_1_1_1_" << chip+1 << "\",\"disable_preamp_chans_skiroc\",\"" << chn << "\")" << std::endl;
            		 NeedReconfigure = true;
			  }//if
	  
        }//chn
      }//chip
    if (__debug) std::cout << "FindNoisy::Analyse : added " << acc << " masked" << std::endl;

    if (__debug) 
    {
     acc = 0 ;

      for (int chip=0; chip<NCHIP; chip++) {
         for (int chn=0; chn<NCHANNELS; chn++) {//bins start at index 1.
	 tmp = h_imgmasked->GetBinContent(chn+1,chip+1);
	   if (tmp  >0 ) {acc++;
	   	   			  h_asumasked->Fill(info->GetX(chip,chn),info->GetY(chip,chn), (int)tmp);
					 }
	  }
      }
    std::cout << "FindNoisy::Analyse : end with " << acc << " masked" << std::endl;
    }

    if (NeedReconfigure==false)  {log << "quit" << std::endl;}

    if (__debug) std::cout << "FindNoisy::Analyse : Last step" << std::endl;


  }//Analyse




//******************************************************************************************************************
void FindNoisyN::ReadFile(TString inputFileName, int iter, int DIFid)
{
TString name;
Double_t tmp;
unsigned acc = 0 ;
__iter=iter;
  if(iter==0){
    if (__debug) std::cout << "FindNoisy::ReadFile : iter=0 /" << __iter << std::endl;
    return;

  }
  else {
    //fout = new TFile(inputFileName+"root","recreate");
    name = "/home/calice/data/FindNoisy"; name += DIFid ; name += ".root";
    fout = new TFile(name,"read");
    if(!fout->IsOpen()){
        std::cout <<"<!> ERROR <!>   File connot be read, reset histo !"<< std::endl;
	// Issue : h_imgmasked_read not created...
        return;}
    if (__debug) std::cout << "FindNoisy::ReadFile : found file " << std::endl;
    //Get data from previous iteration      
    h_imgmasked_read = (TH2D* ) fout->Get("Masked_channels");
    // Copy data in local structure
          //h_imgmasked = (TH2D*) h_imgmasked_read->Clone();  ???
          //h_imgmasked_read->Copy(*h_imgmasked); //????
          //h_imgmasked_read->Clone(*h_imgmasked); //????
      for (int chip=0; chip<NCHIP; chip++) {
         for (int chn=0; chn<NCHANNELS; chn++) {//bins start at index 1.
	   tmp = h_imgmasked_read->GetBinContent(chn+1,chip+1);
	   if (tmp>0) {acc++;}
	   h_imgmasked->SetBinContent(chn+1,chip+1,tmp);
	  }
      }
	  
    if (__debug) std::cout << "FindNoisy::ReadFile : read " << acc << " masked" << std::endl;
    if (__debug) std::cout << "FindNoisy::ReadFile : data copied " << std::endl;
    fout->Close();
  }
  
    if (__debug) std::cout << "FindNoisy::ReadFile : Last step" << std::endl;

  return;


}//ReadFile




//******************************************************************************************************************
void FindNoisyN::WriteFile(TString inputFileName, bool overwrite, int DIFid)
{
TString name;

    if (__iter<0) return;
    if (__debug) std::cout << "FindNoisy::WriteFile : step 0, iter=" << __iter << std::endl;
    name = "/home/calice/data/FindNoisy"; name += DIFid ; name += ".root";

   if(__iter>0){
         if (__debug) std::cout << "FindNoisy::WriteFile : IsOpen step 0" << std::endl;
         std::cout <<"FindNoisy::WriteFile> Updating file"<< std::endl;
	 fout = new TFile(name,"recreate");//update
        if(!fout->IsOpen()){
           std::cout <<"FindNoisy::WriteFile> <!> ERROR <!>   File not found!"<< std::endl;
           return;}
         if (__debug) std::cout << "FindNoisy::WriteFile : IsOpen step opened" << std::endl;
         }
   else {
        if(!overwrite){
            if (__debug) std::cout << "FindNoisy::WriteFile : iter=0 create" << std::endl;
            //fout = new TFile(inputFileName+"root","create");
            fout = new TFile(name,"create");
            if(!fout->IsOpen()){ //issue when file already exists...
               std::cout <<"FindNoisy::WriteFile> <!> ERROR <!>   File already created!"<< std::endl;
               return;}
            }     
        else {
          //fout = new TFile(inputFileName+"root","recreate");
          if (__debug) std::cout << "FindNoisy::WriteFile :  recreate" << std::endl;
          fout = new TFile(name,"recreate"); 
            if(!fout->IsOpen()){ 
               std::cout <<"FindNoisy::WriteFile> <!> ERROR <!>   File not opened !"<< std::endl;
               return;}
        }
   }

   if (__debug) std::cout << "FindNoisy::WriteFile : step 1" << std::endl;

/*       for(int chip = 0;chip<NCHIP;chip++){               
        	  for(int chn = 0;chn<NCHANNELS;chn++){
				 h_asumasked->Fill(info->GetX(chip,chn),info->GetY(chip,chn), h_imgmasked->GetBinContent(chn+1,chip+1));
        		}
   		}
 */

     h_imghit->Write();
     h_imghit_filtred->Write();
     h_imgmasked->Write();
	 h_asumasked->Write();
     g_CumulatednumCol->Write("Cumulated numCol");
     g_CumulatednumCol_filtred->Write("Cumulated numCol, filtered");
     g_CumulatedHits->Write("Cumulated Hits per chip");
     g_CumulatedHits_filtred->Write("Cumulated Hits per chip, filtered");
     //if(iter>0){h_imgmasked_read->Write();}
  
     fout->cd();
     fout->Write(0);
     fout->Close();


   if (__debug) std::cout << "FindNoisy::WriteFile : last step" << std::endl;
  

  return;


}//WriteFile









