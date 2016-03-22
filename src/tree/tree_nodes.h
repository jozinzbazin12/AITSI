/*
 * tree_nodes.h
 *
 *  Created on: 19 mar 2016
 *      Author: Grzegurz
 */

#ifndef SRC_TREE_TREE_NODES_H_
#define SRC_TREE_TREE_NODES_H_

class Node {
public:
	bool last = false;
	bool newLevel = false;
	vector<Node*> children;
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

#endif /* SRC_TREE_TREE_NODES_H_ */
