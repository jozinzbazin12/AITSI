#ifndef SRC_PKB_AST_ASTTree_H_
#define SRC_PKB_AST_ASTTree_H_
class ASTTree {
private:
	//static ASTTree *Atree;

public:
	ASTTree() {

		Tree = new tree<tree_node_<ASTNode*>*>();
	}
	tree<tree_node_<ASTNode*>*> *Tree;

	~ASTTree() {
	}

	//Insert node as last/first child of node pointed to by position. 
	tree<tree_node_<ASTNode*>*>::iterator appendChild(tree<tree_node_<ASTNode*>*>::iterator& nodeIter, tree_node_<ASTNode*>*& child) {
		return Tree->append_child(nodeIter, child);
	}
	tree<tree_node_<ASTNode*>*>::iterator appendRoot(tree_node_<ASTNode*>*& node) {
		Tree->set_head(node);
		return Tree->begin();
	}
	//Insert node as next sibling of node pointed to by position. 
	tree<tree_node_<ASTNode*>*>::iterator appendSibling(tree<tree_node_<ASTNode*>*>::iterator& sibling, tree_node_<ASTNode*>*& node) {
		return Tree->insert_after(sibling, node);
	}
	//Insert node as previous sibling of node pointed to by position. 
	tree<tree_node_<ASTNode*>*>::iterator appendLeftSibling(tree<tree_node_<ASTNode*>*>::iterator& sibling, tree_node_<ASTNode*>*& node) {
		return Tree->insert(sibling, node);
	}
	//return the n-th child of the node at position. 
	tree<tree_node_<ASTNode*>*>::iterator getChild(tree<tree_node_<ASTNode*>*>::iterator& nodeIter, uint16_t childNumber) {
		return Tree->child(nodeIter, childNumber);
	}
	//Return iterator to the parent of a node. 
	tree<tree_node_<ASTNode*>*>::iterator getParent(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->parent(nodeIter);
	}
	//Return iterator to the next sibling of a node. 
	tree<tree_node_<ASTNode*>*>::iterator getNextSibling(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->next_sibling(nodeIter);
	}
	//Return iterator to the previous sibling of a node. 
	tree<tree_node_<ASTNode*>*>::iterator getPrevSibling(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->previous_sibling(nodeIter);
	}
	//Count the number of children of node at position. 
	int getNumberOfChildren(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
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
	int getSize(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->size(nodeIter);
	}
	//Count the number of siblings (left and right) of node at iterator. Total nodes at this level is +1. 
	int getNumberOfSibilings(tree<tree_node_<ASTNode*>*>::iterator& nodeIter) {
		return Tree->number_of_siblings(nodeIter);
	}

	tree<tree_node_<ASTNode*>*>::iterator appendSubTree(tree<tree_node_<ASTNode*>*>::iterator pos, ASTTree* subtree) {
		return Tree->append_child(pos, subtree->getRoot());
	}

	tree<tree_node_<ASTNode*>*>::iterator getRoot() {
		return Tree->begin();
	}

	tree<tree_node_<ASTNode*>*>::iterator parent(tree<tree_node_<ASTNode*>*>::iterator it) {
			return Tree->parent(it);
		}

	void printTree() {
		tree<tree_node_<ASTNode*>*>::iterator begin = Tree->begin();
		tree<tree_node_<ASTNode*>*>::iterator end = Tree->end();
		while (begin != end) {
			for (int i = 0; i < Tree->depth(begin); ++i) {
				cout << "    |";
			}
			if (!(*begin)->data) {
				cout << "??" << endl;
			} else {
				cout << " \\ " << "(" << (*begin)->data->type << " : " << (*begin)->data->value << ")" << endl;
			}
			++begin;
		}
	}

};
#endif
