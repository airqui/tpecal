//
//  ExperimentalSetup.h
//  cobana_project
//
//  Created by Roman Poeschl on 03/08/15.
//
//

#ifndef __cobana_project__ExperimentalSetup__
#define __cobana_project__ExperimentalSetup__
#define MYERR(msg) cerr << endl << "ERROR:" << __FILE__ << ":" << __LINE__ << ": " << msg << endl; exit(1)
#include <stdio.h>
#include <iostream>
#include <vector>
#include "TFile.h"
#include "Mapping.h"
#include "Dif.h"

using namespace std;

class ExperimentalSetup {
    
public:
    static ExperimentalSetup *getInstance();
    //initialisation of the conditions of the current run
    //Practically the method handles changes of conditions data
    //To do can/should be made more elegant in the future
    //At least the steering file should be read in here
    void setRunSetup(std::vector<std::string>);
    //Transfer the files for the difs used in the data taking
    //void addDifFileVec(std::vector<TFile*>);
    //Attention the experiment execution will have to be deferred to another class, otherwise no
    //parallel execution possbile!!!!
    void executeExperiment(std::vector<TFile*>);
    //Return a  given DIF
    Dif getDif(unsigned);    
    //A method to reset values and objects
    void reset();
    //Return the Mapping upon request
    Mapping* getMapping(unsigned);
    //return the number of difs in the setup
    unsigned getNumberOfDifs();
    //Return the information of a given chip type
    //All this is default information that needs to be checked against the external input
    //from a configuration if any. The default settings should allow to run the programme
    //in cases in which a proper config file doesn't exist (e.g. for test bench measurements)
    //Number of chips
    unsigned getNumberofASICs(unsigned, unsigned);
    //Number of channels
    unsigned getNumberofASICChannels(std::string);
    //Buffer depth of the chip (where else can one get this from)
    unsigned getASICBufferDepth(std::string);
private:
    //The ExperimentalSetup is a singleton to facilitate access to central information
    ExperimentalSetup();
    ~ExperimentalSetup();
    static void release();
    static ExperimentalSetup *theInstance;
    //A Vector that holds the mapping files;
    std::vector<std::string> _aFileNameVec;
    //A vector that holds pointer to the different mappings
    std::vector<Mapping*> _aMappingPtrVec;
    //Define the experimentalsetup here, further down the code we only make sanity checks
    unsigned _numDifs;
    std::vector<Dif> _difVec;
    //A double vector to the the number of ASUs connected to the DIF, the chips that are mounted on this ASU and the connection type of the ASU (this information will be used to exploit the input file with the mapping)
    typedef std::pair<std::string, std::pair<unsigned,unsigned> > AsuInfo_t;
    //typedef std::vector< std::vector<std::pair<std::string,unsigned> > > AsuInfoVec_t;
    typedef std::vector< std::vector<AsuInfo_t> > AsuInfoVec_t;
    AsuInfoVec_t _asuInfoVec;
    //Return the number of ASUs attached to a DIF
    unsigned getNumberOfASUs(unsigned);
    //We define some basic information about the used chips
    //The information handled during the data processing will be checkes agains this information for
    //consistency whereever possible
    //The map defines the chip type the associated number of chips and the bufferdepth (the latter information can not
    //be extracted from any configuration file so far!!!!
    typedef std::map<std::string,std::pair<unsigned,unsigned>> ASICInfoMap_t;
    ASICInfoMap_t _asicInfoMap;
};



#endif /* defined(__cobana_project__ExperimentalSetup__) */
