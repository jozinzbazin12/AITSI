/*
 * VarTable.cpp
 *
 *  Created on: 27 mar 2016
 *      Author: Pawel
 */

#include "VarTable.h"

VarTable::VarTable() {
	// TODO Auto-generated constructor stub
	varId = 0;
}

VarTable::~VarTable() {
	// TODO Auto-generated destructor stub
}

int VarTable::addVar(string name){
	variables.push_back(name);
	varId++;
	return varId - 1;
}

int VarTable::getVarId(string name){
	for(int i=0; i<variables.size(); i++){
		if(variables.at(i).compare(name) == 0){
			return i;
		}
	}
	return NULL;
}

string VarTable::getVarName(int id){
	return variables.at(id);
}

