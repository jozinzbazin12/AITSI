#ifndef SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
#define SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
//#include "PQLNode.h"
#include "../PKB/api/PKBApi.h"
#include "../ParserPQL/MatcherPQL.h"
#include <iterator>
#include <sstream>
#include <map>
#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#ifndef SRC_TREE_FIELD_H_
#include "../PQL/Field.h"
#endif
#ifndef SRC_PQL_PQLTREE_H_
#include "../PQL/PQLTree.h"
#endif

namespace std {

class QueryEvaluator {
public:
	QueryEvaluator();
	virtual ~QueryEvaluator();
	vector<string> getResult(PQLTree* tree);
	vector<int> getModifiesResult(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	vector<int> getUsesResult(Field* field1, Field* field2, vector<int> lines,
			string selectValue);
	vector<int> getFollowsResult(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	vector<int> getParentResult(Field* field1, Field* field2, vector<int> lines,
			string selectValue);
	vector<int> getUsesSResult(Field* field1, Field* field2, vector<int> lines,
			string selectValue);
	vector<int> getFollowsSResult(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	vector<int> getParentSResult(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	vector<int> getCallResult(Field* field1, Field* field2, vector<int> lines,
			string selectValue);
	vector<int> getCallStarResult(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	void getWithResult(Field* field1, Field* field2,
				vector<int> lines);
private:
	PKBApi *pkbApi = new PKBApi();
	map<string,vector<int>> withMap;
	set<int> cutSetLines(string fieldValue, set<int> setLines);
	string resultType;
};

} /* namespace std */

#endif /* SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_ */
