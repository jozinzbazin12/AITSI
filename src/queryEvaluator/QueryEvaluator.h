/*
 * queryEvaluator.h
 *
 *  Created on: 19 kwi 2016
 *      Author: Popek
 */

#ifndef SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
#define SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
#include "PQLNode.h"
#include <iterator>

namespace std {

class QueryEvaluator {
public:
	QueryEvaluator();
	virtual ~QueryEvaluator();
	string getResult(PQLTree* tree);
	string getModifiesResult();
	constexpr unsigned int str2int(const char* str, int h = 0);
	string getUsesResult();
	string getFollowsResult();
	string getParentResult();
};

} /* namespace std */

#endif /* SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_ */
