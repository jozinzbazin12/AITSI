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
#include <functional>
#include <algorithm>
#include <map>
#include <iterator>

#include "PQL/tree_util.hh"
#include "PQL/PQLNode.h"
#include "PQL/PQLTree.h"

#include "PQL/Field.h"
#include "Parser/Matcher.h"
#include "PQL/PQLNode.h"
#include "Parser/Exceptions.h"

using namespace std;

class QueryPreProcessor {
public:
	QueryPreProcessor();
	virtual ~QueryPreProcessor();
	void parseQuery(string query);
	Matcher* matcher;
	Exceptions* exc;

private:
	void writeVector(vector<string> &tab) {
		cout << "Kolejny wektor" << endl;
		for (int i = 0; i < tab.size(); i++) {
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
			elems.push_back(s.substr(0, pos));
			positions.push_back(pos);
			pos = s.find(token, pos + 3);
		}

		for (int i = 0; i < positions.size(); i++) {
			pos = positions[i];
		}

		if (positions.size() > 0)
			elems.push_back(s.substr(pos + 3));
		else
			elems.push_back(s.substr(0));

		return elems;
	}

	vector<Field> makeFields(vector<string> &elems) {
		vector<Field> fields;
		vector<string> tmp = elems; //= split(elems[0], ',');

		//writeVector(tmp);

		vector<string> tokens = {"assign","stmt","while","variable","constant","prog_line"};

		//Matcher* matcher = new Matcher();
		for (int i = 0; i < tmp.size(); i++) {
			//cout << "Tmp" << i << ": " << tmp[i] << endl;
			// Sprawdzenie czy deklaracja nie zawiera procName, varName, value albo stmt#
			if (!matcher->checkAll(tmp[i])) {
				for(int j = 0 ; j < tokens.size() ; j ++)
				{
					if (tmp[i].find(tokens[j]) < tmp[i].length()) {
						vector<Field> tmpFields = makeFieldType(tokens[j],tmp[i]);
						for (int j = 0; j < tmpFields.size(); j++) {
							//cout << tmpFields[j].getType() << " " << tmpFields[j].getValue() << endl;
							fields.push_back(tmpFields[j]);
						}

					}
				}
			} else {
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
		for (int i = 0; i < tmp.size(); i++) {
			declarationFields.push_back(Field(type, tmp[i]));
		}

		return declarationFields;
	}

	PQLTree* PqlTree;

	void makeTree(vector<string> &elems)
	{
		if(elems.size() > 1) exc->throwException();
		//if(countSelect(elems[0]) > 1) throwException();

		vector<string> queryMainTokens;
		for(int i = 0 ; i < elems.size() ; i ++)
		{
			if(matcher->checkTokens(elems[i],"select")) {
				//queryMainTokens.push_back("select");
			}
			else {
				exc->throwException();
			}

			if(matcher->checkTokens(elems[i],"such that")) {
				queryMainTokens.push_back("such that");
			}
		}

		//writeVector(queryMainTokens);

		vector<string> elements = splitQuery(elems[0],queryMainTokens);

		//writeVector(queryParts);

		for(int i = 0 ; i < queryParts.size() ; i ++)
		{
			switch(checkType(queryParts[i]))
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
			}
		}

		tree<tree_node_<PQLNode>>::iterator iter;
		PQLTree* tree;
		tree = tree->getInstance();
		PQLNode* node;
		tree_node_<PQLNode>* treeNode;

		node = new QueryNode();
		treeNode = new tree_node_<PQLNode>(*node);
		iter = tree->appendRoot(*treeNode);

		if(selectNodes.size() > 0)
		{
			node = new ResultMainNode();
			treeNode = new tree_node_<PQLNode>(*node);
			iter = tree->appendChild(iter, *treeNode);

			node = selectNodes[0];
			treeNode = new tree_node_<PQLNode>(*node);
			iter = tree->appendChild(iter, *treeNode);

			for(int i = 1 ; i < selectNodes.size() ; i ++)
			{
				node = selectNodes[i];
				treeNode = new tree_node_<PQLNode>(*node);
				iter = tree->appendSibling(iter, *treeNode);
			}
		}

		iter = tree->getRoot();

		if(selectNodes.size() > 0)
		{
			node = new SuchMainNode();
			treeNode = new tree_node_<PQLNode>(*node);
			iter = tree->appendChild(iter, *treeNode);

			node = suchNodes[0];
			treeNode = new tree_node_<PQLNode>(*node);
			iter = tree->appendChild(iter, *treeNode);

			for(int i = 1 ; i < suchNodes.size() ; i ++)
			{
				node = suchNodes[i];
				treeNode = new tree_node_<PQLNode>(*node);
				iter = tree->appendSibling(iter, *treeNode);
			}
		}

		//tree->printTree();
		PqlTree = tree;
	}

	vector<ResultNode*> selectNodes;
	vector<SuchNode*> suchNodes;

	void makeSelectNode(string selectPart)
	{
		string type = "select";
		int startPos = selectPart.find(type);
		//cout << selectPart << endl;
		selectPart = selectPart.substr(startPos+type.length());
		//cout << selectPart << endl;
		selectPart.erase(remove_if(selectPart.begin(), selectPart.end(), ptr_fun<int, int>(isspace)), selectPart.end());
		//cout << selectPart << endl;
		vector<string> selectParts = split(selectPart,',');

		//writeVector(selectParts);

		Field* aktField;
		int dotPos;
		for(int i = 0 ; i < selectParts.size() ; i ++)
		{
			if(matcher->checkAll(selectParts[i]))
			{
				dotPos = selectParts[i].find(".");
				aktField = findField(selectParts[i].substr(0,dotPos));
				if(matcher->checkProcName(selectParts[i])) {
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
				selectNodes.push_back(new ResultNode(aktField));
				//selectNodes.push_back(new Field(aktField->getType(),aktField->getValue(),aktField->isProcName(),aktField->isVarName(),aktField->isVal(),aktField->isStmt()));
			}
			else
			{
				aktField = findField(selectParts[i]);
				selectNodes.push_back(new ResultNode(aktField));
			}
		}

		/*
		for(int i = 0 ; i < selectNodes.size() ; i ++)
		{
			cout << "[-" << i << "] " << selectNodes[i].getField()->printField();
		}*/
	}

	void makeSuchNode(string suchPart)
	{
		string type = "such that";
		int startPos = suchPart.find(type);
		//cout << suchPart << endl;
		suchPart = suchPart.substr(startPos+type.length());
		//cout << suchPart << endl;
		vector<string> suchParts = split(suchPart,"and");

		//writeVector(suchParts);

		string suchType;
		vector<Field> attr;
		bool star;

		for(int i = 0 ; i < suchParts.size() ; i ++)
		{
			suchParts[i].erase(remove_if(suchParts[i].begin(), suchParts[i].end(), ptr_fun<int, int>(isspace)), suchParts[i].end());

			suchType = matcher -> checkSuchThatType(suchParts[i]);
			if(suchType == "") exc->throwUnexpectedTypeOfClause();
			star = matcher->isStar(suchParts[i],suchType.length());

			attr = makeSuchNodeAttributes(suchType, star, suchParts[i]);

			if(attr.size() == 2)
			{
				suchNodes.push_back(new SuchNode(suchType,&attr[0],&attr[1],star));
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

	vector<Field> makeSuchNodeAttributes(string suchtype, bool star, string suchPart)
	{
		vector<Field> attr;
		vector<string> attrParts;
		int pos = suchtype.length() + star;
		suchPart = suchPart.substr(pos);
		//cout << suchPart << endl;
		if(matcher->hasTwoElem(suchPart))
		{
			if(matcher->isBracket(suchPart))
			{
				suchPart.erase(matcher->getPosition(suchPart,"("),1);
				suchPart.erase(matcher->getPosition(suchPart,")"),1);
			}

			attrParts = split(suchPart,',');
			//writeVector(attrParts);

			for(int i = 0; i < attrParts.size() ; i ++)
			{
				if(matcher->isString(attrParts[i]))
				{
					attrParts[i].erase(matcher->getPosition(attrParts[i],"\""),1);
					attrParts[i].erase(matcher->getPosition(attrParts[i],"\""),1);
					attr.push_back(Field("string",attrParts[i]));
				}
				else
				{
					Field* field = findField(attrParts[i]);
					if(field == nullptr)
					{
						if(matcher->is_(attrParts[i]))
							attr.push_back(Field("any","_"));
						else if(matcher->isNumber(attrParts[i]))
							attr.push_back(Field("constant",attrParts[i]));
						else
							exc->throwInvalidTypeOfArguments();
					}
					else
					{
						attr.push_back(Field(field->getType(),field->getValue(),field->isProcName(),field->isVarName(),field->isVal(),field->isStmt()));
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
		for(int i = 0 ; i < fields.size() ; i ++)
		{
			if(fields[i].getValue() == name) return &fields[i];
		}
		return nullptr;
	}

	int checkType(string queryPart)
	{
		if(queryPart.find("select") < queryPart.length()) return 1; //select
		if(queryPart.find("such that") < queryPart.length()) return 2; //such that

		return 0;
	}

	int aktPos;
	int lastPos;
	int queryLength;
	string aktToken;
	string lastToken;
	vector<string> queryParts;

	vector<string> splitQuery(string query,vector<string> tokensElems)
	{
		aktPos = aktToken.length();
		lastPos = query.length();
		queryLength = query.length();

		//cout<< "START " << aktPos << " " << lastPos << " " << queryLength << endl;

		findPositions(query, tokensElems);

		return queryParts;
	}

	void findPositions(string query, vector<string> tokens)
	{
		int tmpPos;
		for(int i = 0 ; i < tokens.size() ; i ++)
		{
			tmpPos = query.find(tokens[i], aktPos);
			//cout << i << " -> " << tokens[i] << " " << aktPos << " " << lastPos << endl;
			if(tmpPos < lastPos && tmpPos < query.length())
			{
				lastPos = tmpPos;
				lastToken = aktToken;
				aktToken = tokens[i];
				//cout << "CHANGE -> " << lastPos << "|" << tmpPos << " " << aktToken << endl;
			}
		}

		//cout<< "NEXT? " << aktPos << " " << lastPos << " " << aktToken.length() << " " << queryLength << endl;

		queryParts.push_back(query.substr(aktPos-lastToken.length(),lastPos));

		query = query.substr(lastPos,queryLength);
		//aktPos = lastPos;// + aktToken.length();
		//lastPos = query.length();

		//cout<< "NEXT? " << aktPos << " " << lastPos << " " << aktToken.length() << " " << queryLength << endl;

		if(getNextPosition(tokens, query)) splitQuery(query, tokens);
	}

	bool getNextPosition(vector<string> tokens, string query)
	{
		for(int i = 0 ; i < tokens.size() ; i ++)
		{
			if(query.find(tokens[i]) < query.length())
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
