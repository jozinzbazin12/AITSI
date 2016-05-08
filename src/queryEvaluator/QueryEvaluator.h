#ifndef SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
#define SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
#include "PQLNode.h"
#include <iterator>

namespace std {

class QueryEvaluator {
public:
	QueryEvaluator();
	virtual ~QueryEvaluator();
	string getResult(PQLTree* tree);
	constexpr unsigned int str2int(const char* str, int h = 0);
	vector<int> getModifiesResult(Field* field1, Field* field2, vector<int> lines);
	vector<int> getUsesResult(Field* field1, Field* field2, vector<int> lines);
	vector<int> getFollowsResult(Field* field1, Field* field2, vector<int> lines, string searchResult);
	vector<int> getParentResult(Field* field1, Field* field2, vector<int> lines);
	vector<int> getUsesSResult(Field* field1, Field* field2, vector<int> lines);
	vector<int> getFollowsSResult(Field* field1, Field* field2, vector<int> lines);
	vector<int> getParentSResult(Field* field1, Field* field2, vector<int> lines);
};

} /* namespace std */

#endif /* SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_ */
