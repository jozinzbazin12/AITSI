/*
 * parser.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
private:
	bool debug;
	fstream stream;
	vector<Syntax*> parsers;
	int count = 0;
	stack<tree<tree_node_<ASTNode*>*>::iterator> openedNodes;

	void parseLine(string str) {
		ASTTree* node = NULL;
		Syntax::currLine = count;
		string line;
		trim(str);
		if (!str.length()) {
			return;
		}
		for (vector<Syntax*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
			try {
				node = (*it)->parseLine(str);
				if (node) {
					tree<tree_node_<ASTNode*>*>::iterator pos = NodeUtil::appendChild(root, openedNodes.top(), node);
					if ((*node->getRoot())->data->newLevel) {
						openedNodes.push(pos);
					}
					if ((*node->getRoot())->data->last) {
						openedNodes.pop();
					}
					break;
				}
			} catch (Exception &e) {
				if (debug) {
					e.print();
				}
			}
		}
		if (!node) {
			RuntimeException e = RuntimeException();
			e.line = count;
			e.parser = "PARSER";
			e.msg = "could not parse line";
			throw e;
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
			parsers.push_back(Syntax::allSynstax[StatementType::PROCEDURE]);
			parsers.push_back(Syntax::allSynstax[StatementType::ASSIGN]);
		}
	}

	void parse() {
		string line;
		do {
			getline(stream, line);
			count++;
			parseLine(line);
		} while (!stream.eof());
		if (openedNodes.size()!=1) {
			cout << openedNodes.size();
			throw RuntimeException(count, "?", "Program is not last node");
		}
	}
}
;

#endif /* PARSER_H_ */
