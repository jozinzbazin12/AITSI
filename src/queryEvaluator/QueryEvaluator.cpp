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
	set<int> setLines;
	tree<tree_node_<PQLNode*>*>::iterator begin = Tree->Tree->begin();
	tree<tree_node_<PQLNode*>*>::iterator end = Tree->Tree->end();
		while (begin != end) {
			for (int i = 0; i<Tree->Tree->depth(begin); ++i){
				if(((*begin)->data->getType())=="resultNode"){
					char* s = (*begin)->data->getField1()->getType();
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
								//varNames = pkb -> /..cos tam../getLineTable() -> getVarNames();
								break;
						case str2int("prog_line"):
								//lines = pkb -> getLineTable() -> getProgLines();
								break;
						case str2int("procedure"):
							//	lines = pkb -> getLineTable() -> getProgLines();
								break;
						case str2int("stmt#"):
								setLines = pkb -> getLineTable() -> getAssignLines();
								setLines = pkb -> getLineTable() -> getWhileLines();
							//	setLines = pkb -> getLineTable() -> getCallLines();
							//	setLines = pkb -> getLineTable() -> getIfLines();
								break;
						case str2int("any"):
								break;
					}
				}
				if(((*begin)->data->getType())=="suchNode"){
					//Sprawdzanie czy wyst¹pi³a relacja Modifiles
					if((*begin)->data->getNodeType()=="modifies"){
						lines = getModifiesResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
						cout<<"Modifies"<<endl;
					}
					//Sprawdzanie czy wyst¹pi³a relacja Parent lub Parent*
					if((*begin)->data->getNodeType()=="parent"){
						//*
						if((*begin)->data->isStar()){
							//lines = getParentSResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
							cout<<"Parent*"<<endl;
						}
						//zwyk³y Parent
						else{
							lines = getParentResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
						    cout<<"Parent"<<endl;
						}
					}
					//Sprawdzanie czy wyst¹pi³a relacja Follows lub Follows*
					if((*begin)->data->getNodeType()=="follows"){
						//*
						if((*begin)->data->isStar()){
							//lines = getFollowsSResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
							cout<<"Follows*"<<endl;
						}
						//zwyk³y Follows
						else{
							lines = getFollowsResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
							cout<<"Follows"<<endl;
						}
					}
					//Sprawdzanie czy wyst¹pi³a relacja Uses lub Uses*
					if((*begin)->data->getNodeType()=="uses"){
						//*
						if((*begin)->data->isStar()){
							//lines = getUsesSResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
							cout<<"Uses*"<<endl;
						}
						//zwyk³y Follows
						else{
							lines = getUsesResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines);
						    cout<<"Uses"<<endl;
						}
					}
				}
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
	set<int> setLines;
	// "any", "assign", "procedure", "while", "constant", "variable", "prog_line", "stmt"
	//if(field1->getType() == "any"){ 1st argument Modifies nigdy nie bedzie ' _ '

	//}
	//FIELD1
	if(field1->getType() == "assign"){
		setLines = pkb -> getLineTable() -> getAssignLines();


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
		setLines = pkb -> getLineTable() -> getAssignLines();
		setLines = pkb -> getLineTable() -> getWhileLines();
		//setLines = pkb -> getLineTable() -> getIfLines();
		//setLines = pkb -> getLineTable() -> getCallLines();

	}
	else if(field1->getType() == "variable"){

	}
	else if(field1->getType() == "prog_line"){

	}

	return lines;
}

vector<int> QueryEvaluator::getParentResult(Field* field1, Field* field2, vector<int> lines){
	set<int> setLines1;
	set<int> setLines2;
	//Sprawdzanie pierwszego parametru (Filed1) w relacji Parent
	if(field1->getType() == "constant" && field2->getType() != "constant"){
		char* s = field1->getValue();
		int param = str2int(s);
		//setLines = pkb -> getLineTables - > getChildLines(param);
		//dopisac sprawdzanie z 3cim parametrem- lines - aby juz wyeliminowac wartosci i zwrocic wynik
	}
	else if(field1->getType() != "constant" && field2->getType() == "constant"){
		char* s = field2->getValue();
		int param = str2int(s);

		//setLines = pkb -> getLineTables - > getParentLines(param);
		// ... tutaj jakby getParentLines - > to sa getFirstWhileLines i get firstIfLines
		//dopisac sprawdzanie z 3cim parametrem- lines - aby juz wyeliminowac wartosci i zwrocic wynik
	}
	else if(field1->getType() == "constant" && field2->getType() == "constant"){
		char* s1 = field1->getValue();
	    int param1 = str2int(s1);
	    char* s2 = field1->getValue();
	    int param2 = str2int(s2);
	    if(pkb -> parent(s1,s2) == true){
	    	return lines;
	    }
	}
	else{
		if(field1->getType() == "stmt"){
			setLines1 = pkb -> getLineTable() -> getWhileLines();
			//setLines1 = pkb -> getLineTable() -> getIfLines();
		}
		else if(field1->getType() == "while"){
			setLines1 = pkb ->getLineTable() -> getWhileLines();
		}
		else if(field1->getType() == "if"){
			//setLines1 = pkb ->getLineTable() ->getIfLines();
		}
		if(field2->getType() == "stmt"){
			setLines2 = pkb -> getLineTable() -> getAssignLines();
			setLines2 = pkb -> getLineTable() -> getWhileLines();
			//setLines = pkb -> getLineTable() -> getIfLines();
			//setLines = pkb -> getLineTable() -> getCallLines();
		}
		else if(field2->getType() == "while"){
			setLines2 = pkb -> getLineTable() -> getWhileLines();
		}
		else if(field2->getType() == "if"){
			//setLines2 = pkb -> getLineTable() -> getIfLines();
		}
		else if(field2->getType() == "call"){
			//setLines2 = pkb -> getLineTable() -> getCallLines();
		}
		else if(field2->getType() == "assign"){
			setLines2 = pkb -> getLineTable() -> getAssignLines();
		}
		else if(field2->getType() == "prog_line"){
			//setLines2 = pkb -> getLineTable() -> getProgLines();
		}
		else if(field2->getType() == ""){

		}
	}

	return lines;
}
vector<int> QueryEvaluator::getParentSResult(Field* field1, Field* field2, vector<int> lines){

	return lines;
}
vector<int>  QueryEvaluator::getFollowsResult(Field* field1, Field* field2, vector<int> lines){

	return lines;
}
vector<int>  QueryEvaluator::getFollowsSResult(Field* field1, Field* field2, vector<int> lines){
	return lines;
}
vector<int>  QueryEvaluator::getUsesResult(Field* field1, Field* field2, vector<int> lines){

	return lines;
}
vector<int>  QueryEvaluator::getUsesSResult(Field* field1, Field* field2, vector<int> lines){

	return lines;
}


} /* namespace std */
