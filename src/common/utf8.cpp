/**
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
 **/

/*
#include "utf8.h"
#include <string.h>



namespace common{ namespace utf8 {

	namespace __
	{
		UnicodeConverter::UnicodeConverter(const char* ch)
		{
		}

		UnicodeConverter::~UnicodeConverter()
		{
		}

		UnicodeConverter::operator  ConvResult()
		{
		}

		UnicodeConverter::operator  uchar()
		{
		}

		UnicodeConverter::operator  uchar*()
		{
		}
	}

	FileIO::FileIO():
		_buffer(NULL),
		_file(NULL){ }

	FileIO::FileIO(const char* file):
		_buffer(NULL),
		_file(NULL)
	{
		open(file);
	}

	FileIO::~FileIO()
	{
		if( _file != NULL )
			delete _file;

		if( _buffer != NULL )
			delete[] _buffer;
	}

	bool FileIO::open(const char* file)
	{
		if( _file != NULL )
			delete _file;

		_position = 0;
		_file = new File(file, File::READ_APPEND_BINARY);

		if( _file == NULL )
		{
			// TODO: throw exeception
		}
		else
			_readAllContentToBuffer();

		return _file != NULL;
	}

	#define _BOM_0 0xEF
	#define _BOM_1 0xBB
	#define _BOM_2 0xBF
	#define _DETECT_BOM(buff) ((buff[0]) == _BOM_0 && (buff[1]) == _BOM_1 && (buff[2]) == _BOM_2)

	// uchar header base
	#define _UCHB_OCT_1 0
	#define _UCHB_OCT_1_N 0x7F
	#define _UCHB_OCT_2 0xC0
	#define _UCHB_OCT_2_N 0x3F
	#define _UCHB_OCT_3 0xE0
	#define _UCHB_OCT_3_N 0x1F
	#define _UCHB_OCT_4 0xF0
	#define _UCHB_OCT_4_N 0x0F

	// NULL karakter
	#define UCT_NULL	0
	// ASCII 7 bites karakter
	#define UCT_ASCII	1
	// Többájtos karakterszekvencia kezdete
	#define UCT_HEAD	2
	// többájtos karakterszekvencia folytatása
	#define UCT_FLOW	4

	#define DETERMINE_UCT(chunk) (((chunk) == 0) ? UCT_NULL :\
		(((chunk) & 0x80) != 0x80 ? UCT_ASCII :\
		(((chunk) & 0x40) != 0x40 ? UCT_FLOW : UCT_HEAD)))

	#define DETERMINE_UTF_SEQ_LENGTH_FROM_HEAD(head) (((head) & _UCHB_OCT_4) == _UCHB_OCT_4 ? 4 :\
		(((head) & _UCHB_OCT_3) == _UCHB_OCT_3 ? 3 :\
		(((head) & _UCHB_OCT_2) == _UCHB_OCT_2 ? 2 : 1)))

	// uchar flow base
	#define _UCFB 0x80

	// uchar size
	#define _UCSL_1 0x80
	#define _UCSL_2 0x800
	#define _UCSL_3 0x10000

	#define UTF_CHUNK_1(x) ( (uchar)(x) )
	#define UTF_CHUNK_2(x) ( ((uchar)(x)) << 6 )
	#define UTF_CHUNK_3(x) ( ((uchar)(x)) << 12 )
	#define UTF_CHUNK_4(x) ( ((uchar)(x)) << 18 )
	#define INIT_UCHAR_FROM_HEAD(head, length) ( UTF_CHUNK_##length( (head) & _UCHB_OCT_##length##_N ) )
	#define APPEND_FBYTE_TO_UCHAR(byte, pos) ( UTF_CHUNK_##pos( (byte) & _UCHB_OCT_##pos##_N ) )



	void FileIO::_readAllContentToBuffer()
	{
		size_t size = _file->size();

		if( _buffer != NULL )
			delete[] _buffer;

		//_buffer = new buffer_t[size];
		unsigned char* buff = new unsigned char[size];
		_file->read(buff, size);

		uchar* buffer = new uchar[size];

		size_t i = 0;

		if( size > 3 && _DETECT_BOM(buff) )
			i = 3;

		_length = 0;
		unsigned short int uct;
		unsigned short int seq_length = 0;
		uchar uch = 0;

		for( ; i<size ; i++)
		{
			uct = DETERMINE_UCT(buff[i]);

			switch( uct )
			{
				case UCT_HEAD:
					seq_length = DETERMINE_UTF_SEQ_LENGTH_FROM_HEAD(buff[i]);
					//uch = INIT_UCHAR_FROM_HEAD(buff[i], seq_length);
					uch =	( seq_length == 1 ? INIT_UCHAR_FROM_HEAD(buff[i], 1) :
							( seq_length == 2 ? INIT_UCHAR_FROM_HEAD(buff[i], 2) :
							( seq_length == 3 ? INIT_UCHAR_FROM_HEAD(buff[i], 3) :
							  INIT_UCHAR_FROM_HEAD(buff[i], 4) )));
					seq_length--;
				break;

				case UCT_FLOW:
					if( uch == 0 ) // error
					{
					}
					else // TODO: vizsgályni, hogy mekkora lesz az új karakter mérete
					{
						if( seq_length == 1 )
						{
							uch |= APPEND_FBYTE_TO_UCHAR(buff[i], 1);
						}
						else if( seq_length == 2 )
						{
							uch |= APPEND_FBYTE_TO_UCHAR(buff[i], 2);
						}
						else
						{
							uch |= APPEND_FBYTE_TO_UCHAR(buff[i], 3);
						}

						seq_length--;
					}
				break;

				case UCT_ASCII:
					seq_length = 0;
					uch = buff[i];
				break;

				case UCT_NULL:
					seq_length = 0;
					uch = 0;
				break;
			}

			if( seq_length == 0 )
			{
				buffer[_length++] = uch;

				//_DUMP_UCHAR(uch);

				if( uch == 0 )
				{
					// vizsgálni, hogy végig ért-e
				}

				uch = 0;
			}
		}

		_buffer = new uchar[_length+1];
		_buffer[_length] = 0;
		memcpy(_buffer, buffer, sizeof(uchar) * _length);

		delete[] buffer;
		delete[] buff;
	}

	uchar FileIO::readc()
	{
		if( _position < _length )
			return _buffer[_position++];

		return 0;
	}

	uchar* FileIO::read()
	{
		if( _length == 0 )
			return NULL;

		uchar* ret = new uchar[_length];
		memcpy(ret, _buffer, sizeof(uchar) * _length);
		return ret;
	}

}}
*/