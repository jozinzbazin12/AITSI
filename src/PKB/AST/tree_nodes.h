/*
 * tree_nodes.h
 *
 *  Created on: 19 mar 2016
 *      Author: Grzegurz
 */

#ifndef SRC_PKB_AST_TREE_NODES_H_
#define SRC_PKB_AST_TREE_NODES_H_

using namespace std;

class Node {
public:
	bool last = false;
	bool newLevel = false;
	vector<Node*> children;
	Node() {
	}
};

class NewLevelNode: public Node {
public:
	NewLevelNode() {
		newLevel = true;
	}
};

class Program: public Node {
};

class Procedure: public NewLevelNode {
public:
	string name;
	Procedure(string name) {
		this->name = name;
	}
};

class Assignment: public Node {
};

class MathNode: public Node {
public:
	string op;
	MathNode(string op) {
		this->op = op;
	}
	MathNode() {
	}
};

class OperandNode: public MathNode {
public:
};
#endif /* SRC_TREE_TREE_NODES_H_ */
