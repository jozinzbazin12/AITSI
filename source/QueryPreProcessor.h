/*
* QueryPreProcessor.h
*
*  Created on: 29 mar 2016
*      Author: Popek
*/

#ifndef SRC_QUERYPREPROCESSOR_H_
#define SRC_QUERYPREPROCESSOR_H_

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <map>
#include <iterator>

#include "ParserPQL/Exceptions.h"
#include "ParserPQL/MatcherPQL.h"
#include "PQL/tree_util.hh"
#include "PQL/PQLNode.h"
#include "PQL/PQLTree.h"

#include "PQL/Field.h"
#include "PQL/PQLNode.h"

using namespace std;

class QueryPreProcessor {
public:
	QueryPreProcessor();
	virtual ~QueryPreProcessor();
	void parseQuery(string query);
	MatcherPQL* matcher;
	Exceptions* exc;

private:
	void writeVector(vector<string> &tab) {
		cout << "Kolejny wektor" << endl;
		for (size_t i = 0; i < tab.size(); i++) {
			cout << "[" << i << "] " << tab[i] << endl;
		}
	}

	vector<string> &split(const string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}

	vector<string> split(const string &s, string token) {
		vector<string> elems;
		vector<size_t> positions;
		size_t pos = s.find(token, 0);
		while (pos != string::npos) {
			//elems.push_back(s.substr(0, pos));
			positions.push_back(pos);
			pos = s.find(token, pos + 3);
		}

		int p1 = 0;
		int p2 = s.length();

		if (positions.size() > 0)
		{
			for (size_t i = 0; i < positions.size(); i++) {
				pos = positions[i];
				elems.push_back(s.substr(p1, pos - p1));
				p1 = pos + 3;
			}
			elems.push_back(s.substr(p1, p2 - p1));
		}
		else
			elems.push_back(s.substr(p1, p2));

		return elems;
	}

	vector<Field> makeFields(vector<string> &elems) {
		vector<Field> fields;
		vector<string> tmp = elems; //= split(elems[0], ',');

									//writeVector(tmp);

		vector<string> tokens = matcher->getTokensList();
		//writeVector(tokens);
		//Matcher* matcher = new Matcher();
		for (size_t i = 0; i < tmp.size(); i++) {
			//cout << "Tmp" << i << ": " << tmp[i] << endl;
			// Sprawdzenie czy deklaracja nie zawiera procName, varName, value albo stmt#
			if (!matcher->checkAll(tmp[i])) {
				for (size_t j = 0; j < tokens.size(); j++)
				{
					if (tmp[i].find(tokens[j]) < tmp[i].length()) {
						vector<Field> tmpFields = makeFieldType(tokens[j], tmp[i]);
						for (size_t j = 0; j < tmpFields.size(); j++) {
							//cout << tmpFields[j].getType() << " " << tmpFields[j].getValue() << endl;
							fields.push_back(tmpFields[j]);
						}

					}
				}
			}
			else {
				exc->throwException();
			}
		}

		return fields;
	}

	vector<Field> makeFieldType(string type, string declaration) {
		//cout << "1->" << declaration << endl;

		declaration.erase(remove_if(declaration.begin(), declaration.end(), ptr_fun<int, int>(isspace)), declaration.end());

		//cout << "2->" << declaration << endl;

		declaration = declaration.substr(type.length(), declaration.length());

		//cout << "3->" << declaration << endl;

		vector<string> tmp = split(declaration, ',');
		vector<Field> declarationFields;
		for (size_t i = 0; i < tmp.size(); i++) {
			declarationFields.push_back(Field(type, tmp[i]));
		}

		return declarationFields;
	}

	PQLTree* PqlTree;

	void makeTree(vector<string> &elems)
	{
		if (elems.size() > 1) exc->throwException();
		//if(countSelect(elems[0]) > 1) throwException();

		vector<string> queryMainTokens;
		for (size_t i = 0; i < elems.size(); i++)
		{
			if (matcher->checkTokens(elems[i], "Select")) {
				//queryMainTokens.push_back("select");
			}
			else {
				exc->throwException();
			}

			if (matcher->checkTokens(elems[i], "such that")) {
				queryMainTokens.push_back("such that");
			}

			if (matcher->checkTokens(elems[i], "with")) {
				queryMainTokens.push_back("with");
			}
		}

		//writeVector(queryMainTokens);

		vector<string> elements = splitQuery(elems[0], queryMainTokens);

		//writeVector(queryParts);

		for (size_t i = 0; i < queryParts.size(); i++)
		{
			//cout<< "Query part: " << queryParts[i] << endl;
			switch (checkType(queryParts[i]))
			{
			case 0:
				exc->throwSplitException();
				break;
			case 1:
				makeSelectNode(queryParts[i]);
				break;
			case 2:
				makeSuchNode(queryParts[i]);
				break;
			case 3:
				makeWithNode(queryParts[i]);
				break;
			}
		}

		tree<tree_node_<PQLNode*>*>::iterator iter;
		PQLNode* node;
		tree_node_<PQLNode*>* treeNode;

		PqlTree = new PQLTree();

		node = new PQLNode("queryNode");
		treeNode = new tree_node_<PQLNode*>(node);
		iter = PqlTree->appendRoot(treeNode);

		if (selectNodes.size() > 0)
		{
			node = new PQLNode("resultMainNode");
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			node = selectNodes[0];
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			for (size_t i = 1; i < selectNodes.size(); i++)
			{
				node = selectNodes[i];
				treeNode = new tree_node_<PQLNode*>(node);
				iter = PqlTree->appendSibling(iter, treeNode);
			}
		}

		if (withNodes.size() > 0)
		{
			iter = PqlTree->getRoot();

			node = new PQLNode("withMainNode");
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			node = withNodes[0];
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			for (size_t i = 1; i < withNodes.size(); i++)
			{
				node = withNodes[i];
				treeNode = new tree_node_<PQLNode*>(node);
				iter = PqlTree->appendSibling(iter, treeNode);
			}
		}

		if (suchNodes.size() > 0)
		{
			iter = PqlTree->getRoot();

			node = new PQLNode("suchMainNode");
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			node = suchNodes[0];
			treeNode = new tree_node_<PQLNode*>(node);
			iter = PqlTree->appendChild(iter, treeNode);

			for (size_t i = 1; i < suchNodes.size(); i++)
			{
				node = suchNodes[i];
				treeNode = new tree_node_<PQLNode*>(node);
				iter = PqlTree->appendSibling(iter, treeNode);
			}
		}

		//tree->printTree();
	}

	vector<PQLNode*> selectNodes;
	vector<PQLNode*> suchNodes;
	vector<PQLNode*> withNodes;

	void makeSelectNode(string selectPart)
	{
		string type = "Select";
		int startPos = selectPart.find(type);
		//cout << selectPart << endl;
		selectPart = selectPart.substr(startPos + type.length());
		//cout << selectPart << endl;
		selectPart.erase(remove_if(selectPart.begin(), selectPart.end(), ptr_fun<int, int>(isspace)), selectPart.end());
		//cout << selectPart << endl;
		vector<string> selectParts = split(selectPart, ',');

		//writeVector(selectParts);

		Field* aktField;
		int dotPos;
		for (size_t i = 0; i < selectParts.size(); i++)
		{
			if (matcher->checkAll(selectParts[i]))
			{
				if (!matcher->checkBOOLEAN(selectParts[i]))
				{
					dotPos = selectParts[i].find(".");
					aktField = findField(selectParts[i].substr(0, dotPos));
					if (aktField != nullptr)
					{
						if (matcher->checkProcName(selectParts[i])) {
							aktField->setProcName(true);
						}

						if (matcher->checkVarName(selectParts[i])) {
							aktField->setVarName(true);
						}

						if (matcher->checkStmt_(selectParts[i])) {
							aktField->setStmt(true);
						}

						if (matcher->checkValue(selectParts[i])) {
							aktField->setVal(true);
						}
						selectNodes.push_back(new PQLNode("resultNode", aktField));
					}
					else
					{
						exc->throwUnexpectedSelectPart();
					}
				}
				else if (matcher->checkBOOLEAN(selectParts[i]))
				{
					aktField = new Field("boolean", "boolean");
					selectNodes.push_back(new PQLNode("resultNode", aktField));
				}
				else
				{
					exc->throwUnexpectedSelectPart();
				}
				//selectNodes.push_back(new Field(aktField->getType(),aktField->getValue(),aktField->isProcName(),aktField->isVarName(),aktField->isVal(),aktField->isStmt()));
			}
			else
			{
				aktField = findField(selectParts[i]);
				if (aktField != nullptr)
				{
					selectNodes.push_back(new PQLNode("resultNode", aktField));
				}
				else
				{
					exc->throwUnexpectedSelectPart();
				}
			}
		}

		/*
		for(int i = 0 ; i < selectNodes.size() ; i ++)
		{
		cout << "[-" << i << "] " << selectNodes[i]->getField1()->printField();
		}
		*/
	}

	void makeSuchNode(string suchPart)
	{
		string type = "such that";
		int startPos = suchPart.find(type);
		//cout << suchPart << endl;
		suchPart = suchPart.substr(startPos + type.length());
		//cout << suchPart << endl;
		vector<string> suchParts = split(suchPart, "and");

		//writeVector(suchParts);

		string suchType;
		vector<Field*> attr;
		bool star;

		for (size_t i = 0; i < suchParts.size(); i++)
		{
			suchParts[i].erase(remove_if(suchParts[i].begin(), suchParts[i].end(), ptr_fun<int, int>(isspace)), suchParts[i].end());

			suchType = matcher->checkSuchThatType(suchParts[i]);
			if (suchType == "") exc->throwUnexpectedTypeOfClause();
			star = matcher->isStar(suchParts[i], suchType.length());

			attr = makeSuchNodeAttributes(suchType, star, suchParts[i]);

			if (attr.size() == 2)
			{
				suchNodes.push_back(new PQLNode("suchNode", suchType, attr[0], attr[1], star));
			}
			else
			{
				exc->throwInvalidNumberOfArguments();
			}

			/*
			for(int i = 0 ; i < attr.size() ; i ++)
			{
			cout << "!!! " << attr[i].printField();
			}
			*/
		}
	}

	vector<Field*> makeSuchNodeAttributes(string suchtype, bool star, string suchPart)
	{
		vector<Field*> attr;
		vector<string> attrParts;
		int pos = suchtype.length() + star;
		suchPart = suchPart.substr(pos);
		//cout << suchPart << endl;
		if (matcher->hasTwoElem(suchPart))
		{
			if (matcher->isBracket(suchPart))
			{
				suchPart.erase(matcher->getPosition(suchPart, "("), 1);
				suchPart.erase(matcher->getPosition(suchPart, ")"), 1);
			}
			//cout << "PreProcessor " << suchPart << endl;
			attrParts = split(suchPart, ',');
			//writeVector(attrParts);

			for (size_t i = 0; i < attrParts.size(); i++)
			{
				if (matcher->isString(attrParts[i]))
				{
					attrParts[i].erase(matcher->getPosition(attrParts[i], "\""), 1);
					attrParts[i].erase(matcher->getPosition(attrParts[i], "\""), 1);
					attr.push_back(new Field("string", attrParts[i]));
				}
				else
				{
					Field* field = findField(attrParts[i]);
					if (field == nullptr)
					{
						if (matcher->is_(attrParts[i]))
							attr.push_back(new Field("any", "_"));
						else if (matcher->isNumber(attrParts[i]))
							attr.push_back(new Field("constant", attrParts[i]));
						else
							exc->throwInvalidTypeOfArguments();
					}
					else
					{
						attr.push_back(new Field(field->getType(), field->getValue(), field->isProcName(), field->isVarName(), field->isVal(), field->isStmt()));
					}
				}
			}
		}
		else
		{
			exc->throwToMuchArguments();
		}

		return attr;
	}

	void makeWithNode(string withPart)
	{
		string type = "with";
		int startPos = withPart.find(type);
		//cout << suchPart << endl;
		withPart = withPart.substr(startPos + type.length());
		//cout << suchPart << endl;
		vector<string> withParts = split(withPart, "and");

		//writeVector(withParts);

		vector<Field*> attr;

		for (size_t i = 0; i < withParts.size(); i++)
		{
			withParts[i].erase(remove_if(withParts[i].begin(), withParts[i].end(), ptr_fun<int, int>(isspace)), withParts[i].end());

			attr = makeWithNodeAttributes(withParts[i]);

			if (attr.size() == 2)
			{
				if (matcher->checkWithAttributes(attr[0], attr[1]))
					withNodes.push_back(new PQLNode("withNode", attr[0], attr[1]));
				else
					exc->throwInvalidWithStatementAttributes();
			}
			else
			{
				exc->throwInvalidNumberOfArguments();
			}

			/*
			for(size_t i = 0 ; i < attr.size() ; i ++)
			{
			cout << "!!! " << attr[i]->printField();
			}
			*/
		}
	}

	vector<Field*> makeWithNodeAttributes(string withPart)
	{
		vector<Field*> attr;
		vector<string> attrParts;
		//cout << suchPart << endl;
		int dotPos;
		if (matcher->withHasTwoElem(withPart))
		{
			attrParts = split(withPart, '=');
			//writeVector(attrParts);

			for (size_t i = 0; i < attrParts.size(); i++)
			{
				attrParts[i].erase(remove_if(attrParts[i].begin(), attrParts[i].end(), ptr_fun<int, int>(isspace)), attrParts[i].end());
				if (matcher->isString(attrParts[i]))
				{
					attrParts[i].erase(matcher->getPosition(attrParts[i], "\""), 1);
					attrParts[i].erase(matcher->getPosition(attrParts[i], "\""), 1);
					attr.push_back(new Field("string", attrParts[i]));
				}
				else
				{
					dotPos = attrParts[i].find('.');

					if (dotPos < attrParts[i].length())
					{
						Field* field = findField(attrParts[i].substr(0, dotPos));
						if (field == nullptr)
						{
							if (matcher->isNumber(attrParts[i]))
								attr.push_back(new Field("constant", attrParts[i]));
							else
								exc->throwInvalidWithStatement();
						}
						else
						{
							attr.push_back(new Field(field->getType(),
								field->getValue(),
								matcher->checkProcName(attrParts[i]),
								matcher->checkVarName(attrParts[i]),
								matcher->checkValue(attrParts[i]),
								matcher->checkStmt_(attrParts[i])));
						}
					}
					else
					{
						Field* field = findField(attrParts[i]);
						if (field == nullptr)
						{
							if (matcher->isNumber(attrParts[i]))
								attr.push_back(new Field("constant", attrParts[i]));
							else
								exc->throwInvalidWithStatement();
						}
						else
						{
							attr.push_back(new Field(field->getType(), field->getValue()));
						}
					}
				}
			}
		}
		else
		{
			exc->throwToMuchArguments();
		}

		return attr;
	}

	Field* findField(string name)
	{
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].getValue() == name) return &fields[i];
		}
		return nullptr;
	}

	int checkType(string queryPart)
	{
		//cout << "[1] " << queryPart.find("select") << endl;
		//cout << "[2] " << queryPart.find("such that") << endl;

		if (queryPart.find("Select") < queryPart.length()) return 1; //select
		if (queryPart.find("such that") < queryPart.length()) return 2; //such that
		if (queryPart.find("with") < queryPart.length()) return 3; //with

		return 0;
	}

	int aktPos;
	int lastPos;
	int queryLength;
	string aktToken;
	string lastToken;
	vector<string> queryParts;

	vector<string> splitQuery(string query, vector<string> tokensElems)
	{
		aktPos = aktToken.length();
		lastPos = query.length();
		queryLength = query.length();

		//cout<< "START " << aktPos << " " << lastPos << " " << queryLength << endl;

		//cout << "Q: " << query << endl;

		findPositions(query, tokensElems);

		return queryParts;
	}

	void findPositions(string query, vector<string> tokens)
	{
		bool isnext = false;
		int tmpPos;
		for (size_t i = 0; i < tokens.size(); i++)
		{
			tmpPos = query.find(tokens[i], aktPos);
			//cout << i << " -> " << tokens[i] << " " << aktPos << " " << lastPos << " " << tmpPos << endl;
			if (tmpPos > 0 && tmpPos < lastPos && tmpPos < query.length())
			{
				isnext = true;
				lastPos = tmpPos;
				lastToken = aktToken;
				aktToken = tokens[i];
				//cout << "CHANGE -> " << lastPos << "|" << tmpPos << " " << aktToken << " " << lastToken << endl;
			}
		}

		//cout<< "NEXT? " << aktPos << " " << lastPos << " " << aktToken.length() << " " << queryLength << endl;

		if (isnext)
			queryParts.push_back(query.substr(aktPos - lastToken.length(), lastPos));
		else
			queryParts.push_back(query);

		//writeVector(queryParts);

		query = query.substr(lastPos, queryLength);
		//aktPos = lastPos;// + aktToken.length();
		//lastPos = query.length();

		//cout << query << endl;

		//cout<< "NEXT? " << aktPos << " " << lastPos << " " << aktToken.length() << " " << queryLength << endl;

		if (getNextPosition(tokens, query)) splitQuery(query, tokens);
	}

	bool getNextPosition(vector<string> tokens, string query)
	{
		for (size_t i = 0; i < tokens.size(); i++)
		{
			if (query.find(tokens[i]) < query.length())
				return true;
		}
		return false;
	}

public:
	vector<Field>& getFields() {
		return fields;
	}

	void setFields(vector<Field>& fields) {
		this->fields = fields;
	}

	PQLTree*& getTree() {
		return PqlTree;
	}

private:
	vector<Field> fields;
};

#endif /* SRC_QUERYPREPROCESSOR_H_ */
