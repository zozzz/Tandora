
#include "File.h"
#include <iostream>
#include <math.h>

namespace common
{
	const char* File::_fileModes[] = {"rb", "wb+", "ab+"};

	/*File::File():
		_exists(false),
		_file(NULL) {}

	File::File(const char* file, Mode mode): _file(NULL)
	{
		open(file, mode);
	}

	File::~File()
    {
        if( _file )
            fclose(_file);
    }

	bool File::open(const char* _filename, File::Mode mode)
    {
        _file = fopen(_filename, _fileModes[mode] );

        return _exists = (_file != NULL);
    }

    void File::close()
    {
        fclose(_file);
        _file = NULL;
        _exists = false;
    }

	int File::readc()
	{
		return getc(_file);
	}

    size_t File::read(void* buffer, size_t bytesToRead)
    {
        return fread(buffer, 1, bytesToRead, _file);
    }

	bool File::writec(int ch)
	{
		return putc(ch, _file) != EOF;
	}

    size_t File::write(const void* buffer, size_t bytesToWrite)
    {
        return fwrite(buffer, 1, bytesToWrite, _file);
    }

    int64_t File::tell() const
    {
        return ftell(_file);
    }

    bool File::seek(int64_t pos, int whence = SEEK_SET)
    {
        return fseek(_file, pos, whence) == 0;
    }

    int64_t File::size() const
    {
        fseek(_file, 0L, SEEK_END);
        int64_t pos = ftell(_file);
        fseek (_file, 0L, SEEK_SET);

        return pos;
    }

    bool File::eof() const
    {
        return feof(_file) != 0;
    }

    bool File::exists() const
    {
    	return _exists;
    };
	 * */

	using namespace unicode;

	File::File(const char* file, Mode mode, Encoding enc, bool readAllContent):
		_enc(enc),
		_size(0),
		_buffer(NULL),
		_position(0),
		_rac(readAllContent)
	{
		_file = fopen(file, _fileModes[mode]);
		if( (_exists = (_file != NULL)) && _rac )
			_readToBuffer();
	}

	File::~File()
	{
		if( _buffer != NULL )
			delete[] _buffer;

		if( _file != NULL )
			fclose(_file);
	}

	void File::_readToBuffer()
	{
		_size = size();
		if( _size == -1L )
		{
			// TODO: error...
		}
		else if( _MAX_ALLOWED_FILE_SIZE < _size )
		{
			// TODO: error...
		}

		_length = ceil(_size / _UNICODE_CHAR_MAX_SIZE);
		ALLOC_ARRAY(_buffer, uchar, _length+1)
		_buffer[_length] = '\0';
		fread(_buffer, _UNICODE_CHAR_MAX_SIZE, _length, _file);

		switch( _enc )
		{
			case DEFAULT:
			case UTF_8:
				_convertBufferToUTF8();
			break;

			default:
				// TODO: error...
			break;
		}
	}

	void File::_convertBufferToUTF8()
	{
		//

		for(long int i=0 ; i<_length ; i++ )
		{
		}
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