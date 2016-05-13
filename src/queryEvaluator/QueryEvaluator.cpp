#include "QueryEvaluator.h"
#include "../globalVars.h"
#include <set>
#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#ifndef SRC_PQL_PQLTREE_H_
#include "../PQL/PQLTree.h"
#endif

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
		for (int i = 0; i < Tree->Tree->depth(begin); ++i) {
			if (((*begin)->data->getType()) == "resultNode") {
				string s = (*begin)->data->getField1()->getType();
				if (s == "assign") {
					lines = pkb->getLineTable()->getAssignLines();
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "while") {
					lines = pkb->getLineTable()->getWhileLines();
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "variable") {
					//varNames = pkb -> /..cos tam../getLineTable() -> getVarNames();
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "prog_line") {
					lines = pkb->getLineTable()->getLines();
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "procedure") {
					lines = pkb->getLineTable()->getLines();
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "stmt") {
					set<int> tmp;
					setLines = pkb->getLineTable()->getOrderedAssignLines();
					tmp = pkb->getLineTable()->getOrderedWhileLines();
					setLines.insert(tmp.begin(), tmp.end());
					tmp = pkb->getLineTable()->getOrderedCallLines();
					setLines.insert(tmp.begin(), tmp.end());
					tmp = pkb->getLineTable()->getOrderedIfLines();
					setLines.insert(tmp.begin(), tmp.end());
					std::copy(setLines.begin(), setLines.end(), lines.begin());
					// lines = setLines;
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "if") {
					lines = pkb->getLineTable()->getIfLines();
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "call") {
					lines = pkb->getLineTable()->getCallLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}

			}
			if (((*begin)->data->getType()) == "suchNode") {
				//Sprawdzanie czy wystapila relacja Modifiles
				if ((*begin)->data->getNodeType() == "modifies") {
					lines = getModifiesResult((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines, selectValue);
					cout << "Modifies" << endl;
				}
				//Sprawdzanie czy wystapila relacja Parent lub Parent*
				if ((*begin)->data->getNodeType() == "parent") {
					//*
					if ((*begin)->data->isStar()) {
						lines = getParentSResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Parent*" << endl;
					}
					//zwykly Parent
					else {
						lines = getParentResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Parent" << endl;
					}
				}
				//Sprawdzanie czy wystapila relacja Follows lub Follows*
				if ((*begin)->data->getNodeType() == "follows") {
					//*
					if ((*begin)->data->isStar()) {
						lines = getFollowsSResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Follows*" << endl;
					}
					//zwykly Follows
					else {
						lines = getFollowsResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Follows" << endl;
					}
				}
				//Sprawdzanie czy wystapila relacja Uses lub Uses*
				if ((*begin)->data->getNodeType() == "uses") {
					//*
					if ((*begin)->data->isStar()) {
						lines = getUsesSResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Uses*" << endl;
					}
					//zwyly Follows
					else {
						lines = getUsesResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Uses" << endl;
					}
				}
			}
		}
		++begin;
	}
	return result;
}


vector<int> QueryEvaluator::getModifiesResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {
	set<int> setLines;
	// "any", "assign", "procedure", "while", "constant", "variable", "prog_line", "stmt"
	//if(field1->getType() == "any"){ 1st argument Modifies nigdy nie bedzie ' _ '

	//}
	//FIELD1
	if (field1->getType() == "assign") {
		setLines = pkb->getLineTable()->getOrderedAssignLines();
	} else if (field1->getType() == "procedure") {

	} else if (field1->getType() == "while") {

	}
	//dla Constant czyli stalych wartosci - pobieram assign list
	//i sprawdzam dla wartosci constant - jezeli prawda to wszystkie assign s¹ ok
	else if (field2->getType() == "constant") {

	}
	//stmt czyli : assign, call, while, if
	else if (field1->getType() == "stmt") {
		set<int> tmp = pkb->getLineTable()->getOrderedAssignLines();
		setLines = tmp;
		tmp = pkb->getLineTable()->getOrderedWhileLines();
		setLines.insert(tmp.begin(), tmp.end());
		tmp = pkb->getLineTable()->getOrderedIfLines();
		setLines.insert(tmp.begin(), tmp.end());
		tmp = pkb->getLineTable()->getOrderedCallLines();
		setLines.insert(tmp.begin(), tmp.end());
	} else if (field1->getType() == "variable") {

	} else if (field1->getType() == "prog_line") {

	}

	return lines;
}

vector<int> QueryEvaluator::getParentResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;
	//Sprawdzanie pierwszego parametru (Filed1) w relacji Parent
	if (field1->getType() == "constant" && field2->getType() != "constant") {
		int param = field1->getIntegerValue();
		setLines1.insert(param);
		if (field2->getType() == "stmt" || field2->getType() == "any") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			set<int> tmp = pkb->getLineTable()->getOrderedWhileLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getLineTable()->getOrderedIfLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getLineTable()->getOrderedCallLines();
			setLines2.insert(tmp.begin(), tmp.end());
		} else if (field2->getType() == "while") {
			setLines2 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field2->getType() == "if") {
			setLines2 = pkb->getLineTable()->getOrderedIfLines();
		} else if (field2->getType() == "call") {
			setLines2 = pkb->getLineTable()->getOrderedCallLines();
		} else if (field2->getType() == "assign") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
		}
	} else if (field1->getType() != "constant"
			&& field2->getType() == "constant") {
		int param = field2->getIntegerValue();
		setLines2.insert(param);
		if (field1->getType() == "stmt") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			set<int> tmp = pkb->getLineTable()->getOrderedIfLines();
			setLines1.insert(tmp.begin(), tmp.end());
		} else if (field1->getType() == "while") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field1->getType() == "if") {
			setLines1 = pkb->getLineTable()->getOrderedIfLines();
		}
	} else if (field1->getType() == "constant"
			&& field2->getType() == "constant") {
		int param1 = field1->getIntegerValue();
		int param2 = field1->getIntegerValue();
		if (pkb->parent->parent(param1, param2) == true) {
			return lines;
		}
	} else {
		if (field1->getType() == "stmt") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			set<int> tmp = pkb->getLineTable()->getOrderedIfLines();
			setLines1.insert(tmp.begin(), tmp.end());
		} else if (field1->getType() == "while") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field1->getType() == "if") {
			setLines1 = pkb->getLineTable()->getOrderedIfLines();
		}
		if (field2->getType() == "stmt" || field2->getType() == "prog_line"
				|| field2->getType() == "any") {
			set<int> tmp = pkb->getLineTable()->getOrderedWhileLines();
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getLineTable()->getOrderedIfLines();
			setLines2.insert(tmp.begin(), tmp.end());
			tmp = pkb->getLineTable()->getOrderedCallLines();
			setLines2.insert(tmp.begin(), tmp.end());
		} else if (field2->getType() == "while") {
			setLines2 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field2->getType() == "if") {
			setLines2 = pkb->getLineTable()->getOrderedIfLines();
		} else if (field2->getType() == "call") {
			setLines2 = pkb->getLineTable()->getOrderedCallLines();
		} else if (field2->getType() == "assign") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
		}
	}
	vector<int> resultPart;
	// Sprawdzenie zaleznosci dla pobranych parametrow setLines1 oraz setLines2 i porównanie ich z wartosciami na lines (szukana wartosc)
	if (setLines1.empty() && setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end();
				++l1) {
			for (set<int>::iterator l2 = setLines2.begin();
					l2 != setLines2.end(); ++l2) {
				if (pkb->parent->parent(*l1, *l2) == true) {
					if (selectValue == field1->getValue()
							&& selectValue == field2->getValue()) {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						//return nullptr;
						return resultPart;
					} else if (selectValue == field1->getValue()
							&& find(lines.begin(), lines.end(), *l1)
									!= lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						resultPart.push_back(*l1);
					} else if (selectValue == field2->getValue()
							&& find(lines.begin(), lines.end(), *l2)
									!= lines.end()) {
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

vector<int> QueryEvaluator::getParentSResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {

	return lines;
}

vector<int> QueryEvaluator::getFollowsResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;

	//Sprawdzanie pierwszego parametru (Filed1) w relacji Follows
	if (field1->getType() == "constant" && field2->getType() != "constant") {
		int param = field1->getIntegerValue();
		//Dodanie wartoœci constant do listy 1
		setLines1.insert(param);
		//Sprawdzanie czym jest drugi parametr i ewentualne pobranie listy
		if (field2->getType() == "stmt" || field2->getType() == "any") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedIfLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedCallLines();
			setLines2.insert(pom.begin(), pom.end());
		} else if (field2->getType() == "while") {
			setLines2 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field2->getType() == "if") {
			setLines2 = pkb->getLineTable()->getOrderedIfLines();
		} else if (field2->getType() == "call") {
			setLines2 = pkb->getLineTable()->getOrderedCallLines();
		} else if (field2->getType() == "assign") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
		}
	} else if (field1->getType() != "constant"
			&& field2->getType() == "constant") {
		int param = field2->getIntegerValue();
		//Dodanie wartoœci constant do listy 2
		setLines2.insert(param);
		//Sprawdzanie czym jest pierwszy parametr i ewentualne pobranie listy
		if (field1->getType() == "stmt" || field1->getType() == "any") {
			setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedIfLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedCallLines();
			setLines1.insert(pom.begin(), pom.end());
		} else if (field1->getType() == "while") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field1->getType() == "if") {
			setLines1 = pkb->getLineTable()->getOrderedIfLines();
		} else if (field1->getType() == "call") {
			setLines1 = pkb->getLineTable()->getOrderedCallLines();
		} else if (field1->getType() == "assign") {
			setLines1 = pkb->getLineTable()->getOrderedAssignLines();
		}
	} else if (field1->getType() == "constant"
			&& field2->getType() == "constant") {
		int param1 = field1->getIntegerValue();
		int param2 = field1->getIntegerValue();
		if (pkb->follows->follows(param1, param2) == true) {
			return lines;
		}
	} else {
		// Pobranie listy dla pierwszego parametru
		// Any - TZN. dowolna wartoœæ z dostêpnych czyli stmt
		if (field1->getType() == "stmt" || field1->getType() == "any") {
			setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedIfLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedCallLines();
			setLines1.insert(pom.begin(), pom.end());
		} else if (field1->getType() == "while") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field1->getType() == "if") {
			setLines1 = pkb->getLineTable()->getOrderedIfLines();
		} else if (field1->getType() == "call") {
			setLines1 = pkb->getLineTable()->getOrderedCallLines();
		} else if (field1->getType() == "assign") {
			setLines1 = pkb->getLineTable()->getOrderedAssignLines();
		}
		// Pobranie listy dla drugiego parametru
		if (field2->getType() == "stmt" || field2->getType() == "any") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedIfLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedCallLines();
			setLines2.insert(pom.begin(), pom.end());
		} else if (field2->getType() == "while") {
			setLines2 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field2->getType() == "if") {
			setLines2 = pkb->getLineTable()->getOrderedIfLines();
		} else if (field2->getType() == "call") {
			setLines2 = pkb->getLineTable()->getOrderedCallLines();
		} else if (field2->getType() == "assign") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
		}
	}

	vector<int> resultPart;
	// Sprawdzenie czy wszystkie parametry by³y dobre, je¿eli nie return pusta lista - TZN. by³ b³¹d przy parsowaniu lub walidacji
	if (setLines1.empty() && setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end();
				++l1) {
			for (set<int>::iterator l2 = setLines2.begin();
					l2 != setLines2.end(); ++l2) {
				if (pkb->follows->follows(*l1, *l2) == true) {
					if (selectValue == field1->getValue()
							&& selectValue == field2->getValue()) {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						return resultPart;
					} else if (selectValue == field1->getValue()
							&& find(lines.begin(), lines.end(), *l1)
									!= lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						resultPart.push_back(*l1);
					} else if (selectValue == field2->getValue()
							&& find(lines.begin(), lines.end(), *l2)
									!= lines.end()) {
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

vector<int> QueryEvaluator::getFollowsSResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {
	return lines;
}

vector<int> QueryEvaluator::getUsesResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {
	return lines;
}

vector<int> QueryEvaluator::getUsesSResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {

	return lines;
}

} /* namespace std */
