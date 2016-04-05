/*
 * DesignExtractor.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_
#define SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_

class DesignExtractor {
public:
	DesignExtractor();
	virtual ~DesignExtractor();
	void start();

private:
	void setFollowRelations();
	void setParentRelations();
	void setLoopsTable();
	void setAssignTable();
};

#endif /* SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_ */
