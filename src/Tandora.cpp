#include "Application.h"

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

	File f2(_ROOT "test\\utf8.txt");
	File f(_ROOT "test\\utf8_2.txt", File::WRITE);
	//unsigned char* hw = (unsigned char*)"Hello World";
	//Reader* reader = f.reader();
	//Reader* reader = new Reader(hw, 11, unicode::Decode::UTF8);
	//Reader* rea = new Reader();
	//unicode::Internal::Test<TP> _TTT;

	//Assert(reader ,!=, NULL);


	uchar* inp = new uchar[10];
	inp[0] = 0xf6;
	inp[1] = 0xfc;
	inp[2] = 0xf3;
	inp[3] = 0x151;
	inp[4] = 0xfa;
	inp[5] = 0xe9;
	inp[6] = 0xe1;
	inp[7] = 0x171;
	inp[8] = 0xed;
	inp[9] = 0;

	Writer wt(inp, 9, Encode::UTF8);

	/*unsigned char x;
	while( (x = wt.next()) )
	{
		_DUMP_UCHAR(x);
	}*/

	f.write(inp, 9);

	//Temp<int (*)(int)> T1(TTest);


	//T1.z();

//	T2.z();

	trace("-----------");

	Reader* reader = f2.reader();
	Reader::Output ch;

	// 0.400, 0.004
	TIMER_N_START(UReader, 1)

	while( (ch = reader->next()) )
	{
		_DUMP_UCHAR(ch);
	}

	//while(){}

		//delete reader;
		reader->reset();

	TIMER_END(UReader)

	/*while( ch )
	{
		_DUMP_UCHAR(ch);
		reader->next(ch);
	}*/

	return 0;
}
