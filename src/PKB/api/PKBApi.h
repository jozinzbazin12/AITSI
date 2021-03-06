/* * PKBApi.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_API_PKBAPI_H_
#define SRC_PKB_API_PKBAPI_H_

#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

class PKBApi {
public:
	PKBApi();
	virtual ~PKBApi();

	bool modifies(int lineNumber, string varName);
	bool uses(int lineNumber, string varName);
	bool parent(int parentLine, int kidLine);
	bool parentStar(int parentLine, int kidLine);
	bool follows(int lineNum, int followerLineNum);
	bool followsStar(int lineNum, int followerLineNum);
	bool calls(int callerId, int calleeId);
	bool callsStar(int callerId, int calleeId);
	bool next(int lineNumber, int nextLineNumber);
	bool nextStar(int lineNumber, int nextLineNumber);

	int getLinesCount();
	vector<int> getAssignLines();
	map<int, string> getAllLines();
	vector<int> getCallLines();
	vector<int> getWhileLines();
	vector<int> getIfLines();
	map<int, vector<int> > getWhileBodyLines(); // w pierwszym parametrze nr tej linijki z napisem while, a w vectorze pozosta�e liniie while
	map<int, vector<int>> getIfBodyLines();
	vector<int> getProceduresLines();
	// jak b�d� potrzebowali stmt to chyba mog� sobie zrobic assign i while zamiast pobierac stmt
	string getLineString(int lineNumber);

	int getProcId(string name);
	int getProcId(int startLine);
	int getMaxProcId();
	int getProcStartLine(int id);
	int getProcStartLine(string name);
	vector<int> getProcedureBodyLines(int procId);
	set<int> getProcedureBodyLinesStar(int procId);
	string getProcName(int id);
};

#endif /* SRC_PKB_API_PKBAPI_H_ */
