#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <istream>
#include <algorithm>
#include <map>
#include <iterator>
#include <stack>
#include <functional>
using namespace std;

class PQL {
private:
	string query;
public:
	PQL();
	void enterQuery();
	string getQuery();
	string processQuery(string query);
	// Lulowe funkcje
	void ltrim(string &str);
	void rtrim(string &str);
	void trim(string &str);
	void removeWhitespaces(string &str);
	string toLower(string str);
	// Koniec Lulowych funkcji
	virtual ~PQL();

};

