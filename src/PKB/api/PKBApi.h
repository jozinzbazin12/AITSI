/* * PKBApi.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_API_PKBAPI_H_
#define SRC_PKB_API_PKBAPI_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

class PKBApi {
public:
	PKBApi();
	virtual ~PKBApi();

	bool modifies(int lineNumber, string varName);
	bool uses(int lineNumber, string varName);
	bool parent(int parentLine, int kidLine);
	bool follows(int lineNum, int followerLineNum);

	int getLinesCount();
	vector<int> getAssignLines();
	map<int, string> getAllLines();
	vector<int> getCallLines();
	vector<int> getWhileLines();
	vector<int> getIfLines();
	map<int, vector<int> > getWhileBodyLines(); // w pierwszym parametrze nr tej linijki z napisem while, a w vectorze pozosta³e liniie while
	map<int, vector<int>> getIfBodyLines();
	// jak bêd¹ potrzebowali stmt to chyba mog¹ sobie zrobic assign i while zamiast pobierac stmt
	string getLineString(int lineNumber);
};

#endif /* SRC_PKB_API_PKBAPI_H_ */
