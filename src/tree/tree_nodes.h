/*
 * tree_nodes.h
 *
 *  Created on: 19 mar 2016
 *      Author: Grzegurz
 */

#ifndef SRC_TREE_TREE_NODES_H_
#define SRC_TREE_TREE_NODES_H_

class Node {
protected:
	static vector<string> possibleChildren;

};

class Procedure: Node {
public:
	string name;
	Procedure(string name) {
		this->name = name;
	}
};
vector<string> Node::possibleChildren = { StatementType::ASSIGN };

class Assignment: Node {
public:
	string name;
	Assignment(string name) {
		this->name = name;
	}
};
//vector<string> Node::possibleChildren = { StatementType::ASSIGN };

#endif /* SRC_TREE_TREE_NODES_H_ */
