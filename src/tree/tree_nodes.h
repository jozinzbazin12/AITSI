/*
 * tree_nodes.h
 *
 *  Created on: 19 mar 2016
 *      Author: Grzegurz
 */

#ifndef SRC_TREE_TREE_NODES_H_
#define SRC_TREE_TREE_NODES_H_

class Node {
};

class Program: public Node {
};

class Procedure: public Node {
public:
	string name;
	Procedure(string name) {
		this->name = name;
	}
};

class Assignment: public Node {
};

#endif /* SRC_TREE_TREE_NODES_H_ */
