/*
 * LinesTable.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_TABLES_LINESTABLE_H_
#define SRC_PKB_TABLES_LINESTABLE_H_

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class LinesTable {
public:
	LinesTable();
	virtual ~LinesTable();
	int getLinesCount();
	vector<int> getAssignLines();
	vector<int> getAllLines();
	vector<int> getCallLines();
	vector<int> getWhileLines();
	vector<int> getIfLines();
	map<int, vector<int> > getWhileHeaderLines(); // w pierwszym parametrze nr tej linijki z napisem while, a w vectorze pozosta³e liniie while
	map<int, vector<int>> getIfHeaderLines();
	// jak bêd¹ potrzebowali stmt to chyba mog¹ sobie zrobic assign i while zamiast pobierac stmt
	string getLineString(int lineNumber); // zwraca test który byl w linii o tym numerze
	void addWhileLine(int loopLine,int stmtLine);
	void addIfLine(int ifLine, int stmtLine);
	void addCallLine(int callLine);
	void addAssignLine(int assignLine);
	void addLine(string text);
	void writeAll();


private:
	int linesCount;
	map<int, string> lines; // tutaj po prostu dla kolejnych numerów linii ich tekst (mo¿na na vector zamienic)
	vector<int> assignLines;
	vector<int> callLines;
	map<int, vector<int>> whileLines;
	map<int, vector<int>> ifLines;

};

#endif /* SRC_PKB_TABLES_LINESTABLE_H_ */
