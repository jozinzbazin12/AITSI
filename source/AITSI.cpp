#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <istream>
#include <algorithm>
#include <map>
#include <iterator>
#include <stack>

using namespace std;
#include "types.h"
#include "PQL/tree_util.hh"
#include "PQL/PQLNode.h"
#include "PQL/PQLTree.h"
#include "QueryPreProcessor.h"
#include "Validator/Validator.h"
#include "PQL/PQL.h"
#include "queryEvaluator/QueryEvaluator.h"

bool debug = false;

string toLower(string str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

void ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}

void rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
}

void trim(string &s) {
	ltrim(s);
	rtrim(s);
}

void removeWhitespaces(string &str) {
	str.erase(remove_if(str.begin(), str.end(), ptr_fun<int, int>(isspace)), str.end());
}

void mainPQL() {
	Validator* v = new Validator();
	QueryPreProcessor* que = new QueryPreProcessor();
	PQL *pql = new PQL();
	pql->enterQuery();
	//pql->processQuery(pql->getQuery());
	//string a = "assign a; select a such that follows(a,\"a\");";
	cout << "**** ZAPYTANIE **************************************************"
		<< endl;
	cout << pql->getQuery() << endl << endl;
	cout << "**** DRZEWO *****************************************************" << endl;
	que->parseQuery(pql->getQuery());
	tree<tree_node_<PQLNode>>::iterator iter;
	PQLTree* tree = que->getTree();
	tree->printTree();
	/////////////////////////////////////////////////////////////////////////////////////////////////
	cout << endl;
	cout << "**** KONIEC DRZEWA*****************************************************" << endl;

	cout << "**** EVALUATOR *****************************************************" << endl;

	QueryEvaluator* queEva = new QueryEvaluator();
	queEva->getResult(tree);

	cout << "**** KONIEC EVALUATORA *****************************************************" << endl;

	cout << "**** WALIDACJA TESTY - KRZYSIEK****************************************" << endl;
	cout << v->checkSelect("select dgdd 4 23") << endl;
	cout << v->checkSelect("select select dgdd 4 23") << endl;
	vector<Field> entities;
	Field* f1 = new Field();
	string s1 = "stmt", s2 = "s", s3 = "variable", s4 = "v";
	f1->setType(s1);
	f1->setValue(s2);
	Field* f2 = new Field();
	f2->setType(s3);
	f2->setValue(s4);
	entities.push_back(*f1);
	entities.push_back(*f2);
	cout << v->checkEntities(entities) << endl;
	cout << v->checkRelationship("modifies(stmt,variable)") << endl;
	cout << v->checkRelationship2("modifies(s,v)") << endl;
	cout << v->checkAttribute("constant.value") << endl;
	cout << "**** WALIDACJA TESTY - KONIEC****************************************" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////
}

#include "exceptions.h"
#include "types.h"
#include "PKB/AST/tree_util.hh"
#include "PKB/AST/ASTNode.h"
#include "PKB/AST/ASTTree.h"
//#include "PKB/AST/tree_nodes.h"
#include "parser/matchers.h"
#include "parser/nodeUtil.h"
#include "parser/syntax.h"
//#include ""

void initSyntax() {
	Syntax* s = new ProcedureSyntax();
	Syntax::allSynstax[s->keyWord] = s;
	Syntax* w = new WhileSyntax();
	Syntax::allSynstax[w->keyWord] = w;
	Syntax* op = new OperandSyntax();
	Syntax::allSynstax[op->keyWord] = op;
	Syntax* callMeBabe = new CallSyntax();
	Syntax::allSynstax[callMeBabe->keyWord] = callMeBabe;
	Syntax* yiff = new IfSyntax();
	Syntax::allSynstax[yiff->keyWord] = yiff;
	Syntax* elze = new ElseSyntax();
	Syntax::allSynstax[elze->keyWord] = elze;
	Syntax* empty = new EmptySyntax();
	Syntax::allSynstax[empty->keyWord] = empty;

	RecursiveSyntax* ass = new AssingmentSyntax();
	Syntax::allSynstax[ass->keyWord] = ass;
	RecursiveSyntax* plus = new MathSyntax("+");
	Syntax::allSynstax[plus->keyWord] = plus;
	RecursiveSyntax* minus = new MathSyntax("-");
	Syntax::allSynstax[minus->keyWord] = minus;
	RecursiveSyntax* multi = new MathSyntax("*", true);
	Syntax::allSynstax[multi->keyWord] = multi;
	RecursiveSyntax* div = new MathSyntax("/");
	Syntax::allSynstax[div->keyWord] = div;
	RecursiveSyntax* par = new ParentsisesSyntax();
	Syntax::allSynstax[par->keyWord] = par;

	ass->parsers.push_back(plus);
	ass->parsers.push_back(minus);
	ass->parsers.push_back(div);
	ass->parsers.push_back(multi);
	ass->parsers.push_back(par);

	plus->parsers.push_back(plus);
	plus->parsers.push_back(minus);
	plus->parsers.push_back(div);
	plus->parsers.push_back(multi);
	plus->parsers.push_back(par);

	minus->parsers.push_back(plus);
	minus->parsers.push_back(minus);
	minus->parsers.push_back(div);
	minus->parsers.push_back(multi);
	minus->parsers.push_back(par);

	div->parsers.push_back(plus);
	div->parsers.push_back(minus);
	div->parsers.push_back(div);
	div->parsers.push_back(multi);
	div->parsers.push_back(par);

	multi->parsers.push_back(plus);
	multi->parsers.push_back(minus);
	multi->parsers.push_back(div);
	multi->parsers.push_back(multi);
	multi->parsers.push_back(par);

	par->parsers.push_back(plus);
	par->parsers.push_back(minus);
	par->parsers.push_back(div);
	par->parsers.push_back(multi);
	par->parsers.push_back(par);
}

#include "parser/parser.h"
#include "globalVars.h"

/*int main(int argc, char** args) {
	if (argc <= 1) {
		cout << "No arguments, to see usage help, use \"help\" parameter";
		return 0;
	}
	//W args[0] jest sciezka do exe.
	string action = string(args[1]);
	if (action == "help") {
		cout << "Usage...\n";
	}
	else if (action == "file") {
		initSyntax();
		Parser parser(args[2]);
		if (!parser.fileExists) {
			cout << "File not found\n";
		}
		else {
			try {
				parser.parse();
				cout << "OK" << endl;
				parser.root->printTree();
				pkb->setASTTree(parser.root);
				designExtractor->start();

				cout << "**********************************************************\n";
				cout << "***********FOLLOWS**\n";
				pkb->getFollows()->writeAll();
				cout << "**********************************************************\n";
				cout << "***********PARENTS**\n";
				pkb->getParent()->writeAll();
				cout << "**********************************************************\n";

				cout << "***********USES**\n";
				pkb->getUses()->writeAll();
				cout << "**********************************************************\n";

				cout << "***********VARIABLES**\n";
				pkb->getVarTable()->writeAll();
				cout << "**********************************************************\n";

				cout << "***********IF LINES**\n";
				pkb->getLineTable()->writeIfLines();
				cout << "**********************************************************\n";

				cout << "***********WHILE LINES**\n";
				pkb->getLineTable()->writeWhileLines();
				cout
					<< "**********************************************************\n";

				cout << "***********CALL LINES**\n";
				pkb->getLineTable()->writeCallLines();
				cout << "**********************************************************\n";

				cout << "***********PROC LINES**\n";
				pkb->getProcTable()->writeProcLines();
				cout << "**********************************************************\n";
				cout << "***********MODIFIES**\n";
				pkb->getModifies()->writeAll();
				cout << "**********************************************************\n";
				cout << "***********LINES**\n";
				pkb->getLineTable()->writeAll();
				cout << "**********************************************************\n";

			}
			catch (RuntimeException &ex) {
				ex.print();
				cout << "FAIL" << endl;
			}
			catch (Exception &ex) {
				ex.print();
				cout << "FAIL" << endl;
			}
		}
	}
	else {
		cout << "Invalid arguments";
	}
	mainPQL();
	return 0;
}*/
