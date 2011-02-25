
#include "File.h"
#include "unicode/unicode.h"
#include "unicode/utf8.h"
#include <math.h>
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

	File::File(const char* file, Mode mode, Encoding enc):
		_fileName(file),
		_size(0),
		_position(0),
		_buffer(NULL),
		_enc(enc)
	{
		_file = fopen(file, _fileModes[mode]);
		if( _file == NULL )
			ex_throw(IOError::FileNotFound, file);

		_size = size();
		if( _size == -1L )
		{
			ex_throwm(IOError, "File: '%s', %s", _fileName, strerror(errno));
		}
		else if( _MAX_ALLOWED_FILE_SIZE < _size )
		{
			ex_throw(IOError::FileIsTooLarge, _fileName, File::formatSize(_size));
		}
	}

	File::~File()
	{
		FREE_ARRAY(_buffer);
	}

	unicode::Reader* File::reader()
	{
		seek(0L, SEEK_SET);
		unsigned char* buffer;
		ALLOC_ARRAY(buffer, unsigned char, _size+1);
		fread(buffer, 1, _size, _file);
		buffer[_size] = '\0';

		// TODO: BOM Detektálás + warning ha eltérő a megadott encodingtól


		/*if( _enc == AUTO_DETECT )
		{
		}
		else if( _enc == UTF_8 )
		{
			return new Reader(buffer, _size, Decode::UTF8);
		}*/

		switch( _enc )
		{
			case UTF_8:
				return new Reader(buffer, _size, Decode::UTF8);
			break;

			case AUTO_DETECT:
			break;

			default:
			break;
		}

		return NULL;
	}

	long int File::write(unicode::Writer* writer, bool writeBOM)
	{
		writer->reset();
		seek(0, SEEK_SET);

		if( writeBOM )
		{
			switch( _enc )
			{
				case UTF_8:
					fputc(_UTF8_BOM_0, _file);
					fputc(_UTF8_BOM_1, _file);
					fputc(_UTF8_BOM_2, _file);
				break;

				case AUTO_DETECT:
				default:
					ex_throwm(Exception, "AUTO_DETECT encoding not support BOM writing!", NULL);
				break;
			}
		}

		unsigned int ch;
		size_t writed = 0;

		while( (ch = writer->next()) )
		{
			_DUMP_UCHAR(ch);
			fputc(ch, _file);
			++writed;
		}

		return writed;
	}

	long int File::write(unicode::uchar* ch, long int size, bool writeBOM)
	{
		Writer* w;

		switch( _enc )
		{
			case UTF_8:
				w = new Writer(ch, size, Encode::UTF8);
			break;

			case AUTO_DETECT:
			default:
				ex_throwm(Exception, "AUTO_DETECT encoding not supported when need to write uchar*!", NULL);
			break;
		}

		long int writed = write(w, writeBOM);
		delete w;
		return writed;
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