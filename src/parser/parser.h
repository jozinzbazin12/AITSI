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
		string str2 = str;
		ASTTree* node = NULL;
		Syntax::currLine = count;
		string line;
		int closed;
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
					closed = (*node->getRoot())->data->closed;
					while (closed > 0) {
						openedNodes.pop();
						closed--;
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
			e.msg = "could not parse line: " + str2;
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
			parsers.push_back(Syntax::allSynstax[StatementType::WHILE]);
		}
	}

	void parse() {
		string line;
		do {
			getline(stream, line);
			count++;
			parseLine(line);
		} while (!stream.eof());
		if (openedNodes.size() != 1) {
			stringstream ss;
			ss << "Program is not last node (" << openedNodes.size() << " too much opened nodes)";
			throw RuntimeException(count, "PARSER", ss.str());
		}
	}
};

#endif /* PARSER_H_ */
