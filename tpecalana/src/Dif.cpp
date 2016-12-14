//
//  Dif.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#include "Dif.h"

Dif::Dif(unsigned difID){
    //Set the DIFiD
    _difID=difID;
    //Set the numbr of ASUs connected to that DIF (will have to come from an external source)
    _numASU=0;
    //Reset the vector that stores the number of events in each acquisition
    _difStatVec.clear();
}


Dif::~Dif(){/* no op*/}


void Dif::init(std::vector< std::pair<std::string, std::pair<unsigned,unsigned> > > asuInfoVec) {
    //The size of the input vector defines the number of asus connected to this dif
    _numASU=asuInfoVec.size();
    for (unsigned iasu=0; iasu<_numASU; iasu++) {
     //set the ASUId
     _asuVec.push_back(ASU (0) );
     _asuVec.at(iasu).init(asuInfoVec.at(iasu).first, asuInfoVec.at(iasu).second.second, _difID);
   }
}


void Dif::dataAnalysis(TFile* aDifFile, int PlaneEventThreshold) {

    std::cout << "New file " << std::endl;
    //Loop over the connected ASUs to retrieve the information needed for a proper exploitation of the data container
    //We assume that the data container from raw2root is aligned to the maximum number of channels and the maximum
    //bufferdepth of the chips in the setup (Let's see how far we get with this assumption
    //Would be interesting to see root files for long slabs!!!!
    
    unsigned NASICS(0);
    unsigned MAXCHAN(0);
    unsigned MAXBUFDEPTH(0);
    for (unsigned iasu=0; iasu<_numASU; iasu++) {
        //std::cout << "Dif::dataAnalysis iasu: " << iasu << std::endl;
        //std::cout << "Number of connected ASICs: " << _asuVec.at(iasu).getNumberofASICs() << std::endl;
        NASICS += _asuVec.at(iasu).getNumberofASICs();
        for (unsigned ichip=0; ichip < _asuVec.at(iasu).getNumberofASICs(); ichip++) {
            //std::cout << "Number of channels of " << ichip << " is: " << _asuVec.at(iasu).getChip(ichip).getNumberOfASICChannels() << std::endl;
            //std::cout << "BufferDepth of " << ichip << " is: " << _asuVec.at(iasu).getChip(ichip).getBufferDepth() << std::endl;
            if (_asuVec.at(iasu).getChip(ichip).getNumberOfASICChannels() > MAXCHAN ) MAXCHAN=_asuVec.at(iasu).getChip(ichip).getNumberOfASICChannels();
            if (_asuVec.at(iasu).getChip(ichip).getBufferDepth() > MAXBUFDEPTH) MAXBUFDEPTH=_asuVec.at(iasu).getChip(ichip).getBufferDepth();
        }
    }
    
    
    std::cout << "Total number of ASUS connected to dif: "  << _difID << " is " << _numASU << std::endl;
    std::cout << "Number of ASIC read by dif: "  << _difID << " is " << NASICS << std::endl;
    std::cout << "Maximum number of channels for an ASIC observed for dif: "  << _difID << " is " << MAXCHAN << std::endl;
    std::cout << "Maximum buffer depth for the ASICs read by dif: "  << _difID << " is " << MAXBUFDEPTH << std::endl;
    
    //The following is very much coupled to the actual structure of the root file
    //This has to be made more generic by avoiding a time consuming back and forth copying
    //Not trivial though ... 
    //We start now to analyse the rot file
    int high_gain[NASICS][MAXBUFDEPTH][MAXCHAN];
    int low_gain[NASICS][MAXBUFDEPTH][MAXCHAN];
    int gain_hit_high[NASICS][MAXBUFDEPTH][MAXCHAN];
    int gain_hit_low[NASICS][MAXBUFDEPTH][MAXCHAN];
    int bcid[NASICS][MAXBUFDEPTH];
    int corrected_bcid[NASICS][MAXBUFDEPTH];
    int badbcid[NASICS][MAXBUFDEPTH];
    int chipid[NASICS];
    int acqNumber;
    int nhits[NASICS][MAXBUFDEPTH];
    
    TTree *t1 = (TTree*) aDifFile->Get("fev10");
    //Set the branch address for the hits with high gain
    
    t1->SetBranchAddress("charge_lowGain",&low_gain);
    t1->SetBranchAddress("charge_hiGain",&high_gain);
    t1->SetBranchAddress("gain_hit_high",&gain_hit_high);
    t1->SetBranchAddress("gain_hit_low",&gain_hit_low);
    t1->SetBranchAddress("bcid",&bcid);
    t1->SetBranchAddress("corrected_bcid",&corrected_bcid);
    t1->SetBranchAddress("badbcid",&badbcid);
    t1->SetBranchAddress("chipid",&chipid);
    t1->SetBranchAddress("acqNumber",&acqNumber);
    t1->SetBranchAddress("nhits",&nhits);

    unsigned nentries(t1->GetEntries());

    std::cout << "nentries " << nentries << std::endl;
    for (unsigned ientr =0; ientr < nentries; ientr++) {

      for(unsigned i=0; i<NASICS; i++) {
	for(unsigned j=0; j<MAXBUFDEPTH; j++ ) {
	  if(nhits[i][j] > PlaneEventThreshold) {
	    for(unsigned k=0; k<MAXCHAN; k++) {
	      low_gain[i][j][k]=0;
	      high_gain[i][j][k]=0;
	    }
	  }
	}
      }
        
        //std::cout << "ientr: " << ientr << std::endl;
        t1->GetEntry(ientr);
        
        unsigned containerEntryofASU(0);
        unsigned ntot(0);
        for (unsigned iasu=0; iasu<_numASU; iasu++) {
	  //Transfer the global buffer information to the ASU (which will then distribute the information to the Chips) 
	  
	  _asuVec.at(iasu).transferChipBufferData(&chipid[containerEntryofASU], &bcid[containerEntryofASU][0], &corrected_bcid[containerEntryofASU][0], &badbcid[containerEntryofASU][0]);
	  //
	  _asuVec.at(iasu).transferChipChannelData(&chipid[containerEntryofASU], &high_gain[containerEntryofASU][0][0], &low_gain[containerEntryofASU][0][0], &gain_hit_high[containerEntryofASU][0][0], &gain_hit_low[containerEntryofASU][0][0]);
	  //Store general information
	  ntot+=_asuVec.at(iasu).getTotalNumberOFTriggers();
	  //Set pointer to next ASU connected to this DIF
	  containerEntryofASU+=_asuVec.at(iasu).getNumberofASICs();
	}
        //std::cout << "acqNumber: " << acqNumber << std::endl;
        //std::cout << "Total number of triggers: " << ntot << std::endl;
	storeAcquisitionInfo(acqNumber, ntot);
    }
    aDifFile->Close();
}

void Dif::storeAcquisitionInfo( unsigned acqnumber, unsigned ntot) {
    _difStatVec.push_back(std::make_pair(acqnumber, ntot));
}

std::vector<std::pair<unsigned, unsigned> > Dif::getDifStatVec() {
    return _difStatVec;
}

ASU Dif::getASU(unsigned iasu) {
  return _asuVec.at(iasu);
}
