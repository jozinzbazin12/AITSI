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
	stack<Node*> openedNodes;
	Node* root = NULL;

	void parseLine(string str) {
		Node* node = NULL;
		Syntax::currLine = count;
		string line;
		for (vector<Syntax*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
			try {
				trim(str);
				if (!str.length()) {
					return;
				}
				node = (*it)->parseLine(str);
				if (node) {
					openedNodes.top()->children.push_back(node);
					if (node->newLevel) {
						openedNodes.push(node);
					}
					if (node->last) {
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

	Parser(string fileName) {
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
		root = new Program();
		openedNodes.push(root);
		do {
			getline(stream, line);
			count++;
			parseLine(line);
		} while (!stream.eof());
		if (root != openedNodes.top()) {
			throw RuntimeException(count, "?", "Program is not last node");
		}
	}
}
;

#endif /* PARSER_H_ */
