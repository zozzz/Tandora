
#include "Exception.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

namespace common
{
	std::deque<Internal::EX_CallStackItem*> Exception::_stack;

	const char* Internal::formatMsg(const char* msg, ...)
	{
		va_list args;
		va_start(args, msg);

		size_t buffSize = (strlen(msg)+1) * sizeof(char);
		char* buffer;
		ALLOC_ARRAY(buffer, char, buffSize+1);
		buffer[buffSize] = '\0';
		int buffNewSize = 0;
		for(;;)
		{
			buffNewSize = vsnprintf(buffer, buffSize+1, msg, args);
			if( buffNewSize == -1 )
			{
				FREE_ARRAY(buffer);
				// some error...
				buffer = NULL;
				break;
			}
			else if( buffNewSize > buffSize )
			{
				RESIZE_ARRAY(buffer, char, buffNewSize+1);
				buffSize = buffNewSize;
				buffer[buffSize] = '\0';
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

		return buffer;
	}

	void Internal::_addStack(const char* _fn, const char* _file, const int _line)
	{
		Exception::_stack.push_back(new Internal::EX_CallStackItem(_fn, _file, _line));
	}

	void Internal::_clearStack()
	{
		Exception::_stack.clear();
	}

	Exception::Exception(const char* _fn, const char* _file, const int _line, const char* _msg) throw():
		func(_fn),
		file(_file),
		line(_line),
		msg(_msg)
	{
		Internal::_addStack(_fn, _file, _line);
	}

	Exception::~Exception() throw()
	{
		/*if( func != NULL )
			delete func;
		if( file != NULL )
			delete file;
		if( msg != NULL )
			delete msg;*/
	};



}
