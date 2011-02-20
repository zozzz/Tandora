
#include "File.h"
#include <iostream>
#include <math.h>
#include "Exception.h"
#include <errno.h>
#include <string.h>

namespace common
{
	const char* File::_fileModes[] = {"rb", "wb+", "ab+"};

	using namespace unicode;

	const char* File::formatSize(size_t size)
	{
		return "";
	}

	File::File(const char* file, Mode mode, Encoding enc, bool readAllContent):
		_enc(enc),
		_size(0),
		_buffer(NULL),
		_position(0),
		_rac(readAllContent),
		_fileName(file)
	{
		_file = fopen(file, _fileModes[mode]);
		if( _exists = (_file != NULL) )
		{
			if( _rac )
				_readToBuffer();
		}
		else
			ex_throw(IOError::FileNotFound, file);
	}

	File::~File()
	{
		FREE_ARRAY(_buffer);

		if( _exists )
			fclose(_file);
	}

	void File::_readToBuffer()
	{
		_size = size();
		if( _size == -1L )
		{
			ex_throwm(IOError, strerror(errno), NULL);
		}
		else if( _MAX_ALLOWED_FILE_SIZE < _size )
		{
			ex_throw(IOError::FileIsTooLarge, _fileName, File::formatSize(_size));
		}

		unsigned char* buffer;
		ALLOC_ARRAY(buffer, unsigned char, _size+1);
		fread(buffer, 1, _size, _file);
		buffer[_size] = '\0';

		ex_try
		{
			switch( _enc )
			{
				case DEFAULT:
				case UTF_8:
					_convertBufferToUTF8(buffer);
				break;

				default:
					// TODO: error...
				break;
			}
		}
		ex_catch(...)
		{
			FREE_ARRAY(buffer);
			ex_rethrow;
		}

		FREE_ARRAY(buffer);
	}

	// 858
	void File::_convertBufferToUTF8(unsigned char* buffer)
	{
		_length = 0;
		ALLOC_ARRAY(_buffer, uchar, _size+1);

		uchar ch;

		if( _size > 2 && _UTF8_DETECT_BOM(buffer) ) // skip BOM
			ch = *buffer += 3;
		else
			ch = *buffer++;


		short int ucharOctLength = 0;
		do
		{
			if( _UTF8_OCT_IS_UTF8(ch) )
			{
				if( _UTF8_OCT_IS_HEAD(ch) )
				{
					ucharOctLength = _UTF8_GET_LENGTH_FROM_HEAD(ch);
					_buffer[_length] = (ch & UTF8Converter::BM_HEADER[ucharOctLength]) << (6 * --ucharOctLength);

				}
				else
				{
					if( ucharOctLength == 0 )
					{
						ex_throw(UnicodeError::Malformed, ch, _length);
					}

					_buffer[_length] |= (ch & _BM_UTF8_FLOW) << (6 * --ucharOctLength);

					if( ucharOctLength == 0 )
						_length++;
				}
			}
			else
			{
				_buffer[_length++] = ch;
			}

			ch = *buffer++;

		}while( ch );

		if( _length != _size )
			RESIZE_ARRAY(_buffer, uchar, _length+1);

		_buffer[_length] = '\0';
	}

	// TODO: unicode support + _buffer slice if _rac == true
	size_t File::read(void* buffer, size_t bytesToRead)
	{
		return fread(buffer, 1, bytesToRead, _file);
	}

	uchar File::readc()
	{
		if( _rac )
		{
			if( _length > _position )
				return _buffer[_position++];
		}
		else if( _exists )
		{

		}

		return 0;
	}
}