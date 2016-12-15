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


// root> .x MergeByN.C+
// root> M=MergeByN();
// root>  M->DoMerge("/home/calice/data/SCurveAll",1,200,250,5);




class MergeByN{
public:


  MergeByN(){
       info = new InfoChip();
       GeoMapOrder = true;

  }


  ~MergeByN(){delete info;}


void DoMerge(TString rootdir, int Slice_Size, int Start, int Stop, int Step);

  
  std::vector <int> fileID;
  std::vector <int> channelOFFSET;


protected:

  enum {
    MEMDEPTH=15,
    NCHANNELS=64,
    NCHIP=16,
    MIN_BCID=4,
    NSLABS=4,
    SLICESIZE=8
       };
    InfoChip * info;

    bool GeoMapOrder;

};


//******************************************************************************************************************

  void MergeByN::DoMerge(TString rootdir,int Slice_Size, int Start, int Stop, int Step){
    //file read order
   for(int i = 0;i<NCHANNELS/Slice_Size;i++){
    fileID.push_back(i*Slice_Size); //one read from ~/0/... to ~/x/...
   }   

   //offset of channels per file
   for(int i = 0;i<NCHANNELS;i=i+NCHANNELS/Slice_Size){
    channelOFFSET.push_back(i);
   }   



    bool __debug = true;
    TString name = "";
    TString outfilename = rootdir;outfilename += "/MergeBy";outfilename += Slice_Size;outfilename += "_dif0"; //ADD outfilename += _dif0;
    outfilename += ".root";
    Double_t dd, x, y;
    TMultiGraph *mg;
    TGraph *g;
    TMultiGraph *mg_f;
    TGraph *g_f;
    TList *mglist;
    TDirectory *dir;
    TDirectory *dir_f;
    TFile *f;
    TFile *fout;
    fout = new TFile(outfilename,"recreate");
    int nchn = 0;
	
    TDirectory *d_plots;name = "Plots";
	d_plots = fout->mkdir(name);
	d_plots->cd();
	TH2D *h_scurves_map = new TH2D("h_scurves_map","scurves_map",NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
	TH2D *h_scurves_geomap= new TH2D("h_scurves_geomap","scurves_geomap",32,-89.5,90-0.5,32,-89.5,90-0.5);
	TH2D *h_scurves_map_f = new TH2D("h_scurves_map_filtred","scurves_map_filtred",NCHANNELS,-0.5,NCHANNELS-0.5,NCHIP,-0.5,NCHIP-0.5);
	TH2D *h_scurves_geomap_f= new TH2D("h_scurves_geomap_filtred","scurves_geomap_filtred",32,-89.5,90-0.5,32,-89.5,90-0.5);
	fout->cd();
    TDirectory *d_hists[NCHIP];name = "Chip";
    for (int nchip=0;nchip<NCHIP;nchip++){ 
        name = "Chip";name += nchip;
        d_hists[nchip] = fout->mkdir(name);
        }

       TCanvas *canvas[NCHIP];
       for(int nchip = 0;nchip<NCHIP;nchip++){
            name = "Scurves_byN_chip_"; name +=nchip;
            canvas[nchip] = new TCanvas(name,name,800,800);   
            canvas[nchip]->cd();
            canvas[nchip]->Divide(8,8);
       }

       TCanvas *canvas_f[NCHIP];
       for(int nchip = 0;nchip<NCHIP;nchip++){
            name = "Scurves_byN_chip_"; name +=nchip; name +="filtred";
            canvas_f[nchip] = new TCanvas(name,name,800,800);   
            canvas_f[nchip]->cd();
            canvas_f[nchip]->Divide(8,8);
       }

    
	fout->cd();
	name = "all";
	TH2D* all  = new  TH2D(name, name, ((Stop-Start)/Step),Start,Stop,NCHANNELS*NCHIP,0,NCHANNELS*NCHIP);
	name = "all_f";
	TH2D* all_f  = new  TH2D(name, name, ((Stop-Start)/Step),Start,Stop,NCHANNELS*NCHIP,0,NCHANNELS*NCHIP);





    for(int slice = 0;slice<fileID.size();slice++){
      //for each slice, fetch channels for all chips
      //and add them to a 64 plots caneva for each chip

      TString filename = rootdir;
      filename += "/";
      //filename += "allevery8_"; 
      filename += fileID[slice];
      filename += "/Scurves_dif0.root"; // CHANGE by filename += "/Scurves_dif0.root";
      
      if (__debug) std::cout << filename << std::endl;
      
      f = new TFile(filename,"read");


      if(f->IsOpen()){
         if (__debug) std::cout << "Found file " << filename << std::endl;


           for(int nchip = 0;nchip<NCHIP;nchip++){
                name ="Chip"; name += nchip;
                dir = (TDirectory * ) f->Get(name);  
                dir->cd();
                mg = (TMultiGraph* ) dir->Get(name);
           
                //for(int nchn = slice*SLICESIZE;nchn<(slice+1)*SLICESIZE;nchn++){ //by8
                for(int index = 0;index<channelOFFSET.size();index++){ //every8
		    		nchn = slice + channelOFFSET[index]; //every8
                    //if (__debug) std::cout << nchn << ", ";
		    		mglist = (TList *) mg->GetListOfGraphs();
                    g = (TGraph *) mglist->At(nchn);                    
                    
                    if (GeoMapOrder) {
                       canvas[nchip]->cd(1 + floor((info->GetX(0,nchn)+67.5+19.5)/5.5) + 8* floor((info->GetY(0,nchn)+67.5+19.5)/5.5));} 
                      else {
                       canvas[nchip]->cd(nchn+1);}
                    g->SetTitle(Form("Channel %d",nchn));
                    g->SetMinimum(0);
					for (int npt=0;npt<1+((Stop-Start)/Step);npt++){
			 			g->GetPoint(npt+1,x,y);
    			 		all->SetBinContent(npt+1,1+NCHANNELS*nchip+nchn,y*1000);
					}

		    		//analyse graph
		    		dd=g->GetMaximum();//not working
					g->GetPoint((Int_t)ceil(g->GetN()/2.0), x, y);
                        		//if (__debug) std::cout << "Max " << dd << " chip:" << nchip << " chn:" << nchn << std::endl;
                        		//if (__debug) std::cout << "Point " << ceil(g->GetN()/2.0) << " x:" << x << " y:" << y << std::endl;
		    		g->SetLineColor(kBlack);
					g->SetLineWidth(1);
		/* 			if (dd<100) {
						//lowstat
		    				g->SetLineColor(12);
						g->SetLineWidth(2);
						}
		 */			if (y>10) {
						//
		    			g->SetLineColor(2);
						g->SetLineWidth(4); 
						h_scurves_map->Fill((Double_t) nchn,(Double_t) nchip, 2);//x,y,weight
						h_scurves_geomap->Fill(info->GetX(nchip,nchn),info->GetY(nchip,nchn), 2);//x,y,weight
						}

                    		g->Draw("ALP");
		    		//canvas[nchip]->Update();
                    		//canvas[nchip]->Modified();

                	}//for nchn


				//same for filtred
                name ="Chip"; name += nchip; name += "filtred";
                dir_f = (TDirectory * ) f->Get(name);  
                dir_f->cd();
                name ="Chip"; name += nchip; name += "_filtered";
                mg_f = (TMultiGraph* ) dir_f->Get(name);
           
                //for(int nchn = slice*SLICESIZE;nchn<(slice+1)*SLICESIZE;nchn++){ //by8
                for(int index = 0;index<channelOFFSET.size();index++){ //every8
		    		nchn = slice + channelOFFSET[index]; //every8
		    		mglist = (TList *) mg_f->GetListOfGraphs();
                    g_f = (TGraph *) mglist->At(nchn);                    
                    canvas_f[nchip]->cd(nchn+1);
                    g_f->SetTitle(Form("Channel %d filtred",nchn));
                    g_f->SetMinimum(0);

					//fill 2D/3D plots
					for (int npt=0;npt<1+((Stop-Start)/Step);npt++){
			 			g_f->GetPoint(npt+1,x,y);
    			 		all_f->SetBinContent(npt+1,1+NCHANNELS*nchip+nchn,y*1000);
					}

		    		//analyse graph
		    		dd=g_f->GetMaximum();//not working
					g_f->GetPoint((Int_t)ceil(g_f->GetN()/2.0), x, y);
                        		//if (__debug) std::cout << "Max " << dd << " chip:" << nchip << " chn:" << nchn << std::endl;
                        		//if (__debug) std::cout << "Point " << ceil(g->GetN()/2.0) << " x:" << x << " y:" << y << std::endl;
		    		g_f->SetLineColor(kBlack);
					g_f->SetLineWidth(1);
		/* 			if (dd<100) {
						//lowstat
		    				g->SetLineColor(12);
						g->SetLineWidth(2);
						}
		 */			if (y>10) {
						//
		    			g_f->SetLineColor(2);
						g_f->SetLineWidth(4); 
						h_scurves_map_f->Fill((Double_t) nchn,(Double_t) nchip, 2);//x,y,weight
						h_scurves_geomap_f->Fill(info->GetX(nchip,nchn),info->GetY(nchip,nchn), 2);//x,y,weight
						}

                    		g_f->Draw("ALP");
		    		//canvas[nchip]->Update();
                    		//canvas[nchip]->Modified();

                }//for nchn
				//if (__debug) std::cout << std::endl;
           }//for nchip
        }//isopen
    }//for slice



    for(int nchip = 0;nchip<NCHIP;nchip++){
        canvas[nchip]->Update();
        canvas[nchip]->Modified();
        d_hists[nchip]->WriteTObject(canvas[nchip]);
        canvas_f[nchip]->Update();
        canvas_f[nchip]->Modified();
        d_hists[nchip]->WriteTObject(canvas_f[nchip]);
    }//for nchip

	d_plots->WriteTObject(h_scurves_map);
	d_plots->WriteTObject(h_scurves_geomap); 
	d_plots->WriteTObject(h_scurves_map_f);
	d_plots->WriteTObject(h_scurves_geomap_f); 

//////////////  OLD //////////////////
/*          
       TCanvas *canvas_map;
       name = "Canva_Test1_";name += i;
       canvas_map = new TCanvas(name,name,800,800);   
       canvas_map->cd();    
       mg->Draw("alp");    


       mglist = (TList *) mg->GetListOfGraphs();
       
       g = (TGraph *) mglist->Last();
       g->SetLineColor(1+i);
       g->SetMarkerColor(1+i);
       g->SetMarkerStyle(20+i);     

           
       mgout->Add(g);
       canvas_first->cd();  
       mgout->SetTitle("Chip 0 Scurves channel 63");  
       mgout->Draw("alp");
       mgout->GetXaxis()->SetTitle("ADC count");
       mgout->GetYaxis()->SetTitle("Number of hits");
*/
    fout->cd();
    fout->Write();
    fout->Close();


if (__debug) std::cout << "Wrote file " << outfilename << std::endl;
	

}//end doMerge




