//
//  RunManager.h
//  cobana_project
//
//  Created by Roman Poeschl on 29/07/15.
//
//

#ifndef __cobana_project__RunManager__
#define __cobana_project__RunManager__

#include <stdio.h>
#include <TFile.h>
#include <vector>

using namespace std;

class RunManager {
    
public:
    RunManager();
    ~RunManager();
    void init();
    //A method to register DIF Files
    void registerDifFile(TFile*);    
    //Return the vector with the DIF Files
    std::vector<TFile*> getDifFileVec();
    //A method to reset everything;
    void reset();

 private:
    //A vector that holds the files for each DIF
    std::vector<TFile*> _difFileVec;

};

#endif /* defined(__cobana_project__RunManager__) */
