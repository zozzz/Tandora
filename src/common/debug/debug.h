/*
 * File:   debug.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 17:18
 */

#ifndef DEBUG_H
#define	DEBUG_H


#ifdef __DEBUG__

#include <iostream>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <math.h>
#include <bitset>

namespace common{ namespace debug {


/*
void __dbg_trace__(int line, const char* fn, ...)
{
	char* msg = new char[1000];
	int pos;

	strcpy(msg, fn);
	pos = strlen(fn);
	strcpy(msg+pos, "(");
	pos+= 2;

	int _lines = 1;
	int _s = 10;

	for(;;)
	{
		if( line / _s < 1 )
			break;
		_lines++;
		_s *= _s;
	}

	std::cout << _lines << std::endl;
	char* _line = new char[_lines+1];
	_line[_lines] = '\0';
	itoa(line, _line, 10);
	strcpy(msg+(pos-1), _line);
	pos += strlen(_line);
	strcpy(msg+(pos-1), ") %s");



	//std::cout << str.c_str() << std::endl;
	//char* msg = str.

	va_list args;
	va_start(args, fn);

	size_t buffSize = (strlen(msg)+1) * sizeof(char);
	char* buffer;
	ALLOC_ARRAY(buffer, char, buffSize+1);
	buffer[buffSize] = '\0';
	int buffNewSize = 0;
	for(;;)
	{
		buffNewSize = vsnprintf(buffer, buffSize+1, msg, args);
		std::cout << buffNewSize << " | " << buffSize << std::endl;
		if( buffNewSize == -1 )
		{
			FREE_ARRAY(buffer);
			// some error...
			buffer = NULL;
			break;
		}
		else if( buffNewSize > buffSize )
		{
			RESIZE_ARRAY(buffer, char, buffNewSize+4);
			buffSize = buffNewSize;
			buffer[buffSize] = '\0';
			strcpy(buffer-4, " %s");
			buffSize += 3;
			std::cout << buffer << std::endl;
			buffNewSize = 0;
		}
		else
		{
			buffer[buffSize] = '\0';
			// evrything is OK
			break;
		}
	}

	va_end(args);

	std::cout << buffer << std::endl;
}
 * */







// DO NOT USE!
#define _assert_(var1, op, var2, _break_) if( !((var1) op (var2)) ) { \
	std::cout << "Assertion failed (" << (var1) << " " << #op << " " << (var2) << ") in: " << __FUNCTION__ << "(" << __LINE__ << ")" << std::endl; \
	if( _break_ ) \
		exit(1); \
	}

#define Assert(var1, op, var2) _assert_(var1, op, var2, false)
#define AssertExit(var1, op, var2) _assert_(var1, op, var2, true)
#define trace(var) std::cout << #var << " = " << (var) << std::endl
// TODO: improve
#define traceb(var) std::cout << #var << " = " << std::bitset<32>(var) << std::endl
#define print(x) std::cout << (x) << std::endl

}}

#else
#define Assert(var1, op, var2)
#define AssertExit(var1, op, var2)
#define trace(var)
#define traceb(var)
#endif


#endif	/* DEBUG_H */

