/*
 * File:   File.h
 * Author: Zozzz
 *
 * Created on 2011. február 16., 20:32
 */



#ifndef FILE_H
#define	FILE_H

#include <stdio.h>

#include "../settings.h"
#include "unicode/unicode.h"
#include "unicode/String.h"

namespace common
{
	class File
	{
	public:
		static const char* formatSize(size_t size);

	public:
		typedef enum
		{
			READ = 0,
			WRITE = 1,
			APPEND = 2
		} Mode;

		typedef enum
		{
			AUTO_DETECT = 0, // try to determine encoding from BOM
			UTF_8 = 1
		} Encoding;

		File(const char* file, Mode mode = READ, Encoding enc = UTF_8);

		~File();

		int readc();

		size_t read(void* buffer, size_t bytesToRead);

		bool writec(unsigned int ch);

		long int write(const void* buffer, size_t bytesToWrite);

		inline long int tell() const
		{
			return ftell(_file);
		};

		inline bool seek(long int offset, int origin)
		{
			return fseek(_file, offset, origin) != -1;
		}

		unicode::Reader* reader();
		unicode::String* getAllContent();

		long int write(unicode::Writer* writer, bool writeBOM = false);
		long int write(unicode::uchar* ch, long int size, bool writeBOM = false);

		inline long int size()
		{
			long int s;
			fseek(_file, 0, SEEK_END);
			s = ftell(_file);
			fseek(_file, 0, SEEK_SET);

			return s;
		};

		inline long int length(){ return _length; };

		inline bool eof() const { return feof(_file) == EOF; };

		inline bool exists() const { return _exists; };


	private:
		static const char* _fileModes[];

		/**
		 * Truw when file is successfully opened
		 */
		bool _exists;

		/**
		 * File pointer
		 */
		FILE* _file;

		/**
		 * Size in bytes
		 */
		long int _size;

		/**
		 * Length in chars
		 */
		long int _length;

		/**
		 * Encoding
		 */
		Encoding _enc;

		Encoding _detectEncoding();

	};
}

#endif	/* FILE_H */

