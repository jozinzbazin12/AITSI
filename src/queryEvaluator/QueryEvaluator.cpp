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

vector<string> QueryEvaluator::getResult(PQLTree *Tree) {
	vector<string> result;
	vector<int> lines;
	set<int> setLines;
	string selectValue;
	string resultType;
	tree<tree_node_<PQLNode*>*>::iterator begin = Tree->Tree->begin();
	tree<tree_node_<PQLNode*>*>::iterator end = Tree->Tree->end();

	while (begin != end) {
		//for (int i = 0; i < Tree->Tree->depth(end); ++i) {

			if (((*begin)->data->getType()) == "resultNode") {
				string s = (*begin)->data->getField1()->getType();
				resultType = s;
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
					lines = pkbApi->getProceduresLines();
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "stmt" || s == "boolean") {
					vector<int> tmp2;
					tmp2 = pkb->getLineTable()->getAssignLines();
					setLines.insert(tmp2.begin(),tmp2.end());
					tmp2 = pkb->getLineTable()->getWhileLines();
					setLines.insert(tmp2.begin(),tmp2.end());
					tmp2 = pkb->getLineTable()->getCallLines();
					setLines.insert(tmp2.begin(),tmp2.end());
					tmp2 = pkb->getLineTable()->getIfLines();
					setLines.insert(tmp2.begin(),tmp2.end());
					std::copy(setLines.begin(), setLines.end(), std::inserter( lines, lines.end() ) );
					selectValue = (*begin)->data->getField1()->getValue();
				} else if (s == "if") {
					lines = pkb->getLineTable()->getIfLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}
			}

			if (((*begin)->data->getType()) == "suchNode") {
				//Sprawdzanie czy wystapila relacja Modifies
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
					//zwykly Uses
					else {
						lines = getUsesResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Uses" << endl;
					}
				}
				// Calls || Calls*
				if ((*begin)->data->getNodeType() == "calls") {
					if ((*begin)->data->isStar()) {
						lines = getCallStarResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Calls* " << endl;
					} else {
						lines = getCallResult((*begin)->data->getField1(),
								(*begin)->data->getField2(), lines,
								selectValue);
						cout << "Calls " << endl;
					}
				}
			}

			if(((*begin)->data->getType()) == "withNode") {
				getWithResult((*begin)->data->getField1(), (*begin)->data->getField2(), lines);
			}
		//}
		++begin;
	}

	cout << "WYNIK: " ;
	if(!lines.empty())
	{
		if(resultType == "boolean")
		{
			result.push_back("TRUE");
			//cout << "TRUE";
		}
		else
		{
			for(int i = 0 ; i < lines.size() ; i++)
			{
				if(resultType == "procedure")
				{
					string name = pkbApi -> getProcName(pkbApi -> getProcId(lines[i]));
					result.push_back(name);
					//cout << name << " ";
				}
				else
				{
					stringstream ss;
					ss << lines[i];
					result.push_back(ss.str());
					//cout << lines[i] << " ";
				}
			}
		}
	}
	else
	{
		if(resultType == "boolean")
		{
			result.push_back("FALSE");
			//cout << "FALSE";
		}
		else
		{
			result.push_back("NONE");
			//cout << "NONE";
		}
	}

	cout << endl;

	return result;
}

vector<int> QueryEvaluator::getModifiesResult(Field* field1, Field* field2,
		vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<string> setString1;
	set<int> setLines2;
	set<string> setString2;
	//Sprawdzanie pierwszego parametru (Filed1) w relacji Modifies
	if (field1->getType() == "constant") {
		int param = field1->getIntegerValue();
		setLines1.insert(param);
		if (field2->getType() == "variable") {
			//dopisac wstawianie variable name do zmiennej pomocniczej SetString2? czy cos takiego
		} else if (field2->getType() == "string"){
			//Dopisac zapisywanie nazwy tej zmiennej w setLines2 - czyli setString2 ??
		//	setString2 = field2->getValue();
					//pkb->getVarTable->getVarName(id..); ????
		}
	} else {
		if (field1->getType() == "stmt") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			setLines1.insert(pkb->getLineTable()->getOrderedIfLines().begin(),pkb->getLineTable()->getOrderedIfLines().end());
			//set<int> tmp = pkb->getLineTable()->getOrderedIfLines();
			//setLines1.insert(tmp.begin(), tmp.end());
			setLines1.insert(pkb->getLineTable()->getOrderedCallLines().begin(),pkb->getLineTable()->getOrderedCallLines().end());
			setLines1.insert(pkb->getLineTable()->getOrderedAssignLines().begin(),pkb->getLineTable()->getOrderedAssignLines().end());

		} else if (field1->getType() == "while") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
		} else if (field1->getType() == "if") {
			setLines1 = pkb->getLineTable()->getOrderedIfLines();
		} else if (field1->getType() == "call") {
			setLines1 = pkb->getLineTable()->getOrderedCallLines();
		} else if (field1->getType() == "assign") {
			setLines1 = pkb->getLineTable()->getOrderedAssignLines();
		} else if (field1->getType() == "procedure" ) {
			//Dopisac dodawanie procedure
		} else if (field1->getType() == "string"){
				//Dopisac dodawanie string
		}
		//Sprawdzanie drugiego parametru w relacji Modifies (moze to byc tylko variable lub var_name)
		if (field2->getType() == "variable" || field2->getType() == "any") {
			// Tutaj dopisac dodawanie variable do setString2
		} else if (field2->getType() == "string") {
			// tutaj do zmiennej setString2 wpierdzielic wartosc konkretna var_name
		}
	}

/*	vector<int> resultPart;
	// Sprawdzenie zaleznosci dla pobranych parametrow setLines1 oraz setLines2 i porównanie ich z wartosciami na lines (szukana wartosc)
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getModifies()->modifies(*l1, var name) == true) {
					if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						//return nullptr;
						return resultPart;
					} else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						resultPart.push_back(*l1);
					} else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
						// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
						resultPart.push_back(*l2);
					} else {
						// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoœci
						return lines;
					}
				}
			}
		}
	}*/
	return lines;
}

vector<int> QueryEvaluator::getParentResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
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
		if (pkb->getParent()->parent(param1, param2) == true) {
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

	// Skrócenie listy parametru 1 bior¹c pod uwagê czêœci zapytania z 'with'
	setLines1 = cutSetLines(field1->getValue(), setLines1);
	// Skrócenie listy parametru 2 bior¹c pod uwagê czêœci zapytania z 'with'
	setLines2 = cutSetLines(field2->getValue(), setLines2);

	vector<int> resultPart;
	// Sprawdzenie zaleznosci dla pobranych parametrow setLines1 oraz setLines2 i porównanie ich z wartosciami na lines (szukana wartosc)
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getParent()->parent(*l1, *l2) == true) {
					if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						//return nullptr;
						return resultPart;
					} else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						resultPart.push_back(*l1);
					} else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
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
			if (pkb->getParent()->parent(param1, param2) == true) {
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

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêœci zapytania z 'with'
		setLines1 = cutSetLines(field1->getValue(), setLines1);
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêœci zapytania z 'with'
		setLines2 = cutSetLines(field2->getValue(), setLines2);

		vector<int> resultPart;
		// Sprawdzenie zaleznosci dla pobranych parametrow setLines1 oraz setLines2 i porównanie ich z wartosciami na lines (szukana wartosc)
		if (!setLines1.empty() && !setLines2.empty()) {
			for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
				for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
					if (pkb->getParent()->parentStar(*l1, *l2) == true) {
						if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
							// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
							//return nullptr;
							return resultPart;
						} else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
							resultPart.push_back(*l1);
						} else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
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

vector<int> QueryEvaluator::getFollowsResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;

	//cout << "LINES" << endl;
	//for (int i = 0; i < lines.size(); i++)
	//	cout << lines[i] << " ";
	//cout << endl;

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
	} else if (field1->getType() == "constant" && field2->getType() == "constant") {
		int param1 = field1->getIntegerValue();
		int param2 = field2->getIntegerValue();
		if (pkb->getFollows()->follows(param1, param2) == true) {
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

	// Skrócenie listy parametru 1 bior¹c pod uwagê czêœci zapytania z 'with'
	setLines1 = cutSetLines(field1->getValue(), setLines1);
	// Skrócenie listy parametru 2 bior¹c pod uwagê czêœci zapytania z 'with'
	setLines2 = cutSetLines(field2->getValue(), setLines2);

	vector<int> resultPart;
	// Sprawdzenie czy wszystkie parametry by³y dobre, je¿eli nie return pusta lista - TZN. by³ b³¹d przy parsowaniu lub walidacji
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getFollows()->follows(*l1, *l2) == true) {
					if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						//cout << "-" << endl;
						return resultPart;
					} else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						//cout << "L1 " << *l1 << endl;
						resultPart.push_back(*l1);
					} else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
						// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
						//cout << "L2 " << *l2 << endl;
						resultPart.push_back(*l2);
					} else {
						// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoœci
						//cout << "ALL" << endl;
						return lines;
					}
				}
			}
		}
	}

	return resultPart;
}

vector<int> QueryEvaluator::getFollowsSResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	set<int> setLines1;
	set<int> setLines2;

	//cout << "LINES" << endl;
	//for (int i = 0; i < lines.size(); i++)
	//	cout << lines[i] << " ";
	//cout << endl;

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
	} else if (field1->getType() == "constant" && field2->getType() == "constant") {
		int param1 = field1->getIntegerValue();
		int param2 = field2->getIntegerValue();
		if (pkb->getFollows()->follows(param1, param2) == true) {
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

	/*
	for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1)
		cout << *l1 << " ";
	cout << endl;
	for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2)
		cout << *l2 << " ";
	cout << endl;
	*/

	// Skrócenie listy parametru 1 bior¹c pod uwagê czêœci zapytania z 'with'
	setLines1 = cutSetLines(field1->getValue(), setLines1);
	// Skrócenie listy parametru 2 bior¹c pod uwagê czêœci zapytania z 'with'
	setLines2 = cutSetLines(field2->getValue(), setLines2);

	vector<int> resultPart;
	// Sprawdzenie czy wszystkie parametry by³y dobre, je¿eli nie return pusta lista - TZN. by³ b³¹d przy parsowaniu lub walidacji
	if (!setLines1.empty() && !setLines2.empty()) {
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if (pkb->getFollows()->followsStar(*l1, *l2) == true) {
					//cout << "FOLLOWS TRUE" << endl;
					if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
						// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
						//cout << "-" << endl;
						return resultPart;
					} else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
						// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
						//cout << "L1 " << *l1 << endl;
						resultPart.push_back(*l1);
					} else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
						// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
						//cout << "L2 " << *l2 << endl;
						resultPart.push_back(*l2);
					} else {
						// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoœci
						//cout << "ALL" << endl;
						return lines;
					}
				}
			}
		}
	}

	return resultPart;
}

vector<int> QueryEvaluator::getUsesResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	return lines;
}

vector<int> QueryEvaluator::getUsesSResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	return lines;
}

vector<int> QueryEvaluator::getCallResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	//cout << "getCallResult" << endl;
	set<int> candidatesForParameter1;
	set<int> candidatesForParameter2;
	vector<int> resultPart;
	//cout << "getCallResult before getting fields types" << endl;
	if(field1 == nullptr || field2 == nullptr){
		//cout << "One of the fields is nullptr" << endl;
		return resultPart;
	} else {
		//cout << "Both fields are != nullptr" << endl;
	}
	string firstParameterType = field1->getType();
	string secondParameterType = field2->getType();
	//cout << "getCallResult after getting fields types: " << firstParameterType << " " << secondParameterType << endl;
	int firstProcedureId = -1;
	int secondProcedureId = -1;

	// tworze liste z mozliwymi wartosciami parametru pierwszego
	//cout << "firstParameterType" << endl;
	if (firstParameterType == "string") {
		// call("SuperProcedura",_)
		//cout << "call(\"SuperProcedura\",_)" << endl;
		firstProcedureId = pkbApi->getProcId(field1->getValue());
		if (firstProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter1.insert(firstProcedureId);
	} else if (firstParameterType == "constant"){
		// call(15,_)
		//cout << "call(15,_)" << endl;
		firstProcedureId = pkbApi->getProcId(field1->getIntegerValue());
		if (firstProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter1.insert(firstProcedureId);
	} else {
		// call(p,_)
		//cout << "call(p,_)" << endl;
		for( int i = 0; i < pkbApi->getMaxProcId(); i++){
			candidatesForParameter1.insert(i);
		}
	}

	// tworze liste z mozliwymi wartosciami parametru drugiego
	//cout << "secondParameterType" << endl;
	if (secondParameterType == "string") {
		// call(_,"SuperProcedura")
		//cout << "call(_,\"SuperProcedura\")" << endl;
		secondProcedureId = pkbApi->getProcId(field2->getValue());
		if (secondProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter2.insert(secondProcedureId);
	} else if (secondParameterType == "constant"){
		// call(_,123)
		//cout << "call(_,123)" << endl;
		secondProcedureId = pkbApi->getProcId(field2->getIntegerValue());
		if (secondProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter2.insert(secondProcedureId);
	} else {
		// call(_,p)
		//cout << "call(_,p)" << endl;
		for( int i = 0; i < pkbApi->getMaxProcId(); i++){
			candidatesForParameter2.insert(i);
		}
	}

	// Skrócenie listy parametru 1 bior¹c pod uwagê czêœci zapytania z 'with'
	candidatesForParameter1 = cutSetLines(field1->getValue(), candidatesForParameter1);
	// Skrócenie listy parametru 2 bior¹c pod uwagê czêœci zapytania z 'with'
	candidatesForParameter2 = cutSetLines(field2->getValue(), candidatesForParameter2);

	//cout << "candidatesForParameter1: " << candidatesForParameter1.size() << " candidatesForParameter2: " << candidatesForParameter2.size() << endl;

	if (firstParameterType == "constant" && secondParameterType == "constant") {
		if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
			return lines;
		}
		return resultPart;
	} else if (firstParameterType == "any" && secondParameterType == "any"){
		// any wiec jesli spelnia wymagania to wszystkie linie sa dobre
		if (pkbApi->calls(firstProcedureId, secondProcedureId)){
			return lines;
		}
		// jesli nie spelnia to wszystkie zle, nie ma procedur  w programie hahahah :D
		return resultPart;
	} else if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean"){
		// w zapytaniu dwie takie same wartosci, a nie sa to any wiec zwracam pusta, bo nie mozna wywolywac rekurencyjnie
		return resultPart;
	} else {
		for (set<int>::iterator parameter1 = candidatesForParameter1.begin(); parameter1 != candidatesForParameter1.end(); ++parameter1) {
			for (set<int>::iterator parameter2 = candidatesForParameter2.begin(); parameter2 != candidatesForParameter2.end(); ++parameter2) {
			    if(pkbApi->calls(*parameter1, *parameter2))
			    {
			    	if (selectValue == field1->getValue() && selectValue != "boolean") {
						// dodaje mozliwosc z par1 do wyniku gdy call(par1,*) gdzie * = 'any','const','var'
						resultPart.push_back(pkbApi->getProcStartLine(*parameter1));
					} else if (selectValue == field2->getValue() && selectValue != "boolean") {
						// dodaje mozliwosc z par2 do wyniku gdy call(*,par2) gdzie * = 'any','const','var'
						resultPart.push_back(pkbApi->getProcStartLine(*parameter2));
					} else {
						// zwracam wszystkie czy call(1,_) lub call(_,1)
						return lines;
					}
			    }
			}
		}
	}

	return resultPart;
}

vector<int> QueryEvaluator::getCallStarResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
	//cout << "getCallStarResult" << endl;
	set<int> candidatesForParameter1;
	set<int> candidatesForParameter2;
	vector<int> resultPart;
	//cout << "getCallResult before getting fields types" << endl;
	if(field1 == nullptr || field2 == nullptr){
		//cout << "One of the fields is nullptr" << endl;
		return resultPart;
	} else {
		//cout << "Both fields are != nullptr" << endl;
	}
	string firstParameterType = field1->getType();
	string secondParameterType = field2->getType();
	//cout << "getCallResult after getting fields types: " << firstParameterType << " " << secondParameterType << endl;
	int firstProcedureId = -1;
	int secondProcedureId = -1;

	// tworze liste z mozliwymi wartosciami parametru pierwszego
	//cout << "firstParameterType" << endl;
	if (firstParameterType == "string") {
		// call*("SuperProcedura",_)
		//cout << "call*(\"SuperProcedura\",_)" << endl;
		firstProcedureId = pkbApi->getProcId(field1->getValue());
		if (firstProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter1.insert(firstProcedureId);
	} else if (firstParameterType == "constant"){
		// call*(15,_)
		//cout << "call*(15,_)" << endl;
		firstProcedureId = pkbApi->getProcId(field1->getIntegerValue());
		if (firstProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter1.insert(firstProcedureId);
	} else {
		// call*(p,_)
		//cout << "call*(p,_)" << endl;
		for( int i = 0; i < pkbApi->getMaxProcId(); i++){
			candidatesForParameter1.insert(i);
		}
	}

	// tworze liste z mozliwymi wartosciami parametru drugiego
	//cout << "secondParameterType" << endl;
	if (secondParameterType == "string") {
		// call*(_,"SuperProcedura")
		//cout << "call*(_,\"SuperProcedura\")" << endl;
		secondProcedureId = pkbApi->getProcId(field2->getValue());
		if (secondProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter2.insert(secondProcedureId);
	} else if (secondParameterType == "constant"){
		// call*(_,123)
		//cout << "call*(_,123)" << endl;
		secondProcedureId = pkbApi->getProcId(field2->getIntegerValue());
		if (secondProcedureId == -1) {
			return resultPart;
		}
		candidatesForParameter2.insert(secondProcedureId);
	} else {
		// call*(_,p)
		//cout << "call*(_,p)" << endl;
		for( int i = 0; i < pkbApi->getMaxProcId(); i++){
			candidatesForParameter2.insert(i);
		}
	}

	//cout << "candidatesForParameter1: " << candidatesForParameter1.size() << " candidatesForParameter2: " << candidatesForParameter2.size() << endl;
	/*
	for (set<int>::iterator parameter1 = candidatesForParameter1.begin(); parameter1 != candidatesForParameter1.end(); ++parameter1) {
		cout << *parameter1 << " ";
	}
	cout << endl;
	for (set<int>::iterator parameter1 = candidatesForParameter2.begin(); parameter1 != candidatesForParameter2.end(); ++parameter1) {
		cout << *parameter1 << " ";
	}
	cout << endl;
	*/

	// Skrócenie listy parametru 1 bior¹c pod uwagê czêœci zapytania z 'with'
	candidatesForParameter1 = cutSetLines(field1->getValue(), candidatesForParameter1);
	// Skrócenie listy parametru 2 bior¹c pod uwagê czêœci zapytania z 'with'
	candidatesForParameter2 = cutSetLines(field2->getValue(), candidatesForParameter2);

	if (firstParameterType == "constant" && secondParameterType == "constant") {
		if (pkbApi->callsStar(firstProcedureId, secondProcedureId)) {
			return lines;
		}
		return resultPart;
	} else if (firstParameterType == "any" && secondParameterType == "any"){
		if (pkbApi->callsStar(firstProcedureId, secondProcedureId)){
			return lines;
		}
		return resultPart;
	} else if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean"){
		return resultPart;
	} else {
		for (set<int>::iterator parameter1 = candidatesForParameter1.begin(); parameter1 != candidatesForParameter1.end(); ++parameter1) {
			for (set<int>::iterator parameter2 = candidatesForParameter2.begin(); parameter2 != candidatesForParameter2.end(); ++parameter2) {
				if(pkbApi->callsStar(*parameter1, *parameter2))
				{
					if (selectValue == field1->getValue() && selectValue != "boolean") {
						// dodaje mozliwosc z par1 do wyniku gdy call(par1,*) gdzie * = 'any','const','var'
						resultPart.push_back(pkbApi->getProcStartLine(*parameter1));
					} else if (selectValue == field2->getValue() && selectValue != "boolean") {
						// dodaje mozliwosc z par2 do wyniku gdy call(*,par2) gdzie * = 'any','const','var'
						resultPart.push_back(pkbApi->getProcStartLine(*parameter2));
					} else {
						// zwracam wszystkie czy call(1,_) lub call(_,1)
						return lines;
					}
				}
			}
		}
	}

	return resultPart;
}

void QueryEvaluator::getWithResult(Field* field1, Field* field2, vector<int> lines) {

	/*
	 * stmt	(if,while,assign,call)			stmt# +
	 * assign								stmt# +
	 * while								stmt# +
	 * variable								varName +
	 * constant								value +
	 * prog_line							stmt# +
	 * if									stmt# +
	 * call									stmt# +
	 * procedure							procName +
	 * string
	 */


	set<int> setLines1;
	set<int> setLines2;
	string value1;
	string value2;

	if(field1->getType() == "stmt") {
		setLines1 = pkb->getLineTable()->getOrderedAssignLines();
		set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
		setLines1.insert(pom.begin(), pom.end());
		pom = pkb->getLineTable()->getOrderedIfLines();
		setLines1.insert(pom.begin(), pom.end());
		pom = pkb->getLineTable()->getOrderedCallLines();
		setLines1.insert(pom.begin(), pom.end());
	}
	else if(field1->getType() == "while") {
		setLines1 = pkb->getLineTable()->getOrderedWhileLines();
	}
	else if(field1->getType() == "if") {
		setLines1 = pkb->getLineTable()->getOrderedIfLines();
	}
	else if(field1->getType() == "call") {
		setLines1 = pkb->getLineTable()->getOrderedCallLines();
	}
	else if(field1->getType() == "assign") {
		setLines1 = pkb->getLineTable()->getOrderedAssignLines();
	}
	else if(field1->getType() == "prog_line") {
		vector<int> tmp1;
		tmp1 = pkb->getLineTable()->getLines();
		setLines1.insert(tmp1.begin(),tmp1.end());
	}
	else if(field1->getType() == "constant") {
		MatcherPQL* m = new MatcherPQL();
		if(m->isNumber(field1->getValue()))
		{
			int param = field1->getIntegerValue();
			setLines1.insert(param);
		}
		else
		{
			vector<int> tmp1;
			tmp1 = pkb->getLineTable()->getLines();
			setLines1.insert(tmp1.begin(),tmp1.end());
		}
		delete m;
	}
	else if(field1->getType() == "variable") {
		value1 = field1->getValue();
	} else if(field1->getType() == "procedure") {
		vector<int> tmp1;
		tmp1 = pkb->getProcTable()->getProceduresLines();
		setLines1.insert(tmp1.begin(),tmp1.end());
	} else if(field1->getType() == "string") {
		value1 = field1->getValue();
	}

	if(field2->getType() == "stmt") {
		setLines2 = pkb->getLineTable()->getOrderedAssignLines();
		set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
		setLines2.insert(pom.begin(), pom.end());
		pom = pkb->getLineTable()->getOrderedIfLines();
		setLines2.insert(pom.begin(), pom.end());
		pom = pkb->getLineTable()->getOrderedCallLines();
		setLines2.insert(pom.begin(), pom.end());
	}
	else if(field2->getType() == "while") {
		setLines2 = pkb->getLineTable()->getOrderedWhileLines();
	}
	else if(field2->getType() == "if") {
		setLines2 = pkb->getLineTable()->getOrderedIfLines();
	}
	else if(field2->getType() == "call") {
		setLines2 = pkb->getLineTable()->getOrderedCallLines();
	}
	else if(field2->getType() == "assign") {
		setLines2 = pkb->getLineTable()->getOrderedAssignLines();
	}
	else if(field2->getType() == "prog_line") {
		vector<int> tmp2;
		tmp2 = pkb->getLineTable()->getLines();
		setLines2.insert(tmp2.begin(),tmp2.end());
	}
	else if(field2->getType() == "constant") {
		MatcherPQL* m = new MatcherPQL();
		if(m->isNumber(field2->getValue()))
		{
			int param = field2->getIntegerValue();
			setLines2.insert(param);
		}
		else
		{
			vector<int> tmp2;
			tmp2 = pkb->getLineTable()->getLines();
			setLines2.insert(tmp2.begin(),tmp2.end());
		}
		delete m;
	}
	else if(field2->getType() == "variable") {
		value2 = field2->getValue();
	} else if(field2->getType() == "procedure") {
		vector<int> tmp2;
		tmp2 = pkb->getProcTable()->getProceduresLines();
		setLines2.insert(tmp2.begin(),tmp2.end());
	} else if(field2->getType() == "string") {
		value2 = field2->getValue();
	}

	/*
	cout << "Lista 1: ";
	if(!setLines1.empty())
	{
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1)
			cout << *l1 << " ";
		cout << endl;
	}
	else
		cout << value1 << endl;

	cout << "Lista 2: ";
	if(!setLines2.empty())
	{
		for (set<int>::iterator l1 = setLines2.begin(); l1 != setLines2.end(); ++l1)
			cout << *l1 << " ";
		cout << endl;
	}
	else
		cout << value2 << endl;
	*/

	vector<int> withLines;
	if(field1->getType() != "procedure" && field1->getType() != "variable" && field1->getType() != "string" &&
	   field2->getType() != "procedure" && field2->getType() != "variable" && field2->getType() != "string")
	{
		for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
			for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
				if(*l1 == *l2 && find(lines.begin(), lines.end(), *l1) != lines.end())
					withLines.push_back(*l1);
			}
		}
		map.insert(std::pair<string,vector<int>>(field1->getValue(),withLines));
		map.insert(std::pair<string,vector<int>>(field2->getValue(),withLines));
	}
	else if(field1->getType() == "procedure" && field2->getType() == "string")
	{
		int line = pkb->getProcTable()->getProcStartLine(pkb->getProcTable()->getProcId(value2));
		if(find(lines.begin(), lines.end(), line) != lines.end())
		{
			withLines.push_back(line);
		}
		map.insert(std::pair<string,vector<int>>(field1->getValue(),withLines));
	}
	else if(field2->getType() == "procedure" && field1->getType() == "string")
	{
		int line = pkb->getProcTable()->getProcStartLine(pkb->getProcTable()->getProcId(value1));
		if(find(lines.begin(), lines.end(), line) != lines.end())
		{
			withLines.push_back(line);
		}
		map.insert(std::pair<string,vector<int>>(field2->getValue(),withLines));
	}
	else if(field1->getType() == "variable" && field2->getType() == "string")
	{
		vector<int> l = pkb->getLineTable()->getLines();
		for(size_t i = 0; i < l.size() ; i++)
		{
			if(pkb->getUses()->uses(l[i],value2) && find(lines.begin(), lines.end(), l[i]) != lines.end()) {
				withLines.push_back(l[i]);
			}
		}
		map.insert(std::pair<string,vector<int>>(field1->getValue(),withLines));
	}
	else if(field2->getType() == "variable" && field1->getType() == "string")
	{
		vector<int> l = pkb->getLineTable()->getLines();
		for(size_t i = 0; i < l.size() ; i++)
		{
			if(pkb->getUses()->uses(l[i],value1) && find(lines.begin(), lines.end(), l[i]) != lines.end()) {
				withLines.push_back(l[i]);
			}
		}
		map.insert(std::pair<string,vector<int>>(field2->getValue(),withLines));
	}
	else if(field1->getType() == "string" && field2->getType() == "string" && value1 == value2)
	{
		vector<int> l = pkb->getLineTable()->getLines();
		for(size_t i = 0; i < l.size() ; i++)
		{
			if(find(lines.begin(), lines.end(), l[i]) != lines.end()) {
				withLines.push_back(l[i]);
			}
		}
		map.insert(std::pair<string,vector<int>>("all",withLines));
	}
	else if(field1->getType() == "string" && field2->getType() == "string" && value1 != value2)
	{
		map.insert(std::pair<string,vector<int>>("all",withLines));
	}
}


set<int> QueryEvaluator::cutSetLines(string fieldValue, set<int> setLines)
{
	vector<int> fieldMap = map[fieldValue];
	set<int> setLines1tmp;
	vector<int> allMap = map["all"];
	if(!allMap.empty())
	{
		if(!fieldMap.empty())
		{
			for (set<int>::iterator l1 = setLines.begin(); l1 != setLines.end(); ++l1) {
				if(find(fieldMap.begin(), fieldMap.end(), *l1) != fieldMap.end())
				{
					setLines1tmp.insert(*l1);
				}
			}
			setLines.clear();
			setLines.insert(setLines1tmp.begin(), setLines1tmp.end());
		}
	}
	else
	{
		setLines.clear();
	}

	return setLines;
}

} /* namespace std */
