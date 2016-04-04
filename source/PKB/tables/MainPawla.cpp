/*
 * MainPawla.cpp
 *
 *  Created on: 30 mar 2016
 *      Author: Pawel
 */

#include "../PKB.h"
#include "../api/PKBApi.h"

int main3() {
	PKB& pkb = PKB::getInstance();
	pkb.getModifies()->add(1 ,1);
	PKBApi api;

	pkb.getModifies()->writeAll();
	api.pkb.getModifies()->writeAll();
	return 0;
}

