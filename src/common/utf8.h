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

#include <deque>
#include "File.h"

#ifdef __DEBUG__
#include <iostream>
#include <bitset>
#endif

namespace common{ namespace utf8
{

	// utf-8 char
	typedef unsigned int uchar;

	// uchar header base
	#define _UCHB_OCT1 0
	#define _UCHB_OCT2 0xC0
	#define _UCHB_OCT3 0xE0
	#define _UCHB_OCT4 0xF0

	// uchar flow base
	#define _UCFB 0x80

	// uchar size
	#define _UCSL_1 0x80
	#define _UCSL_2 0x800
	#define _UCSL_3 0x10000

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

	#define DETERMINE_UTF_SIZE_FROM_HEAD(head) (((head) & _UCHB_OCT4) == _UCHB_OCT4 ? 4 :\
		(((head) & _UCHB_OCT3) == _UCHB_OCT3 ? 3 :\
		(((head) & _UCHB_OCT2) == _UCHB_OCT2 ? 2 : 1)))

	#ifdef __DEBUG__
		#define _DUMP_UCHAR(ch) std::cout << ((common::utf8::uchar)(ch)) << " | "; \
				std::cout.setf(std::ios::hex, std::ios::basefield); \
				std::cout.setf(std::ios::showbase); \
				std::cout << (common::utf8::uchar)(ch) << " | "; \
				std::cout.unsetf(std::ios::showbase); \
				std::cout.unsetf(std::ios::hex); \
				std::cout << bitset<8>((ch) >> 24) << " " << \
				bitset<8>((ch) >> 16) << " " << \
				bitset<8>((ch) >> 8) << " " << \
				bitset<8>((ch)) << std::endl; std::cout.unsetf(std::ios::hex | std::ios::uppercase);
	#else
		#define _DUMP_UCHAR(uchar)
	#endif


	class ustring: public std::deque<uchar*>
	{
	public:
		ustring(const char* str);
		ustring(char str);

		operator const char*(){};

		ustring operator + (const ustring& str){};
		ustring operator + (const uchar& str){};
		ustring operator + (const char& str){};

		void slice(int start, int end = 0);
		void append(ustring* str);
		void append(uchar ch);
		void append(const char* str);
		void append(char ch);
	};

	// TODO: megnézni mi a különbség sebességben a file open módokban: rb, ab+

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
}}

#endif	/* UTF8_H */

