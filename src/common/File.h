/*
 * File:   File.h
 * Author: Zozzz
 *
 * Created on 2011. február 16., 20:32
 */

#include <stdio.h>
#include "unicode/unicode.h"

#ifndef FILE_H
#define	FILE_H

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

		File(const char* file, Mode mode = READ, Encoding enc = UTF_8);

		~File();

		int readc();

		size_t read(void* buffer, size_t bytesToRead);

		bool writec(unsigned int ch);

		size_t write(const void* buffer, size_t charsToWrite);

		long int tell() const;

		bool seek(long int offset, int origin);
		
		unicode::Iterator* iterator();
		unicode::ustring* getAllContent();

		inline long int size()
		{
			if( _size == 0 )
			{
				fseek(_file, 0, SEEK_END);
				_size = ftell(_file);
				fseek(_file, 0, SEEK_SET);
			}

			return _size;
		};

		inline long int length(){ return _length; };

		inline bool eof() const { return feof(_file) == EOF; };

		inline bool exists() const { return _exists; };
		
		
	private:
		static const char* _fileModes[];

		/**
		 * Read all content from file when open
		 */
		bool _rac;

		/**
		 * Truw when file is successfully opened
		 */
		bool _exists;

		/**
		 * File pointer
		 */
		FILE* _file;

		/**
		 * File full path
		 */
		const char* _fileName;

		/**
		 * Size in bytes
		 */
		long int _size;

		/**
		 * Length in chars
		 */
		long int _length;

		/**
		 * Current position in buffer
		 */
		long int _position;

		/**
		 * Internal buffer for unicode chars
		 */
		unicode::uchar* _buffer;
		
		/**
		 * Encoding
		 */
		Encoding _enc;


		//void _readToBuffer();
		//void _convertBufferToUTF8(unsigned char* buffer);
	};
}

#endif	/* FILE_H */

