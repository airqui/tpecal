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


// .x RAW2ROOT.C+

using std::cout;
using std::endl;

class RAW2ROOT {

public:
  RAW2ROOT(){
    recordEvent = false;
    _debug=true;

    chipIds.push_back(0x0000);//chip 2
    chipIds.push_back(0x0001);//chip 4
    chipIds.push_back(0x0002);//chip 3
    chipIds.push_back(0x0003);//chip 1
    chipIds.push_back(0x0004);//chip 
    chipIds.push_back(0x0005);//chip 
    chipIds.push_back(0x0006);//chip 
    chipIds.push_back(0x0007);//chip 
    chipIds.push_back(0x0008);//chip 
    chipIds.push_back(0x0009);//chip 
    chipIds.push_back(0x000A);//chip 
    chipIds.push_back(0x000B);//chip 
    chipIds.push_back(0x000C);//chip 
    chipIds.push_back(0x000D);//chip 
    chipIds.push_back(0x000E);//chip 
    chipIds.push_back(0x000F);//chip 
    chipIds.push_back(0x0010);//chip 
    
    /*chipIds.push_back(0x0011);//chip 4
      chipIds.push_back(0x0012);//chip 3
      chipIds.push_back(0x0013);//chip 1
      chipIds.push_back(0x0014);//chip 
      chipIds.push_back(0x0015);//chip 
      chipIds.push_back(0x0016);//chip 
      chipIds.push_back(0x0017);//chip 
      chipIds.push_back(0x0018);//chip 
      chipIds.push_back(0x0019);//chip 
      chipIds.push_back(0x001A);//chip 
      chipIds.push_back(0x001B);//chip 
      chipIds.push_back(0x001C);//chip 
      chipIds.push_back(0x001D);//chip 
      chipIds.push_back(0x001E);//chip 
      chipIds.push_back(0x001F);//chip 
      chipIds.push_back(0x0020);//chip
    
      chipIds.push_back(0x0021);//chip 4
      chipIds.push_back(0x0022);//chip 3
      chipIds.push_back(0x0023);//chip 1
      chipIds.push_back(0x0024);//chip 
      chipIds.push_back(0x0025);//chip 
      chipIds.push_back(0x0026);//chip 
      chipIds.push_back(0x0027);//chip 
      chipIds.push_back(0x0028);//chip 
      chipIds.push_back(0x0029);//chip 
      chipIds.push_back(0x002A);//chip 
      chipIds.push_back(0x002B);//chip 
      chipIds.push_back(0x002C);//chip 
      chipIds.push_back(0x002D);//chip 
      chipIds.push_back(0x002E);//chip 
      chipIds.push_back(0x002F);//chip 
      chipIds.push_back(0x0030);//chip 
    
      chipIds.push_back(0x0031);//chip 4    
      chipIds.push_back(0x0032);//chip 3
      chipIds.push_back(0x0033);//chip 1
      chipIds.push_back(0x0034);//chip 
      chipIds.push_back(0x0035);//chip 
      chipIds.push_back(0x0036);//chip 
      chipIds.push_back(0x0037);//chip 
      chipIds.push_back(0x0038);//chip 
      chipIds.push_back(0x0039);//chip 
      chipIds.push_back(0x003A);//chip 
      chipIds.push_back(0x003B);//chip 
      chipIds.push_back(0x003C);//chip 
      chipIds.push_back(0x003D);//chip 
      chipIds.push_back(0x003E);//chip 
      chipIds.push_back(0x003F);//chip 
      chipIds.push_back(0x0040);//chip */
       
    info = new InfoChip(); R2Rstate=-1;

  };
  ~RAW2ROOT(){
    delete info;
  };
  void ReadFile(TString inputFileName,  bool overwrite=false, int PlaneEventThreshold=20, int MinBcidIncrement=3, int maxevt=99999999);

protected:

  enum {
    MEMDEPTH=15,
    NCHANNELS=64,
    NCHIP=16,
    MIN_BCID=5,
    MIN_D_BCID=3,
    CHIPHEAD=4, 
    CHIPENDTAG=2,
    NEGDATA_THR=10 //event with data below are tagged badbcid+=32
  };

  int R2Rstate;

  void readEvent(std::vector < unsigned short int > & eventData, int PlaneEventThreshold=20, int MinBcidIncrement=3);
  void Initialisation();
  void analyse_hits();
  void plotHistos();
  void treeInit();
  void printEvent(std::vector < unsigned short int > & eventData);
  void DebugMode(bool t){ _debug=t; }
  void searchNmax(int N, int Size, Float_t * table, Float_t * tableout ,int * ranks);
  int  GetTree(TString rootfilename);
  int  BuildTimedEvents(TString rootfilename);

  TH1F* h_hit_high[NCHIP]; //hist of all hits per chip per channel
  TH1F* h_hit_filtered[NCHIP];//hist of good hits per chip per channel
  TH1F* h_hit_filtered_histo[NCHIP];//hist of nb of good hits per chip 
  TH1F* h_nCol[NCHIP];
  TH1F* h_bcid[NCHIP];
  TH1D* h_TagHist[NCHIP];
  TH2D* TagHist;
  
  TH2I* h_chnnb;
  TH2I* h_chipnb;

  //TH2F* h2_hits_per_chip[NCHIP];
  TH2F* h2_hits;
  TH2F* h2_hits_Acq;
  TH1F* h_hits_BCID[NCHIP][NCHANNELS];
  TH1F* h_col_Acq[NCHIP];

  //TH1F* h_chargeHigh[NCHIP][NCHANNELS];
  //TH1F* h_chargeLow[NCHIP][NCHANNELS];

  //TH1F* h_chargeHigh_withHit[NCHIP][NCHANNELS];
  //TH1F* h_chargeLow_withHit[NCHIP][NCHANNELS];

  TDirectory *dPlots ;

  TFile* fout;
  TTree* tree;
  
  TFile *finroot;
  TTree* fev10read;

  int bcid[NCHIP][MEMDEPTH];
  int corrected_bcid[NCHIP][MEMDEPTH];
  int badbcid[NCHIP][MEMDEPTH];
  int nhits[NCHIP][MEMDEPTH];
  int charge_low[NCHIP][MEMDEPTH][NCHANNELS];
  int charge_high[NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_low[NCHIP][MEMDEPTH][NCHANNELS];
  int gain_hit_high[NCHIP][MEMDEPTH][NCHANNELS];
  int event;
  //int numbcid;
  int numCol[NCHIP];
  int chipID[NCHIP];
  int acqNumber;

  //int allbcid[NCHIP*MEMDEPTH];

  bool recordEvent;
  bool _debug;

  std::vector <int> chipIds;

  InfoChip * info;
};

//******************************************************************************************************************

void RAW2ROOT::Initialisation() {
  fout->cd(); R2Rstate=-1;

  tree = new TTree("fev10","fev10");

  tree->Branch("event",&event,"event/I");
  //tree->Branch("chipid",&chipID,"chipid/I");
  //tree->Branch("nbcid",&numbcid,"nbcid/I");
  tree->Branch("acqNumber",&acqNumber,"acqNumber/I");
  //tree->Branch("nColumns",&numCol,"nColumns/I");
 
  TString name;

  name = "chipid[";
  name+=NCHIP; name+="]/I";
  tree->Branch("chipid",chipID,name);

  name = "nColumns[";
  name+=NCHIP; name+="]/I";
  tree->Branch("nColumns",numCol,name);

  name = "bcid[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("bcid",bcid,name);

  name = "corrected_bcid[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("corrected_bcid",corrected_bcid,name);

  name = "badbcid[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("badbcid",badbcid,name);

  name = "nhits[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]/I";
  tree->Branch("nhits",nhits,name);

  name = "lowGain[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("charge_lowGain",charge_low,name);

  name = "highGain[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("charge_hiGain",charge_high,name);

  name = "gain_hit_low[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("gain_hit_low",gain_hit_low,name);

  name = "gain_hit_high[";
  name+=NCHIP; name+="][";name+=MEMDEPTH; name+="]["; name+=NCHANNELS; name+="]/I";
  tree->Branch("gain_hit_high",gain_hit_high,name);

  h2_hits = new TH2F("Hits_XY","Hits",32,-89.5,90-0.5,32,-89.5,90-0.5);
  h2_hits_Acq = new TH2F("Hits_XY_Acq","Hits per Acq",32,-89.5,90-0.5,32,-89.5,90-0.5);

  for (int j=0; j<NCHIP; j++) {
    //name = "hits_XY_chip_"; 
    //name+=j+1;
    //h2_hits_per_chip[j] = new TH2F(name,name,32,-89.5,90-0.5,32,-89.5,90-0.5);

    name = "hits_chip_"; 
    name+=j+1;
    h_hit_high[j] = new TH1F(name,name,NCHANNELS,0,NCHANNELS);

    name = "hits_filtered_chip_"; 
    name+=j+1;
    h_hit_filtered[j] = new TH1F(name,name,NCHANNELS,0,NCHANNELS);

    name = "nCol_chip_"; 
    name+=j+1;
    h_nCol[j] = new TH1F(name,name,MEMDEPTH+1,0,MEMDEPTH+1);

    name = "bcid_chip_"; 
    name+=j+1;
    h_bcid[j] = new TH1F(name,name,4096,0,4096);

    name = "col_chip_Acq"; 
    name+=j+1;
    h_col_Acq[j] = new TH1F(name,name,4,1,5);

  }

  dPlots->cd();
  for (int j=0; j<NCHIP; j++) {
    for (int i=0; i<NCHANNELS; i++) {
      name = "chargeHigh_chip_"; 
      name+=j+1;
      name+="_channel_";
      name+=i;

      //h_chargeHigh[j][i] = new TH1F(name,name,4096,0,4096);

      name+="_withHit";
      //h_chargeHigh_withHit[j][i] = new TH1F(name,name,4096,0,4096);

      name = "chargeLow_chip_"; 
      name+=j+1;
      name+="_channel_";
      name+=i;
      //h_chargeLow[j][i] = new TH1F(name,name,4096,0,4096);

      name+="_withHit";
      //h_chargeLow_withHit[j][i] = nevoid RAW2ROOT::analyse_hits()w TH1F(name,name,4096,0,4096);


      name = "hits_chip_"; 
      name+=j+1;
      name+="_channel_";
      name+=i;
      name+="_fct_BCID";
      h_hits_BCID[j][i] = new TH1F(name,name,4096,0,4096);
    }
    name = "TagHist_"; 
    name+=j+1;
    h_TagHist[j]  = new  TH1D(name, name,16,-0.5,15.5);
  }


  // map of channel number
  fout->cd();
  h_chnnb = new TH2I("Map_chn_nb","Map_chn_nb",32,-89.5,90-0.5,32,-89.5,90-0.5);
  h_chipnb = new TH2I("Map_chip_nb","Map_chip_nb",32,-89.5,90-0.5,32,-89.5,90-0.5);
  for(int chip = 0;chip<NCHIP;chip++){
    for(int chn = 0;chn<NCHANNELS;chn++){
      h_chnnb->Fill(info->GetX(chip,chn),info->GetY(chip,chn),chn);
      h_chipnb->Fill(info->GetX(chip,chn),info->GetY(chip,chn),chip);
      //std::cout << info->GetX(chip,chn)  << "," << info->GetY(chip,chn)  << " : " << chn << std::endl;

    }
  }
  // map of channel number

  fout->cd();

  return;
}






//******************************************************************************************************************

void RAW2ROOT::treeInit() { //init data for a single SPILL ?
  for (int k=0; k<NCHIP; k++) {
    for (int i=0; i<MEMDEPTH; i++) {
      //allbcid[i+k*MEMDEPTH]=0;
      bcid[k][i]=-999;
      badbcid[k][i]=0;
      corrected_bcid[k][i]=-999;
      nhits[k][i]=-999;
      for (int j=0; j<NCHANNELS; j++) {
        charge_low[k][i][j]=-999;
        charge_high[k][i][j]=-999;
        gain_hit_low[k][i][j]=-999;
        gain_hit_high[k][i][j]=-999;
      }
    }
  
    chipID[k]=-999;
    numCol[k]=-999;
  }

  recordEvent = true;

  return;
}


//******************************************************************************************************************
void RAW2ROOT::searchNmax(int N, int Size, Float_t * table, Float_t * tableout , int * ranks) {
  //assumes positive numbers, tableout and ranks initialized to 0
  int i=0;
  for (int dat=1;dat<Size+1;dat++){
    i=-1;
    while (i<N-1) {
      if (table[dat]>tableout[i+1]) {i++;} else {break;}
    }
    if (i>0) {
      for (int j=0;j<i;j++){tableout[j]=tableout[j+1]; ranks[j]=ranks[j+1];}
      tableout[i]=table[dat]; ranks[i]=dat; 
    } else if (i==0) {tableout[i]=table[dat]; ranks[i]=dat;}
  }

  return;}



void RAW2ROOT::analyse_hits() {
  // for each chip one look to the mean ratio of  (hits)/(total events) and (good hits)/(hits)
  // good hits are identified according to algo in readEvent function
  // total events is the cumulated sum of columns over spills
  TString name;
  cout << "ANALYSE HITS"<<endl;

  //one get total events using sum(bin x bin value) from the histo of the number of columns
  Double_t TotEvents[NCHIP]; //Double_t
  Double_t UnFilteredEvents[NCHIP]; //Double_t
  for (int nchip=0; nchip<NCHIP; nchip++) {
    TotEvents[nchip]=0;UnFilteredEvents[nchip]=0;
    for (int nbin=2;nbin<17;nbin++) {TotEvents[nchip]=TotEvents[nchip]+ (nbin-1)*h_nCol[nchip]->GetBinContent(nbin) ; //ncol=nbin-1
      //cout << nbin-1 << ":" <<h_nCol[nchip]->GetBinContent(nbin) << "  ";
    }
    //cout << endl;
    UnFilteredEvents[nchip] = TotEvents[nchip] - h_TagHist[nchip]->GetEntries();

    cout << "   get " << UnFilteredEvents[nchip] << " out of " << TotEvents[nchip] << " events for chip " << nchip ;
    cout << "  (" << UnFilteredEvents[nchip]*100/TotEvents[nchip] << "%)"<<endl;}
   
  //one get the number of hits for all chips, all channels  
  //cout << "   get " << h_nCol[k]->GetEntries() << " events for chip " << k << endl;
  //h_hit_filtered[k]->Fill(ichan);
  //h_chargeHigh[k][ichan]->Fill()
  //h_chargeHigh_withHit[k][ichan]->Fill()

  //map of Tagged events, % of tagged evt per chip per column
  name = "TagHist"; 
  TagHist  = new  TH2D(name, name, NCHIP,-0.5,NCHIP-0.5,15,0.5,15.5);
  for (int nchip=0; nchip<NCHIP; nchip++) {
    Double_t *bins = h_TagHist[nchip]->GetArray(); 
    for (int col=1; col<16; col++) {//bins start at index 1.
      TagHist->SetBinContent(nchip+1,col, bins[col]*100/ TotEvents[nchip] );
    }
  }


  //map of hit per chip per channel, normalized to the total number of event
  name = "HitMapHist"; 
  TH2D* HitMapHist  = new  TH2D(name, name, 16,-0.5,15.5,64,-0.5,63.5);
  name = "HitMapFilteredHist"; 
  TH2D* HitMapFilteredHist  = new  TH2D(name, name, 16,-0.5,15.5,64,-0.5,63.5);

  //Float_t HitMapFilteredData[NCHIP][NCHANNELS];
  //Double_t HitMapData[NCHIP][NCHANNELS];
  for (int nchip=0; nchip<NCHIP; nchip++) {
    Float_t *bins = h_hit_high[nchip]->GetArray(); 
    Float_t *binsfiltered = h_hit_filtered[nchip]->GetArray();     
    for (int chn=0; chn<NCHANNELS; chn++) {//bins start at index 1.
      HitMapHist->SetBinContent(nchip+1,chn+1, bins[chn+1]);// / (tree->GetEntries()*MEMDEPTH)); //TotEvents[nchip]);
      HitMapFilteredHist->SetBinContent(nchip+1,chn+1, binsfiltered[chn+1]);// / (tree->GetEntries()*MEMDEPTH)); //TotEvents[nchip]);
      //HitMapData[nchip][chn] = (Double_t) bins[chn+1] / TotEvents[nchip];
      //HitMapFilteredData[nchip][chn] = binsfiltered[chn+1] ;
      //cout << bins[chn+1] << endl;
    }
  }


  return;
  /*



  //analyse per chip
  const Float_t MaxNoiseLevel = 0.005;
  const Float_t MinNoiseLevel = 0.001; //per channel
  const Float_t SafetyFactor = 2; 
  const int ScanChannelNb = 3;
  Float_t tmpF=0;
  Float_t tmpF2=0;
  Float_t MedianHitRate[NCHIP];
  TH1F* HitStatHist[NCHIP];
  TH1F* HitRepartitionHist[NCHIP];
  TH1F* h; Double_t NBevt;
  Float_t binval[102];
  int HitStatus[NCHIP][ScanChannelNb+1]; //0=status(-1=decrease trig;+15=inc trig;+1=change chn), 1..3=channel list (pos=increase trigthreshold, neg=decrease, 0=nothing)
  for (int nchip=0; nchip<NCHIP; nchip++) {
  for (int scanch=0;scanch<ScanChannelNb+1;scanch++){HitStatus[nchip][scanch]=0;}
  }


   
  //I. check if number of event is not enough or too high
  for (int nchip=0; nchip<NCHIP; nchip++) {
  if (TotEvents[nchip]<1.4*tree->GetEntries()*MEMDEPTH*MinNoiseLevel*NCHANNELS) {HitStatus[nchip][0] = -15;} 
  else if ( TotEvents[nchip] > 0.4*tree->GetEntries()*MEMDEPTH) {HitStatus[nchip][0] = 15;} //too high if more than 40% potential events
  }

  //II. determine if the mean number of hit_filtered is in expected range
  // too low...|......|...ok...|......|......too high
  //         min/sf  min      max   max*sf
  Float_t tmp[ScanChannelNb];
  int rtmp[ScanChannelNb];
  Float_t MeanHitsRate[NCHIP];
  Float_t PartitionFirstBinAbove[NCHIP];
  Float_t PartitionMax[NCHIP];
  Float_t PartitionMaxChn[NCHIP];
  //Double_t tmp=0;
  cout << " -----  Mean hit ratio " << endl;
  for (int nchip=0; nchip<NCHIP; nchip++) {

  //work on ***unfiltered hits***
  //Float_t *bins = h_hit_filtered[nchip]->GetArray();     
  Float_t *bins = h_hit_high[nchip]->GetArray();     
  for (int i=0;i<3;i++) {tmp[i]=0;rtmp[i]=0;};
  searchNmax(ScanChannelNb,NCHANNELS,bins,tmp,rtmp);
  //build hist of hit bins
  name = "HitStatHist";       name+=nchip+1;
  binval[0]=-0.9;binval[101]=h_hit_filtered[nchip]->GetMaximum()+1;
  for (int i=0; i<25; i++) {binval[i+1]=i-0.5;}; //this 25 should be set accoring to MaxNoiseLevel
  for (int i=25; i<101; i++) {binval[i]=25-0.5 + (i-25)*h_hit_filtered[nchip]->GetMaximum()/75;};


  //HitStatHist[nchip]  = new  TH1F(name, name, ceil((h_hit_filtered[nchip]->GetMaximum()-h_hit_filtered[nchip]->GetMinimum())/25),h_hit_filtered[nchip]->GetMinimum(),h_hit_filtered[nchip]->GetMaximum());
  HitStatHist[nchip]  = new  TH1F(name, name, 100,binval);
  name = "HitRepartitionHist";       name+=nchip+1;
  HitRepartitionHist[nchip]  = new  TH1F(name, name, 64,0,63);
  for (int chn=0; chn<NCHANNELS; chn++) {//bins start at index 1.
  HitStatHist[nchip]->Fill(bins[chn+1]);
  }
  //compute median 
  for (int ibin=2;ibin<HitStatHist[nchip]->GetNbinsX();ibin++) {
  if (HitStatHist[nchip]->Integral(1,ibin)>31) {MedianHitRate[nchip]=binval[ibin-1];break;}
  }
  cout << "    Median of chip " << nchip << ": " << MedianHitRate[nchip]<< "|" <<HitStatHist[nchip]->GetMean() <<endl;

  //compute partition
  tmpF=0;tmpF2=0;
  for (int ibin=2;ibin<HitStatHist[nchip]->GetNbinsX();ibin++) {
  tmpF += HitStatHist[nchip]->GetBinContent(ibin)*HitStatHist[nchip]->GetBinCenter(ibin);      
  tmpF2 = HitStatHist[nchip]->Integral(1,ibin)  ;
  HitRepartitionHist[nchip]->SetBinContent(tmpF2,tmpF/ (tree->GetEntries()*MEMDEPTH*NCHANNELS));
  //if (nchip==1) {cout << "   "<<tmpF2<<"|"<< tmpF<< endl;}

  }
  PartitionMaxChn[nchip]=HitRepartitionHist[nchip]->GetMaximumBin();
  PartitionMax[nchip]=HitRepartitionHist[nchip]->GetBinContent(PartitionMaxChn[nchip]);
  if (PartitionMax[nchip]>MinNoiseLevel) {PartitionFirstBinAbove[nchip]=HitRepartitionHist[nchip]->FindFirstBinAbove((Double_t)MinNoiseLevel);}
  else {PartitionFirstBinAbove[nchip]=99;}
  cout << "    Partition of chip " << nchip << ": " << PartitionFirstBinAbove[nchip]<< "|" <<PartitionMax[nchip]<<"@"<< PartitionMaxChn[nchip]<<endl;
      
      
  //debug
      
  //if stat is low but Median of hits higher than expected with MaxNoise then decrease chip trigger and cut noisy channels
  //unless just decrease chip trigger
  if (HitStatus[nchip][0] == -15) {
  if (MedianHitRate[nchip] > MaxNoiseLevel*tree->GetEntries()) {//tune factor ??
  HitStatus[nchip][0] = -31;
  //find channels to cut : the one with higher hit rate
  for (int scanch=1;scanch<ScanChannelNb+1;scanch++) {HitStatus[nchip][scanch] = rtmp[ScanChannelNb-scanch];}
  }
  //verify that stat is relevant for each channel to cut
  //TODO
  continue;}

  //if stat is too much then check if it is due to a few channels only (in this case, cut the channels) 
  if (HitStatus[nchip][0] == 15) {
  if (MedianHitRate[nchip] < HitStatHist[nchip]->GetMean()/2) {//tune factor ??
  HitStatus[nchip][0] = 31;
  //find channels to cut : the one with higher hit rate
  for (int scanch=1;scanch<ScanChannelNb+1;scanch++) {HitStatus[nchip][scanch] = rtmp[ScanChannelNb-scanch];}
  }
   
  continue;}

  //debug
  //cout << "  chip "  << nchip << ":";
  //for (int scanch=0;scanch<ScanChannelNb;scanch++) {cout << " max"<<scanch<<":"<<tmp[scanch]<< " rank:"<<rtmp[scanch];}
  //cout << endl;


  //where stat is ~ok
  //check if median/mean too high then increase gtrig
  if (MedianHitRate[nchip] > HitStatHist[nchip]->GetMean()/2) {//tune factor ??
  HitStatus[nchip][0]=15;
  continue;}
	 
  //if a majority of channel have low hit count
  //check filtering stats
  if (UnFilteredEvents[nchip]/TotEvents[nchip] < .7) {//tune factor ??
  //high filtering, work on unfiltered hits
  h=h_hit_high[nchip];NBevt=TotEvents[nchip];}
  else {h=h_hit_filtered[nchip];NBevt=UnFilteredEvents[nchip];}
  //if one do not filter too much then
  //work on ***filtered hits***

      
      
  MeanHitsRate[nchip]=h->Integral() / NBevt;
  //cout << "   " << MeanHitsRate[nchip] << " for chip " << nchip << "(" << MeanHitsRate2[nchip]<< ")" << endl;
  //diagnose
  if (MeanHitsRate[nchip]<MinNoiseLevel*NCHANNELS/SafetyFactor) {//consider that with safety factor (of 10) one may decrase gtrig
  HitStatus[nchip][0]=-1;
  for (int scanch=1;scanch<ScanChannelNb+1;scanch++){
  HitStatus[nchip][scanch] = 0; 
  }
  }
  else if (MeanHitsRate[nchip]>MaxNoiseLevel*NCHANNELS*SafetyFactor) {//consider that with safety factor (of 10) one may incrase gtrig
  HitStatus[nchip][0]=10;
  for (int scanch=1;scanch<ScanChannelNb+1;scanch++){
  HitStatus[nchip][scanch] = 0; 
  }
  }	
  else if (MeanHitsRate[nchip]<MinNoiseLevel*NCHANNELS) { //noise seems good, check whether one or few chn are out of bound
  HitStatus[nchip][0]=1;
  // find channels with larger noise
  if (h->GetBinContent(h->GetMaximumBin()) / NBevt > MaxNoiseLevel ) {HitStatus[nchip][1] = h->GetMaximumBin();}
  else if (h->GetBinContent(h->GetMinimumBin()) / NBevt < MinNoiseLevel ){HitStatus[nchip][1] = h->GetMinimumBin()*-1;}
  else {HitStatus[nchip][1]=0;}
  for (int scanch=2;scanch<ScanChannelNb+1;scanch++) {HitStatus[nchip][scanch] = rtmp[ScanChannelNb-scanch];}
  }
  else if (MeanHitsRate[nchip]>MaxNoiseLevel*NCHANNELS) { //noise seem close to be acceptable, check whether one or few chn could be responsible
  HitStatus[nchip][0]=1;                                   //externhal software will manage...
  if (h->GetBinContent(h->GetMaximumBin()) / NBevt > MaxNoiseLevel ) {HitStatus[nchip][1] = h->GetMaximumBin();}
  else if (h->GetBinContent(h->GetMinimumBin()) / NBevt < MinNoiseLevel ){HitStatus[nchip][1] = h->GetMinimumBin()*-1;}
  else {HitStatus[nchip][1]=0;};
  for (int scanch=2;scanch<ScanChannelNb+1;scanch++){HitStatus[nchip][scanch] = rtmp[ScanChannelNb-scanch];}
  }
         
  else {
  HitStatus[nchip][0]=0;
  }
  //if (HitStatus[nchip][0])
  //delete HitStatHist;

  }


  //output log  
  /*
  for (int nchip=0; nchip<NCHIP; nchip++) {
  if     ( HitStatus[nchip][0]== 0) {cout << "   nothing  to do for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]== 1) {cout << "   change channel for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]==10) {cout << "   check channels & increase Gtrig for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]==15) {cout << "   increase Gtrig for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]==31) {cout << "   increase Gtrig and cut for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]==-1) {cout << "   check channels & decrease Gtrig for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]==-7) {cout << "   cut channels for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]==-15) {cout << "   decrease Gtrig for chip " << nchip << endl;}
  else if (HitStatus[nchip][0]==-31) {cout << "   decrease Gtrig and cut for chip " << nchip << endl;}
  for (int scanch=1;scanch<ScanChannelNb+1;scanch++){
  if (HitStatus[nchip][scanch]==0) {}
  else if (HitStatus[nchip][scanch]>0) {cout << "       " << HitStatus[nchip][scanch] << endl;}
  else if (HitStatus[nchip][scanch]<0) {cout << "       " << HitStatus[nchip][scanch] << endl;}
  }
  }
  */   

 
  return;
}



//******************************************************************************************************************

void RAW2ROOT::plotHistos() {
  h2_hits_Acq->Scale(1./event);
  h_col_Acq[0]->Scale(1./event);
  h_col_Acq[1]->Scale(1./event);
  h_col_Acq[2]->Scale(1./event);
  h_col_Acq[3]->Scale(1./event);

  fout->cd();
  fout->Write(0);
  fout->Close();

  return;
}

//******************************************************************************************************************

void RAW2ROOT::printEvent(std::vector < unsigned short int > & eventData) {
  if (_debug) {
    cout << "printing event for debug" << endl;
    for (unsigned int i=0; i<eventData.size(); i++) {
      cout << i << " 0x" << hex << eventData[i] << dec << endl;
    }
  }
  return;
}

//******************************************************************************************************************

void RAW2ROOT::readEvent(std::vector < unsigned short int > & eventData , int PlaneEventThreshold, int MinBcidIncrement) {
  if (_debug) cout << "analysing event with " << eventData.size() << " entries " << event << endl;

  //bool suspiciousEvent=false;

  // check we have a decent amount of data
  /* if (eventData.size()<10) {
     cout << "WARNING tiny event" << endl;
     printEvent(eventData);
     return;
     }
  */
  //std::vector< unsigned short int > eventChip;

  unsigned short int last=0;
  //unsigned int previousChipEndIndex = 0;
  unsigned int chipStartIndex = 0;

  int rawDataSize=0;
  int nColumns = 0;
  int local_offset = 0; //adapt to redundant chip ID word, value can be 0 or 1
  int previousBCID = -999;

  //unsigned int currentChip=0;
  bool isValidChip = false;

  for(unsigned int i=0;i<eventData.size();i++){
    //cout<<" i =  "<<i<<endl;

    if( eventData[i] == 0xfffd){// find start chip tag
      chipStartIndex=i+CHIPHEAD;
      if (_debug) cout << "   start chip";
    }

    if(last == 0xfffe){// find end chip tag

      /*if( eventData[i] == 0xff01){// chip 1
      //currentChip=0;
      isValidChip=true;
      //cout<<"CHIP "<<currentChip<<endl;
      }
      if( eventData[i] == 0xff02){// chip 2
      //currentChip=1;
      isValidChip=true;
      //cout<<"CHIP "<<currentChip<<endl;
      }
      if( eventData[i] == 0xff03){// chip 3
      //currentChip=2;
      isValidChip=tru CHIP 12 Begin..e;inputFileName
      //cout<<"CHIP "<<currentChip<<endl;
      }
      if( eventData[i] == 0xff04){// chip 4
      //currentChip=3;
      isValidChip=true;
      //cout<<"CHIP "<<currentChip<<endl;
      }*/


      if( (eventData[i] > 0xff00) & (eventData[i] < 0xffff)){// chips was fffc
        //currentChip=0;
        isValidChip=true;
	//if (_debug) cout <<"      CHIP "<<currentChip<<endl;
      }



      if(isValidChip){
        const int offset=2; //was 2
        rawDataSize = i-chipStartIndex-CHIPENDTAG;
        local_offset = (rawDataSize-offset)%(1+NCHANNELS*2);

        if(local_offset!=0){
          cout<<"<!> WARNING <!> Additionnal data words detected"<<endl;
          last=eventData[i];
        }
                
        nColumns = (rawDataSize-offset-local_offset)/(1+NCHANNELS*2);
        if (_debug) cout<<"Chip data: "  <<" size: "<<rawDataSize<<" col: "<<nColumns<<" evt: "<<(rawDataSize-offset)%(1+NCHANNELS*2) 
			<<"   local_offset :"<< local_offset  << endl;

        if((rawDataSize-offset-local_offset)%(1+NCHANNELS*2)!=0){
          cout<<"<!> ERROR <!> Bad data size"<<endl;
          last=eventData[i];
          continue;
        }

        if (nColumns>MEMDEPTH){
          cout << "<!> ERROR <!> Bad number of columns" <<endl;
          last=eventData[i];
          continue;
        }

        //test of the chip id
        int chipid = eventData[i-2];//was 3
	//cout << " %%%%%%%%%%% " << chipid << endl;
        bool isGoodChipNumber=false;
        for(unsigned int iChip = 0 ; iChip<chipIds.size();iChip++){
          if(chipid==chipIds[iChip]) isGoodChipNumber=true;
        }
        if(!isGoodChipNumber){
          cout << "<!> ERROR <!> Bad Chip ID: " << chipid <<endl;
          last=eventData[i];
          continue;
        }

        //cout<<"       * chip = "<<chipid<<endl;

        //Fill variables of the tree
        chipID[chipid]=chipid;
        int c = info->GetASUChipNumberFromChipID(chipid);

        numCol[chipid]= nColumns;
        h_nCol[c]->Fill(nColumns);
        h_col_Acq[c]->Fill(nColumns);

        previousBCID = -999;

        int loopBCID = 0;
        // now loop over the data, fill the charge values
        for (int ibc=0; ibc<nColumns; ibc++) {

          //fill BCID
          bcid[chipid][ibc]=eventData[i-3-1*local_offset-ibc] & 0x0fff ;  // !!!   index to be verified   !!!
          //allbcid[ibc+chipid*MEMDEPTH]= bcid[chipid][ibc];

          if(bcid[chipid][ibc]-previousBCID < 0) loopBCID++;
          corrected_bcid[chipid][ibc] = bcid[chipid][ibc]+loopBCID*4096;

          h_bcid[c]->Fill(bcid[chipid][ibc]);

          // fill the charges
          int ichan(0);
          // range for this column
          int begin =  i-3 - 1*local_offset - nColumns -  ibc*NCHANNELS*2; // !!!   index to be verified   !!!
          int end = begin - NCHANNELS;
          for (int jj = begin; jj>end; jj--) {

            if (ibc<MEMDEPTH && ichan<NCHANNELS){
              charge_low[chipid][ibc][ichan]=eventData[jj] & 0x0fff;
              //h_chargeLow[c][ichan]->Fill(charge_low[chipid][ibc][ichan]);

              gain_hit_low[chipid][ibc][ichan]= (eventData[jj] >> 12 ) & 0xf;
              //if(gain_hit_low[chipid][ibc][ichan]%2==1){
              //h_chargeLow_withHit[c][ichan]->Fill(charge_low[chipid][ibc][ichan]);
              //}

            }
            else {
              cout << "<!> ERROR <!> Low Gain : Bad number of columns: " << ibc << " or bad number of channels: " << ichan << endl;
            }
            ichan++;
          }


          // analyse hits and bcids
          begin=end; 
          end=begin - NCHANNELS;
          ichan=0;
          int count_hits = 0;
          for (int jj = begin; jj>end; jj--) {
          
            if (ibc<MEMDEPTH && ichan<NCHANNELS){
              charge_high[chipid][ibc][ichan]=eventData[jj] & 0x0fff;
              //h_chargeHigh[c][ichan]->Fill(charge_high[chipid][ibc][ichan]);

              gain_hit_high[chipid][ibc][ichan] = (eventData[jj] >> 12 ) & 0xf;
              if(gain_hit_high[chipid][ibc][ichan]%2==1){
                count_hits++;
                h_hit_high[c]->Fill(ichan);
                //h2_hits_per_chip[c]->Fill( info->GetX(c,ichan),info->GetY(c,ichan) );
                h2_hits->Fill(info->GetX(c,ichan),info->GetY(c,ichan) );
                h2_hits_Acq->Fill(info->GetX(c,ichan),info->GetY(c,ichan) );
                //h_chargeHigh_withHit[c][ichan]->Fill(charge_high[chipid][ibc][ichan]);
                h_hits_BCID[c][ichan]->Fill(bcid[chipid][ibc]);
              }
            } else {
              cout << "<!> ERROR <!> High Gain : Bad number of columns: " << ibc << " or bad number of channels: " << ichan << endl;
            }
            ichan++;
          }
          nhits[chipid][ibc]=count_hits;

          //if(bcid[chipid][ibc]-previousBCID > 0 && (bcid[chipid][ibc] - previousBCID < 6 ) && nhits[chipid][ibc]==0) badbcid[chipid][ibc]=3;
          //if(bcid[chipid][ibc]-previousBCID < 0 && (4096+bcid[chipid][ibc] - previousBCID < 6 ) && nhits[chipid][ibc]==0 ) badbcid[chipid][ibc]=3;	  

          //if(bcid[chipid][ibc]-previousBCID > 0 && (bcid[chipid][ibc] - previousBCID < 6 ) && nhits[chipid][ibc]!=0) badbcid[chipid][ibc]=2;
          //if(bcid[chipid][ibc]-previousBCID < 0 && (4096+bcid[chipid][ibc] - previousBCID < 6 ) && nhits[chipid][ibc]!=0 ) badbcid[chipid][ibc]=2;	  

          //if(bcid[chipid][ibc]-previousBCID > 0 && (bcid[chipid][ibc] - previousBCID < 2 ) && nhits[chipid][ibc]==0) badbcid[chipid][ibc]=1;
          //if(bcid[chipid][ibc]-previousBCID < 0 && (4096+bcid[chipid][ibc] - previousBCID < 2 ) && nhits[chipid][ibc]==0 ) badbcid[chipid][ibc]=1;	  

          previousBCID = bcid[chipid][ibc];

        }

        isValidChip=false;     

      }
 
    }
    //  else{
    //  cout<<"NO CHIP FOUND"<<endl;
    //  }

    last=eventData[i];
  }

  //cout<<"-----------------------------"<<endl;
  
  //add tags
  int  count_negdata=0;




  for (int k=0; k<NCHIP; k++) {
    //only for valid chips in this spill
    if (chipID[k]>=0) {
      for (int ibc=0; ibc<numCol[k]; ibc++) {
      
	//tag plane events
	if (nhits[k][ibc] > PlaneEventThreshold) {
	  badbcid[k][ibc]=16; h_TagHist[k]->Fill(ibc);
        } else {
	  //tag successive bcids
	  if (ibc==0) {
	    badbcid[k][ibc]=0;	//first col is always ok
	    for (int ichan=0; ichan<NCHANNELS; ichan++) {
	      if(gain_hit_high[k][ibc][ichan]%2==1){
		h_hit_filtered[k]->Fill(ichan);
		//h_chargeHigh_withHit[k][ichan]->Fill(charge_high[k][ibc][ichan]);
	      }
	    }
	  } else {
	    for (int ichan=0; ichan<NCHANNELS; ichan++) {
	      if(gain_hit_high[k][ibc][ichan]%2==1){
		h_hit_filtered[k]->Fill(ichan);
		//h_chargeHigh_withHit[k][ichan]->Fill(charge_high[k][ibc][ichan]);
	      }
	    }
	    //irles 
	    if( ( corrected_bcid[k][ibc] - corrected_bcid[k][ibc-1]) > 0  &&  (corrected_bcid[k][ibc] - corrected_bcid[k][ibc-1]) < 16 ) {
	      //if (corrected_bcid[k][ibc] > corrected_bcid[k][ibc-1] + MinBcidIncrement) {
	      badbcid[k][ibc]=corrected_bcid[k][ibc] - corrected_bcid[k][ibc-1];
	      h_TagHist[k]->Fill(ibc);
	    } else {
	      badbcid[k][ibc]=0; //h_TagHist[k]->Fill(ibc);
	    }
       	  }//if ibc == 0
	   
   	  /* for (int i=0; i<NCHIP*MEMDEPTH; i++) {
	     if(bcid[k][ibc] - allbcid[i] > 0 && (bcid[k][ibc] - allbcid[i] < 6 ) && nhits[k][ibc]==0) badbcid[k][ibc]=3;
	     //if(bcid[k][ibc] - allbcid[i] < 0 && (4096+bcid[k][ibc] - allbcid[i] < 6 ) && nhits[k][ibc]==0 ) badbcid[k][ibc]=3;	  
	     
	     if(bcid[k][ibc] - allbcid[i] > 0 && (bcid[k][ibc] - allbcid[i] < 6 ) && nhits[k][ibc]!=0) badbcid[k][ibc]=2;
	     //if(bcid[k][ibc] - allbcid[i] < 0 && (4096+bcid[k][ibc] - allbcid[i] < 6 ) && nhits[k][ibc]!=0 ) badbcid[k][ibc]=2;	  
	     
	     //	cout<<k<<"  "<<ibc<<"  "<<bcid[k][ibc]<<"  "<<allbcid[i]<<"  "<<badbcid[k][ibc]<<"  "<<bcid[k][ibc] - allbcid[i]<<endl;
	     
	     }
	     if(bcid[k][ibc] - bcid[k][ibc-1] > 0 && (bcid[k][ibc] - bcid[k][ibc-1] < 2 ) && nhits[k][ibc]==0) badbcid[k][ibc]=1;
	     //if(bcid[k][ibc] - bcid[k][ibc-1] < 0 && (4096+bcid[k][ibc] - bcid[k][ibc-1] < 2 ) && nhits[k][ibc]==0 ) badbcid[k][ibc]=1;
	     */
	  
	}//nhits
	
	//tag zero data
	count_negdata=0;

        for (int ichan=0; ichan<NCHANNELS; ichan++) {
	  if  (charge_high[k][ibc][ichan] < NEGDATA_THR) {
	    count_negdata++;
	  }
	}//ichan
	if (count_negdata>1) {
	  badbcid[k][ibc]+=32;
	}
	
	
      }//ibc
    }//chipID
  }//k

  return;
}

//******************************************************************************************************************

void RAW2ROOT::ReadFile(TString inputFileName, bool overwrite, int PlaneEventThreshold, int MinBcidIncrement, int maxevt) {
  cout <<endl;
  cout << "            **** READING FILE " << inputFileName << " ****"<<endl;

  event=0;
  acqNumber=0;

  if(!overwrite){
    fout = new TFile(inputFileName+".root","create");
    if(!fout->IsOpen()){
      cout<<"<!> ERROR <!>   File already created!"<<endl;
      return;
    }
  }
  else {
    fout = new TFile(inputFileName+".root","recreate");
  }

  dPlots = fout->mkdir("plots");

  Initialisation();
  ifstream fin;
  unsigned short int dataResult=0;


  //int rawDataSize=0;
  //int nColumns = 0;
  bool outlog = false;
  bool altTag = false;
  int countchipdata=0;
  int countchip=0;

  fin.open(inputFileName.Data(), ios_base::in|ios_base::binary);
  //int nline=0;
  std::vector < unsigned short int > packetData;
  std::vector < unsigned short int > eventData;
  unsigned short int lastTwo[2]={0};

  if(fin.is_open())
    while (fin.read((char *)&dataResult, sizeof dataResult) && event<maxevt ) {

      packetData.push_back(dataResult);countchipdata++;

      if(dataResult == 0xFFFE) {//END of CHIP
        altTag = true; 
      }

      if(lastTwo[1] == 0xFFFC){ 
        if (outlog) cout << "   SPILL "<< lastTwo[0] << " " << dataResult << " START--->" <<endl;
        countchip=0;
      }
      if(lastTwo[1] == 0xFFFD){ 
        if (outlog) cout << "       CHIP "<< lastTwo[0] - 65280 << " Begin..."; countchipdata=0;
      }
      
      if(lastTwo[1] == 0xFFFE){ 
        if (outlog) cout << "...End CHIP "<< lastTwo[0] - 65280 << " with " << countchipdata-2-3 << " words" <<endl;
	if (countchipdata>100) {countchip++;};
      }

      if(lastTwo[1] == 0xFFFF){ 
        if (outlog) cout << "   ----->END SPILL "<< lastTwo[0] << " " << dataResult << "  with " << countchip << " chips"<< endl<<acqNumber<<endl;
        packetData.clear();
        lastTwo[0]=lastTwo[1]=dataResult=0;
        altTag = false;
      }
      
      if(lastTwo[1] == 0x2020 && lastTwo[0] == 0x2020 && dataResult == 0xFFFF){// SPILL END
        if(recordEvent){
	  if (countchip>0){
	    if (outlog) cout<<"ReadEvent, packet data size= "<<packetData.size()<<" for "<<countchip<<" chips"<<endl;
	    readEvent(packetData, PlaneEventThreshold, MinBcidIncrement);
	    event++;
	    tree->Fill();}
        }
	/* packetData.clear();
	   lastTwo[0]=lastTwo[1]=dataResult=0;
	   altTag = false;*/
      }
      
      if(lastTwo[1] == 0x5053 && lastTwo[0] == 0x4C49 && dataResult == 0x2020){ //New SPILL: extract SPILL number 
        if(altTag){ 
	  //if (outlog) cout <<){
          cout<<"<!> WARNING <!> New spill without end flag of the previous spill - some chips found "<<endl;
	  /*if(recordEvent){
            readEvent(packetData);
            event++;
            tree->Fill();
	    }*/
        }
        acqNumber= packetData[packetData.size()-5]*65536+packetData[packetData.size()-4];
        //cout<<"SPILL = "<<acqNumber<<endl;

        treeInit();

        packetData.clear();
        lastTwo[0]=lastTwo[1]=dataResult=0;
        altTag = false;

      }

      lastTwo[1] = lastTwo[0];
      lastTwo[0] = dataResult;

    }


  cout <<endl;
  cout << "           **** Finished reading file ****" << endl;
  cout << "           ****  with  "<< tree->GetEntries()<< "  entries  ****" << endl;
  cout <<endl;

  analyse_hits();
  plotHistos();

  return;
}

//******************************************************************************************************************






////////////////////////  GETTREE   ////////////////////////  
int RAW2ROOT::GetTree (TString rootfilename) { //from raw2root 1st pass
  //open pre-precessed data
  finroot = new TFile(rootfilename,"read");
  if (! finroot ) {return 0;}
  fev10read = (TTree*)finroot->Get("fev10"); //rawdata

  //get data from file
  fev10read->SetBranchAddress("gain_hit_high",gain_hit_high );
  fev10read->SetBranchAddress("gain_hit_low",gain_hit_low );
  fev10read->SetBranchAddress("charge_hiGain",charge_high );
  fev10read->SetBranchAddress("charge_lowGain",charge_low );
  fev10read->SetBranchAddress("bcid", bcid);
  fev10read->SetBranchAddress("badbcid", badbcid);
  fev10read->SetBranchAddress("nhits", nhits);
  fev10read->SetBranchAddress("event", &event);
  fev10read->SetBranchAddress("acqNumber", &acqNumber);
  fev10read->SetBranchAddress("corrected_bcid",corrected_bcid);
  fev10read->SetBranchAddress("nColumns",numCol);
  fev10read->SetBranchAddress("chipid",chipID);

  std::cout << "PlotFool::GetTree : Get tree from "<< rootfilename.Data() << std::endl;
	
  R2Rstate=1;
  return 1;

}//method GetTree




////////////////////////  BUILDTIMEDEVENTS   ////////////////////////  
int RAW2ROOT::BuildTimedEvents (TString rootfilename) {
  if (R2Rstate<0) return 0;
  TTree* fev10write;

  int Wevent;
  int WacqNumber;
  int Wbcid;
  int Wsca;
  int Wcorrected_bcid;
  int Wbadbcid[NCHIP];
  int Wnhits[NCHIP];
  int Wcharge_low[NCHIP][NCHANNELS];
  int Wcharge_high[NCHIP][NCHANNELS];
  int Wgain_hit_low[NCHIP][NCHANNELS];
  int Wgain_hit_high[NCHIP][NCHANNELS];

  int REFbcid = 0;
  bool LASTbcid = false;

  TString name;

 
  if (! GetTree(rootfilename)) {
    std::cout << "RAW2ROOT ERROR: "<< rootfilename.Data() << " not found" << std::endl;
    return 0;
  }


  //create new file
  TFile *fout = new TFile(rootfilename.ReplaceAll(".root","_TimeEvent.root"),"recreate");

  //create new tree
  fev10write = new TTree("fev10","fev10");
  //create branches
  fev10write->Branch("event",&Wevent,"event/I");
  fev10write->Branch("sca",&Wsca,"sca/I");
  fev10write->Branch("bcid",&Wbcid,"bcid/I");
  fev10write->Branch("corrected_bcid",&Wcorrected_bcid,"corrected_bcid/I");
  fev10write->Branch("acqNumber",&WacqNumber,"acqNumber/I");

  name = "badbcid[";
  name+=NCHIP;  name+="]/I";
  fev10write->Branch("badbcid",Wbadbcid,name);

  name = "nhits[";
  name+=NCHIP;  name+="]/I";
  fev10write->Branch("nhits",Wnhits,name);

  name = "lowGain[";
  name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
  fev10write->Branch("charge_lowGain",Wcharge_low,name);

  name = "highGain[";
  name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
  fev10write->Branch("charge_hiGain",Wcharge_high,name);

  name = "gain_hit_low[";
  name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
  fev10write->Branch("gain_hit_low",Wgain_hit_low,name);

  name = "gain_hit_high[";
  name+=NCHIP; name+="]["; name+=NCHANNELS; name+="]/I";
  fev10write->Branch("gain_hit_high",Wgain_hit_high,name);
	
	

  for(unsigned entry = 0 ;entry<fev10read->GetEntries();entry++){
    fev10read->GetEntry(entry);
    //create new event for each different bcid value
	    
    /*   	
		while (!LASTbcid) {
		//search if it is the last bcid
		chip=0;col=0;LASTbcid = true;
		while (chip<NCHIP && col < NCOL) {
		if (corrected_bcid[chip][col]>=REFbcid) {
		LASTbcid = false;
		//memorize smallest step
		if (DELTAbcid==0) {DELTAbcid=corrected_bcid[chip][col]-REFbcid;};
		if (corrected_bcid[chip][col]-REFbcid < DELTAbcid) {DELTAbcid=corrected_bcid[chip][col]-REFbcid;}
		}
		col++;
		if (col>NCOL) {chip++;col=0;}
		}//while
		
		//inc REFbcid
		if (!LASTbcid) {
		REFbcid+=DELTAbcid;
		// fill Wevent	
		for (int chip=0; chip<NCHIP; chip++) {
		//only for valid chips in this spill
		if (chipID[k]>=0) {
		for (int col=0; col<numCol[k]; col++) {
		if (corrected_bcid[chip][col]=REFbcid) {
		//append data to Wevent
							
		// TO DO <--------
							
		}
		}
		}
		}//for
		}//if 
		
		}//while
		
		
    */


    //fev10write->Fill();
	
  }//entry



  return 1;
}//method BuildTimedEvents



