
#include "utf8.h"

namespace common { namespace utf8 {

	FileStream::FileStream()
	{
		_file = new File();
	}

	FileStream::~FileStream()
	{
		delete _file;
	}

	bool FileInputStream::_open(const char* file)
	{
		_file()->open(file, File::READ_BINARY);
	}


#define _CBT_CHUNK(x, s) ((common::utf8::uchar)( (x) << (s) ))
#define _CONVERT_BUFFER_TO_UCHAR_1(buff) _CBT_CHUNK(buff[0], 0)
#define _CONVERT_BUFFER_TO_UCHAR_2(buff) (\
	_CBT_CHUNK(buff[0], 8) | \
	_CBT_CHUNK(buff[1], 0) )
#define _CONVERT_BUFFER_TO_UCHAR_3(buff) (\
	_CBT_CHUNK(buff[0], 16) | \
	_CBT_CHUNK(buff[1], 8) | \
	_CBT_CHUNK(buff[2], 0) )
#define _CONVERT_BUFFER_TO_UCHAR_4(buff) (\
	_CBT_CHUNK(buff[0], 24) | \
	_CBT_CHUNK(buff[1], 16) | \
	_CBT_CHUNK(buff[2], 8) | \
	_CBT_CHUNK(buff[3], 0) )
#define _CONVERT_BUFFER_TO_UCHAR(buff, len) _CONVERT_BUFFER_TO_UCHAR_##len(buff)

	uchar FileInputStream::readc()
	{
		if( _file()->eof() )
			return UCT_NULL;

		using namespace std;

		int ch[4];
		ch[0] = _file()->readc();
		char uct = DETERMINE_UCT(ch[0]);

		if( uct == UCT_HEAD )
		{
			size_t length = DETERMINE_UTF_SIZE_FROM_HEAD(ch[0]);
			if( length == 1 )
				return _CONVERT_BUFFER_TO_UCHAR(ch, 1);

			for(unsigned short i=1 ; i<length ; i++)
				ch[i] = _file()->readc();

			if( length == 2 )
				return _CONVERT_BUFFER_TO_UCHAR(ch, 2);
			else if( length == 3 )
				return _CONVERT_BUFFER_TO_UCHAR(ch, 3);
			else
				return _CONVERT_BUFFER_TO_UCHAR(ch, 4);
		}
		else if( uct == UCT_ASCII )
		{
			return (uchar)ch[0];
		}
		else
		{
			// TODO: throw error
		}

		return UCT_NULL;
	}

	bool FileOutputStream::_open(const char* file)
	{
		_file()->open(file, File::APPEND_BINARY);
	}

	bool FileIO::_open(const char* file)
	{
		_file()->open(file, File::APPEND_BINARY);
	}
}}