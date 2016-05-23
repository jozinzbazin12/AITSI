#ifndef SRC_PKB_AST_ASTTree_H_
#define SRC_PKB_AST_ASTTree_H_

using namespace std;

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

	//Compute the depth to the root or to a fixed other iterator.
	int getDepth(tree<tree_node_<ASTNode*>*>::iterator& nodeIter){
		return Tree->depth(nodeIter);
	}

	tree<tree_node_<ASTNode*>*>::iterator appendSubTree(tree<tree_node_<ASTNode*>*>::iterator pos, ASTTree* subtree) {
		return Tree->append_child(pos, subtree->getRoot());
	}

	tree<tree_node_<ASTNode*>*>::iterator getRoot() {
		return Tree->begin();
	}

	//Return sibling iterator to the first child of given node.
	tree<tree_node_<ASTNode*>*>::iterator getFirstChild(tree<tree_node_<ASTNode*>*>::iterator& it) {
			return Tree->begin(it);
		}
	//Return sibling end iterator for children of given node.
	tree<tree_node_<ASTNode*>*>::iterator getLastChild(tree<tree_node_<ASTNode*>*>::iterator& it) {
				return Tree->end(it);
			}

	tree<tree_node_<ASTNode*>*>::iterator getEnd() {
			return Tree->end();
		}


	tree<tree_node_<ASTNode*>*>::leaf_iterator getEndOfLeaf(tree<tree_node_<ASTNode*>*>::iterator& it){
			return Tree->end_leaf(it);
		}

	tree<tree_node_<ASTNode*>*>::leaf_iterator getBeginLeaf(tree<tree_node_<ASTNode*>*>::iterator& it){
				return Tree->begin_leaf(it);
			}

	tree<tree_node_<ASTNode*>*>::iterator parent(tree<tree_node_<ASTNode*>*>::iterator& it) {
			return Tree->parent(it);
		}
	//checks if iterator is in tree
	bool isValid(tree<tree_node_<ASTNode*>*>::iterator& it){
		return Tree->is_valid(it);
	}

	tree<tree_node_<vector<int>*>*> getCFGTree(tree<tree_node_<ASTNode*>*>::iterator begin, tree<tree_node_<ASTNode*>*>::iterator end) {
		tree<tree_node_<vector<int>*>*>* cfgTree = new tree<tree_node_<vector<int>*>*>();
		int i = 0;
		tree_node_<vector<int>*>* actualNode = new tree_node_<vector<int>*>();
		while ( begin != end) {
			actualNode->data->push_back(begin.node->data->data->lineNumber);
			if (begin.node->data->data->type == "CONDITIONAL") {
				cfgTree->insert(i,actualNode);
				cfgTree->insert(i+1, actualNode);
				tree<tree_node_<vector<int>*>*> ifStatement = getCFGTree(begin.node->first_child->first_child, begin.node->first_child->last_child);
				cfgTree->insert_subtree_after(i,ifStatement.begin());
				if (begin.node->first_child != begin.node->last_child) {
					tree<tree_node_<vector<int>*>*> elseStatement = getCFGTree(begin.node->last_child->first_child, begin.node->last_child->last_child);
					cfgTree->insert_subtree_after(i,elseStatement.begin());
				}
				actualNode = new tree_node_<vector<int>*>();
				i++;
			} else if (begin.node->data->data->type == "LOOP") {
				cfgTree->insert(i,actualNode);
				cfgTree->insert(i+1, actualNode);
				tree<tree_node_<vector<int>*>*> whileStatement = getCFGTree(begin.node->first_child->first_child, begin.node->first_child->last_child);
				cfgTree->insert_subtree_after(i,whileStatement.begin());
				actualNode = new tree_node_<vector<int>*>();
				i++;
			}

			i++;
			begin++;
		}
		return cfgTree;
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
				cout << " \\ " << "(" << (*begin)->data->type << " : " << (*begin)->data->value << ")" <<" : " << (*begin)->data->lineNumber << ")" << endl;
			}
			++begin;
		}
	}

};
#endif
