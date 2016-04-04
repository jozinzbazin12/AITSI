/*
 * PQLTree.h
 *
 *  Created on: 30 mar 2016
 *      Author: Popek
 */

#ifndef SRC_PQL_PQLTREE_H_
#define SRC_PQL_PQLTREE_H_

using namespace std;

class PQLTree
{
private:
	//static PQLTree *Ptree;
	PQLTree() {
		Tree = new tree<tree_node_<PQLNode>>();
	}

public:

	tree<tree_node_<PQLNode>> *Tree;

	static PQLTree* getInstance()
	{
		PQLTree *Ptree;
		Ptree = new PQLTree();
		return Ptree;

		/*if (Ptree == NULL) {
			Ptree = new PQLTree();
			return Ptree;
		}
		else
		{
			return Ptree;
		}*/

	}

	~PQLTree()
	{
	}

	//Insert node as last/first child of node pointed to by position.
	tree<tree_node_<PQLNode>>::iterator appendChild(tree<tree_node_<PQLNode>>::iterator& nodeIter,tree_node_<PQLNode>& child) {
		return Tree->append_child(nodeIter,child);
	}
	tree<tree_node_<PQLNode>>::iterator appendRoot(tree_node_<PQLNode>& node) {
		Tree->set_head(node);
		return Tree->begin();
	}
	//Insert node as next sibling of node pointed to by position.
	tree<tree_node_<PQLNode>>::iterator appendSibling(tree<tree_node_<PQLNode>>::iterator& sibling, tree_node_<PQLNode>& node) {
		return Tree->insert_after(sibling, node);
	}
	//Insert node as previous sibling of node pointed to by position.
	tree<tree_node_<PQLNode>>::iterator appendLeftSibling(tree<tree_node_<PQLNode>>::iterator& sibling, tree_node_<PQLNode>& node) {
		return Tree->insert(sibling, node);
	}
	//return the n-th child of the node at position.
	tree<tree_node_<PQLNode>>::iterator getChild(tree<tree_node_<PQLNode>>::iterator& nodeIter,uint16_t childNumber) {
		return Tree->child(nodeIter, childNumber);
	}
	//Return iterator to the parent of a node.
	tree<tree_node_<PQLNode>>::iterator getParent(tree<tree_node_<PQLNode>>::iterator& nodeIter) {
		return Tree->parent(nodeIter);
	}
	//Return iterator to the next sibling of a node.
	tree<tree_node_<PQLNode>>::iterator getNextSibling(tree<tree_node_<PQLNode>>::iterator& nodeIter) {
		return Tree->next_sibling(nodeIter);
	}
	//Return iterator to the previous sibling of a node.
	tree<tree_node_<PQLNode>>::iterator getPrevSibling(tree<tree_node_<PQLNode>>::iterator& nodeIter) {
		return Tree->previous_sibling(nodeIter);
	}
	//Count the number of children of node at position.
	int getNumberOfChildren(tree<tree_node_<PQLNode>>::iterator& nodeIter) {
		return Tree->number_of_children(nodeIter);
	}
	//Check if tree is empty.
	bool isEmpty() {
		return Tree->empty();
	}
	//Count the total number of nodes.
	int getSize() {
		return Tree->size();
	}
	//Count the total number of nodes below the indicated node (plus one).
	int getSize(tree<tree_node_<PQLNode>>::iterator& nodeIter) {
		return Tree->size(nodeIter);
	}
	//Count the number of siblings (left and right) of node at iterator. Total nodes at this level is +1.
	int getNumberOfSibilings(tree<tree_node_<PQLNode>>::iterator& nodeIter) {
		return Tree->number_of_siblings(nodeIter);
	}

	//Insert node as previous sibling of node pointed to by position.
	tree<tree_node_<PQLNode>>::iterator getRoot() {
		return Tree->begin();
	}

	void printTree() {
		tree<tree_node_<PQLNode>>::iterator begin = Tree->begin();
		tree<tree_node_<PQLNode>>::iterator end = Tree->end();
		while (begin != end) {
			for (int i = 0; i<Tree->depth(begin); ++i)
				cout << "    |";
				cout << " \\ " << (begin.node->data.data.nodeType);
			cout << endl;
			++begin;
		}
	}

};

#endif /* SRC_PQL_PQLTREE_H_ */
