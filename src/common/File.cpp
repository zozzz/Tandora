

#include <iostream>
#include <math.h>
#include <errno.h>
#include <string.h>

#include "../settings.h"
#include "File.h"
#include "Allocator.h"
#include "Exception.h"

namespace common
{
	const char* File::_fileModes[] = {"rb", "wb+", "ab+"};

	using namespace unicode;

	const char* File::formatSize(size_t size)
	{
		return "";
	}

	File::File(const char* file, Mode mode, Encoding enc):
		_enc(enc),
		_size(0),
		_buffer(NULL),
		_position(0),
		_fileName(file)
	{
		_file = fopen(file, _fileModes[mode]);
		if( _file == NULL )
			ex_throw(IOError::FileNotFound, file);
	}

	File::~File()
	{
		FREE_ARRAY(_buffer);		
	}
	
	unicode::Iterator* File::iterator()
	{
		_size = size();
		if( _size == -1L )
		{
			ex_throwm(IOError, "File: '%s', %s", _fileName, strerror(errno));
		}
		else if( _MAX_ALLOWED_FILE_SIZE < _size )
		{
			ex_throw(IOError::FileIsTooLarge, _fileName, File::formatSize(_size));
		}

		unsigned char* buffer;
		ALLOC_ARRAY(buffer, unsigned char, _size+1);
		fread(buffer, 1, _size, _file);
		buffer[_size] = '\0';
		
		unicode::Iterator* it;
		
		ex_try
		{
			switch( _enc )
			{
				case AUTO:
				case UTF_8:
					it = new unicode::Internal::UTF8Iterator<unsigned char>(buffer, _size);
				break;

				default:
					ex_throw(Exception, "Undefined encoding: %d", _enc);
				break;
			}
		}
		ex_catch(...)
		{
			if( it != NULL )
				delete it;
			ex_rethrow;
		}
		
		return it;
	}
	
	unicode::ustring* File::getAllContent()
	{
		return NULL;
	}

	size_t File::read(void* buffer, size_t bytesToRead)
	{
		return fread(buffer, 1, bytesToRead, _file);
	}

	int File::readc()
	{
		return getc(_file);
	}
}