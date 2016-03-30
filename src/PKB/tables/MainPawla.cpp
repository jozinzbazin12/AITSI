/*
 * MainPawla.cpp
 *
 *  Created on: 30 mar 2016
 *      Author: Pawel
 */

#include "../relations/Modifies.h"

int main3() {
	Modifies * modifies = new Modifies();
	modifies->add(1, 30);
	modifies->add(1, 32);
	modifies->add(2, 40);
	modifies->add(1, 34);
	modifies->add(2, 42);
	modifies->add(1, 36);
	modifies->add(3, 50);
	modifies->writeAll();

	return 0;
}

