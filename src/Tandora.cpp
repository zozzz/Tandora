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

//#define _ROOT "D:\\Works\\cpp\\Tandora\\"
#define _ROOT "C:\\Users\\Zozzz\\Documents\\Tandora\\"

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

		
		FILE* _file = fopen(_ROOT "test\\UIComponent.as", "rb");
		
		perror("x: ");
		
		fseek(_file, 0, SEEK_END);
		long _fsize = ftell(_file);
		fseek(_file, 0, SEEK_SET);
		
		unsigned char* _fileContent = new unsigned char[_fsize+1];
		fread(_fileContent, 1, _fsize, _file);
		_fileContent[_fsize] = '\0';
		
		//trace(*uit->next());
		
		unsigned char* _ittest = (unsigned char*)"Hello World";
		trace(_ittest);
		
		Iterator::Value _ch;
		
		for( int i=0 ; i<11 ; i++ )
		{
			trace((unsigned int)_ittest[i]);
		}
		
		int II = 0;
		Iterator::Value* wch;
		TIMER_N_START(iterator, 100)
		
			//unicode::Iterator* uit = new unicode::Internal::UTF8Iterator<unsigned char>(_fileContent, _fsize);
			//unicode::Iterator* uit = new unicode::Internal::UTF8Iterator<unsigned char>(_ittest, 11);
				
			File f(_ROOT "test\\UIComponent.as");
			unicode::Iterator* uit = f.iterator();
		
		
			for( Iterator::Value* ch  ; *(ch = uit->next()) != 0 ; )
			{
				
				
			}
			
			delete uit;
			
		TIMER_END(iterator)
		
		unsigned char _x_ = 200;
		
		
		

		
		/*
		uchar	_oValue = 20;
		uchar* _refValue;
		uchar _cloneValue;
		
		TIMER_N_START(passByRef, 200000000)
			_refValue = &_oValue;
			if( *_refValue == 0 )
			{
			}
		TIMER_END(passByRef)
		
		TIMER_N_START(clone, 200000000)
			_cloneValue = _oValue;
			if( _cloneValue == 0 )
			{
			}
		TIMER_END(clone)
		 */
	}
	ex_catch( Exception ex )
	{
		cout << ex.what() << endl;
	}

	return 0;
}
