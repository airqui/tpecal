#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bitset>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <TH1F.h>
#include <TMultiGraph.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TF1.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <sstream>
#include <string>
#include "InfoChip.C"


// root> .x Merge4Scurves.C+
// root> M=Merge4Scurves();
// root>  M->DoMerge("/home/calice/data/SCurveAll",false,140,220,10);



class Merge4Scurves{
public:


  Merge4Scurves(){
     NOISE_THRESHOLD=0.005;

  }


  ~Merge4Scurves(){}



  void DoMerge(TString rootname, bool filterEvents = false, int Start = 140, int Stop = 220, int Step = 1){

  //const int Start = 140;
  //const int Step = 10; //5
  //const int Stop = 220;


    bool debug = true;
    TString name = "";
    TString outfilename = rootname;outfilename += "/Scurves_dif0"; // CHANGE TO outfilename += "/Scurves_dif0";
    if(filterEvents) outfilename += "_filtered";
    outfilename += ".root";

    TFile *f;
    TFile *fout;
    fout = new TFile(outfilename,"recreate");        TDirectory *d_hists[NCHIP];name = "Chip"; TDirectory *d_hists_f[NCHIP];
    for (int nchip=0;nchip<NCHIP;nchip++){ 
        name = "Chip";name += nchip;
        d_hists[nchip] = fout->mkdir(name);
	name += "filtred";
        d_hists_f[nchip] = fout->mkdir(name);
        }
    TGraph *gr[NCHIP][NCHANNELS];
    TGraph *gr_f[NCHIP][NCHANNELS];
      for (int nchip=0;nchip<NCHIP;nchip++){ 
          d_hists[nchip]->cd();
	  for (int nchn=0;nchn<NCHANNELS;nchn++){ 
	    gr[nchip][nchn] = new TGraph(1+((Stop-Start)/Step)); //set number of point via parameter!!
	  }
          d_hists_f[nchip]->cd();
	  for (int nchn=0;nchn<NCHANNELS;nchn++){ 
	    gr_f[nchip][nchn] = new TGraph(1+((Stop-Start)/Step)); //set number of point via parameter!!
	  }
	}
    //TTree *tree[NSLABS] ;
    
    //name = "HitMapHist"; 
    TH2D* HitMapHist ;
    TH2D* HitMapHist_filtred ;


    //int entries[NSLABS];
    //int maxEntries = 0;
    //int entry[NSLABS];  

    for(int i = 0;Start+Step*i<Stop+1;i++){
      //entries[i] = 0;
      //entry[i]=0;  


      TString filename = rootname;
      filename += "/";
      filename += "scurve_trig";
      filename += Start+Step*i;
      filename += "_by_dif0.raw.root";
      f = new TFile(filename,"read");

      if(f->IsOpen()){
        cout << "Found file " << filename << endl;
	HitMapHist = (TH2D* ) f->Get("HitMapHist");
	HitMapHist_filtred = (TH2D* ) f->Get("HitMapFilteredHist");
	
      for (int nchip=0;nchip<NCHIP;nchip++){ 
          for (int nchn=0;nchn<NCHANNELS;nchn++){ 
	  gr[nchip][nchn]->SetPoint(i,Start+Step*i,HitMapHist->GetBinContent(nchip+1,nchn+1));
	  gr_f[nchip][nchn]->SetPoint(i,Start+Step*i,HitMapHist_filtred->GetBinContent(nchip+1,nchn+1));
	  }
	}
	//Double_t *Data = HitMapHist->GetArray();
	
	if (debug) cout << "    " << HitMapHist->GetMaximum() << endl;
	
	//HitMapHist->Draw("alp");
	
	/*tree[i] = (TTree*)f[i]->Get("fev8");


	tree[i]->SetBranchAddress("chipid", chipID_in[i]);
	//int acq = -999;
	tree[i]->SetBranchAddress("acqNumber", &acqNumber_in[i]);
	//acqNumber_in[i]= acq;

	tree[i]->SetBranchAddress("bcid"           , bcid_in[i]);
	tree[i]->SetBranchAddress("badbcid"        , badbcid_in[i]);
	tree[i]->SetBranchAddress("nColumns"       , numCol_in[i]);
	tree[i]->SetBranchAddress("gain_hit_high"  , gain_hit_high_in[i] );
	tree[i]->SetBranchAddress("charge_hiGain"  , charge_high_in[i] );
	tree[i]->SetBranchAddress("gain_hit_low"   , gain_hit_low_in[i] );
	tree[i]->SetBranchAddress("charge_lowGain" , charge_low_in[i] );

	entries[i] = tree[i]->GetEntries();
	if(entries[i]>maxEntries)maxEntries = entries[i];*/
      }//isopen
    }//for i



// //////////////////  S curves analysis  ////////////////////
/*   Double_t x,y;
   int tmp;
    ofstream log;
    log.open("./Scurve.log"); //TODO : take name from "infilename"

      for (int nchip=0;nchip<NCHIP;nchip++){ 
          for (int nchn=0;nchn<NCHANNELS;nchn++){ 
	     for (int npt=0;npt<1+((Stop-Start)/Step);npt++){
               if (npt==5) {cout << gr[nchip][nchn]->GetPoint(npt+1,x,y) << endl;}
               //if ( gr[nchip][nchn]->GetPoint(npt+1,x,y) < NOISE_THRESHOLD) {
	         //log << nchip << " " << nchn << " " << Start + npt * Step << std::endl;
		 //break;}

	    }
	  }
	}
   log.close();*/
// ////////////////// /////////////////  ////////////////////


/*
    treeout = new TTree("fev8","fev8");



  
    TString name;

    treeout->Branch("acqNumber",&acqNumber,"acqNumber/I");

    name = "chipid[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="]/I";
    treeout->Branch("chipid",chipID,name);

    name = "nColumns[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="]/I";
    treeout->Branch("nColumns",numCol,name);

    name = "bcid[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
    treeout->Branch("bcid",bcid,name);

    name = "badbcid[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
    treeout->Branch("badbcid",badbcid,name);

    name = "lowGain[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
    treeout->Branch("charge_lowGain",charge_low,name);

    name = "highGain[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
    treeout->Branch("charge_hiGain",charge_high,name);

    name = "gain_hit_low[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
    treeout->Branch("gain_hit_low",gain_hit_low,name);

    name = "gain_hit_high[";
    name+=NSLABS; name+="][";
    name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
    treeout->Branch("gain_hit_high",gain_hit_high,name);



    //  currentSpill[i]=0;  

    int currentSpill=0;
    // for(int i=0;i<maxEntries;i++){
    bool continueScan = true;
    int maxEvents=2000000000;
    int evt = 0;
    bool isSlabEnd[NSLABS];
    for(int i=0;i<NSLABS;i++) isSlabEnd[i]=false;


    while(continueScan && evt<maxEvents){
      treeInit();
      currentSpill=2000000000;//INT_MAX;
      for(int i = 0;i<NSLABS;i++){
	if(f[i]->IsOpen()){
	  tree[i]->GetEntry(entry[i]);
	  //cout<<"-> current = "<<entry[i]<<"    acq = "<<acqNumber_in[i]<<endl;
	  if(currentSpill>acqNumber_in[i] && !isSlabEnd[i])currentSpill=acqNumber_in[i];
	}
      }



      for(int slab = 0;slab<NSLABS;slab++){
	if(currentSpill==acqNumber_in[slab] && entries[slab]>entry[slab]){
	  entry[slab]++;
	  //cout<<"-> current = "<<slab<<"    acq = "<<acqNumber_in[slab]<<endl;


	  for (int k=0; k<NCHIP; k++) {
	    int i =0;
	    int loopBCID = 0;
	    for (int iraw=0; iraw<MEMDEPTH; iraw++) {
	      if(filterEvents){
		if(iraw==0) i = iraw;
		else if(bcid_in[slab][k][iraw] - bcid_in[slab][k][iraw-1]>5) i = iraw;
		else if(bcid_in[slab][k][iraw] - bcid_in[slab][k][iraw-1]<0 && bcid_in[slab][k][iraw]+4096 - bcid_in[slab][k][iraw-1]>5){
		   i = iraw;
                   loopBCID++;
		}
		else continue;
	      }
	      else i = iraw;

	      bcid[slab][k][i] = bcid_in[slab][k][i]+loopBCID*4096;
	      //cout<<bcid[slab][k][i]<<"  "<<slab<<"  "<<k<<"  "<<i<<endl;
//	      if(bcid_in[slab][k][i]!=1) cout<<slab<<"  "<<k<<"  "<<i<<"  "<<bcid_in[slab][k][i]<<endl;
//	      if(badbcid_in[slab][k][i]!=-999) cout<<slab<<"  "<<k<<"  "<<i<<"  "<<badbcid_in[slab][k][i]<<endl;

	      badbcid[slab][k][i]=badbcid_in[slab][k][i];
	      for (int j=0; j<NCHANNELS; j++) {
		charge_low[slab][k][i][j]=charge_low_in[slab][k][i][j];
		charge_high[slab][k][i][j]=charge_high_in[slab][k][i][j];
		gain_hit_low[slab][k][i][j]=gain_hit_low_in[slab][k][i][j];
		gain_hit_high[slab][k][i][j]=gain_hit_high_in[slab][k][i][j];
		//if(charge_high_in[slab][k][i][j]>1000)cout<<" "<<slab<<"  "<<k<<"  "<<i<<"  "<<j<<"  "<<charge_high_in[slab][k][i][j]  <<endl;
	      }
	    }
	    //if(numCol_in[slab][k]>1000) cout<<slab<<"  "<<k<<"  "<<numCol_in[slab][k]<<endl;
	    chipID[slab][k]=chipID_in[slab][k];
	    numCol[slab][k]=numCol_in[slab][k];
  
	  }
	  acqNumber=currentSpill;

	}
	else {
	  //cout<<"rerore"<<endl;
	}



      }
      treeout->Fill();
      //cout<<"SPILL = "<<currentSpill<<endl;

    
      evt++;
      continueScan = false;
      for(int i = 0;i<NSLABS;i++){
	//cout<<"   slab = "<<i<<"   tot = "<<entries[i]<<"   current = "<<entry[i]  <<endl;
	isSlabEnd[i]=true;
	if(entries[i]>entry[i]){
           continueScan = true;
	   isSlabEnd[i]=false;
	}
      }

    }

*/



   fout->cd();
   name = "all";
   Double_t x,y;
   TH2D* all  = new  TH2D(name, name, ((Stop-Start)/Step),Start,Stop,NCHANNELS*NCHIP,0,NCHANNELS*NCHIP);
   name = "all_f";
   TH2D* all_f  = new  TH2D(name, name, ((Stop-Start)/Step),Start,Stop,NCHANNELS*NCHIP,0,NCHANNELS*NCHIP);
      for (int nchip=0;nchip<NCHIP;nchip++){ 
	  for (int nchn=0;nchn<NCHANNELS;nchn++){ 
	    for (int npt=0;npt<1+((Stop-Start)/Step);npt++){
	     gr[nchip][nchn]->GetPoint(npt+1,x,y);
	     //cout << x << "   " << y  <<endl;
             all->SetBinContent(npt+1,1+NCHANNELS*nchip+nchn,y*1000);
	     gr_f[nchip][nchn]->GetPoint(npt+1,x,y);
             all_f->SetBinContent(npt+1,1+NCHANNELS*nchip+nchn,y*1000);
	  }
	  }
	  }
	  //all->Write();


    TMultiGraph *mg[NCHIP];
    TMultiGraph *mg_f[NCHIP];

      for (int nchip=0;nchip<NCHIP;nchip++){ 
          d_hists[nchip]->cd();
	      mg[nchip] = new TMultiGraph();
	  for (int nchn=0;nchn<NCHANNELS;nchn++){ 
	    mg[nchip]->Add(gr[nchip][nchn]); 
	  }
      
	  name = "Chip"; name +=nchip;
	  mg[nchip]->Write(name);
	}

      for (int nchip=0;nchip<NCHIP;nchip++){ 
          d_hists_f[nchip]->cd();
	      mg_f[nchip] = new TMultiGraph();
	  for (int nchn=0;nchn<NCHANNELS;nchn++){ 
	    mg_f[nchip]->Add(gr_f[nchip][nchn]); 
	  }
      
	  name = "Chip"; name +=nchip;
	  name +="_filtered";
	  mg_f[nchip]->Write(name);
	}
    //fout->cd();
    //fout->Write();
    //fout->Close();

  //}



    TCanvas *canvas_map[NCHIP];

      for (int nchip=0;nchip<NCHIP;nchip++){ 
          d_hists[nchip]->cd();
          
          name = "Canva_Chip";name += nchip;
          canvas_map[nchip] = new TCanvas(name,name,800,800);
          canvas_map[nchip]->cd();
          canvas_map[nchip]->Divide(8,8);

	         for (int nchn=0;nchn<NCHANNELS;nchn++){ 
                canvas_map[nchip]->cd(nchn+1);
                gr[nchip][nchn]->SetTitle(Form("Chip %d, channel %d",nchip+1,nchn+1));
                gr_f[nchip][nchn]->SetMinimum(0);
                gr[nchip][nchn]->Draw("alp");
	         }
         d_hists[nchip]->WriteTObject(canvas_map[nchip]);
 
	}

    TCanvas *canvas_map_f[NCHIP];

      for (int nchip=0;nchip<NCHIP;nchip++){ 
          d_hists_f[nchip]->cd();
          
          name = "Canva_Chip";name += nchip;name +="_filtered";
          canvas_map_f[nchip] = new TCanvas(name,name,800,800);
          canvas_map_f[nchip]->cd();
          canvas_map_f[nchip]->Divide(8,8);

	         for (int nchn=0;nchn<NCHANNELS;nchn++){ 
                canvas_map_f[nchip]->cd(nchn+1);
                gr_f[nchip][nchn]->SetTitle(Form("Chip %d, channel %d  filtred",nchip+1,nchn+1));
                gr_f[nchip][nchn]->SetMinimum(0);
                gr_f[nchip][nchn]->Draw("alp");
	         }
         d_hists_f[nchip]->WriteTObject(canvas_map_f[nchip]);
 
	}

    fout->cd();
    fout->Write();
    fout->Close();



  }//end doMerge

/*
  void treeInit() {
    for (int slab=0; slab<NSLABS; slab++) {
      for (int k=0; k<NCHIP; k++) {
	for (int i=0; i<MEMDEPTH; i++) {
	  bcid[slab][k][i]= -999;
	  badbcid[slab][k][i]= -999;
	  bcid_in[slab][k][i]= -999;
	  badbcid_in[slab][k][i]= -999;
	  for (int j=0; j<NCHANNELS; j++) {
	    charge_low[slab][k][i][j]= -999;
	    charge_high[slab][k][i][j]= -999;
	    gain_hit_low[slab][k][i][j]= -999;
	    gain_hit_high[slab][k][i][j]= -999;
	    charge_low_in[slab][k][i][j]= -999;
	    charge_high_in[slab][k][i][j]= -999;
	    gain_hit_low_in[slab][k][i][j]= -999;
	    gain_hit_high_in[slab][k][i][j]= -999;
	  }
	}
  
	chipID[slab][k]= -999;
	numCol[slab][k]= -999;
  
	chipID_in[slab][k]= -999;
	numCol_in[slab][k]= -999;
  
      }
    }
    acqNumber= -999;    TDirectory *d_hists = fout->mkdir("hists");

  }
*/
protected:
  //TFile *fout;
  //TTree *treeout;

 



  //int currentSpill[10];  

  Float_t NOISE_THRESHOLD; //ratio hit/(Nevt*Ncol)

  enum {
    MEMDEPTH=15,
    NCHANNELS=64,
    NCHIP=16,
    MIN_BCID=4,
    NSLABS=4
  };

/*
  int bcid[NSLABS][NCHIP][MEMDEPTH];
  int badbcid[NSLABS][NCHIP][MEMDEPTH];
  int charge_low[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int charge_high[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_low[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_high[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int numCol[NSLABS][NCHIP];
  int chipID[NSLABS][NCHIP];
  int acqNumber;


  int bcid_in[NSLABS][NCHIP][MEMDEPTH];
  int badbcid_in[NSLABS][NCHIP][MEMDEPTH];
  int charge_low_in[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int charge_high_in[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_low_in[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_high_in[NSLABS][NCHIP][MEMDEPTH][NCHANNELS];
  int numCol_in[NSLABS][NCHIP];
  int chipID_in[NSLABS][NCHIP];
  int acqNumber_in[NSLABS];
*/

};
