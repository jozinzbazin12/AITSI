/*
 * ProcTable.cpp
 *
 *  Created on: 27 mar 2016
 *      Author: Pawel
 */

#include "ProcTable.h"

ProcTable::ProcTable() {
	procId = 0;
}

ProcTable::~ProcTable() {
	// TODO Auto-generated destructor stub
}

int ProcTable::addProc(string name){
	procedures.push_back(name);
	procId++;
	return procId - 1;
}

int ProcTable::getProcId(string name){
	for( int i=0; i<procedures.size(); i++){
		if(procedures.at(i).compare(name) == 0){
			return i;
		}
	}
	return NULL;
}

string ProcTable::getProcName(int id){
	return procedures.at(id);
}

