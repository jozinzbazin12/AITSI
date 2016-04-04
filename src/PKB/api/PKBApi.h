/* * PKBApi.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_API_PKBAPI_H_
#define SRC_PKB_API_PKBAPI_H_

#include "../PKB.h"

class PKBApi {
public:
	PKBApi();
	virtual ~PKBApi();

	bool modifies(int lineNumber, string varName);
	bool uses(int lineNumber, string varName);
	bool parent(int parentLine, int kidLine);
	bool follows(int lineNum, int followerLineNum);
	vector<int> getAssignLines();
	map<int, vector<int> > getWhileLines(); // w pierwszym parametrze nr tej linijki z napisem while, a w vectorze pozosta³e liniie while
	// jak bêd¹ potrzebowali stmt to chyba mog¹ sobie zrobic assign i while zamiast pobierac stmt
	string getLineString(int lineNumber);

private:
	PKB& pkb = PKB::getInstance();
};

#endif /* SRC_PKB_API_PKBAPI_H_ */
