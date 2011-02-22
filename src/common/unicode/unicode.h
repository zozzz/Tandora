/*
 * File:   unicode.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 22:35
 */

// http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/cppdir/RandomStuff.htm

#ifndef UNICODE_H
#define	UNICODE_H

#include "../Exception.h"
#include "../Allocator.h"
#include "../../settings.h"
#include "../debug/debug.h"
#include <stdio.h> // for SEEK_SET, SEEK_CUR, SEEK_END

#define IS_ASCII_BYTE(byte) ( ((byte) & 0x7F) != 0 )

#define _UNICODE_STRING_BUFFER_INIT_SIZE_IN_CHAR 250

#ifdef _WCHAR_T_DEFINED
	static const size_t _WCHAR_T_SIZE = sizeof(wchar_t);
#else
	#if !defined(_UNICODE_CHAR_MAX_SIZE) || _UNICODE_CHAR_MAX_SIZE == 2
		typedef unsigned short int wchar_t;
		static const size_t _WCHAR_T_SIZE = 2;
	#else
		typedef unsigned int wchar_t;
		static const size_t _WCHAR_T_SIZE = 4;
	#endif
#endif

namespace common { namespace unicode
{

	/*

	 Hogyan használnám:
	 * UnicodeString str = L"Árvíztűrőtükörfúrógép";
	 * str += " XYZ";
	 *
	 * str.charAt(1) -> uchar;
	 *
	 * File fio("File");
	 * UnicodeString* fc = fio.readAll();
	 * UnicodeIterator uci = fio.iterator();
	 *
	 * UIterator* z = fio.iterator();
	 * UIterator* x = utf8 L"Árvíz";
	 * UString
	 *





	 */

	#pragma pack(push, 1)
		typedef struct
		{
			typedef unsigned int ValueType;

			/**
			 * Decoded char code
			 */
			ValueType value;

			/**
			 * Encoded format length in byte
			 */
			unsigned char length;

			bool operator == (int cmp) { return value == cmp; }
			bool operator != (int cmp) { return value != cmp; }
			bool operator >= (int cmp) { return value >= cmp; }
			bool operator <= (int cmp) { return value <= cmp; }
			bool operator >  (int cmp) { return value >  cmp; }
			bool operator <  (int cmp) { return value <  cmp; }

		} uchar;
	#pragma pack(pop)

	/*
	#if !defined(_UNICODE_CHAR_MAX_SIZE) || _UNICODE_CHAR_MAX_SIZE == 2
		#define _UNICODE_CHAR_MAX_SIZE 2
		typedef unsigned short int uchar;
		#define _UTF8_MAX 65535
	#elif _UNICODE_CHAR_MAX_SIZE == 4
		typedef unsigned int uchar;
		#define _UTF8_MAX 2097152
	#else
		#error "Not supported unicode char size"
		#define _UTF8_MAX 0
	#endif
	 */

	/* need operator overloads for uchar */




	//------------------------------------------------------------------------------
	// Abstract class for UTF-8, UTF-16, etc.. strings
	//------------------------------------------------------------------------------
	class ustring
	{
	public:
		//typedef Allocator<uchar> Alloc;

		ustring(size_t bufferSize = _UNICODE_STRING_BUFFER_INIT_SIZE_IN_CHAR);
		ustring(uchar* ch, size_t bufferSize = _UNICODE_STRING_BUFFER_INIT_SIZE_IN_CHAR);

	private:
		//Alloc _storage;
	};


	//------------------------------------------------------------------------------
	// Abstract class for UTF-8, UTF-16, etc.. iterators
	//------------------------------------------------------------------------------
	class Iterator
	{
	public:
		typedef const uchar* Value;

		Iterator():
			_converted(NULL),
			_convLength(0),
			_position(0) {};

		virtual ~Iterator()
		{
			if( _converted )
				FREE_ARRAY(_converted);
		};

		void seek(int offset, int origin = SEEK_CUR)
		{
			switch( origin )
			{
				case SEEK_SET:
				break;

				case SEEK_CUR:
				break;

				case SEEK_END:
				break;
			}
		}

		Value begin()
		{
			_position = 0;
			return current();
		};

		Value next()
		{
			++_position;
			return current();
		}

		Value prev()
		{
			if( _position > 0 )
			{
				--_position;
				return &_converted[_position];
			}
			return NULL;
		}

		virtual Value current() = 0;

	protected:
		uchar* _converted;
		size_t _convLength;
		size_t _position;
	};




	/*
	class UTF8String;

	class UTF8Converter
	{
	public:
		static int BM_HEADER[5];
		static int LS_OCT[5];

		UTF8Converter(const char* buffer);
		UTF8Converter(const char* buffer, size_t length);
		UTF8Converter(const wchar_t* ch);
		UTF8Converter(const wchar_t* ch, size_t length);
		UTF8Converter(const int number);
		UTF8Converter(const double number);
		UTF8Converter(const float number);

		~UTF8Converter()
		{
			// Reciver handle the _buffer pointer
		};

		size_t length() const { return _length; }

		operator uchar* () const { return _buffer; };
		operator UTF8String* ();


	private:
		uchar* _buffer;
		size_t _length;

		void _conv(const char* buffer, size_t size);
		void _conv(const wchar_t* buffer, size_t size);
	};

	class UTF8String
	{
	public:
		UTF8String();
		~UTF8String();
	};
	 * */




	_EX_PRE_DECL(UnicodeError, common::Exception)
	_EX_PRE_DECL(Malformed, UnicodeError)

	_EX_DECL(UnicodeError, common::Exception, "Invalid char")
		_EX_DECL_SUB_ERR(unicode, Malformed)
	_EX_DECL_END

	_EX_DECL_MSG(UnicodeError, Malformed, "Malformed character \\u%X at %d!")

	#include "utf8.h"
}}

#ifdef __DEBUG__
	#include <iostream>
	#include <bitset>

	#if _UNICODE_CHAR_MAX_SIZE == 2
		#define _DUMP_CHAR_BINARY(ch) std::cout << std::bitset<8>((ch) >> 8) << " " << \
			std::bitset<8>(ch) << std::endl;
	#elif _UNICODE_CHAR_MAX_SIZE == 4
		#define _DUMP_CHAR_BINARY(ch) std::cout << std::bitset<8>((ch) >> 24) << " " << \
			std::bitset<8>((ch) >> 16) << " " << \
			std::bitset<8>((ch) >> 8) << " " << \
			std::bitset<8>((ch)) << std::endl;
	#endif

	#define _DUMP_UCHAR(ch) std::cout << ((common::unicode::uchar)(ch)) << " | "; \
			std::cout.setf(std::ios::hex, std::ios::basefield); \
			std::cout.setf(std::ios::showbase); \
			std::cout << (common::unicode::uchar)(ch) << " | "; \
			std::cout.unsetf(std::ios::showbase); \
			std::cout.unsetf(std::ios::hex); \
			_DUMP_CHAR_BINARY(ch) \
			std::cout.unsetf(std::ios::hex | std::ios::uppercase);
#else
	#define _DUMP_UCHAR(uchar)
#endif

#endif	/* UNICODE_H */

