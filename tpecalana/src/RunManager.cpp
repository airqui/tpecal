//
//  RunManager.cpp
//  cobana_project
//
//  Created by Roman Poeschl on 29/07/15.
//
//

#include "RunManager.h"

RunManager::RunManager(){
_difFileVec.clear();
} 
    
RunManager::~RunManager(){/*no op*/}



void  RunManager::init() {
}

void RunManager::registerDifFile(TFile* aFilePtr) {
  //Register the output file for each DIF of this run (May need to take care of the right ordering of the DIFs)
  _difFileVec.push_back(aFilePtr);
}

std::vector<TFile*> RunManager::getDifFileVec() {
  return _difFileVec;
}

void  RunManager::reset() {
_difFileVec.clear();
}
