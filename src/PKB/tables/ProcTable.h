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
	virtual vector<int> getProceduresBodyLines(int procStartLine);
	virtual void addProcBodyLine(int procStartLine, int stmtLine);


private:
	vector<string> procedures;
	vector<int> proceduresStartLines;
	map<int, vector<int>> proceduresLines;
	int procId;
};

#endif /* SRC_TABLES_PROCTABLE_H_ */
