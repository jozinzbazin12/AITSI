/*
 * PKB.cpp
 *
 *  Created on: 31 mar 2016
 *      Author: Pawel
 */

#include "PKB.h"

PKB& PKB::getInstance(){
	static PKB* pkb = new PKB();
	return *pkb;
}

PKB::~PKB() {
	delete modifies;
	delete uses;
	delete parent;
	delete follows;
	delete varTable;
}

