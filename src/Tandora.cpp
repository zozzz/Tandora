#include <iostream>
#include "global.h"
#include "parser/Token.h"
#include "parser/TandoraTokens.h"
#include "parser/TandoraParser.h"
#include <bitset>
#include <string.h>
#include "Application.h"

#include <stdio.h>
#include <stdarg.h>
#include "common/common.h"
#include <limits.h>

#define _ROOT "D:\\Works\\cpp\\Tandora\\"

using namespace std;
using namespace common;
using namespace common::unicode;
using namespace tandora;

struct XXX{ int z; };

#pragma pack(push, 1)
typedef struct
{
	unsigned int c;
	unsigned char z;

} zchar;
#pragma pack(pop)

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

	//_WAIT_FOR_KEYP
	//std::string dummy;
	//std::getline(std::cin, dummy);

	//TandoraParser parser;
	//parser.inputFile("../unittest/tandora_src/ftest.as");

	// JavaScriptCompiler compiler;
	// compiler.optimize();
	// compiler.compile(parser);

	//stringstream str;

	//File f(_ROOT"test\\utf8.txt", File::READ);

	//cout << f.exists() << endl;
	//cout << f.readc() << endl;

	/*utf8::FileIO fs(_ROOT"test\\utf8.txt");
	utf8::uchar ch = fs.readc();

	_DUMP_UCHAR(ch);

	_DUMP_UCHAR(0xc381);
	 * */

	/*
	Timer t1;

	for(int i= 0 ; i<_IT_ ; i++)
		utf8::FileIO fio(_ROOT"test\\UIComponent.as");


	t1.stop();
	cout << t1.last() << " | " << t1.last() / _IT_ << endl;
	 * */


	//File file(_ROOT"test\\utf8.txt");

	//cout << file.readc() << endl;

	/*for(int i=0 ; i<3 ; i++)
	{
		unicode::utf8c ch = file.readc();

		_DUMP_UCHAR(ch);
	}*/



	//ALLOC_ARRAY(_buff, char, 100);


	//( false ? cout << "True" << endl : cout << "False" << endl );

	//traceb(16);



	ex_try
	{
		trace(sizeof(uchar));

		#define _IT_ 10000
		TIMER_N_START(uchar, _IT_)

		uchar* zc = new uchar[2000];
		delete[] zc;

		TIMER_END(uchar)

		TIMER_N_START(uint, _IT_)

		unsigned int* ui = new unsigned int[2000];
		delete[] ui;

		TIMER_END(uint)

		char ch = 200;

		
		unicode::Iterator* uit = new unicode::Internal::UTF8Iterator<char>("HelloWorld");
		uit->begin();
	}
	ex_catch( Exception ex )
	{
		cout << ex.what() << endl;
	}

	return 0;
}
