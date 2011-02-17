
#include "File.h"
#include <iostream>

namespace common
{
	const char* File::_fileModes[] = { "r", "w", "a", "w+", "rb", "wb", "ab", "ab+" };

	File::File():
		_exists(false),
		_file(NULL) {}

	File::File(const char* file, Mode mode): _file(NULL)
	{
		std::cout << file << std::endl;
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
}