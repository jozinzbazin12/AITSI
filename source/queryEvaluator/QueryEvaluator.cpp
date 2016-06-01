#include "QueryEvaluator.h"
#include "../globalVars.h"
#include <set>
#include <iterator>
#include <map>
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
		tree<tree_node_<PQLNode*>*>::iterator begin = Tree->Tree->begin();
		tree<tree_node_<PQLNode*>*>::iterator end = Tree->Tree->end();
		bool isModifies = false;
		bool isUses = false;

		/***** Wype³nienie pomocniczej usesLines i usesIDs *****/

		map<int, vector<int>> tmpUsesLines = pkb->getUses()->getAllUses();
		for (map<int, vector<int>>::iterator it = tmpUsesLines.begin(); it != tmpUsesLines.end(); ++it)
		{
			//usesIDs.insert((*it).first);
			vector<int> tmp = (*it).second;
			for (size_t i = 0; i < tmp.size(); i++)
			{
				//usesLines.insert(tmp[i]);
				//usesPairs.insert(std::pair<string,int>(std::make_pair(pkb->getVarTable()->getVarName((*it).first),tmp[i])));
			}
		}

		/*
		cout << "PAIRS " << endl;
		for (set<std::pair<string,int>>::iterator i = usesPairs.begin(); i != usesPairs.end(); ++i) {
		cout << (*i).first << " " << (*i).second << endl;
		}
		cout << endl;
		*/
		/***** Szukanie odpowiedzi przy pomocy relacji *****/

		while (begin != end) {
			//for (int i = 0; i < Tree->Tree->depth(end); ++i) {

			if (((*begin)->data->getType()) == "resultNode") {
				string s = (*begin)->data->getField1()->getType();
				resultType = s;
				if (s == "assign") {
					lines = pkb->getLineTable()->getAssignLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}
				else if (s == "while") {
					lines = pkb->getLineTable()->getWhileLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}
				else if (s == "variable") {
					lines = pkb->getLineTable()->getLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}
				else if (s == "prog_line") {
					lines = pkb->getLineTable()->getLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}
				else if (s == "procedure") {
					lines = pkbApi->getProceduresLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}
				else if (s == "stmt" || s == "boolean") {
					vector<int> tmp2;
					tmp2 = pkb->getLineTable()->getAssignLines();
					setLines.insert(tmp2.begin(), tmp2.end());
					tmp2 = pkb->getLineTable()->getWhileLines();
					setLines.insert(tmp2.begin(), tmp2.end());
					tmp2 = pkb->getLineTable()->getCallLines();
					setLines.insert(tmp2.begin(), tmp2.end());
					tmp2 = pkb->getLineTable()->getIfLines();
					setLines.insert(tmp2.begin(), tmp2.end());
					std::copy(setLines.begin(), setLines.end(), std::inserter(lines, lines.end()));
					selectValue = (*begin)->data->getField1()->getValue();
				}
				else if (s == "if") {
					lines = pkb->getLineTable()->getIfLines();
					selectValue = (*begin)->data->getField1()->getValue();
				}
			}

			if (((*begin)->data->getType()) == "suchNode") {
				//Sprawdzanie czy wystapila relacja Modifies
				if ((*begin)->data->getNodeType() == "modifies") {
					lines = getModifiesResult((*begin)->data->getField1(),
						(*begin)->data->getField2(), lines, selectValue);
					//cout << "Modifies" << endl;
					isModifies = true;
				}
				//Sprawdzanie czy wystapila relacja Parent lub Parent*
				if ((*begin)->data->getNodeType() == "parent") {
					//*
					if ((*begin)->data->isStar()) {
						lines = getParentSResult((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines,
							selectValue);
						//cout << "Parent*" << endl;
					}
					//zwykly Parent
					else {
						lines = getParentResult((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines,
							selectValue);
						//cout << "Parent" << endl;
					}
				}
				//Sprawdzanie czy wystapila relacja Follows lub Follows*
				if ((*begin)->data->getNodeType() == "follows") {
					//*
					if ((*begin)->data->isStar()) {
						lines = getFollowsSResult((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines,
							selectValue);
						//cout << "Follows*" << endl;
					}
					//zwykly Follows
					else {
						lines = getFollowsResult((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines,
							selectValue);
						//cout << "Follows" << endl;
					}
				}
				//Sprawdzanie czy wystapila relacja Uses lub Uses*
				if ((*begin)->data->getNodeType() == "uses") {
					//*
					if ((*begin)->data->isStar()) {
						//cout << "Uses*" << endl;
					}
					//zwykly Uses
					else {
						vector<int> tmpLines = getUsesResult((*begin)->data->getField1(), (*begin)->data->getField2(), lines, selectValue);
						for (size_t i = 0; i < tmpLines.size(); i++)
						{
							if (find(lines.begin(), lines.end(), tmpLines[i]) == lines.end())
								lines.push_back(tmpLines[i]);
						}

						//cout << "Uses" << endl;
						isUses = true;
					}
				}
				// Calls || Calls*
				if ((*begin)->data->getNodeType() == "calls") {
					if ((*begin)->data->isStar()) {
						lines = getCallStarResult((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines,
							selectValue);
						//cout << "Calls* " << endl;
					}
					else {
						lines = getCallResult((*begin)->data->getField1(),
							(*begin)->data->getField2(), lines,
							selectValue);
						//cout << "Calls " << endl;
					}
				}

				if ((*begin)->data->getNodeType() == "affects" || (*begin)->data->getNodeType() == "next") {
					vector<int> empty;
					lines = empty;
				}
			}

			if (((*begin)->data->getType()) == "withNode") {
				getWithResult((*begin)->data->getField1(), (*begin)->data->getField2(), lines);
			}
			//}
			++begin;
		}

		/*
		cout << "PAIRS 2 " << endl;
		for (set<std::pair<string,int>>::iterator i = usesPairs.begin(); i != usesPairs.end(); ++i) {
		cout << (*i).first << " " << (*i).second << endl;
		}
		cout << endl;
		*/

		cout << "WYNIK: ";
		result.clear();

		/*
		for(size_t i = 0 ; i < lines.size() ; i++)
		{
		cout << "---> " << lines[i] << endl;
		}
		*/
		if (isUses && !isModifies)
		{
			set<int> uL;
			for (set<std::pair<string, int>>::iterator l1 = usesPairs.begin(); l1 != usesPairs.end(); ++l1) {
				uL.insert((*l1).second);
			}

			vector<int> tmpL;
			for (size_t i = 0; i < lines.size(); i++) {
				if (find(uL.begin(), uL.end(), lines[i]) != uL.end() && find(tmpL.begin(), tmpL.end(), lines[i]) == tmpL.end())
					tmpL.push_back(lines[i]);
			}
			lines = tmpL;

			/*
			for(size_t i = 0 ; i < lines.size() ; i++)
			{
			cout << "LINES ---> " << lines[i] << endl;
			}
			*/
			cutUsesPairs(lines);
		}

		/*
		for (set<std::pair<string,int>>::iterator i = usesPairs.begin(); i != usesPairs.end(); ++i) {
		cout << "[PAIR] " << (*i).first << " " << (*i).second << endl;
		}
		*/

		if (!lines.empty())
		{
			if (resultType == "boolean")
			{
				result.push_back("true");
				//cout << "TRUE";
			}
			else
			{
				for (size_t i = 0; i < lines.size(); i++)
				{
					//cout << "LINIA: " << lines[i] << endl;
					if (resultType == "procedure")
					{
						string name = pkbApi->getProcName(pkbApi->getProcId(lines[i]));
						if (find(result.begin(), result.end(), name) >= result.begin())
							result.push_back(name);
						//cout << name << " ";
					}
					else if (resultType == "variable")
					{
						map<int, vector<int>> varUsesLines = pkb->getUses()->getAllUses();
						map<int, vector<int>> varModifiesLines = pkb->getModifies()->getAllModifies();
						vector<int> variableIds;
						if (withMap.count(selectValue) > 0) variableIds = withMap[selectValue];

						if (isUses && !isModifies) // Wyst¹pi³a tylko relacja -> Uses
						{
							for (map<int, vector<int>>::iterator it = varUsesLines.begin(); it != varUsesLines.end(); ++it)
							{
								vector<int> tmp = (*it).second;
								if (find(tmp.begin(), tmp.end(), lines[i]) != tmp.end())
								{
									string name = pkb->getVarTable()->getVarName((*it).first);
									std::pair<string, int> pair = { pkb->getVarTable()->getVarName((*it).first), lines[i] };

									if (find(result.begin(), result.end(), name) == result.end()
										&& (find(variableIds.begin(), variableIds.end(), (*it).first) != variableIds.end() || variableIds.empty())
										&& find(usesPairs.begin(), usesPairs.end(), pair) != usesPairs.end())
										result.push_back(name);
								}
							}
						}
						else if (!isUses && isModifies) // Wyst¹pi³a tylko relacja -> Modifies
						{
							for (map<int, vector<int>>::iterator it = varModifiesLines.begin(); it != varModifiesLines.end(); ++it)
							{
								vector<int> tmp = (*it).second;
								if (find(tmp.begin(), tmp.end(), lines[i]) != tmp.end())
								{
									string name = pkb->getVarTable()->getVarName((*it).first);
									if (find(result.begin(), result.end(), name) == result.end() && (find(variableIds.begin(), variableIds.end(), (*it).first) != variableIds.end() || variableIds.empty()))
										result.push_back(name);
								}
							}
						}
						else if (isUses && isModifies) // Wyst¹pi³y obie relacje -> Uses i Modifies
						{
							vector<string> resultUses;
							for (map<int, vector<int>>::iterator it = varUsesLines.begin(); it != varUsesLines.end(); ++it)
							{
								vector<int> tmp = (*it).second;
								if (find(tmp.begin(), tmp.end(), lines[i]) != tmp.end())
								{
									string name = pkb->getVarTable()->getVarName((*it).first);
									if (find(resultUses.begin(), resultUses.end(), name) == resultUses.end() && (find(variableIds.begin(), variableIds.end(), (*it).first) != variableIds.end() || variableIds.empty()))
										resultUses.push_back(name);
								}
							}

							vector<string> resultModifies;
							for (map<int, vector<int>>::iterator it = varModifiesLines.begin(); it != varModifiesLines.end(); ++it)
							{
								vector<int> tmp = (*it).second;
								if (find(tmp.begin(), tmp.end(), lines[i]) != tmp.end())
								{
									string name = pkb->getVarTable()->getVarName((*it).first);
									if (find(resultModifies.begin(), resultModifies.end(), name) == resultModifies.end() && (find(variableIds.begin(), variableIds.end(), (*it).first) != variableIds.end() || variableIds.empty()))
										resultModifies.push_back(name);
								}
							}

							for (size_t iU = 0; iU < resultUses.size(); iU++)
							{
								for (size_t iM = 0; iM < resultModifies.size(); iM++)
								{
									if (resultUses[iU] == resultModifies[iM] && find(result.begin(), result.end(), resultUses[iU]) == result.end())
									{
										result.push_back(resultUses[iU]);
									}
								}
							}
						}
					}
					else
					{
						stringstream ss;
						ss << lines[i];
						if (find(result.begin(), result.end(), ss.str()) >= result.begin())
							result.push_back(ss.str());
						//cout << lines[i] << " ";
					}
				}
			}
		}
		else
		{
			if (resultType == "boolean")
			{
				result.push_back("false");
				//cout << "FALSE";
			}
			else
			{
				//result.push_back("NONE");
				//cout << "NONE";
			}
		}

		cout << endl;

		return result;
	}

	vector<int> QueryEvaluator::getModifiesResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
		set<int> setLines1;
		set<int> setLines2;

		if (field1->getType() == "constant" && field2->getType() == "constant")
		{
			int param1 = field1->getIntegerValue();
			string param2 = pkb->getVarTable()->getVarName(field2->getIntegerValue());
			setLines1.insert(param1);
			if (!param2.empty())
			{
				setLines2.insert(field2->getIntegerValue());
			}
		}
		else if (field1->getType() == "constant" && field2->getType() != "constant")
		{
			int param1 = field1->getIntegerValue();
			setLines1.insert(param1);

			if (field2->getType() == "variable")
			{
				vector<string> tmp = pkb->getVarTable()->getAllVar();
				for (size_t i = 0; i < tmp.size(); i++)
				{
					setLines2.insert(pkb->getVarTable()->getVarId(tmp[i]));
				}
			}
			else if (field2->getType() == "string")
			{
				int param2 = pkb->getVarTable()->getVarId(field2->getValue());
				if (param2 != -1)
					setLines2.insert(param2);
			}
		}
		else if (field1->getType() != "constant" && field2->getType() == "constant")
		{
			//if
			//while
			//assign
			//procedure
			//call
			if (field1->getType() == "if")
			{
				map<int, vector<int>> ifLinesStart = pkb->getLineTable()->getIfBodyLines();

				for (map<int, vector<int>>::iterator it = ifLinesStart.begin(); it != ifLinesStart.end(); ++it) {
					vector<int> ifLines = (*it).second;
					//setLines1.insert((*it).first);
					for (size_t i = 0; i < ifLines.size(); i++)
					{
						setLines1.insert(ifLines[i]);
					}
				}
			}
			else if (field1->getType() == "while")
			{
				map<int, vector<int>> whileLinesStart = pkb->getLineTable()->getWhileBodyLines();

				for (map<int, vector<int>>::iterator it = whileLinesStart.begin(); it != whileLinesStart.end(); ++it) {
					vector<int> whileLines = (*it).second;
					//setLines1.insert((*it).first);
					for (size_t i = 0; i < whileLines.size(); i++)
					{
						setLines1.insert(whileLines[i]);
					}
				}
			}
			else if (field1->getType() == "assign")
			{
				setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			}
			else if (field1->getType() == "procedure")
			{
				map<int, vector<int>> procLinesStart = pkb->getProcTable()->getProceduresBodyLines();

				for (map<int, vector<int>>::iterator it = procLinesStart.begin(); it != procLinesStart.end(); ++it)
				{
					vector<int> procLines = (*it).second;
					int procParam = pkb->getProcTable()->getProcStartLine((*it).first);
					setLines1.insert(procParam);
					for (size_t i = 0; i < procLines.size(); i++)
					{
						setLines1.insert(procLines[i]);
					}
				}
			}
			else if (field1->getType() == "call")
			{
				vector<int> callLinesStart = pkb->getLineTable()->getCallLines();
				for (size_t i = 0; i < callLinesStart.size(); i++)
				{
					string procName = pkb->getLineTable()->getCalledProcName(callLinesStart[i]);
					int procId = pkb->getProcTable()->getProcId(procName);
					if (procId != -1)
					{
						vector<int> callLines = pkb->getProcTable()->getProcedureBodyLines(procId);

						for (size_t j = 0; j < callLines.size(); j++)
						{
							setLines1.insert(callLines[j]);
						}
					}
				}
			}
			else if (field1->getType() == "string")
			{
				int procId = pkb->getProcTable()->getProcId(field1->getValue());
				if (procId != -1)
				{
					vector<int> procLines = pkb->getProcTable()->getProcedureBodyLines(procId);

					for (size_t j = 0; j < procLines.size(); j++)
					{
						setLines1.insert(procLines[j]);
					}
				}
			}

			int param2 = field2->getIntegerValue();
			setLines2.insert(param2);
		}
		else
		{
			//Sprawdzenie atrybutu z pola 1
			if (field1->getType() == "if")
			{
				map<int, vector<int>> ifLinesStart = pkb->getLineTable()->getIfBodyLines();

				for (map<int, vector<int>>::iterator it = ifLinesStart.begin(); it != ifLinesStart.end(); ++it) {
					vector<int> ifLines = (*it).second;
					//setLines1.insert((*it).first);
					for (size_t i = 0; i < ifLines.size(); i++)
					{
						setLines1.insert(ifLines[i]);
					}
				}
			}
			else if (field1->getType() == "while")
			{
				map<int, vector<int>> whileLinesStart = pkb->getLineTable()->getWhileBodyLines();

				for (map<int, vector<int>>::iterator it = whileLinesStart.begin(); it != whileLinesStart.end(); ++it) {
					vector<int> whileLines = (*it).second;
					//setLines1.insert((*it).first);
					for (size_t i = 0; i < whileLines.size(); i++)
					{
						setLines1.insert(whileLines[i]);
					}
				}
			}
			else if (field1->getType() == "assign")
			{
				setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			}
			else if (field1->getType() == "procedure")
			{
				map<int, vector<int>> procLinesStart = pkb->getProcTable()->getProceduresBodyLines();

				for (map<int, vector<int>>::iterator it = procLinesStart.begin(); it != procLinesStart.end(); ++it)
				{
					vector<int> procLines = (*it).second;
					int procParam = pkb->getProcTable()->getProcStartLine((*it).first);
					setLines1.insert(procParam);
					for (size_t i = 0; i < procLines.size(); i++)
					{
						setLines1.insert(procLines[i]);
					}
				}
			}
			else if (field1->getType() == "call")
			{
				vector<int> callLinesStart = pkb->getLineTable()->getCallLines();
				for (size_t i = 0; i < callLinesStart.size(); i++)
				{
					string procName = pkb->getLineTable()->getCalledProcName(callLinesStart[i]);
					int procId = pkb->getProcTable()->getProcId(procName);
					if (procId != -1)
					{
						vector<int> callLines = pkb->getProcTable()->getProcedureBodyLines(procId);

						for (size_t j = 0; j < callLines.size(); j++)
						{
							setLines1.insert(callLines[j]);
						}
					}
				}
			}
			else if (field1->getType() == "string")
			{
				int procId = pkb->getProcTable()->getProcId(field1->getValue());
				if (procId != -1)
				{
					vector<int> procLines = pkb->getProcTable()->getProcedureBodyLines(procId);

					for (size_t j = 0; j < procLines.size(); j++)
					{
						setLines1.insert(procLines[j]);
					}
				}
			}

			//Sprawdzenie atrybutu z pola 2
			if (field2->getType() == "variable")
			{
				vector<string> tmp = pkb->getVarTable()->getAllVar();
				for (size_t i = 0; i < tmp.size(); i++)
				{
					setLines2.insert(pkb->getVarTable()->getVarId(tmp[i]));
				}
			}
			else if (field2->getType() == "string")
			{
				int param2 = pkb->getVarTable()->getVarId(field2->getValue());
				if (param2 != -1)
					setLines2.insert(param2);
			}
		}

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		if (field1->getType() != "constant")
			setLines1 = cutSetLines(field1->getValue(), setLines1);
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'

		/*
		set<int> tmpSetLines2;
		map<int,vector<int>> varLines = pkb -> getUses() ->getAllUses();
		for (set<int>::iterator l1 = setLines2.begin(); l1 != setLines2.end(); ++l1)
		{
		for (map<int, vector<int>>::iterator it = varLines.begin(); it != varLines.end(); ++it)
		{
		if(*l1 == (*it).first)
		{
		vector<int> tmp = (*it).second;
		for(size_t j = 0 ; j < tmp.size() ; j++)
		{
		tmpSetLines2.insert(tmp[j]);
		}
		}
		}
		}
		*/
		if (field2->getType() != "constant")
			setLines2 = cutSetLines(field2->getValue(), setLines2);

		/*
		setLines2.clear();
		for (set<int>::iterator l1 = tmpSetLines2.begin(); l1 != tmpSetLines2.end(); ++l1) {
		for (map<int, vector<int>>::iterator it = varLines.begin(); it != varLines.end(); ++it)
		{
		vector<int> tmp = (*it).second;
		for(size_t j = 0 ; j < tmp.size() ; j++)
		{
		if(tmp[j] == *l1 && find(setLines2.begin(),setLines2.end(), *l1) == setLines2.end())
		{
		setLines2.insert((*it).first);
		}
		}
		}
		}
		*/

		vector<int> resultPart;
		// Sprawdzenie czy wszystkie parametry by³y dobre, je¿eli nie return pusta lista - TZN. by³ b³¹d przy parsowaniu lub walidacji
		if (!setLines1.empty() && !setLines2.empty()) {
			for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
				for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
					if (pkb->getModifies()->modifies(*l1, pkb->getVarTable()->getVarName(*l2)) == true) {
						if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
							// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
							//cout << "-" << endl;
							return resultPart;
						}
						else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
							//cout << "L1 " << *l1 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
								resultPart.push_back(*l1);
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
							//cout << "L2 " << *l2 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
								resultPart.push_back(*l1);
						}
						else {
							// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoci
							//cout << "ALL" << endl;
							return lines;
						}
					}
				}
			}
		}

		return resultPart;
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
				setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}
		else if (field1->getType() != "constant"
			&& field2->getType() == "constant") {
			int param = field2->getIntegerValue();
			setLines2.insert(param);
			if (field1->getType() == "stmt" || field1->getType() == "any") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
				set<int> tmp = pkb->getLineTable()->getOrderedIfLines();
				setLines1.insert(tmp.begin(), tmp.end());
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
				setLines1 = pkb->getLineTable()->getOrderedIfLines();
			}
		}
		else if (field1->getType() == "constant"
			&& field2->getType() == "constant") {
			int param1 = field1->getIntegerValue();
			int param2 = field1->getIntegerValue();
			if (pkb->getParent()->parent(param1, param2) == true) {
				return lines;
			}
		}
		else {
			if (field1->getType() == "stmt" || field1->getType() == "any") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
				set<int> tmp = pkb->getLineTable()->getOrderedIfLines();
				setLines1.insert(tmp.begin(), tmp.end());
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
				setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		//for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) cout << "(1.1) " << *l1 << endl;
		if (field1->getType() != "constant" && field1->getType() != "any")
			setLines1 = cutSetLines(field1->getValue(), setLines1);
		//for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) cout << "(1.2) " << *l1 << endl;
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'
		//for (set<int>::iterator l1 = setLines2.begin(); l1 != setLines2.end(); ++l1) cout << "(2.1) " << *l1 << endl;
		if (field2->getType() != "constant" && field2->getType() != "any")
			setLines2 = cutSetLines(field2->getValue(), setLines2);
		//for (set<int>::iterator l1 = setLines2.begin(); l1 != setLines2.end(); ++l1) cout << "(2.2) " << *l1 << endl;

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
						}
						else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
								resultPart.push_back(*l1);
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
							// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
							if (find(resultPart.begin(), resultPart.end(), *l2) >= resultPart.end())
								resultPart.push_back(*l2);
						}
						else {
							// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoci
							return lines;
						}
					}
				}
			}
		}
		return resultPart;
	}

	vector<int> QueryEvaluator::getParentSResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
				setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}
		else if (field1->getType() != "constant"
			&& field2->getType() == "constant") {
			int param = field2->getIntegerValue();
			setLines2.insert(param);
			if (field1->getType() == "stmt" || field1->getType() == "any") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
				set<int> tmp = pkb->getLineTable()->getOrderedIfLines();
				setLines1.insert(tmp.begin(), tmp.end());
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
				setLines1 = pkb->getLineTable()->getOrderedIfLines();
			}
		}
		else if (field1->getType() == "constant"
			&& field2->getType() == "constant") {
			int param1 = field1->getIntegerValue();
			int param2 = field1->getIntegerValue();
			if (pkb->getParent()->parent(param1, param2) == true) {
				return lines;
			}
		}
		else {
			if (field1->getType() == "stmt" || field1->getType() == "any") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
				set<int> tmp = pkb->getLineTable()->getOrderedIfLines();
				setLines1.insert(tmp.begin(), tmp.end());
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
				setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		if (field1->getType() != "constant" && field1->getType() != "any")
			setLines1 = cutSetLines(field1->getValue(), setLines1);
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'
		if (field2->getType() != "constant" && field2->getType() != "any")
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
						}
						else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
								resultPart.push_back(*l1);
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
							// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
							if (find(resultPart.begin(), resultPart.end(), *l2) >= resultPart.end())
								resultPart.push_back(*l2);
						}
						else {
							// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoci
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
			//Dodanie wartoci constant do listy 1
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
				setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}
		else if (field1->getType() != "constant"
			&& field2->getType() == "constant") {
			int param = field2->getIntegerValue();
			//Dodanie wartoci constant do listy 2
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
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
				setLines1 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field1->getType() == "call") {
				setLines1 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field1->getType() == "assign") {
				setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}
		else if (field1->getType() == "constant" && field2->getType() == "constant") {
			int param1 = field1->getIntegerValue();
			int param2 = field2->getIntegerValue();
			if (pkb->getFollows()->follows(param1, param2) == true) {
				return lines;
			}
		}
		else {
			// Pobranie listy dla pierwszego parametru
			// Any - TZN. dowolna wartoæ z dostêpnych czyli stmt
			if (field1->getType() == "stmt" || field1->getType() == "any") {
				setLines1 = pkb->getLineTable()->getOrderedAssignLines();
				set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
				setLines1.insert(pom.begin(), pom.end());
				pom = pkb->getLineTable()->getOrderedIfLines();
				setLines1.insert(pom.begin(), pom.end());
				pom = pkb->getLineTable()->getOrderedCallLines();
				setLines1.insert(pom.begin(), pom.end());
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
				setLines1 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field1->getType() == "call") {
				setLines1 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field1->getType() == "assign") {
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
				setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		if (field1->getType() != "constant" && field1->getType() != "any")
			setLines1 = cutSetLines(field1->getValue(), setLines1);
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'
		if (field2->getType() != "constant" && field2->getType() != "any")
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
						}
						else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
							//cout << "L1 " << *l1 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
								resultPart.push_back(*l1);
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
							// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
							//cout << "L2 " << *l2 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l2) >= resultPart.end())
								resultPart.push_back(*l2);
						}
						else {
							// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoci
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
			//Dodanie wartoci constant do listy 1
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
				setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}
		else if (field1->getType() != "constant"
			&& field2->getType() == "constant") {
			int param = field2->getIntegerValue();
			//Dodanie wartoci constant do listy 2
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
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
				setLines1 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field1->getType() == "call") {
				setLines1 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field1->getType() == "assign") {
				setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			}
		}
		else if (field1->getType() == "constant" && field2->getType() == "constant") {
			int param1 = field1->getIntegerValue();
			int param2 = field2->getIntegerValue();
			if (pkb->getFollows()->follows(param1, param2) == true) {
				return lines;
			}
		}
		else {
			// Pobranie listy dla pierwszego parametru
			// Any - TZN. dowolna wartoæ z dostêpnych czyli stmt
			if (field1->getType() == "stmt" || field1->getType() == "any") {
				setLines1 = pkb->getLineTable()->getOrderedAssignLines();
				set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
				setLines1.insert(pom.begin(), pom.end());
				pom = pkb->getLineTable()->getOrderedIfLines();
				setLines1.insert(pom.begin(), pom.end());
				pom = pkb->getLineTable()->getOrderedCallLines();
				setLines1.insert(pom.begin(), pom.end());
			}
			else if (field1->getType() == "while") {
				setLines1 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field1->getType() == "if") {
				setLines1 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field1->getType() == "call") {
				setLines1 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field1->getType() == "assign") {
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
			}
			else if (field2->getType() == "while") {
				setLines2 = pkb->getLineTable()->getOrderedWhileLines();
			}
			else if (field2->getType() == "if") {
				setLines2 = pkb->getLineTable()->getOrderedIfLines();
			}
			else if (field2->getType() == "call") {
				setLines2 = pkb->getLineTable()->getOrderedCallLines();
			}
			else if (field2->getType() == "assign") {
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

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		if (field1->getType() != "constant" && field1->getType() != "any")
			setLines1 = cutSetLines(field1->getValue(), setLines1);
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'
		if (field2->getType() != "constant" && field2->getType() != "any")
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
						}
						else if (selectValue == field1->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l1) != lines.end()) {
							// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
							//cout << "L1 " << *l1 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
								resultPart.push_back(*l1);
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean" && find(lines.begin(), lines.end(), *l2) != lines.end()) {
							// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
							//cout << "L2 " << *l2 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l2) >= resultPart.end())
								resultPart.push_back(*l2);
						}
						else {
							// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoci
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
		set<int> setLines1;
		set<int> setLines2;

		if (field1->getType() == "constant" && field2->getType() != "constant")
		{
			int param1 = field1->getIntegerValue();
			setLines1.insert(param1);

			if (field2->getType() == "variable" || field2->getType() == "any")
			{
				vector<string> tmp = pkb->getVarTable()->getAllVar();
				for (size_t i = 0; i < tmp.size(); i++)
				{
					setLines2.insert(pkb->getVarTable()->getVarId(tmp[i]));
				}
			}
			else if (field2->getType() == "string")
			{
				int param2 = pkb->getVarTable()->getVarId(field2->getValue());
				if (param2 != -1)
					setLines2.insert(param2);
			}
		}
		else if (field1->getType() != "constant" && field2->getType() != "constant")
		{
			//Sprawdzenie atrybutu z pola 1
			if (field1->getType() == "if")
			{
				map<int, vector<int>> ifLinesStart = pkb->getLineTable()->getIfBodyLines();

				for (map<int, vector<int>>::iterator it = ifLinesStart.begin(); it != ifLinesStart.end(); ++it) {
					vector<int> ifLines = (*it).second;
					setLines1.insert((*it).first);
					for (size_t i = 0; i < ifLines.size(); i++)
					{
						setLines1.insert(ifLines[i]);
					}
				}
			}
			else if (field1->getType() == "while")
			{
				map<int, vector<int>> whileLinesStart = pkb->getLineTable()->getWhileBodyLines();

				for (map<int, vector<int>>::iterator it = whileLinesStart.begin(); it != whileLinesStart.end(); ++it) {
					vector<int> whileLines = (*it).second;
					setLines1.insert((*it).first);
					for (size_t i = 0; i < whileLines.size(); i++)
					{
						setLines1.insert(whileLines[i]);
					}
				}
			}
			else if (field1->getType() == "assign")
			{
				setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			}
			else if (field1->getType() == "procedure")
			{
				map<int, vector<int>> procLinesStart = pkb->getProcTable()->getProceduresBodyLines();

				for (map<int, vector<int>>::iterator it = procLinesStart.begin(); it != procLinesStart.end(); ++it)
				{
					vector<int> procLines = (*it).second;
					int procParam = pkb->getProcTable()->getProcStartLine((*it).first);
					setLines1.insert(procParam);
					for (size_t i = 0; i < procLines.size(); i++)
					{
						setLines1.insert(procLines[i]);
					}
				}
			}
			else if (field1->getType() == "call")
			{
				vector<int> callLinesStart = pkb->getLineTable()->getCallLines();
				for (size_t i = 0; i < callLinesStart.size(); i++)
				{
					string procName = pkb->getLineTable()->getCalledProcName(callLinesStart[i]);
					int procId = pkb->getProcTable()->getProcId(procName);
					if (procId != -1)
					{
						vector<int> callLines = pkb->getProcTable()->getProcedureBodyLines(procId);

						for (size_t j = 0; j < callLines.size(); j++)
						{
							setLines1.insert(callLines[j]);
						}
					}
				}
			}
			else if (field1->getType() == "string")
			{
				int procId = pkb->getProcTable()->getProcId(field1->getValue());
				if (procId != -1)
				{
					vector<int> procLines = pkb->getProcTable()->getProcedureBodyLines(procId);

					for (size_t j = 0; j < procLines.size(); j++)
					{
						setLines1.insert(procLines[j]);
					}
				}
			}
			else if (field1->getType() == "any" || field1->getType() == "stmt")
			{
				vector<int> allLines = pkb->getLineTable()->getLines();
				for (size_t j = 0; j < allLines.size(); j++)
				{
					setLines1.insert(allLines[j]);
				}
			}

			//Sprawdzenie atrybutu z pola 2
			if (field2->getType() == "variable" || field2->getType() == "any")
			{
				vector<string> tmp = pkb->getVarTable()->getAllVar();
				for (size_t i = 0; i < tmp.size(); i++)
				{
					setLines2.insert(pkb->getVarTable()->getVarId(tmp[i]));
				}
			}
			else if (field2->getType() == "string")
			{
				int param2 = pkb->getVarTable()->getVarId(field2->getValue());
				if (param2 != -1)
					setLines2.insert(param2);
			}
		}

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		if (field1->getType() != "constant" && field1->getType() != "any")
			setLines1 = cutSetLines(field1->getValue(), setLines1);
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'
		if (field2->getType() != "constant" && field2->getType() != "any")
			setLines2 = cutSetLines(field2->getValue(), setLines2);

		vector<int> resultPart;
		// Sprawdzenie czy wszystkie parametry by³y dobre, je¿eli nie return pusta lista - TZN. by³ b³¹d przy parsowaniu lub walidacji
		if (!setLines1.empty() && !setLines2.empty()) {
			for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
				for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
					if (pkb->getUses()->uses(*l1, pkb->getVarTable()->getVarName(*l2)) == true) {

						cout << "USES: " << pkb->getVarTable()->getVarName(*l2) << " --- " << *l1 << endl;
						usesPairs.insert(std::pair<string, int>(std::make_pair(pkb->getVarTable()->getVarName((*l2)), *l1)));

						if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
							// Je¿eli oba parametry s¹ takie same a nie s¹ to constant to znaczy ¿e nie ma odpowiedzi
							//cout << "-" << endl;
							return resultPart;
						}
						else if (selectValue == field1->getValue() && selectValue != "boolean") {
							// Je¿eli pierwszy parametr jest tym którego szukamy to wybieramy z listy pierwszej
							//cout << "L1 " << *l1 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
							{
								resultPart.push_back(*l1);
							}
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean") {
							// Je¿eli drugi parametr jest tym którego szukamy to wybieramy z listy drugiej
							//cout << "L2 " << *l2 << endl;
							if (find(resultPart.begin(), resultPart.end(), *l1) >= resultPart.end())
							{
								resultPart.push_back(*l1);
							}
						}
						else {
							// Je¿eli ¿aden parametr nie jest tym którego szukamy to zwracamy wszystkie wartoci
							//cout << "ALL" << endl;
							return lines;
						}
					}
				}
			}
		}

		return resultPart;
	}

	vector<int> QueryEvaluator::getCallResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
		set<int> candidatesForParameter1;
		set<int> candidatesForParameter2;
		set<int> resultPart;
		vector<int> returnIt;
		if (field1 == nullptr || field2 == nullptr) {
			return returnIt;
		}
		string firstParameterType = field1->getType();
		string secondParameterType = field2->getType();
		int firstProcedureId = -1;
		int secondProcedureId = -1;

		// tworze liste z mozliwymi wartosciami parametru pierwszego
		if (firstParameterType == "string") {
			// call("SuperProcedura",_)
			firstProcedureId = pkbApi->getProcId(field1->getValue());
			if (firstProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter1.insert(firstProcedureId);
		}
		else if (firstParameterType == "constant") {
			// call(15,_)
			firstProcedureId = pkbApi->getProcId(field1->getIntegerValue());
			if (firstProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter1.insert(firstProcedureId);
		}
		else {
			// call(p,_)
			for (int i = 0; i <= pkbApi->getMaxProcId(); i++) {
				candidatesForParameter1.insert(i);
			}
		}

		// tworze liste z mozliwymi wartosciami parametru drugiego
		if (secondParameterType == "string") {
			// call(_,"SuperProcedura")
			secondProcedureId = pkbApi->getProcId(field2->getValue());
			if (secondProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter2.insert(secondProcedureId);
		}
		else if (secondParameterType == "constant") {
			// call(_,123)
			secondProcedureId = pkbApi->getProcId(field2->getIntegerValue());
			if (secondProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter2.insert(secondProcedureId);
		}
		else {
			// call(_,p)
			for (int i = 0; i <= pkbApi->getMaxProcId(); i++) {
				candidatesForParameter2.insert(i);
			}
		}

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		if (field1->getType() != "constant" && field1->getType() != "any") {
			candidatesForParameter1 = cutSetLines(field1->getValue(), candidatesForParameter1);
		}
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'
		if (field2->getType() != "constant" && field2->getType() != "any") {
			candidatesForParameter2 = cutSetLines(field2->getValue(), candidatesForParameter2);
		}

		if (firstParameterType == "constant" && secondParameterType == "constant") {
			if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				resultPart.insert(pkbApi->getProcStartLine(firstProcedureId));
			}
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else if (firstParameterType == "string"  && secondParameterType == "string") {
			if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				resultPart.insert(pkbApi->getProcStartLine(firstProcedureId));
			}
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else if (firstParameterType == "any" && secondParameterType == "any") {
			// any wiec jesli spelnia wymagania to wszystkie linie sa dobre
			if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				for (int i = 0; i <= pkbApi->getMaxProcId(); i++) {
					resultPart.insert(pkbApi->getProcStartLine(i));
				}
			}
			// jesli nie spelnia to wszystkie zle, nie ma procedur  w programie hahahah :D
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
			// w zapytaniu dwie takie same wartosci, a nie sa to any wiec zwracam pusta, bo nie mozna wywolywac rekurencyjnie
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else {
			for (set<int>::iterator parameter1 = candidatesForParameter1.begin(); parameter1 != candidatesForParameter1.end(); ++parameter1) {
				for (set<int>::iterator parameter2 = candidatesForParameter2.begin(); parameter2 != candidatesForParameter2.end(); ++parameter2) {
					if (pkbApi->calls(*parameter1, *parameter2) && *parameter1 != *parameter2)
					{
						if (selectValue == field1->getValue() && selectValue != "boolean") {
							// dodaje mozliwosc z par1 do wyniku gdy call(par1,*) gdzie * = 'any','const','var'
							if (find(resultPart.begin(), resultPart.end(), pkbApi->getProcStartLine(*parameter1)) == resultPart.end())
								resultPart.insert(pkbApi->getProcStartLine(*parameter1));
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean") {
							// dodaje mozliwosc z par2 do wyniku gdy call(*,par2) gdzie * = 'any','const','var'
							if (find(resultPart.begin(), resultPart.end(), pkbApi->getProcStartLine(*parameter2)) == resultPart.end())
								resultPart.insert(pkbApi->getProcStartLine(*parameter2));
						}
						else {
							// zwracam wszystkie czy call(1,_) lub call(_,1)
							if (field1->getType() != "constant" && field1->getType() != "string") {
								resultPart.insert(pkbApi->getProcStartLine(*parameter1));
							}
							if (field2->getType() != "constant" && field2->getType() != "string") {
								resultPart.insert(pkbApi->getProcStartLine(*parameter2));
							}
						}
					}
				}
			}
		}
		copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
		return returnIt;
	}

	vector<int> QueryEvaluator::getCallStarResult(Field* field1, Field* field2, vector<int> lines, string selectValue) {
		//cout << "getCallStarResult" << endl;
		set<int> candidatesForParameter1;
		set<int> candidatesForParameter2;
		set<int> resultPart;
		vector<int> returnIt;
		//cout << "getCallResult before getting fields types" << endl;
		if (field1 == nullptr || field2 == nullptr) {
			//cout << "One of the fields is nullptr" << endl;
			return returnIt;
		}
		else {
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
				return returnIt;
			}
			candidatesForParameter1.insert(firstProcedureId);
		}
		else if (firstParameterType == "constant") {
			// call*(15,_)
			//cout << "call*(15,_)" << endl;
			firstProcedureId = pkbApi->getProcId(field1->getIntegerValue());
			if (firstProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter1.insert(firstProcedureId);
		}
		else {
			// call*(p,_)
			//cout << "call*(p,_)" << endl;
			for (int i = 0; i < pkbApi->getMaxProcId(); i++) {
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
				return returnIt;
			}
			candidatesForParameter2.insert(secondProcedureId);
		}
		else if (secondParameterType == "constant") {
			// call*(_,123)
			//cout << "call*(_,123)" << endl;
			secondProcedureId = pkbApi->getProcId(field2->getIntegerValue());
			if (secondProcedureId == -1) {
				return returnIt;
			}
			candidatesForParameter2.insert(secondProcedureId);
		}
		else {
			// call*(_,p)
			//cout << "call*(_,p)" << endl;
			for (int i = 0; i < pkbApi->getMaxProcId(); i++) {
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

		// Skrócenie listy parametru 1 bior¹c pod uwagê czêci zapytania z 'with'
		if (field1->getType() != "constant" && field1->getType() != "any") {
			candidatesForParameter1 = cutSetLines(field1->getValue(), candidatesForParameter1);
		}
		// Skrócenie listy parametru 2 bior¹c pod uwagê czêci zapytania z 'with'
		if (field2->getType() != "constant" && field2->getType() != "any") {
			candidatesForParameter2 = cutSetLines(field2->getValue(), candidatesForParameter2);
		}

		if (firstParameterType == "constant" && secondParameterType == "constant") {
			if (pkbApi->callsStar(firstProcedureId, secondProcedureId)) {
				resultPart.insert(pkbApi->getProcStartLine(firstProcedureId));
			}
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else if (firstParameterType == "string"  && secondParameterType == "string") {
			if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				resultPart.insert(pkbApi->getProcStartLine(firstProcedureId));
			}
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else if (firstParameterType == "any" && secondParameterType == "any") {
			if (pkbApi->calls(firstProcedureId, secondProcedureId)) {
				for (int i = 0; i <= pkbApi->getMaxProcId(); i++) {
					resultPart.insert(pkbApi->getProcStartLine(i));
				}
			}
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else if (selectValue == field1->getValue() && selectValue == field2->getValue() && selectValue != "boolean") {
			copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
			return returnIt;
		}
		else {
			for (set<int>::iterator parameter1 = candidatesForParameter1.begin(); parameter1 != candidatesForParameter1.end(); ++parameter1) {
				for (set<int>::iterator parameter2 = candidatesForParameter2.begin(); parameter2 != candidatesForParameter2.end(); ++parameter2) {
					if (pkbApi->callsStar(*parameter1, *parameter2) && *parameter1 != *parameter2)
					{
						if (selectValue == field1->getValue() && selectValue != "boolean") {
							// dodaje mozliwosc z par1 do wyniku gdy call(par1,*) gdzie * = 'any','const','var'
							if (find(resultPart.begin(), resultPart.end(), pkbApi->getProcStartLine(*parameter1)) == resultPart.end())
								resultPart.insert(pkbApi->getProcStartLine(*parameter1));
						}
						else if (selectValue == field2->getValue() && selectValue != "boolean") {
							// dodaje mozliwosc z par2 do wyniku gdy call(*,par2) gdzie * = 'any','const','var'
							if (find(resultPart.begin(), resultPart.end(), pkbApi->getProcStartLine(*parameter2)) == resultPart.end())
								resultPart.insert(pkbApi->getProcStartLine(*parameter2));
						}
						else {
							// zwracam wszystkie czy call(1,_) lub call(_,1)
							if (field1->getType() != "constant" && field1->getType() != "string") {
								resultPart.insert(pkbApi->getProcStartLine(*parameter1));
							}
							if (field2->getType() != "constant" && field2->getType() != "string") {
								resultPart.insert(pkbApi->getProcStartLine(*parameter2));
							}
						}
					}
				}
			}
		}
		copy(resultPart.begin(), resultPart.end(), back_inserter(returnIt));
		return returnIt;
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

		if (field1->getType() == "stmt") {
			setLines1 = pkb->getLineTable()->getOrderedAssignLines();
			set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedIfLines();
			setLines1.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedCallLines();
			setLines1.insert(pom.begin(), pom.end());
		}
		else if (field1->getType() == "while") {
			setLines1 = pkb->getLineTable()->getOrderedWhileLines();
		}
		else if (field1->getType() == "if") {
			setLines1 = pkb->getLineTable()->getOrderedIfLines();
		}
		else if (field1->getType() == "call") {
			setLines1 = pkb->getLineTable()->getOrderedCallLines();
		}
		else if (field1->getType() == "assign") {
			setLines1 = pkb->getLineTable()->getOrderedAssignLines();
		}
		else if (field1->getType() == "prog_line") {
			vector<int> tmp1;
			tmp1 = pkb->getLineTable()->getLines();
			setLines1.insert(tmp1.begin(), tmp1.end());
		}
		else if (field1->getType() == "constant") {
			MatcherPQL* m = new MatcherPQL();
			if (m->isNumber(field1->getValue()))
			{
				int param = field1->getIntegerValue();
				setLines1.insert(param);
			}
			else
			{
				vector<int> tmp1;
				tmp1 = pkb->getLineTable()->getLines();
				setLines1.insert(tmp1.begin(), tmp1.end());
			}
			delete m;
		}
		else if (field1->getType() == "variable") {
			value1 = field1->getValue();
		}
		else if (field1->getType() == "procedure") {
			vector<int> tmp1;
			tmp1 = pkb->getProcTable()->getProceduresLines();
			setLines1.insert(tmp1.begin(), tmp1.end());
		}
		else if (field1->getType() == "string") {
			value1 = field1->getValue();
		}

		if (field2->getType() == "stmt") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
			set<int> pom = pkb->getLineTable()->getOrderedWhileLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedIfLines();
			setLines2.insert(pom.begin(), pom.end());
			pom = pkb->getLineTable()->getOrderedCallLines();
			setLines2.insert(pom.begin(), pom.end());
		}
		else if (field2->getType() == "while") {
			setLines2 = pkb->getLineTable()->getOrderedWhileLines();
		}
		else if (field2->getType() == "if") {
			setLines2 = pkb->getLineTable()->getOrderedIfLines();
		}
		else if (field2->getType() == "call") {
			setLines2 = pkb->getLineTable()->getOrderedCallLines();
		}
		else if (field2->getType() == "assign") {
			setLines2 = pkb->getLineTable()->getOrderedAssignLines();
		}
		else if (field2->getType() == "prog_line") {
			vector<int> tmp2;
			tmp2 = pkb->getLineTable()->getLines();
			setLines2.insert(tmp2.begin(), tmp2.end());
		}
		else if (field2->getType() == "constant") {
			MatcherPQL* m = new MatcherPQL();
			if (m->isNumber(field2->getValue()))
			{
				int param = field2->getIntegerValue();
				setLines2.insert(param);
			}
			else
			{
				vector<int> tmp2;
				tmp2 = pkb->getLineTable()->getLines();
				setLines2.insert(tmp2.begin(), tmp2.end());
			}
			delete m;
		}
		else if (field2->getType() == "variable") {
			value2 = field2->getValue();
		}
		else if (field2->getType() == "procedure") {
			vector<int> tmp2;
			tmp2 = pkb->getProcTable()->getProceduresLines();
			setLines2.insert(tmp2.begin(), tmp2.end());
		}
		else if (field2->getType() == "string") {
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
		if (field1->getType() != "procedure" && field1->getType() != "variable" && field1->getType() != "string" &&
			field2->getType() != "procedure" && field2->getType() != "variable" && field2->getType() != "string")
		{
			for (set<int>::iterator l1 = setLines1.begin(); l1 != setLines1.end(); ++l1) {
				for (set<int>::iterator l2 = setLines2.begin(); l2 != setLines2.end(); ++l2) {
					if (*l1 == *l2 && find(lines.begin(), lines.end(), *l1) != lines.end())
						withLines.push_back(*l1);
				}
			}
			withMap.insert(std::pair<string, vector<int>>(field1->getValue(), withLines));
			withMap.insert(std::pair<string, vector<int>>(field2->getValue(), withLines));
		}
		else if (field1->getType() == "procedure" && field2->getType() == "string")
		{
			int line = pkb->getProcTable()->getProcStartLine(pkb->getProcTable()->getProcId(value2));
			if (find(lines.begin(), lines.end(), line) != lines.end())
			{
				withLines.push_back(line);
			}
			withMap.insert(std::pair<string, vector<int>>(field1->getValue(), withLines));
		}
		else if (field2->getType() == "procedure" && field1->getType() == "string")
		{
			int line = pkb->getProcTable()->getProcStartLine(pkb->getProcTable()->getProcId(value1));
			if (find(lines.begin(), lines.end(), line) != lines.end())
			{
				withLines.push_back(line);
			}
			withMap.insert(std::pair<string, vector<int>>(field2->getValue(), withLines));
		}
		else if (field1->getType() == "variable" && field2->getType() == "string")
		{
			vector<int> l = pkb->getLineTable()->getLines();
			for (size_t i = 0; i < l.size(); i++)
			{
				if (pkb->getUses()->uses(l[i], value2) && find(lines.begin(), lines.end(), l[i]) != lines.end()) {
					int param2 = pkb->getVarTable()->getVarId(value2);
					if (param2 != -1)
						withLines.push_back(param2);
				}
			}
			withMap.insert(std::pair<string, vector<int>>(field1->getValue(), withLines));
		}
		else if (field2->getType() == "variable" && field1->getType() == "string")
		{
			vector<int> l = pkb->getLineTable()->getLines();
			for (size_t i = 0; i < l.size(); i++)
			{
				if (pkb->getUses()->uses(l[i], value1) && find(lines.begin(), lines.end(), l[i]) != lines.end()) {
					int param2 = pkb->getVarTable()->getVarId(value1);
					if (param2 != -1)
						withLines.push_back(param2);
				}
			}
			withMap.insert(std::pair<string, vector<int>>(field2->getValue(), withLines));
		}
		else if (field1->getType() == "string" && field2->getType() == "string" && value1 == value2)
		{
			vector<int> l = pkb->getLineTable()->getLines();
			for (size_t i = 0; i < l.size(); i++)
			{
				if (find(lines.begin(), lines.end(), l[i]) != lines.end()) {
					withLines.push_back(l[i]);
				}
			}
			withMap.insert(std::pair<string, vector<int>>("all", withLines));
		}
		else if (field1->getType() == "string" && field2->getType() == "string" && value1 != value2)
		{
			vector<int> empty;
			withMap.insert(std::pair<string, vector<int>>("all", empty));
		}
	}

	set<int> QueryEvaluator::cutSetLines(string fieldValue, set<int> setLines)
	{
		vector<int> fieldMap;
		if (withMap.count(fieldValue) > 0) fieldMap = withMap[fieldValue];
		set<int> setLines1tmp;
		vector<int> allMap;
		if (withMap.count("all") > 0) allMap = withMap["all"];

		if (!withMap.empty())
		{
			if (!allMap.empty())
			{
				for (set<int>::iterator l1 = setLines.begin(); l1 != setLines.end(); ++l1) {
					if (find(allMap.begin(), allMap.end(), *l1) != allMap.end())
					{
						setLines1tmp.insert(*l1);
					}
				}
				setLines.clear();
				setLines.insert(setLines1tmp.begin(), setLines1tmp.end());
			}

			if (allMap.empty() && withMap.count("all") > 0)
			{
				setLines.clear();
			}

			if (!fieldMap.empty())
			{
				for (set<int>::iterator l1 = setLines.begin(); l1 != setLines.end(); ++l1) {
					if (find(fieldMap.begin(), fieldMap.end(), *l1) != fieldMap.end())
					{
						setLines1tmp.insert(*l1);
					}
				}
				setLines.clear();
				setLines.insert(setLines1tmp.begin(), setLines1tmp.end());
			}
		}
		return setLines;
	}

	void QueryEvaluator::cutUsesPairs(vector<int> lines)
	{
		vector<string> usesName;
		for (set<std::pair<string, int>>::iterator i = usesPairs.begin(); i != usesPairs.end(); ++i) {
			if (find(usesName.begin(), usesName.end(), (*i).first) == usesName.end())
				usesName.push_back((*i).first);
		}

		for (size_t i = 0; i < usesName.size(); i++)
		{
			for (size_t j = 0; j < lines.size(); j++)
			{
				std::pair<string, int> tmp = { usesName[i],lines[j] };
				if (find(usesPairs.begin(), usesPairs.end(), tmp) == usesPairs.end())
				{
					for (set<std::pair<string, int>>::iterator it = usesPairs.begin(); it != usesPairs.end(); ++it)
					{
						if ((*it).first == tmp.first) usesPairs.erase((*it));
					}
				}
			}
		}
	}
} /* namespace std */