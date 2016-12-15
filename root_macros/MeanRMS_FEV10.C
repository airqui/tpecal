//MRR, RC

#include <iostream>
#include <fstream>

#include "TString.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "InfoChip.C"
////#include "TImage.h"


// .x MeanRMS_FEV10.C+
// MRMS=MeanRMS_FEV10("/home/calice/data/SCurveAll_FEV10/scurve_trig210_by_dif0.raw.root");

class MeanRMS_FEV10 {

public:

  MeanRMS_FEV10 (TString infilename) {
    TString name,title;
    //Color_t Color[4]={kBlack,kRed,kGreen,kBlue};
    //TPaveStats *ps;
    //TH1F *h_in;

    //open pre-precessed data
    TFile *fin = new TFile(infilename,"read");
    TTree *fev10 = (TTree*)fin->Get("fev10"); //rawdata

    //event data structures
    int charge_high[NCHIP][NCOL][NCHANNELS];
    int gain_hit_high[NCHIP][NCOL][NCHANNELS];
    int bcid[NCHIP][NCOL];
    int badbcid[NCHIP][NCOL];


    //computed data structures
    Float_t HG_means[NCHIP][NCOL][NCHANNELS];   
    Float_t HG_rms[NCHIP][NCOL][NCHANNELS]; 
     Float_t HG_means_filtred[NCHIP][NCOL][NCHANNELS];   
    Float_t HG_rms_filtred[NCHIP][NCOL][NCHANNELS]; 
    //Float_t means[NCHIP][NCHANNELS];
    //Float_t rmss [NCHIP][NCHANNELS];
    Float_t tmp_data[NCHANNELS];//used to fill TGraphErrors
    Float_t tmp_rms[NCHANNELS];//used to fill TGraphErrors

    Float_t channels [NCHANNELS];
    for (int i=0;i<NCHANNELS;i++) channels[i]=i;
    Float_t zeros[NCHANNELS];
    for (int i=0;i<NCHANNELS;i++) zeros[i]=0.0;
  
    TH2D *h_img[NCOL];
    TH2D *h_img_filtred[NCOL];
    TH2D *h_imgrms[NCOL];
    TH2D *h_imgrms_filtred[NCOL];
    TH2D *h_asu[NCOL];
    TH2D *h_asu_filtred[NCOL];
    TH2D *h_asurms[NCOL];
    TH2D *h_asurms_filtred[NCOL];

    //get data from file
    fev10->SetBranchAddress("gain_hit_high",gain_hit_high );
    fev10->SetBranchAddress("charge_hiGain",charge_high );
    fev10->SetBranchAddress("bcid", bcid);
    fev10->SetBranchAddress("badbcid", badbcid);



    //create new file
    TFile *fout = new TFile(infilename.ReplaceAll(".root","_pedestal.root"),"recreate");
    TDirectory *d_hists = fout->mkdir("hists");
    TDirectory *d_plots = fout->mkdir("plots");
    TDirectory *d_filtred = fout->mkdir("filtred");
    TDirectory *d_bcids = fout->mkdir("bcids");
    TDirectory *d_maps = fout->mkdir("maps");

    //plots structures for processed data
      //global plots (all sca columns, per chip=color, per channel=x)
    //TCanvas *canvas_mean = new TCanvas("h_chargeHighMeans_all","h_chargeHighMeans_all",800,800);
    //TCanvas *canvas_rms  = new TCanvas("h_chargeHighRMSs_all","h_chargeHighRMSs_all",800,800);
    //TCanvas *canvas_meanrms[NCHIP];
	bool __en_canvas = false;
    TCanvas *canvas_mean_sca[NCHIP];
    TH1F *h_mean_chip[NCHIP];
    TH1F *h_rms_chip[NCHIP];
    TH1D *h_HG_chip_col_chn[NCHIP][NCOL][NCHANNELS];
    TH1D *h_HG_chip_col_chn_filtred[NCHIP][NCOL][NCHANNELS];
    TH1D *h_HG_chip_col_chn_withhit[NCHIP][NCOL][NCHANNELS];
    TH1D *h_HG_chip_col_chn_filtred_withhit[NCHIP][NCOL][NCHANNELS];
    TH1D *h_BCIDdiff_chip_col[NCHIP][NCOL];
    TGraphErrors *meanrms[NCHIP];
    TGraphErrors *meanrmscol[NCHIP][NCOL];
    
    TH1D *h_rms_per_col_allchips[NCOL];

    TH2D *h_imghit[NCOL];
    TH2D *h_imghit_filtred[NCOL];

    TH2D *h_imgcoherenthit_filtred[NCOL];

	Float_t tmean, trms, tampl;
	Double_t par[3];
	TF1 *ff ;//= new TF1("ff","gaus(0)",250, 350);; //fit function


    info = new InfoChip();

	Int_t HISTnbin = 100;
	Double_t HISTmin = 200;//epected range for pedestal
	Double_t HISTmax = 600;

std::cout << " ^^^^^ STEP 1 ^^^^^" << std::endl;


      //for each column, plot mean and rms, per chip and per channel
      for(int chip = 0;chip<NCHIP;chip++){
        if (__en_canvas) {
		//one plot per chip with one color per sca
        name  = Form("chargeHighMeans_chip_%d",chip);
        title = Form("chargeHigh Means and RMS per channel. Chip %d",chip+1);
        canvas_mean_sca[chip] = new TCanvas(name,title,800,800);}

        for(int col = 0;col<NCOL;col++){
            name  = Form("BCID_chip_%d_col_%d",chip+1,col);
            title = Form("BCID, Chip %d column %d ",chip+1, col);
            if (col==0) {h_BCIDdiff_chip_col[chip][col] = new TH1D(name,title,100,0,4096);}//bins,min,max
            if (col>0) {h_BCIDdiff_chip_col[chip][col] = new TH1D(name,title,100,0,100);}//bins,min,max

          for(int chn = 0;chn<NCHANNELS;chn++){
            //create histograms per chip per column and per channel
            name  = Form("chargeHigh_chip_%d_chn_%d_col_%d",chip+1,chn,col);
            title = Form("chargeHigh, Chip %d channel %d column %d ",chip+1, chn, col);
            h_HG_chip_col_chn[chip][col][chn] = new TH1D(name,title,HISTnbin,HISTmin,HISTmax);//bins,min,max
            name  = Form("chargeHigh_filt_chip_%d_chn_%d_col_%d",chip+1,chn,col);
            title = Form("chargeHigh, Filtred Chip %d channel %d column %d ",chip+1, chn, col);
            h_HG_chip_col_chn_filtred[chip][col][chn] = new TH1D(name,title,HISTnbin,HISTmin,HISTmax);//bins,min,max
            name  = Form("chargeHigh_chip_%d_chn_%d_col_%d_withhit",chip+1,chn,col);
            title = Form("chargeHigh, Chip %d channel %d column %d with hit",chip+1, chn, col);
            h_HG_chip_col_chn_withhit[chip][col][chn] = new TH1D(name,title,HISTnbin,HISTmin,HISTmax);//bins,min,max
            name  = Form("chargeHigh_filt_chip_%d_chn_%d_col_%d_withhit",chip+1,chn,col);
            title = Form("chargeHigh, Filtred Chip %d channel %d column %d with hit",chip+1, chn, col);
            h_HG_chip_col_chn_filtred_withhit[chip][col][chn] = new TH1D(name,title,HISTnbin,HISTmin,HISTmax);//bins,min,max
          }
        }
     }


        for(int col = 0;col<NCOL;col++){
            name  = Form("rms_per_col_all_chips_col_%d",col);
            title = Form("rms_per_col_all_chips_col_%d",col);
            h_rms_per_col_allchips[col] = new TH1D(name,title,100,0,25);//bins,min,max
            name  = Form("hit_per_col_all_chips_col_%d",col);
            title = Form("hit_per_col_all_chips_col_%d",col);    
	    h_imghit[col] = new TH2D(name,title,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
            name  = Form("hit_filtered_per_col_all_chips_col_%d",col);
            title = Form("hit_filtered_per_col_all_chips_col_%d",col);    
	    h_imghit_filtred[col] = new TH2D(name,title,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
            name  = Form("coherent hit_filtered_per_col_%d",col);
            title = Form("coherent hit_filtered_per_col_%d",col);    
	    h_imgcoherenthit_filtred[col] = new TH2D(name,title,NCHANNELS*NCHIP,-0.5,NCHANNELS*NCHIP-0.5,NCHANNELS*NCHIP,-0.5,NCHANNELS*NCHIP-0.5);

	}


std::cout << " ^^^^^ STEP 2 ^^^^^" << std::endl;


std::cout << "   Entries : " << fev10->GetEntries() << std::endl;

   //fill histo with all entries
    for(unsigned entry = 0 ;entry<fev10->GetEntries();entry++){
      fev10->GetEntry(entry);
      for(int chip = 0;chip<NCHIP;chip++){
        for(int col = 0;col<NCOL;col++){
            if (col==0) {h_BCIDdiff_chip_col[chip][col]->Fill(bcid[chip][col]);}
            if (col>0) {h_BCIDdiff_chip_col[chip][col]->Fill(bcid[chip][col]-bcid[chip][col-1]);}
          for(int chn = 0;chn<NCHANNELS;chn++){
            h_HG_chip_col_chn[chip][col][chn]->Fill(charge_high[chip][col][chn]);
            //if (col==0 || ((col>0) && ((bcid[chip][col]-bcid[chip][col-1]) > 5))) {
	    	if (gain_hit_high[chip][col][chn]%2==1) {
			   h_HG_chip_col_chn_withhit[chip][col][chn]->Fill(charge_high[chip][col][chn]);
	    	   h_imghit[col]->Fill((Double_t) chn,(Double_t) chip,1);
	    	   if (badbcid[chip][col]<1) {
			   		h_imghit_filtred[col]->Fill((Double_t) chn,(Double_t) chip,1);
			   		for(int chnb = 0;chnb<NCHANNELS;chnb++){
						if (gain_hit_high[chip][col][chnb]%2==1) {h_imgcoherenthit_filtred[col]->Fill((Double_t) chip*NCHANNELS + chn,(Double_t) chip*NCHANNELS + chnb,1);}
					}

			   }
	    	}//gain
            if (col==0 || ((col>0) && (badbcid[chip][col] != 0))) {
                   h_HG_chip_col_chn_filtred[chip][col][chn]->Fill(charge_high[chip][col][chn]);
                   if (gain_hit_high[chip][col][chn]%2==1) {h_HG_chip_col_chn_filtred_withhit[chip][col][chn]->Fill(charge_high[chip][col][chn]);}
			}    
          }//chn
        }//col
      }//chip

    }//entries


std::cout << " ^^^^^ STEP 3 ^^^^^" << std::endl;


  //extract mean and rms from histo, create 1D vectors and canevas    
      for(int chip = 0;chip<NCHIP;chip++){
        if (__en_canvas) {canvas_mean_sca[chip]->cd();
        canvas_mean_sca[chip]->Divide(3,5);}
        for(int col = 0;col<NCOL;col++){ 
          if (__en_canvas) canvas_mean_sca[chip]->cd(col+1);     
          for(int chn = 0;chn<NCHANNELS;chn++){
          //HG_means[chip][col][chn] = h_HG_chip_col_chn[chip][col][chn]->GetMean();
          //HG_rms[chip][col][chn]   = h_HG_chip_col_chn[chip][col][chn]->GetRMS();
        		  tmean = h_HG_chip_col_chn[chip][col][chn]->GetMean();
        		  trms   = h_HG_chip_col_chn[chip][col][chn]->GetRMS();
				  tampl = h_HG_chip_col_chn[chip][col][chn]->GetBinContent(h_HG_chip_col_chn[chip][col][chn]->GetMaximumBin());
// 	Int_t HISTnbin = 100;
// 	Double_t HISTmin = 200;//epected range for pedestal
// 	Double_t HISTmax = 600;
				  
				  // here verify that tmean ~ maxbin, unless re-fit with maxbin as mean
				  
				  ff = new TF1("ff","gaus(0)",(Double_t) tmean-2*trms, (Double_t) tmean+2*trms);
				  ff->SetParameter(0,tampl);ff->SetParameter(1,tmean);ff->SetParameter(2,trms);
        		  h_HG_chip_col_chn[chip][col][chn]->Fit("ff","QR") ;//GetMean();
				  //ff->GetParameter(par);
				  if (ff->GetParameter(1) < 350 && ff->GetParameter(1) > 250 ) {		 
        		  		HG_means[chip][col][chn] = (Float_t) ff->GetParameter(1) ;}
				  else {HG_means[chip][col][chn] = 0;}
				  
				  if (ff->GetParameter(2) < 30 ) {		 
        		  		HG_rms[chip][col][chn]   = (Float_t) ff->GetParameter(2);}
				  else {HG_rms[chip][col][chn]   = 50;}
				  //std::cout << tmean << " <> " <<  ff->GetParameter(1) << std::endl;
				  delete ff;

          //HG_means_filtred[chip][col][chn] = h_HG_chip_col_chn_filtred[chip][col][chn]->GetMean();
          //HG_rms_filtred[chip][col][chn]   = h_HG_chip_col_chn_filtred[chip][col][chn]->GetRMS();
	  
        		  tmean = h_HG_chip_col_chn_filtred[chip][col][chn]->GetMean();
        		  trms   = h_HG_chip_col_chn_filtred[chip][col][chn]->GetRMS();
				  ff = new TF1("ff","gaus(0)",(Double_t) tmean-2*trms, (Double_t) tmean+2*trms);
				  ff->SetParameter(0,1);ff->SetParameter(1,tmean);ff->SetParameter(2,trms);
        		  h_HG_chip_col_chn_filtred[chip][col][chn]->Fit("ff","QR") ;//GetMean();
				  //ff->GetParameter(par);				 
        		  HG_means_filtred[chip][col][chn] = (Float_t) ff->GetParameter(1) ;//GetMean();
        		  HG_rms_filtred[chip][col][chn]   = (Float_t) ff->GetParameter(2);
				  //std::cout << tmean << " <> " <<  ff->GetParameter(1) << std::endl;
				  delete ff;
	      //std::cout << "   DATA : " << HG_rms[chip][col][chn] << std::endl;
	  
          tmp_data[chn] = HG_means[chip][col][chn];
          tmp_rms[chn] =  HG_rms[chip][col][chn];
          }
		  if (__en_canvas) {
        	meanrmscol[chip][col] = new TGraphErrors(NCHANNELS,channels,tmp_data,zeros,tmp_rms);
        	meanrmscol[chip][col]->SetTitle(Form("HG Means, RMS for Chip %d, column %d",chip+1,col+1));
        	meanrmscol[chip][col]->SetMarkerStyle(kFullCircle);
        	meanrmscol[chip][col]->SetMarkerColor(1+(col%15));//Color[1+(col%4)]);
        	meanrmscol[chip][col]->Draw("alp");
        	//plots->WriteTObject(meanrmscol[chip][col]);
			}
        }
      if (__en_canvas) d_plots->WriteTObject(canvas_mean_sca[chip]);
      }



    for(int col = 0;col<NCOL;col++){ 
      for(int chip = 0;chip<NCHIP;chip++){
          for(int chn = 0;chn<NCHANNELS;chn++){
          h_rms_per_col_allchips[col]->Fill(HG_rms[chip][col][chn]);
        }
      }
      d_plots->WriteTObject(h_rms_per_col_allchips[col]);
      d_plots->WriteTObject(h_imghit[col]);
      d_plots->WriteTObject(h_imghit_filtred[col]);
	  d_plots->WriteTObject(h_imgcoherenthit_filtred[col]);
    }




std::cout << " ^^^^^ STEP 4 ^^^^^" << std::endl;


   /*Int_t MyPalette[100];
   Double_t r[]    = {0., 0.0, 1.0, 1.0, 1.0};
   Double_t g[]    = {0., 0.0, 0.0, 1.0, 1.0};
   Double_t b[]    = {0., 1.0, 0.0, 0.0, 1.0};
   Double_t stop[] = {0., .25, .50, .75, 1.0};
   Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 100);
   for (int i=0;i<100;i++) MyPalette[i] = FI+i;*/

////TIP = TImagePalette(1,0);
// 
// TCanvas *canvas_map = new TCanvas("canvas_map","map",800,800);
// canvas_map->cd();
// canvas_map->Divide(1,2);
// canvas_map->cd(1);
//TImage *img = TImage::Create();
//TImage *imgrms = TImage::Create();
//TH2D *h_img[col] = new TH2D("h_img","map",NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);

for (int col = 0;col<NCOL;col++){
      name = "h_img"; 
      name+=col;
      h_img[col] = new TH2D(name,name,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
	  name+="_filtred";
      h_img_filtred[col] = new TH2D(name,name,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
      name = "h_imgrms"; 
      name+=col;
      h_imgrms[col] = new TH2D(name,name,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
	  name+="_filtred";
      h_imgrms_filtred[col] = new TH2D(name,name,NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);

      name = "h_asu"; 
      name+=col;
      //h_asu[col] = new TH2D(name,name,32,-89.5,90-0.5,32,-89.5,90-0.5);
	  name+="_filtred";
      h_asu_filtred[col] = new TH2D(name,name,32,-89.5,90-0.5,32,-89.5,90-0.5);
      name = "h_asurms"; 
      name+=col;
      //h_asurms[col] = new TH2D(name,name,32,-89.5,90-0.5,32,-89.5,90-0.5);
	  name+="_filtred";
      h_asurms_filtred[col] = new TH2D(name,name,32,-89.5,90-0.5,32,-89.5,90-0.5);

	    h_img[col]->SetMinimum(-150.0);h_img[col]->SetMaximum(150.0);
	    h_imgrms[col]->SetMinimum(0);h_img[col]->SetMaximum(20);
	    h_img_filtred[col]->SetMinimum(-150.0);h_img[col]->SetMaximum(150.0);
	    h_imgrms_filtred[col]->SetMinimum(0);h_img[col]->SetMaximum(20);

	    //h_asu[col]->SetMinimum(-150.0);h_img[col]->SetMaximum(150.0);
	    //h_asurms[col]->SetMinimum(0);h_img[col]->SetMaximum(20);
	    h_asu_filtred[col]->SetMinimum(-150.0);h_img[col]->SetMaximum(150.0);
	    h_asurms_filtred[col]->SetMinimum(0);h_img[col]->SetMaximum(20);


}
//TH2D *h_img_filtred = new TH2D("h_img_filtred","map_filtred",NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
//TH2D *h_imgrms_filtred = new TH2D("h_imgrms_filtred","maprms_filtred",NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
// TH2D *h_asurms = new TH2D("h_asurms","asurms",32,-89.5,90-0.5,32,-89.5,90-0.5);
// TH2D *h_asurms_filtred = new TH2D("h_asurms_filtred","asurms_filtred",32,-89.5,90-0.5,32,-89.5,90-0.5);



      for(int chip = 0;chip<NCHIP;chip++){
        //for(int col = 0;col<NCOL;col++){ 
          for(int chn = 0;chn<NCHANNELS;chn++){
          //map for col=0
        	  for (int col = 0;col<NCOL;col++){
          		  h_img[col]->Fill((Double_t) chn,(Double_t) chip, HG_means[chip][col][chn]-250);//x,y,weight
            	  h_img_filtred[col]->Fill((Double_t) chn,(Double_t) chip, HG_means_filtred[chip][col][chn]-250);//x,y,weight
            	  h_imgrms[col]->Fill((Double_t) chn,(Double_t) chip, HG_rms[chip][col][chn]);//x,y,weight
            	  h_imgrms_filtred[col]->Fill((Double_t) chn,(Double_t) chip, HG_rms_filtred[chip][col][chn]);//x,y,weight
				  h_asu_filtred[col]->Fill(info->GetX(chip,chn),info->GetY(chip,chn), HG_means_filtred[chip][col][chn]-250);//x,y,weight
				  //h_asu[col]->Fill(info->GetX(chip,chn),info->GetY(chip,chn), HG_means[chip][col][chn]-250);//x,y,weight
				  h_asurms_filtred[col]->Fill(info->GetX(chip,chn),info->GetY(chip,chn), HG_rms_filtred[chip][col][chn]);//x,y,weight
				  //h_asurms[col]->Fill(info->GetX(chip,chn),info->GetY(chip,chn), HG_rms[chip][col][chn]);//x,y,weight
	  			}
	  
          }
      }
//canvas_map->Update();

////img->SetImage((const Double_t *)h_img->GetArray(), h_img->GetNbinsX() + 2, 
////                  h_img->GetNbinsY() + 2, &TIP); //gWebImagePalette);
////img->Draw();
//h_img->Draw("colz");
//h_img->Write();
//d_plots->WriteTObject(h_img);
//d_plots->WriteTObject(h_img_filtred);
//canvas_map->cd(2);
//h_imgrms[0]->Draw("colz");
	  for (int col = 0;col<NCOL;col++){
    	//d_maps->WriteTObject(h_img[col]);
    	//d_maps->WriteTObject(h_imgrms[col]);
    	d_maps->WriteTObject(h_img_filtred[col]);
    	d_maps->WriteTObject(h_imgrms_filtred[col]);
    	//d_maps->WriteTObject(h_asu[col]);
    	//d_maps->WriteTObject(h_asurms[col]);
    	d_maps->WriteTObject(h_asu_filtred[col]);
    	d_maps->WriteTObject(h_asurms_filtred[col]);
	  }

//h_imgrms->Write();
////imgrms->SetImage((const Double_t *)h_imgrms->GetArray(), h_imgrms->GetNbinsX() + 2, 
////                  h_imgrms->GetNbinsY() + 2, &TIP); //gWebImagePalette);
////imgrms->Draw();
//d_plots->WriteTObject(canvas_map);


//////  below : Miguel
/*

    for (int chip=0;chip<NCHIP;chip++) {

      name  = Form("h_chargeHighMeans_chip_%d",chip+1);
      title = Form("Histogram chargeHigh Means chip %d",chip+1);
      h_mean_chip[chip] = new TH1F(name,title,100,0,500);
      h_mean_chip[chip]->SetLineColor(Color[chip]);

      name  = Form("h_chargeHighRMSs_chip_%d",chip+1);
      title = Form("Histogram chargeHigh RMS chip %d",chip+1);
      h_rms_chip[chip] = new TH1F(name,title,100,0,25);
      h_rms_chip[chip]->SetLineColor(Color[chip]);

      for (int channel=0;channel<NCHANNELS;channel++) {
        h_in = (TH1F*)fin->Get(Form("plots/chargeHigh_chip_%d_channel_%d",chip+1,channel));
        h_mean_chip[chip]->Fill(h_in->GetMean());
        h_rms_chip [chip]->Fill(h_in->GetRMS());
        means[chip][channel] = h_in->GetMean();
        rmss [chip][channel] = h_in->GetRMS();
      }

      canvas_mean->cd();
      h_mean_chip[chip]->Draw(chip?"sames":"");
      canvas_mean->Update();
      ps = (TPaveStats*)h_mean_chip[chip]->FindObject("stats");
      ps->SetY1NDC(chip*0.15+0.3);
      ps->SetY2NDC(chip*0.15+0.4);
      canvas_mean->Modified();

      canvas_rms->cd();
      h_rms_chip[chip]->Draw(chip?"sames":"");
      canvas_rms->Update();
      ps = (TPaveStats*)h_rms_chip[chip]->FindObject("stats");
      ps->SetY1NDC(chip*0.15+0.3);
      ps->SetY2NDC(chip*0.15+0.4);
      canvas_rms->Modified();

      name  = Form("chargeHighMeans_chip_%d",chip+1);
      title = Form("chargeHigh Means and RMS per channel. Chip %d",chip+1);
      canvas_meanrms[chip] = new TCanvas(name,title,800,800);
      meanrms[chip] = new TGraphErrors(NCHANNELS,channels,means[chip],zeros,rmss[chip]);
      meanrms[chip]->SetTitle(Form("chargeHigh Means and RMS per channel. Chip %d",chip+1));
      meanrms[chip]->SetMarkerStyle(kFullCircle);
      meanrms[chip]->SetMarkerColor(kRed);
      //meanrms[chip]->Draw("alp");

      d_hists->WriteTObject(h_mean_chip[chip]);
      d_hists->WriteTObject(h_rms_chip[chip]);
      d_plots->WriteTObject(canvas_meanrms[chip]);
    }
    d_hists->WriteTObject(canvas_mean);
    d_hists->WriteTObject(canvas_rms);

*/




// Diagnostics, 
    ofstream log;
    log.open("./log.txt"); //TODO : take name from "infilename"
    Float_t rms_thres = h_rms_per_col_allchips[0]->GetMean()+h_rms_per_col_allchips[0]->GetRMS();
    std::cout << "   RMS threshold: " << rms_thres << std::endl;
    log << "#!/usr/bin/python2"<< std::endl;
    log << "import sys, time"  << std::endl;
    log << "from pycaldaq import *" << std::endl;

      for(int chip = 0;chip<NCHIP;chip++){
          for(int chn = 0;chn<NCHANNELS;chn++){
	  //if (HG_rms[chip][0][chn]>rms_thres) {
	  if (h_imghit[0]->GetBinContent(chn+1,chip+1)> fev10->GetEntries()/50) {
	  
	  log << "reconfigure(\"skiroc_1_1_1_" << chip+1 << "\",\"disallow_trig_chans_skiroc\",\"" << chn << "\")" << std::endl;
	  //log << "reconfigure(\"skiroc_1_1_1_" << chip+1 << "\",\"disable_preamp_chans_skiroc\",\"" << chn << "\")" << std::endl;

	  }
	  
        }
      }





std::cout << " ^^^^^  SAVE  ^^^^^" << std::endl;


  //save histo    
      for(int chip = 0;chip<NCHIP;chip++){
        for(int col = 0;col<NCOL;col++){ 
          d_bcids->WriteTObject(h_BCIDdiff_chip_col[chip][col]);         
          for(int chn = 0;chn<NCHANNELS;chn++){
          //d_hists->WriteTObject(h_HG_chip_col_chn[chip][col][chn]);
          d_filtred->WriteTObject(h_HG_chip_col_chn_filtred[chip][col][chn]);
          d_filtred->WriteTObject(h_HG_chip_col_chn_filtred_withhit[chip][col][chn]);
          }
        }
      }




std::cout << " ^^^^^  END   ^^^^^" << std::endl;




    //fout->Close();

std::cout << " ^^^^^  WRITE  ^^^^^" << std::endl;


    /*delete canvas_mean;
    delete canvas_rms;
    delete canvas_meanrms[0];
    delete canvas_meanrms[1];
    delete canvas_meanrms[2];
    delete canvas_meanrms[3];*/





std::cout << " ^^^^^  DELETE  ^^^^^" << std::endl;

/*
      for(int chip = 0;chip<NCHIP;chip++){
        delete canvas_meanrms[chip];
        delete canvas_mean_sca[chip];
        for(int col = 0;col<NCOL;col++){
          delete h_BCIDdiff_chip_col[chip][col];
          for(int chn = 0;chn<NCHANNELS;chn++){
            delete h_HG_chip_col_chn[chip][col][chn] ;
            delete h_HG_chip_col_chn_filtred[chip][col][chn] ;
          }
        }
      }
*/


  }//constructor


  ~MeanRMS_FEV10(){
    delete info;
  }//destructor

private:
  enum {
    NCHANNELS=64,
    NCOL=15,
    NCHIP=16
  };

    InfoChip * info;

};
