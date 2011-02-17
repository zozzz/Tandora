/*
 * File:   File.h
 * Author: Zozzz
 *
 * Created on 2011. február 16., 20:32
 */

#ifndef FILE_H
#define	FILE_H

#include <stdio.h>
#include <inttypes.h>

namespace common
{
	class File
	{
	public:
		typedef enum
		{
			READ = 0,
			WRITE,
			APPEND,
			READ_WRITE,
			READ_BINARY,
			WRITE_BINARY,
			APPEND_BINARY,
			READ_APPEND_BINARY
		} Mode;

		File();

		File(const char* file, Mode mode);

		~File();

		bool open(const char* file, Mode mode);

		void close();

		int readc();

		size_t read(void* buffer, size_t bytesToRead);

		bool writec(int ch);

		size_t write(const void* buffer, size_t bytesToWrite);

		int64_t tell() const;

		bool seek(int64_t pos, int whence);

		int64_t size() const;

		bool eof() const;

		bool exists() const;

	private:
		bool _exists;
		FILE* _file;
		static const char* _fileModes[];
	};
}

#endif	/* FILE_H */

