//
//  ExperimentalSetup.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#include "ExperimentalSetup.h"
#include "Dif.h"
//#include "global.h"

//using namespace globalvariables;

ExperimentalSetup *ExperimentalSetup::theInstance = 0;

ExperimentalSetup::ExperimentalSetup(){
    //create some basin information about the ASICs in the setup
    _asicInfoMap["SKIROC"]=make_pair(64, 15);
    //Clear the vector of Difs
    _difVec.clear();
    //Clear the vector of Mappings
    _aMappingPtrVec.clear();
}

ExperimentalSetup::~ExperimentalSetup(){}


ExperimentalSetup *ExperimentalSetup::getInstance()
{
    if (!theInstance)
    theInstance = new ExperimentalSetup();
    return theInstance;
}

void ExperimentalSetup::release()
{
    if( theInstance != NULL ) delete theInstance;
    theInstance = NULL;
}


void ExperimentalSetup::reset() {
  _difVec.clear();
  _aMappingPtrVec.clear();
}


void ExperimentalSetup::setRunSetup(std::vector<std::string> aFileNameVec) {
    //Different ASUs can have different mappings, these mappings are encoded here
    _aFileNameVec=aFileNameVec;
    //Loop over the input vector containing the mapping files and Create a entry in the mapping vector for each new ASU mapping
    for (unsigned imap=0; imap<_aFileNameVec.size(); imap++) {
        _aMappingPtrVec.push_back(new Mapping () );
        _aMappingPtrVec.at(imap)->init( _aFileNameVec.at(imap) );
    }

    
    //In principle all the following should be retrieved from the daq file or a conditions data base
    //For the moment we define the setup for each run here
    _numDifs=1;
    std::cout << "numDifs: " << _numDifs << std::endl;
    //Loop over the difs and initialise each dif with its number and each connected ASU with a corresponding mapping file (vector element of vector with mapping file
    for (unsigned idif=0; idif < _numDifs; idif++) {
        //Now we assign to each DIF the number of ASUs and their type. This information will be properly
        //propagated through the program
        //Here we need to put in some knowledge (can we get this from an external file?)
        _asuInfoVec.push_back( std::vector< AsuInfo_t  > () );
        //Define the number of ASUs belonging to a DIF
        //Comment: At the moment we kow that we have only one ASU per DIF
        unsigned numASU(1);
        for(unsigned iasu=0;iasu<numASU;iasu++) {
             //Define the ASU type
	  if (iasu==0) _asuInfoVec.at(idif).push_back(make_pair("SKIROC", make_pair(16,0)));
	  //globalvariables::getEnabledChipsVec().size(),0)));
         }
        _difVec.push_back( Dif(idif) );
        _difVec.at(idif).init(_asuInfoVec.at(idif));
    }

   

}



void ExperimentalSetup::executeExperiment(std::vector<TFile*> aFileVec, int PlaneEventThreshold = 30) {

    if(_difVec.size() != aFileVec.size()) {
        std::cout << " Warning the number of dif files do not correspond to the number of difs in the experiment" << std::endl;
        std::cout << "Configured number of difs is " << _difVec.size() << std::endl;
        std::cout << "Configured number of dif files is " << aFileVec.size() << std::endl;
        std::cout << "Will limit the read files to the number of difs" << std::endl;
        std::cout << "Make sure that this is what you want" << std::endl;
    }

    cout<<_difVec.size()<<" "<<aFileVec.size()<<endl;

    //Read in the dif files
    if(_difVec.size() < aFileVec.size()) {
      for (unsigned ifile=0; ifile < aFileVec.size(); ifile++)
	_difVec.at(_difVec.size()-1).dataAnalysis(aFileVec.at(ifile),PlaneEventThreshold);
    }  else {
      for (unsigned idif=0; idif < _difVec.size(); idif++) _difVec.at(idif).dataAnalysis(aFileVec.at(idif),PlaneEventThreshold);
    }
   
}



/*
void ExperimentalSetup::executeExperiment() {
  //Run over the DIfs and analyse the data
  //This has to be parallelised 
  //i.e. the execution has to be dispatched to different CPU cores or different machines (=> future version)
  for (unsigned idif=0; idif < _numDifs; idif++) {
    _difVec.at(idif).dataAnalysis();
  }
}
*/

unsigned ExperimentalSetup::getNumberOfDifs() {
    return _numDifs;
}

Dif ExperimentalSetup::getDif(unsigned idif) {
  return _difVec.at(idif);
}


unsigned ExperimentalSetup::getNumberOfASUs(unsigned idif) {
    return _asuInfoVec.at(idif).size();
}

Mapping* ExperimentalSetup::getMapping(unsigned itype){
    if(itype <_aMappingPtrVec.size()) {
        return _aMappingPtrVec.at(itype);
    } else {
        std::cout << "ExperimentalSetup::getMapping: WARNING" << std::endl;
        std::cout << "Requested type " << itype << " doesn't exist" << std::endl;
        std::cout << "Only " << _aMappingPtrVec.size() << " mapping types are known" << std::endl;
        std::cout << "Will return NULL Pointer" << std::endl;
    }
    
    return nullptr;
}

unsigned ExperimentalSetup::getNumberofASICChannels(std::string chipType) {
    unsigned numchans(0);
    //Request the ASIC Map and return the number of channels for this chip type if possible
    ASICInfoMap_t::iterator asicinfomap_iter=_asicInfoMap.find(chipType);
    if (asicinfomap_iter != _asicInfoMap.end() ) {
        numchans=(*asicinfomap_iter).second.first;
    } else {
        std::cout << "ExperimentalSetup::getNumberofASICChannels: Chiptype " << chipType << " not defined!!!" << std::endl;
        std::cout << "Will return 0 channels" << std::endl;
    }
    return numchans;
}

unsigned ExperimentalSetup::getASICBufferDepth(std::string chipType) {
    unsigned numbufs(0);
    //Request the ASIC Map and return the buffer depth for this chip type if possible
    ASICInfoMap_t::iterator asicinfomap_iter=_asicInfoMap.find(chipType);
    if (asicinfomap_iter != _asicInfoMap.end() ) {
        numbufs=(*asicinfomap_iter).second.second;
    } else {
        std::cout << "ExperimentalSetup::getNumberofASICChannels: Chiptype " << chipType << " not defined!!!" << std::endl;
        std::cout << "Will return 0 buffer depth" << std::endl;
    }
    return numbufs;
}

unsigned ExperimentalSetup::getNumberofASICs(unsigned idif, unsigned iasu) {
  return _asuInfoVec.at(idif).at(iasu).second.first;
}

