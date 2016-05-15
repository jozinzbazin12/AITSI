/*
 * ProcTable.cpp
 *
 *  Created on: 27 mar 2016
 *      Author: Pawel
 */

#include "ProcTable.h"
#include <iostream>
ProcTable::ProcTable() {
	procId = 0;
}

ProcTable::~ProcTable() {
	// TODO Auto-generated destructor stub
}

int ProcTable::addProc(string name, int startLine){
	procedures.push_back(name);
	proceduresStartLines.push_back(startLine);
	procId++;
	return procId - 1;
}

int ProcTable::getProcId(string name){
	for(unsigned i=0; i<procedures.size(); i++){
		if(procedures[i].compare(name) == 0){
			return i;
		}
	}
	return -1;
}

int ProcTable::getProcId(int startLine){
	for(unsigned i=0; i<proceduresStartLines.size(); i++){
		if(proceduresStartLines[i] == startLine){
			return i;
		}
	}
	return -1;
}

int ProcTable::getMaxProcId(){
	return procId - 1;
}

int ProcTable::getProcStartLine(int id){
	return proceduresStartLines[id];
}

int ProcTable::getProcStartLine(string name){
	return proceduresStartLines[getProcId(name)];
}

string ProcTable::getProcName(int id){
	return procedures[id];
}
void ProcTable::writeProcLines(){
	for (vector<string>::iterator iter = procedures.begin();
				iter != procedures.end(); ++iter) {
			cout << (*iter)<< " ";
		}
	cout<<endl;
	for (vector<int>::iterator iter = proceduresStartLines.begin();
				iter != proceduresStartLines.end(); ++iter) {
			cout << (*iter)<< " ";
		}
	cout<<endl;
}

vector<int> ProcTable::getProceduresLines(){
	return proceduresStartLines;
}
