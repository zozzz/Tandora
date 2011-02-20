/*
 * File:   unicode.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 22:35
 */

#ifndef UNICODE_H
#define	UNICODE_H

#include "Exception.h"

namespace common { namespace unicode
{
	#if !defined(_UNICODE_CHAR_MAX_SIZE) || _UNICODE_CHAR_MAX_SIZE == 2
		#define _UNICODE_CHAR_MAX_SIZE 2
		typedef unsigned short int uchar;
		typedef uchar utf8c;
		#define _UTF8_MAX 65535
	#elif _UNICODE_CHAR_MAX_SIZE == 4
		typedef unsigned int uchar;
		typedef uchar utf8c;
		#define _UTF8_MAX 2097152
	#else
		#error "Not supported unicode char size"
		#define _UTF8_MAX 0
	#endif

	#define _BM_UTF8_FLOW 0x3F

	#define _BM_UTF8_HEAD_1 0x7F
	#define _BM_UTF8_HEAD_2 0x1F
	#define _BM_UTF8_HEAD_3 0x0F
	#define _BM_UTF8_HEAD_4 0x07

	#define _T_UTF8_OCT_NULL	0x1
	#define _T_UTF8_OCT_HEAD	0x2
	#define _T_UTF8_OCT_FLOW	0x4
	#define _T_UTF8_OCT_ASCII	0x8

	#define _UTF8_OCT_IS_UTF8(oct) ( ((oct) & 0x80) == 0x80 )
	#define _UTF8_OCT_IS_HEAD(oct) ( ((oct) & 0xC0) == 0xC0 )
	#define _UTF8_OCT_IS_ASCII(oct) ( ((oct) & 0x7F) != 0 )

	#define _UTF8_DETERMINE_OCT_TYPE(oct) (\
		((oct) & 0x80) == 0x80 ? ( ((oct) & 0xC0) == 0xC0 ? _T_UTF8_OCT_HEAD : _T_UTF8_OCT_FLOW ) : \
		((oct) & 0x7F) != 0 ? _T_UTF8_OCT_ASCII : _T_UTF8_OCT_NULL)

	#define _UTF8_BOM_0 0xEF
	#define _UTF8_BOM_1 0xBB
	#define _UTF8_BOM_2 0xBF
	#define _UTF8_DETECT_BOM(buff) ((buff[0]) == _UTF8_BOM_0 && (buff[1]) == _UTF8_BOM_1 && (buff[2]) == _UTF8_BOM_2)

	#define _UTF8_GET_LENGTH_FROM_HEAD(oct) ( oct < 0xE0 ? 2 : ( oct < 0xF0 ? 3 : 4) )

	#ifdef U8
		#warning "common::unicode -> 'U8' is previously defined, please do not use!"
	#else
		#define U8 (common::unicode::UTF8Converter)
	#endif

	class UTF8Converter
	{
	public:
		static int BM_HEADER[5];
		static int LS_OCT[5];

		UTF8Converter();
		~UTF8Converter();
	};

	class UTF8String
	{
	public:
		UTF8String();
		~UTF8String();
	};


	_EX_PRE_DECL(UnicodeError, common::Exception)
	_EX_PRE_DECL(Malformed, UnicodeError)

	_EX_DECL(UnicodeError, common::Exception, "Invalid char")
		_EX_DECL_SUB_ERR(unicode, Malformed)
	_EX_DECL_END

	_EX_DECL_MSG(UnicodeError, Malformed, "Malformed character \\u%X at %d!")
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

