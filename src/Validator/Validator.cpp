/*
 * Validator.cpp
 *
 *  Created on: 2 kwi 2016
 *      Author: Krzysiek
 */

#include "Validator.h"

Validator::Validator() {
	// TODO Auto-generated constructor stub
	init();
}

Validator::~Validator() {
	// TODO Auto-generated destructor stub
}

void Validator::init() {
	addEntity("program");
	addEntity("procedure");
	addEntity("stmtLst");
	addEntity("stmt");
	addEntity("assign");
	addEntity("call");
	addEntity("while");
	addEntity("if");
	addEntity("plus");
	addEntity("minus");
	addEntity("times");
	addEntity("variable");
	addEntity("constant");
	addEntity("prog_line");

	addAttribute("procedure", "procname", "NAME");
	addAttribute("variable", "varname", "NAME");
	addAttribute("constant", "value", "INTEGER");
	addAttribute("stmt", "stmt#", "INTEGER");
	addAttribute("procedure", "procname", "NAME");

	addRelationship("modifies", 2, "procedure", "variable");
	addRelationship("modifies", 2, "stmt", "variable");
	addRelationship("uses", 2, "procedure", "variable");
	addRelationship("uses", 2, "stmt", "variable");
	addRelationship("calls", 2, "procedure", "procedure");
	addRelationship("calls*", 2, "procedure", "procedure");
	addRelationship("parent", 2, "stmt", "stmt");
	addRelationship("parent*", 2, "stmt", "stmt");
	addRelationship("follows", 2, "stmt", "stmt");
	addRelationship("follows*", 2, "stmt", "stmt");
	addRelationship("next", 2, "prog_line", "prog_line");
	addRelationship("next*", 2, "prog_line", "prog_line");
	addRelationship("affects", 2, "assign", "assign");
	addRelationship("affects*", 2, "assign", "assign");
}

void Validator::addEntity(string entity) {
	entityTab.push_back(entity);
}

void Validator::showEntities() {
	cout << "Entities:" << endl;
	for (vector<string>::iterator it = entityTab.begin(); it != entityTab.end();
			++it) {
		cout << *it << endl;
	}
}

void Validator::addAttribute(string entity, string attribute, string type) {
	vector<string> attributeCellTab;
	attributeCellTab.push_back(entity);
	attributeCellTab.push_back(attribute);
	attributeCellTab.push_back(type);
	attributeTab.push_back(attributeCellTab);
}

void Validator::showAttributes() {
	cout << "Attributes:" << endl;
	for (vector<vector<string> >::iterator it = attributeTab.begin(); it != attributeTab.end(); ++it) {
		for (vector<string>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			cout << *it2 << ' ';
		}
		cout << endl;
	}
}

void Validator::addRelationship(string relationship, int argsAmount, string type1, string type2) {
	vector<string> relationshipCellTab;
	relationshipCellTab.push_back(relationship);
	relationshipCellTab.push_back(to_string(argsAmount));
	relationshipCellTab.push_back(type1);
	relationshipCellTab.push_back(type2);
	relationshipTab.push_back(relationshipCellTab);
}

void Validator::showRelationships() {
	cout << "Relationships:" << endl;
	for (vector<vector<string> >::iterator it = relationshipTab.begin(); it != relationshipTab.end(); ++it) {
		for (vector<string>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			cout << *it2 << ' ';
		}
		cout << endl;
	}
}

bool Validator::checkSelect(string query) {
	vector<string> v;
	split(query, ' ', v);
	int selects = 0;
	for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
		if(*it == "select")
			selects++;
	if(selects == 1) {
		return true;
	}
	return false;
}

bool Validator::checkEntities(vector<Field> declarations) {
	vector<string> symbols;
	for (vector<Field>::iterator it = declarations.begin(); it != declarations.end(); ++it) {
		if(find(entityTab.begin(), entityTab.end(), it->getType()) != entityTab.end())
				continue;
			else
				return false;
		symbols.push_back(it->getValue());
	}
	vector<string> temp;
	for (vector<string>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
		if(find(temp.begin(), temp.end(), *it) != temp.end()) {
			return false;
		} else {
			temp.push_back(*it);
		}
	}
	queryDeclarations = declarations;
	return true;
}

bool Validator::checkRelationship(string relationship) {
	int leftBrackets = 0, rightBrackets = 0, stars = 0;
	for(char& c : relationship) {
		if(c == '(')
			leftBrackets++;
		else if(c == ')')
			rightBrackets++;
		else if(c == '*')
			stars++;
	}
	if(leftBrackets != 1 || rightBrackets != 1 || stars > 1)
		return false;
	vector<string> v1;
	split(relationship, '(', v1);
	string rel = v1[0];
	string attributes = v1[1];
	v1.clear();
	split(attributes, ')', v1);
	attributes = v1[0];
	v1.clear();
	bool validRel = false;
	vector<int> relIdxs;
	for(size_t i=0; i < relationshipTab.size(); i++) {
		if(rel == relationshipTab[i][0]) {
			validRel = true;
			relIdxs.push_back(i);
		}
	}
	if(!validRel)
		return false;
	split(attributes, ',', v1);
	if(v1.size() != (size_t) stoi(relationshipTab[relIdxs[0]][1]))
		return false;
	string *attributesArray = new string[v1.size()];
	bool *attrValidatorsArray = new bool[v1.size()];
	for(size_t i = 0; i < v1.size(); i++) {
		attributesArray[i] = v1[i];
		attrValidatorsArray[i] = false;
	}
	for(size_t i=0; i < relIdxs.size(); i++) {
		for(size_t j = 0; j < v1.size(); j++) {
			if(attributesArray[j] == relationshipTab[relIdxs[i]][j+2])
				attrValidatorsArray[j] = true;
		}
		bool wrongAttr = false;
		for(size_t j = 0; j < v1.size(); j++) {
			if(attrValidatorsArray[j] == false) {
				wrongAttr = true;
				break;
			}
		}
		if(!wrongAttr)
			break;
		if(wrongAttr && i == relIdxs.size() - 1)
			return false;
	}
	return true;
}

bool Validator::checkAttribute(string attribute) {    //MUSI JESZCZE JAKOS SPRAWDZAC I POBIERAC TYP ATRYBUTU
    int dots = 0;
    for(char& c : attribute) {
        if(c == '.')
            dots++;
    }
    if(dots >1)
        return false;
    vector<string> v1;
    split(attribute, '.', v1);
    string entity = v1[0], type = v1[1];
    bool validEntity = false, validType = false;
    int entityIndex;
    for(size_t i=0; i < attributeTab.size(); i++) {
        if(entity == attributeTab[i][0]) {
            validEntity = true;
            entityIndex = i;
            break;
        }
    }
    if(!validEntity)
        return false;
    if(type == attributeTab[entityIndex][1])
        validType = true;
    if(!validType)
            return false;
    return true;
}
