#ifndef COMMONS_H
#define COMMONS_H

// void mydelete(void *ptr) { if (ptr != nullptr) { delete(ptr); ptr = nullptr; } }
#define mydelete(p) { if ((p) != nullptr) { delete((p)); (p) = nullptr; } }

const int MYSUCCESS = 0;
const int MYERROR = -1;

#include <string>

namespace psychotest {

	const std::string STRUCTPATH = "test/";
	const std::string OUTPUTPATH = "output/";
	const std::string FIGPATH = "fig/";
	const std::string ERRPATH = "err/";

}

#endif