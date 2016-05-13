/*
 * parser.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "../globalVars.h"

class Parser {
private:
	bool debug;
	fstream stream;
	vector<Syntax*> parsers;
	int count = 0;
	stack<tree<tree_node_<ASTNode*>*>::iterator> openedNodes;

	void throwException(string msg) {
		RuntimeException e = RuntimeException();
		e.line = count;
		e.parser = "PARSER";
		e.msg = msg;
		throw e;
	}

	void parseLine(string str) {
		string str2 = str;
		ASTTree* node = NULL;
		Syntax::currLine = count;
		string line;
		Syntax* s;
		int closed;
		trim(str);
		if (!str.length()) {
			return;
		}
		for (vector<Syntax*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
			try {
				s = *it;
				node = s->parseLine(str);
				if (node) {
					string type = (*node->getRoot())->data->type;
					if (type != NodeName::FAKE_OPEN && type != NodeName::FAKE_CLOSE) {
						tree<tree_node_<ASTNode*>*>::iterator pos = NodeUtil::appendChild(root, openedNodes.top(), node);
						if (s->keyWord == Keywords::ELSE) {
							((ElseSyntax*) s)->validate(root, pos);
						} else if (s->keyWord == Keywords::PROCEDURE) {
							((ProcedureSyntax*) s)->validate(root, pos);
						}
						if ((*node->getRoot())->data->newLevel) {
							openedNodes.push(pos);
						}
						closed = (*node->getRoot())->data->closed;
						while (closed > 0) {
							openedNodes.pop();
							closed--;
						}
						break;
					}
				}
			} catch (Exception &e) {
				if (e.runtime) {
					throw e;
				}
				if (debug) {
					e.print();
				}
			}
		}
		if (node && (*node->getRoot())->data->type == NodeName::FAKE_CLOSE) {
			closed = (*node->getRoot())->data->closed;
			while (closed > 0) {
				openedNodes.pop();
				closed--;
			}
		}
		if (!node) {
			throwException("could not parse line: " + str2);
		}
	}

public:
	bool fileExists = false;
	ASTTree* root = new ASTTree();

	Parser(string fileName) {
		tree_node_<ASTNode*>* program = NodeUtil::createProgramNode();
		tree<tree_node_<ASTNode*>*>::iterator it = root->appendRoot(program);
		openedNodes.push(it);
		stream.open(fileName.c_str());
		if (stream.good() && stream.is_open()) {
			fileExists = true;
			cout << "File " + fileName + " found!\n";
			//Tu dodawac parsery polecen
			parsers.push_back(Syntax::allSynstax[Keywords::PROCEDURE]);
			parsers.push_back(Syntax::allSynstax[Keywords::ASSIGN]);
			parsers.push_back(Syntax::allSynstax[Keywords::WHILE]);
			parsers.push_back(Syntax::allSynstax[Keywords::CALL]);
			parsers.push_back(Syntax::allSynstax[Keywords::IF]);
			parsers.push_back(Syntax::allSynstax[Keywords::ELSE]);
			parsers.push_back(Syntax::allSynstax[NodeName::FAKE_OPEN]);
		}
	}

	void parse() {
		string line;
		LinesTable * linesTable = pkb -> getLineTable();
		do {
			getline(stream, line);
			linesTable -> addLine(line);
			count++;
			parseLine(line);
//			root->printTree();
		} while (!stream.eof());
		if (openedNodes.size() != 1) {
			stringstream ss;
			ss << "Program is not last node (" << openedNodes.size() << " too much opened nodes)";
			throw RuntimeException(count, "PARSER", ss.str());
		}
	}
};

#endif /* PARSER_H_ */
