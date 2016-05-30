/*
* ProcTable.cpp
*
*  Created on: 27 mar 2016
*      Author: Pawel
*/

#include "ProcTable.h"
#include <iostream>
ProcTable::ProcTable() {
	procId = 0;
}

ProcTable::~ProcTable() {
	// TODO Auto-generated destructor stub
}

int ProcTable::addProc(string name, int startLine) {
	procedures.push_back(name);
	proceduresStartLines.push_back(startLine);
	procId++;
	return procId - 1;
}
void ProcTable::addProcBodyLine(int procId, int stmtLine) {
	proceduresLines[procId].push_back(stmtLine);
}
map<int, vector<int>> ProcTable::getProceduresBodyLines() {
	return proceduresLines;
}

int ProcTable::getProcId(string name) {
	for (unsigned i = 0; i < procedures.size(); i++) {
		if (procedures[i].compare(name) == 0) {
			return i;
		}
	}
	return -1;
}

int ProcTable::getProcId(int startLine) {
	for (unsigned i = 0; i < proceduresStartLines.size(); i++) {
		if (proceduresStartLines[i] == startLine) {
			return i;
		}
	}
	return -1;
}

int ProcTable::getMaxProcId() {
	return procId - 1;
}

int ProcTable::getProcStartLine(int id) {
	return proceduresStartLines[id];
}

int ProcTable::getProcStartLine(string name) {
	return proceduresStartLines[getProcId(name)];
}

string ProcTable::getProcName(int id) {
	return procedures[id];
}
void ProcTable::writeProcLines() {
	for (vector<string>::iterator iter = procedures.begin();
		iter != procedures.end(); ++iter) {
		cout << (*iter) << " ";
	}
	cout << endl;
	for (vector<int>::iterator iter = proceduresStartLines.begin();
		iter != proceduresStartLines.end(); ++iter) {
		cout << (*iter) << " ";
	}
	cout << endl;
}
void ProcTable::writeProcBodyLines() {
	for (map<int, vector<int> >::iterator iter = proceduresLines.begin();
		iter != proceduresLines.end(); ++iter) {
		vector<int> tempVec = (*iter).second;
		int key = (*iter).first;
		cout << key << " :";
		for (unsigned i = 0; i < tempVec.size(); i++) {
			cout << " " << tempVec[i];
		}
		cout << endl;
	}

}

vector<int> ProcTable::getProceduresLines() {
	return proceduresStartLines;
}

vector<int> ProcTable::getProcedureBodyLines(int procId) {  // JAK TO ZABEZPIECZYC PRZED WYJEBANIEM JAK NIE BEDZIE TAKIEGO KLUCZA ?!!??!
	if (proceduresLines.count(procId) > 0)
		return proceduresLines[procId];

	vector<int> empty;
	return empty;
}

set<int> ProcTable::getProcedureBodyLinesStar(int procId, Calls * calls) {
	int const sooBigNumberCuzVisual = 653000;
	bool visited[sooBigNumberCuzVisual];
	for (int i = 0; i<procId; i++)
		visited[i] = false;
	set<int> procedureLines;
	procedureLines = procedureRecur(procId, calls, procedureLines, visited);
	return procedureLines;
}

set<int> ProcTable::procedureRecur(int procId, Calls * calls, set<int> procedureLines, bool visited[]) {
	vector<int> temp = getProcedureBodyLines(procId);
	for (int i : temp) {
		procedureLines.insert(i);
	}

	for (int i : (calls->getCallees(procId))) {
		if (!visited[i]) {
			visited[i] = true;
			procedureLines = procedureRecur(i, calls, procedureLines, visited);
		}
	}
	return procedureLines;
}
