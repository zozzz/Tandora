#include "Application.h"
#include <string.h>
#include "parser/JSONParser.h"


#define _ROOT "D:\\Works\\cpp\\Tandora\\"
//#define _ROOT "C:\\Users\\Zozzz\\Documents\\Tandora\\"

using namespace std;
using namespace common;
using namespace common::unicode;
//using namespace tandora;

template<typename T>
class Temp
{
public:
	Temp(T cb)
	{
		cb(10);
	};

	void x(){ trace("Temp:x"); };
	void z(){ trace("Temp:z"); };
};

int TTest(int z)
{
	trace(z);
	return 0;
}


int main(int argc, char **argv)
{

	#ifdef __UNITTEST__
		// return UnitTest::run(argc, argv);
	#endif

	#ifdef __PERFTEST__
		// return PerformanceTest::run(argc, argv);
	#endif

	#ifndef __UNITTEST__
	#ifndef __PERFTEST__
		//return Application::run(argc, argv);
	#endif
	#endif



	_WAIT_FOR_KEYP exit(0);
	return 0;
}
