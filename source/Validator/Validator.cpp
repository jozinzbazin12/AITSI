/*
* Validator.cpp
*
*  Created on: 2 kwi 2016
*      Author: Krzysiek
*/

#include "Validator.h"

/*
Meta symbols:

a* -- repetition of a, 0 or more times
a+ -- repetition of a, 1 or more times
a | b -- a or b
brackets ( and ) are used for grouping

Lexical tokens:

LETTER : A-Z | a-z -- capital or small letter
DIGIT : 0-9
NAME : LETTER (LETTER | DIGIT)* -- procedure names and variables are strings of letters, and digits, starting with a letter
INTEGER : DIGIT+ -- constants are sequences of digits

Grammar rules:

program : procedure+
procedure : ‘procedure’ proc_name ‘{‘ stmtLst ‘}’
procedure : stmtLst
stmtLst : stmt+
stmt : call | while | if | assign
call : ‘call’ proc_name ‘;’
while : ‘while’ var_name ‘{‘ stmtLst ‘}’
while: variable stmtLst
if : ‘if’ var_name ‘then’ ‘{‘ stmtLst ‘}’ ‘else’ ‘{‘ stmtLst ‘}’
if : variable stmtLst stmtLst
assign : var_name ‘=’ expr ‘;’
assign : variable expr
expr : expr ‘+’ term | expr ‘-’ term | term
expr : plus | minus | times | ref
plus : expr expr
minus : expr expr
times : expr expr
ref : variable | constant
term : term ‘*’ factor | factor
factor : var_name | const_value | ‘(’ expr ‘)’
var_name : NAME
proc_name : NAME
const_value : INTEGER

Attributes and attribute value types:

procedure.procName, call.procName, variable.varName : NAME
constant.value : INTEGER
stmt.stmt# : INTEGER


Program design entities:

program, procedure
stmtLst, stmt, assign, call, while, if
plus, minus, times
variable, constant
prog_line

Attributes and attribute value types:

procedure.procName, variable.varName : NAME
constant.value : INTEGER
stmt.stmt# : INTEGER (numbers assigned to statements for the purpose of reference)
call.procName : NAME

Program design entity relationships:

Modifies (procedure, variable)
Modifies (stmt, variable)
Uses (procedure, variable)
Uses (stmt, variable)
Calls (procedure 1, procedure 2)
Calls* (procedure 1, procedure 2)
Parent (stmt 1, stmt 2)
Parent* (stmt 1, stmt 2)
Follows (stmt 1, stmt 2)
Follows* (stmt 1, stmt 2)
Next (prog_line 1, prog_line 2)
Next* (prog_line 1, prog_line 2)
Affects (assign 1, assign2)
Affects* (assign 1, assign2)
*/

/*
* 	TODO:
* 	Parent i Follows z liczbami naturalnymi dodatnimi po obu stronach
* 	Calls ze stringami po obu stronach
* 	Modifies i Uses ze stringami po obu stronach lub liczba naturala dodatnia lewo stringiem prawo
* 	Next z liczbami naturalnymi dodatnimi po obu stronach
* 	W Next moga byc takie same zmienne obie
* 	Affects liczbami naturalnymi dodatnimi po obu stronach
* 	Te stringi to w ciapkach musza byc
* 	Modifies (_, “x”’) and Uses (_, “x”) are not allowed, w reszcie moze byc _ po obu stronach
* 	jakies te wzory z tymi _ ale tego juz raczej nie bede sprawdzal
*/

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
	addAttribute("call", "procname", "NAME");

	addRelationship("modifies", 2, "procedure", "variable");
	addRelationship("modifies", 2, "stmt", "variable");
	addRelationship("modifies", 2, "assign", "variable");
	addRelationship("modifies", 2, "call", "variable");
	addRelationship("modifies", 2, "while", "variable");
	addRelationship("modifies", 2, "if", "variable");
	addRelationship("modifies", 2, "prog_line", "variable");
	addRelationship("uses", 2, "procedure", "variable");
	addRelationship("uses", 2, "stmt", "variable");
	addRelationship("uses", 2, "assign", "variable");
	addRelationship("uses", 2, "call", "variable");
	addRelationship("uses", 2, "while", "variable");
	addRelationship("uses", 2, "if", "variable");
	addRelationship("uses", 2, "prog_line", "variable");
	addRelationship("calls", 2, "procedure", "procedure");
	addRelationship("calls*", 2, "procedure", "procedure");
	addRelationship("parent", 2, "stmt", "stmt");
	addRelationship("parent", 2, "stmt", "prog_line");
	addRelationship("parent", 2, "while", "prog_line");
	addRelationship("parent", 2, "if", "prog_line");
	addRelationship("parent", 2, "prog_line", "prog_line");
	addRelationship("parent", 2, "prog_line", "stmt");
	addRelationship("parent", 2, "prog_line", "assign");
	addRelationship("parent", 2, "prog_line", "call");
	addRelationship("parent", 2, "prog_line", "while");
	addRelationship("parent", 2, "prog_line", "if");
	addRelationship("parent", 2, "while", "stmt");
	addRelationship("parent", 2, "if", "stmt");
	addRelationship("parent", 2, "stmt", "assign");
	addRelationship("parent", 2, "stmt", "call");
	addRelationship("parent", 2, "stmt", "while");
	addRelationship("parent", 2, "stmt", "if");
	addRelationship("parent", 2, "while", "while");
	addRelationship("parent", 2, "if", "if");
	addRelationship("parent", 2, "while", "assign");
	addRelationship("parent", 2, "if", "assign");
	addRelationship("parent", 2, "while", "call");
	addRelationship("parent", 2, "if", "call");
	addRelationship("parent", 2, "if", "while");
	addRelationship("parent", 2, "while", "if");
	addRelationship("parent*", 2, "stmt", "stmt");
	addRelationship("parent*", 2, "while", "stmt");
	addRelationship("parent*", 2, "if", "stmt");
	addRelationship("parent*", 2, "stmt", "assign");
	addRelationship("parent*", 2, "stmt", "call");
	addRelationship("parent*", 2, "stmt", "while");
	addRelationship("parent*", 2, "stmt", "if");
	addRelationship("parent*", 2, "while", "while");
	addRelationship("parent*", 2, "if", "if");
	addRelationship("parent*", 2, "while", "assign");
	addRelationship("parent*", 2, "if", "assign");
	addRelationship("parent*", 2, "while", "call");
	addRelationship("parent*", 2, "if", "call");
	addRelationship("parent*", 2, "if", "while");
	addRelationship("parent*", 2, "while", "if");
	addRelationship("follows", 2, "stmt", "stmt");
	addRelationship("follows", 2, "stmt", "prog_line");
	addRelationship("follows", 2, "while", "prog_line");
	addRelationship("follows", 2, "if", "prog_line");
	addRelationship("follows", 2, "call", "prog_line");
	addRelationship("follows", 2, "assign", "prog_line");
	addRelationship("follows", 2, "prog_line", "prog_line");
	addRelationship("follows", 2, "prog_line", "stmt");
	addRelationship("follows", 2, "prog_line", "assign");
	addRelationship("follows", 2, "prog_line", "call");
	addRelationship("follows", 2, "prog_line", "while");
	addRelationship("follows", 2, "prog_line", "if");
	addRelationship("follows", 2, "assign", "stmt");
	addRelationship("follows", 2, "call", "stmt");
	addRelationship("follows", 2, "while", "stmt");
	addRelationship("follows", 2, "if", "stmt");
	addRelationship("follows", 2, "stmt", "assign");
	addRelationship("follows", 2, "stmt", "call");
	addRelationship("follows", 2, "stmt", "while");
	addRelationship("follows", 2, "stmt", "if");
	addRelationship("follows", 2, "assign", "assign");
	addRelationship("follows", 2, "call", "call");
	addRelationship("follows", 2, "while", "while");
	addRelationship("follows", 2, "if", "if");
	addRelationship("follows", 2, "assign", "call");
	addRelationship("follows", 2, "assign", "while");
	addRelationship("follows", 2, "assign", "if");
	addRelationship("follows", 2, "call", "assign");
	addRelationship("follows", 2, "while", "assign");
	addRelationship("follows", 2, "if", "assign");
	addRelationship("follows", 2, "call", "assign");
	addRelationship("follows", 2, "call", "while");
	addRelationship("follows", 2, "call", "if");
	addRelationship("follows", 2, "assign", "call");
	addRelationship("follows", 2, "while", "call");
	addRelationship("follows", 2, "if", "call");
	addRelationship("follows", 2, "assign", "while");
	addRelationship("follows", 2, "call", "while");
	addRelationship("follows", 2, "if", "while");
	addRelationship("follows", 2, "while", "if");
	addRelationship("follows*", 2, "stmt", "stmt");
	addRelationship("follows*", 2, "stmt", "prog_line");
	addRelationship("follows*", 2, "while", "prog_line");
	addRelationship("follows*", 2, "if", "prog_line");
	addRelationship("follows*", 2, "call", "prog_line");
	addRelationship("follows*", 2, "assign", "prog_line");
	addRelationship("follows*", 2, "prog_line", "prog_line");
	addRelationship("follows*", 2, "prog_line", "stmt");
	addRelationship("follows*", 2, "prog_line", "assign");
	addRelationship("follows*", 2, "prog_line", "call");
	addRelationship("follows*", 2, "prog_line", "while");
	addRelationship("follows*", 2, "prog_line", "if");
	addRelationship("follows*", 2, "assign", "stmt");
	addRelationship("follows*", 2, "call", "stmt");
	addRelationship("follows*", 2, "while", "stmt");
	addRelationship("follows*", 2, "if", "stmt");
	addRelationship("follows*", 2, "stmt", "assign");
	addRelationship("follows*", 2, "stmt", "call");
	addRelationship("follows*", 2, "stmt", "while");
	addRelationship("follows*", 2, "stmt", "if");
	addRelationship("follows*", 2, "assign", "assign");
	addRelationship("follows*", 2, "call", "call");
	addRelationship("follows*", 2, "while", "while");
	addRelationship("follows*", 2, "if", "if");
	addRelationship("follows*", 2, "assign", "call");
	addRelationship("follows*", 2, "assign", "while");
	addRelationship("follows*", 2, "assign", "if");
	addRelationship("follows*", 2, "call", "assign");
	addRelationship("follows*", 2, "while", "assign");
	addRelationship("follows*", 2, "if", "assign");
	addRelationship("follows*", 2, "call", "assign");
	addRelationship("follows*", 2, "call", "while");
	addRelationship("follows*", 2, "call", "if");
	addRelationship("follows*", 2, "assign", "call");
	addRelationship("follows*", 2, "while", "call");
	addRelationship("follows*", 2, "if", "call");
	addRelationship("follows*", 2, "assign", "while");
	addRelationship("follows*", 2, "call", "while");
	addRelationship("follows*", 2, "if", "while");
	addRelationship("follows*", 2, "while", "if");
	addRelationship("next", 2, "prog_line", "prog_line");
	addRelationship("next*", 2, "prog_line", "prog_line");
	addRelationship("affects", 2, "assign", "assign");
	addRelationship("affects", 2, "prog_line", "assign");
	addRelationship("affects", 2, "assign", "prog_line");
	addRelationship("affects", 2, "prog_line", "prog_line");
	addRelationship("affects*", 2, "assign", "assign");
	addRelationship("affects*", 2, "prog_line", "assign");
	addRelationship("affects8", 2, "assign", "prog_line");
	addRelationship("affects*", 2, "prog_line", "prog_line");
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
		if (*it == "select")
			selects++;
	if (selects == 1) {
		return true;
	}
	return false;
}

bool Validator::checkEntities(vector<Field> declarations) {
	vector<string> symbols;
	for (vector<Field>::iterator it = declarations.begin(); it != declarations.end(); ++it) {
		if (it->getType() == "" || it->getValue() == "")
			return false;
		if (find(entityTab.begin(), entityTab.end(), it->getType()) != entityTab.end())
			continue;
		else
			return false;
		symbols.push_back(it->getValue());
	}
	vector<string> temp;
	for (vector<string>::iterator it = symbols.begin(); it != symbols.end(); ++it) {
		if (find(temp.begin(), temp.end(), *it) != temp.end()) {
			return false;
		}
		else {
			temp.push_back(*it);
		}
	}
	queryDeclarations = declarations;
	return true;
}

bool Validator::checkRelationship(string relationship) {
	int leftBrackets = 0, rightBrackets = 0, stars = 0;
	for (char& c : relationship) {
		if (c == '(')
			leftBrackets++;
		else if (c == ')')
			rightBrackets++;
		else if (c == '*')
			stars++;
	}
	if (leftBrackets != 1 || rightBrackets != 1 || stars > 1)
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
	for (size_t i = 0; i < relationshipTab.size(); i++) {
		if (rel == relationshipTab[i][0]) {
			validRel = true;
			relIdxs.push_back(i);
		}
	}
	if (!validRel)
		return false;
	split(attributes, ',', v1);
	if (v1.size() != (size_t)stoi(relationshipTab[relIdxs[0]][1]))
		return false;
	string *attributesArray = new string[v1.size()];
	bool *attrValidatorsArray = new bool[v1.size()];
	for (size_t i = 0; i < v1.size(); i++) {
		attributesArray[i] = v1[i];
		attrValidatorsArray[i] = false;
	}
	for (size_t i = 0; i < relIdxs.size(); i++) {
		for (size_t j = 0; j < v1.size(); j++) {
			if (attributesArray[j] == relationshipTab[relIdxs[i]][j + 2])
				attrValidatorsArray[j] = true;
		}
		bool wrongAttr = false;
		for (size_t j = 0; j < v1.size(); j++) {
			if (attrValidatorsArray[j] == false) {
				wrongAttr = true;
				break;
			}
		}
		if (!wrongAttr)
			break;
		if (wrongAttr && i == relIdxs.size() - 1)
			return false;
	}
	return true;
}

bool Validator::checkRelationship2(string relationship) {
	int leftBrackets = 0, rightBrackets = 0, stars = 0;
	for (char& c : relationship) {
		if (c == '(')
			leftBrackets++;
		else if (c == ')')
			rightBrackets++;
		else if (c == '*')
			stars++;
	}
	if (leftBrackets != 1 || rightBrackets != 1 || stars > 1)
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
	for (size_t i = 0; i < relationshipTab.size(); i++) {
		if (rel == relationshipTab[i][0]) {
			validRel = true;
			relIdxs.push_back(i);
		}
	}
	if (!validRel)
		return false;
	split(attributes, ',', v1);
	if (v1.size() != (size_t)stoi(relationshipTab[relIdxs[0]][1]))
		return false;
	vector<string> symbols;
	for (vector<Field>::iterator it = queryDeclarations.begin(); it != queryDeclarations.end(); ++it) {
		if (it->getType() == "" || it->getValue() == "")
			return false;
		symbols.push_back(it->getValue());
	}
	string *attributesArray = new string[v1.size()];
	bool *attrValidatorsArray = new bool[v1.size()];
	for (size_t i = 0; i < v1.size(); i++) {
		attributesArray[i] = v1[i];
		attrValidatorsArray[i] = false;
		if (find(symbols.begin(), symbols.end(), attributesArray[i]) != symbols.end())
			continue;
		else
			return false;
	}
	for (size_t i = 0; i < queryDeclarations.size(); i++) {
		for (size_t j = 0; j < v1.size(); j++) {
			if (attributesArray[j] == queryDeclarations[i].getValue())
				attributesArray[j] = queryDeclarations[i].getType();
		}
	}
	for (size_t i = 0; i < relIdxs.size(); i++) {
		for (size_t j = 0; j < v1.size(); j++) {
			if (attributesArray[j] == relationshipTab[relIdxs[i]][j + 2])
				attrValidatorsArray[j] = true;
		}
		bool wrongAttr = false;
		for (size_t j = 0; j < v1.size(); j++) {
			if (attrValidatorsArray[j] == false) {
				wrongAttr = true;
				break;
			}
		}
		if (!wrongAttr)
			break;
		if (wrongAttr && i == relIdxs.size() - 1)
			return false;
	}
	return true;
}


bool Validator::checkAttribute(string attribute) {	//MUSI JESZCZE JAKOS SPRAWDZAC I POBIERAC TYP ATRYBUTU
	int dots = 0;
	for (char& c : attribute) {
		if (c == '.')
			dots++;
	}
	if (dots >1)
		return false;
	vector<string> v1;
	split(attribute, '.', v1);
	string entity = v1[0], type = v1[1];
	bool validEntity = false, validType = false;
	int entityIndex;
	for (size_t i = 0; i < attributeTab.size(); i++) {
		if (entity == attributeTab[i][0]) {
			validEntity = true;
			entityIndex = i;
			break;
		}
	}
	if (!validEntity)
		return false;
	if (type == attributeTab[entityIndex][1])
		validType = true;
	if (!validType)
		return false;
	return true;
}
