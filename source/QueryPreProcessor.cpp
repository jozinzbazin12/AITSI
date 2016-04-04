/*
 * QueryPreProcessor.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Popek
 */

#include "QueryPreProcessor.h"

#include "ParserPQL/MatcherPQL.h"

using namespace std;

QueryPreProcessor::QueryPreProcessor() {
	matcher = new MatcherPQL();
	exc = new Exceptions();
}

QueryPreProcessor::~QueryPreProcessor() {
	delete matcher;
	delete exc;
}

void QueryPreProcessor::parseQuery(string query) {
	vector<string> first_split = split(query, ';'); //
	vector<string> result_part;
	vector<string> query_part;

	MatcherPQL* m = new MatcherPQL();
	for(size_t i = 0 ; i < first_split.size() ; i ++)
	{
		if(!m->checkTokens(first_split[i], "select"))
			result_part.push_back(first_split[i]);
		else
			query_part.push_back(first_split[i]);
	}
	delete m;

	if(query_part.size() == 0) {
		exc->throwException();
	}

	//writeVector(result_part);

	//cout << m->checkProcName(result_part[0]) << endl;
	//cout << m->checkVarName(result_part[0]) << endl;
	//cout << m->checkStmt_(result_part[0]) << endl;
	//cout << m->checkAll(result_part[0]) << endl;

	vector<Field> fields = makeFields(result_part);
	setFields(fields);

	//writeVector(query_part);

	makeTree(query_part);
	/*
	for (int j = 0; j < fields.size(); j++) {
		cout << fields[j].getType() << " " << fields[j].getValue() << endl;
	}*/
}
