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
	string selectValue;
	tree<tree_node_<PQLNode*>*>::iterator begin = Tree->Tree->begin();
	tree<tree_node_<PQLNode*>*>::iterator end = Tree->Tree->end();
		while (begin != end) {
			for (int i = 0; i<Tree->Tree->depth(begin); ++i){
				if(((*begin)->data->getType())=="resultNode"){
					char* s = (*begin)->data->getField1()->getType();
					int i= str2int(s);
					switch (i)
					{
					    // W zmiennej lines znajduja sie wszystkie szukane mozliwosci, jakie moga sie pojawic - pozniej na podstawie relacji s¹ modyfikowane i redukowane
						case str2int("assign"):
								lines = pkb -> getLineTable() -> getAssignLines();
						        selectValue = (*begin)->data->getField1()->getValue();
								break;
						case str2int("while"):
								lines = pkb -> getLineTable() -> getWhileLines();
								selectValue = (*begin)->data->getField1()->getValue();
								break;
						case str2int("variable"):
								//varNames = pkb -> /..cos tam../getLineTable() -> getVarNames();
								selectValue = (*begin)->data->getField1()->getValue();
								break;
						case str2int("prog_line"):
								//lines = pkb -> getLineTable() -> getProgLines();
								selectValue = (*begin)->data->getField1()->getValue();
								break;
						case str2int("procedure"):
							//	lines = pkb -> getLineTable() -> getProgLines();
								selectValue = (*begin)->data->getField1()->getValue();
								break;
						case str2int("stmt"):
								set<int> tmp;
								setLines = pkb -> getLineTable() -> getAssignLines();
								tmp = pkb -> getLineTable() -> getWhileLines();
								setLines.insert(tmp.begin(),tmp.end());
							/*	tmp  = pkb -> getLineTable() -> getCallLines();
								setLines.insert(tmp.begin(),tmp.end());
								tmp  = pkb -> getLineTable() -> getIfLines();
								setLines.insert(tmp.begin(),tmp.end());*/
								lines=setLines;
								selectValue = (*begin)->data->getField1()->getValue();
								break;
						case str2int("if"):
								//lines = pkb -> getLineTable() -> getIfLines();
								selectValue = (*begin)->data->getField1()->getValue();
								break;
						case  str2int("call"):
								//lines = pkb -> getLineTable() -> getCallLines();
								selectValue = (*begin)->data->getField1()->getValue();
								break;
					}
				}
				if(((*begin)->data->getType())=="suchNode"){
					//Sprawdzanie czy wystapila relacja Modifiles
					if((*begin)->data->getNodeType()=="modifies"){
						lines = getModifiesResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines, selectValue);
						cout<<"Modifies"<<endl;
					}
					//Sprawdzanie czy wystapila relacja Parent lub Parent*
					if((*begin)->data->getNodeType()=="parent"){
						//*
						if((*begin)->data->isStar()){
							lines = getParentSResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines, selectValue);
							cout<<"Parent*"<<endl;
						}
						//zwykly Parent
						else{
							lines = getParentResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines, selectValue);
						    cout<<"Parent"<<endl;
						}
					}
					//Sprawdzanie czy wystapila relacja Follows lub Follows*
					if((*begin)->data->getNodeType()=="follows"){
						//*
						if((*begin)->data->isStar()){
							lines = getFollowsSResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines,selectValue);
							cout<<"Follows*"<<endl;
						}
						//zwykly Follows
						else{
							lines = getFollowsResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines,selectValue);
							cout<<"Follows"<<endl;
						}
					}
					//Sprawdzanie czy wystapila relacja Uses lub Uses*
					if((*begin)->data->getNodeType()=="uses"){
						//*
						if((*begin)->data->isStar()){
							lines = getUsesSResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines, selectValue);
							cout<<"Uses*"<<endl;
						}
						//zwyly Follows
						else{
							lines = getUsesResult((*begin)->data->getField1(),(*begin)->data->getField2(),lines, selectValue);
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

vector<int> QueryEvaluator::getModifiesResult(Field* field1, Field* field2, vector<int> lines, string selectValue){
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

vector<int> QueryEvaluator::getParentResult(Field* field1, Field* field2, vector<int> lines, string selectValue){
	set<int> setLines1;
	set<int> setLines2;
	//Sprawdzanie pierwszego parametru (Filed1) w relacji Parent
	if(field1->getType() == "constant" && field2->getType() != "constant"){
		char* s = field1->getValue();
		int param = str2int(s);
        setLines1.insert(param);
        if(field2->getType() == "stmt" || field2->getType() == "any") {
        	setLines2 = pkb -> getLineTable() -> getAssignLines();
        	set<int> tmp = pkb -> getLineTable() -> getWhileLines();
        	setLines2.insert(tmp.begin(), tmp.end());
        	//Dodac tak jak wy¿ej
        	/*tmp = pkb -> getLineTable() -> getIfLines();
        	setLines2.insert(tmp.begin(), tmp.end());
        	tmp  = pkb -> getLineTable() -> getCallLines();
        	setLines2.insert(tmp.begin(), tmp.end());*/
        }
        else if(field2->getType() == "while") {
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
	}
	else if(field1->getType() != "constant" && field2->getType() == "constant"){
		char* s = field2->getValue();
		int param = str2int(s);
		setLines2.insert(param);
		if(field1->getType() == "stmt"){
			setLines1 = pkb -> getLineTable() -> getWhileLines();
			/*set<int> tmp = pkb -> getLineTable() -> getIfLines();
			setLines1.insert(tmp.begin(),tmp.end());*/
		}
		else if(field1->getType() == "while"){
			setLines1 = pkb ->getLineTable() -> getWhileLines();
		}
		else if(field1->getType() == "if"){
			//setLines1 = pkb ->getLineTable() ->getIfLines();
		}
	}
	else if(field1->getType() == "constant" && field2->getType() == "constant"){
		char* s1 = field1->getValue();
	    int param1 = str2int(s1);
	    char* s2 = field1->getValue();
	    int param2 = str2int(s2);
	    if(pkb -> parent(param1,param2) == true){
	    	return lines;
	    }
	}
	else{
		if(field1->getType() == "stmt"){
			setLines1 = pkb -> getLineTable() -> getWhileLines();
			/*set<int> tmp = pkb -> getLineTable() -> getIfLines();
			setLines1.insert(tmp.begin(),tmp.end()); */

		}
		else if(field1->getType() == "while"){
			setLines1 = pkb ->getLineTable() -> getWhileLines();
		}
		else if(field1->getType() == "if"){
			//setLines1 = pkb ->getLineTable() ->getIfLines();
		}
		if(field2->getType() == "stmt" ||  field2->getType() == "prog_line"
				|| field2->getType() == "any"){
			set<int> tmp = pkb -> getLineTable() -> getWhileLines();
			setLines2 = pkb -> getLineTable() -> getAssignLines();
			setLines2.insert(tmp.begin(),tmp.end());
			/*tmp = pkb -> getLineTable() -> getIfLines();
			setLines2.insert(tmp.begin(),tmp.end());
			tmp = pkb -> getLineTable() -> getCallLines();
			setLines2.insert(tmp.begin(),tmp.end());*/
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
	}
	vector<int> resultPart;
	// Sprawdzenie zaleznosci dla pobranych parametrow setLines1 oraz setLines2 i porównanie ich z wartosciami na lines (szukana wartosc)
	if(setLines1 != nullptr && setLines2 != nullptr) {
		for(set<int>::iterator l1 = setLines1.begin() ; l1 < setLines1.end() ; ++l1) {
			for(set<int>::iterator l2 = setLines2.begin() ; l2 < setLines2.end() ; ++l2) {
				if(pkb -> parent(*l1,*l2) == true) {
					if(selectValue == field1->getValue() && selectValue == field2->getValue()) {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						return nullptr;
					}
					else if(selectValue == field1->getValue() && find(lines.begin(), lines.end(),*l1) != lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						resultPart.push_back(*l1);
					}
					else if(selectValue == field2->getValue() && find(lines.begin(), lines.end(),*l2) != lines.end()) {
						// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
						resultPart.push_back(*l2);
					} else {
						// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoœci
						return lines;
					}
				}
			}
		}
	}
	lines = resultPart;
	return lines;
}

vector<int> QueryEvaluator::getParentSResult(Field* field1, Field* field2, vector<int> lines, string selectValue){

	return lines;
}

vector<int>  QueryEvaluator::getFollowsResult(Field* field1, Field* field2, vector<int> lines, string selectValue){
	set<int> setLines1;
	set<int> setLines2;

	//Sprawdzanie pierwszego parametru (Filed1) w relacji Follows
	if(field1->getType() == "constant" && field2->getType() != "constant"){
		char* s = field1->getValue();
		int param = str2int(s);
		//Dodanie wartoœci constant do listy 1
		setLines1.insert(param);
		//Sprawdzanie czym jest drugi parametr i ewentualne pobranie listy
		if(field2->getType() == "stmt" || field2->getType() == "any") {
			setLines2 = pkb -> getLineTable() -> getAssignLines();
			set<int> pom = pkb -> getLineTable() -> getWhileLines();
			setLines2.insert(pom.begin(), pom.end());

			//Dodac tak jak wy¿ej
			//setLines2 = pkb -> getLineTable() -> getIfLines();
			//setLines2 = pkb -> getLineTable() -> getCallLines();
		}
		else if(field2->getType() == "while") {
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
	}
	else if(field1->getType() != "constant" && field2->getType() == "constant"){
		char* s = field2->getValue();
		int param = str2int(s);
		//Dodanie wartoœci constant do listy 2
		setLines2.insert(param);
		//Sprawdzanie czym jest pierwszy parametr i ewentualne pobranie listy
		if(field1->getType() == "stmt" || field1->getType() == "any") {
			setLines1 = pkb -> getLineTable() -> getAssignLines();
			set<int> pom = pkb -> getLineTable() -> getWhileLines();
			setLines1.insert(pom.begin(), pom.end());

			//Dodac tak jak wy¿ej
			//setLines1 = pkb -> getLineTable() -> getIfLines();
			//setLines1 = pkb -> getLineTable() -> getCallLines();
		}
		else if(field1->getType() == "while") {
			setLines1 = pkb -> getLineTable() -> getWhileLines();
		}
		else if(field1->getType() == "if"){
			//setLines1 = pkb -> getLineTable() -> getIfLines();
		}
		else if(field1->getType() == "call"){
			//setLines1 = pkb -> getLineTable() -> getCallLines();
		}
		else if(field1->getType() == "assign"){
			setLines1 = pkb -> getLineTable() -> getAssignLines();
		}
	}
	else if(field1->getType() == "constant" && field2->getType() == "constant"){
		char* s1 = field1->getValue();
		int param1 = str2int(s1);
		char* s2 = field1->getValue();
		int param2 = str2int(s2);
		if(pkb -> follows(param1,param2) == true){
			return lines;
		}
	}
	else{
		// Pobranie listy dla pierwszego parametru
		// Any - TZN. dowolna wartoœæ z dostêpnych czyli stmt
		if(field1->getType() == "stmt" || field1->getType() == "any") {
			setLines1 = pkb -> getLineTable() -> getAssignLines();
			set<int> pom = pkb -> getLineTable() -> getWhileLines();
			setLines1.insert(pom.begin(), pom.end());

			//Dodac tak jak wy¿ej
			//setLines1 = pkb -> getLineTable() -> getIfLines();
			//setLines1 = pkb -> getLineTable() -> getCallLines();
		}
		else if(field1->getType() == "while") {
			setLines1 = pkb -> getLineTable() -> getWhileLines();
		}
		else if(field1->getType() == "if"){
			//setLines1 = pkb -> getLineTable() -> getIfLines();
		}
		else if(field1->getType() == "call"){
			//setLines1 = pkb -> getLineTable() -> getCallLines();
		}
		else if(field1->getType() == "assign"){
			setLines1 = pkb -> getLineTable() -> getAssignLines();
		}

		// Pobranie listy dla drugiego parametru
		if(field2->getType() == "stmt" || field2->getType() == "any") {
			setLines2 = pkb -> getLineTable() -> getAssignLines();
			set<int> pom = pkb -> getLineTable() -> getWhileLines();
			setLines2.insert(pom.begin(), pom.end());

			//Dodac tak jak wy¿ej
			//setLines2 = pkb -> getLineTable() -> getIfLines();
			//setLines2 = pkb -> getLineTable() -> getCallLines();
		}
		else if(field2->getType() == "while") {
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
	}

	vector<int> resultPart;
	// Sprawdzenie czy wszystkie parametry by³y dobre, je¿eli nie return pusta lista - TZN. by³ b³¹d przy parsowaniu lub walidacji
	if(setLines1 != nullptr && setLines2 != nullptr) {
		for(set<int>::iterator l1 = setLines1.begin() ; l1 < setLines1.end() ; ++l1) {
			for(set<int>::iterator l2 = setLines2.begin() ; l2 < setLines2.end() ; ++l2) {
				if(pkb -> follows(*l1,*l2) == true) {
					if(selectValue == field1->getValue() && selectValue == field2->getValue()) {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						return nullptr;
					}
					else if(selectValue == field1->getValue() && find(lines.begin(), lines.end(),*l1) != lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						resultPart.push_back(*l1);
					}
					else if(selectValue == field2->getValue() && find(lines.begin(), lines.end(),*l2) != lines.end()) {
						// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
						resultPart.push_back(*l2);
					} else {
						// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoœci
						return lines;
					}
				}
			}
		}
	}
	lines = resultPart;
	return lines;
}

vector<int>  QueryEvaluator::getFollowsSResult(Field* field1, Field* field2, vector<int> lines, string selectValue){
	return lines;
}




vector<int>  QueryEvaluator::getUsesResult(Field* field1, Field* field2, vector<int> lines, string selectValue){
	return lines;
}

vector<int>  QueryEvaluator::getUsesSResult(Field* field1, Field* field2, vector<int> lines, string selectValue){

	return lines;
}


} /* namespace std */
