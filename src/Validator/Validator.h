/*
 * Validator.h
 *
 *  Created on: 2 kwi 2016
 *      Author: Krzysiek
 */
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "../PQL/Field.h"
#include <algorithm>
#include "globalne.h"

using namespace std;

#ifndef VALIDATOR_H_
#define VALIDATOR_H_

class Validator {
public:
	Validator();
	virtual ~Validator();
	void init();
	void addEntity(string entity);
	void showEntities();
	void addAttribute(string entity, string attribute, string type);
	void showAttributes();
	void addRelationship(string relationship, int argsAmount, string type1, string type2);
	void showRelationships();
	bool checkSelect(string query);
	bool checkEntities(vector<Field> declarations);
	bool checkRelationship(string relationship);
	bool checkAttribute(string attribute);
private:
	vector<string> entityTab;
	vector<vector<string> > attributeTab;
	vector<vector<string> > relationshipTab;
	vector<Field> queryDeclarations;
};

#endif /* VALIDATOR_H_ */
