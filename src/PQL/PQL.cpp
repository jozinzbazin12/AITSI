#include "PQL.h"
//#include "tree_util.hh"
//#include "PQLNode.h"
//#include "PQLTree.h"
//#include "../QueryPreProcessor.h"

using namespace std;

PQL::PQL() {
	// TODO Auto-generated constructor stub
}

PQL::~PQL() {
	// TODO Auto-generated destructor stub
}
//Lulowe Funkcje
string PQL::toLower(string str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

void PQL::ltrim(string &str) {
	str.erase(str.begin(), find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace))));
}

void PQL::rtrim(string &str) {
	str.erase(find_if(str.rbegin(), str.rend(), not1(ptr_fun<int, int>(isspace))).base(), str.end());
}

void PQL::trim(string &str) {
	ltrim(str);
	rtrim(str);
}

void PQL::removeWhitespaces(string &str) {
	str.erase(remove_if(str.begin(), str.end(), ptr_fun<int, int>(isspace)), str.end());
}
// koniec lulowych

string PQL::getQuery()
{
	return this->query;
}

string PQL::processQuery(string query)
{
	string processedQuery;
	processedQuery = toLower(query);
	trim(processedQuery);
	cout<<"processQuery: "<<processedQuery<<endl;
	return processedQuery;
}

void PQL:: enterQuery()
{
	cout << "Enter Query: \n";
	getline(cin,query);
	query=processQuery(query);
}
