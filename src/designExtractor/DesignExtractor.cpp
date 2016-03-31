/*
 * DesignExtractor.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() {
	PKB pkb = PKB::getInstance();
	Follows * follows = pkb.getFollows();
}

DesignExtractor::~DesignExtractor() {
	// TODO Auto-generated destructor stub
}

