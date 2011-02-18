/*
 * File:   utf8.h
 * Author: Zozzz
 *
 * Created on 2011. február 16., 19:40
 * http://hu.wikipedia.org/wiki/UTF-8
 */

#ifndef UTF8_H
#define	UTF8_H

#include "../global.h"

#include "File.h"

#ifdef __DEBUG__
#include <iostream>
#include <bitset>
#endif

namespace common{ namespace utf8
{

	#if _UNICODE_CHAR_SIZE == 2
		typedef unsigned short int uchar;
		#define _UCHAR_MAX 65535
	#elif _UNICODE_CHAR_SIZE == 4
		typedef unsigned int uchar;
		#define _UCHAR_MAX 2097152
	#else
		#warning "Not supported unicode char size"
		#define _UCHAR_MAX 0
	#endif

	namespace __
	{
		struct ConvResult
		{
			size_t length;
			uchar* data;
		};

		struct UnicodeConverter
		{
			uchar* _storage;

			UnicodeConverter(const char* ch);
			~UnicodeConverter();


			operator uchar();
			operator uchar*();

			operator ConvResult();
		};

	}

	#ifdef u
		#warning "common::utf8 -> 'u' is previously defined, please do not use!"
	#else
		#define u (common::utf8::__::UnicodeConverter)
	#endif

	/*
	 std::cout << bitset<8>((ch) >> 24) << " " << \
				bitset<8>((ch) >> 16) << " " << \
				bitset<8>((ch) >> 8) << " " << \
				bitset<8>((ch)) << std::endl; std::cout.unsetf(std::ios::hex | std::ios::uppercase);

	 */

	#ifdef __DEBUG__
		#define _DUMP_UCHAR(ch) std::cout << ((common::utf8::uchar)(ch)) << " | "; \
				std::cout.setf(std::ios::hex, std::ios::basefield); \
				std::cout.setf(std::ios::showbase); \
				std::cout << (common::utf8::uchar)(ch) << " | "; \
				std::cout.unsetf(std::ios::showbase); \
				std::cout.unsetf(std::ios::hex); \
				std::cout << std::bitset<8>((ch) >> 24) << " " << \
				std::bitset<8>((ch) >> 16) << " " << \
				std::bitset<8>((ch) >> 8) << " " << \
				std::bitset<8>((ch)) << std::endl; std::cout.unsetf(std::ios::hex | std::ios::uppercase);
	#else
		#define _DUMP_UCHAR(uchar)
	#endif


	class ustring
	{
	public:
		ustring();
		ustring(const char* ch);
		~ustring();

		// operators: +, []

	protected:
		uchar* _storage;
	};


	class FileIO
	{
	public:
		FileIO();
		FileIO(const char* file);
		~FileIO();

		bool open(const char* file);
		uchar readc();
		uchar* read();

		//size_t writec(const uchar ch);
		size_t write(const uchar* ch, size_t length = 0);

	private:
		/*typedef unsigned char buffer_t;

		buffer_t* _buffer;
		*/
		uchar* _buffer;
		File* _file;
		size_t _position;
		size_t _length;

		void _readAllContentToBuffer();
	};




	// TODO: megnézni mi a különbség sebességben a file open módokban: rb, ab+

	/*
	class FileStream
	{
	public:
		FileStream();
		FileStream(const char* file){ _file = new File(); open(file); };
		virtual ~FileStream();

		inline bool open(const char* file){ return _open(file); }

	protected:
		virtual bool _open(const char* file) = 0;
		virtual File* _file(){ return __file; };

	private:
		File* __file;
	};

	class FileInputStream: public FileStream
	{
	public:
		FileInputStream(): FileStream(){};
		FileInputStream(const char* file): FileStream(file){};

		uchar readc();

	protected:
		virtual bool _open(const char* file);
	};

	class FileOutputStream: public FileStream
	{
	public:
		FileOutputStream(): FileStream(){};
		FileOutputStream(const char* file): FileStream(file){};

	protected:
		virtual bool _open(const char* file);
	};

	class FileIO: public FileInputStream, public FileOutputStream
	{
	public:
		FileIO(): FileInputStream(){};
		FileIO(const char* file): FileInputStream(file){};

	protected:
		virtual bool _open(const char* file);
	};
	 * */
}}

#endif	/* UTF8_H */

