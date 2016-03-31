/*
 * DesignExtractor.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_
#define SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_

#include "../PKB/PKB.h"

class DesignExtractor {
public:
	DesignExtractor();
	virtual ~DesignExtractor();
	void FillFollowers();
};

#endif /* SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_ */
