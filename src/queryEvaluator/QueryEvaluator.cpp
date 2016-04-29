#include "QueryEvaluator.h"
#include "../globalVars.h"
#include <set>

namespace std {

QueryEvaluator::QueryEvaluator() {
	// TODO Auto-generated constructor stub

}

QueryEvaluator::~QueryEvaluator() {
	// TODO Auto-generated destructor stub
}

string QueryEvaluator::getResult(PQLTree *Tree) {
	string result;
	vector<int> lines;

	tree<tree_node_<PQLNode*>*>::iterator begin = Tree->Tree->begin();
	tree<tree_node_<PQLNode*>*>::iterator end = Tree->Tree->end();
		while (begin != end) {
			for (int i = 0; i<Tree->Tree->depth(begin); ++i){
				if(((*begin)->data->getType())=="resultNode"){
					char* s = (*begin)->data->getField1();
					int i= str2int(s);
					switch (i)
					{
						case str2int("assign"):
								lines = pkb -> getLineTable() -> getAssignLines();
								break;
						case str2int("string"):
								break;
						case str2int("while"):
								lines = pkb -> getLineTable() -> getWhileLines();
								break;
						case str2int("variable"):
								break;
						case str2int("prog_line"):
								break;
						case str2int("procedure"):
								break;
						case str2int("stmt#"):
								break;
						case str2int("any"):
								break;
					}
				}
				if(((*begin)->data->getType())=="suchNode")
				{
					if((*begin)->data->getNodeType()=="modifies"){
						lines = getModifiesResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
						cout<<"Modifies";
					}
					if((*begin)->data->getNodeType()=="parent"){
						lines = getParentResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
						cout<<"Parent";
					}
					if((*begin)->data->getNodeType()=="follows"){
						lines = getFollowsResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
						cout<<"Follows";
					}
					if((*begin)->data->getNodeType()=="uses"){
						lines = getUsesResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
					    cout<<"Uses";
					}
				}
		//	cout << endl;
			}
			++begin;
		}
	return result;
}

constexpr unsigned int QueryEvaluator::str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

vector<int> QueryEvaluator::getModifiesResult(Field* field1, Field* field2, vector<int> lines){
	set<int> myset;
	// "any", "assign", "procedure", "while", "constant", "variable", "prog_line", "stmt"
	//if(field1->getType() == "any"){ 1st argument Modifies nigdy nie bedzie ' _ '

	//}
	//FIELD1
	if(field1->getType() == "assign"){

	}
	else if(field1->getType() == "procedure"){

	}
	else if(field1->getType() == "while"){

	}
	//dla Constant czyli stalych wartosci - pobieram assign list
	//i sprawdzam dla wartosci constant - jezeli prawda to wszystkie assign s¹ ok
	else if(field2->getType() == "constant"){

	}
	//stmt czyli : assign, call, while, if
	else if(field1->getType() == "stmt"){
		lines = pkb -> getLineTable() -> getAssignLines();
		lines

	}
	else if(field1->getType() == "variable"){

	}
	else if(field1->getType() == "prog_line"){

	}

	return lines;
}

vector<int> QueryEvaluator::getParentResult(Field* field1, Field* field2, vector<int> lines){

	return lines;
}

vector<int>  QueryEvaluator::getFollowsResult(Field* field1, Field* field2, vector<int> lines){

	return lines;
}

vector<int>  QueryEvaluator::getUsesResult(Field* field1, Field* field2, vector<int> lines){

	return lines;
}


} /* namespace std */
