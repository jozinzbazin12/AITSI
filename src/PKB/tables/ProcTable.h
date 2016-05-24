/*
 * ProcTable.h
 *
 *  Created on: 27 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_TABLES_PROCTABLE_H_
#define SRC_PKB_TABLES_PROCTABLE_H_

#include<string>
#include<vector>
#include<map>
#include<set>
#include "../relations/Calls.h"
using namespace std;

class ProcTable {
public:
	ProcTable();
	virtual ~ProcTable();
	virtual int addProc(string name, int startLine);
	virtual int getProcId(string name);
	virtual int getProcId(int startLine);
	virtual int getMaxProcId();
	virtual int getProcStartLine(int id);
	virtual int getProcStartLine(string name);
	virtual string getProcName(int id);
	virtual void writeProcLines();
	virtual void writeProcBodyLines();
	virtual vector<int> getProceduresLines();
	virtual map<int, vector<int>> getProceduresBodyLines();
	virtual vector<int> getProcedureBodyLines(int procId);
	virtual set<int> getProcedureBodyLinesStar(int procId, Calls * calls);
	virtual void addProcBodyLine(int procStartLine, int stmtLine);

private:
	vector<string> procedures;
	vector<int> proceduresStartLines;
	map<int, vector<int>> proceduresLines;
	int procId;

	set<int> procedureRecur(int procId, Calls * calls, set<int> procedureLines, bool visited[]);

};

#endif /* SRC_TABLES_PROCTABLE_H_ */
