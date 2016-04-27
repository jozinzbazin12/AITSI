/*
 * queryEvaluator.cpp
 *
 *  Created on: 19 kwi 2016
 *      Author: Popek
 */

#include "QueryEvaluator.h"

namespace std {

QueryEvaluator::QueryEvaluator() {
	// TODO Auto-generated constructor stub

}

QueryEvaluator::~QueryEvaluator() {
	// TODO Auto-generated destructor stub
}

string QueryEvaluator::getResult(PQLTree *Tree) {
	string result;

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
								break;
						case str2int("string"):
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
						getModifiesResult();
						cout<<"Modifies";
					}
					if((*begin)->data->getNodeType()=="parent"){
						getParentResult();
						cout<<"Parent";
					}
					if((*begin)->data->getNodeType()=="follows"){
						getFollowsResult();
						cout<<"Follows";
					}
					if((*begin)->data->getNodeType()=="uses"){
						getUsesResult();
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

string QueryEvaluator::getModifiesResult(){
	string modResult;
	return modResult;
}

string QueryEvaluator::getParentResult(){
	string modResult;
	return modResult;
}

string QueryEvaluator::getFollowsResult(){
	string modResult;
	return modResult;
}

string QueryEvaluator::getUsesResult(){
	string modResult;
	return modResult;
}


} /* namespace std */
