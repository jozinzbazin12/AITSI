/*
 * PKB.cpp
 *
 *  Created on: 31 mar 2016
 *      Author: Pawel
 */

#include "PKB.h"

PKB::~PKB() {
	delete modifies;
	delete uses;
	delete parent;
	delete follows;
	delete varTable;
}

