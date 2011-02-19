/*
 * File:   unicode.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 22:35
 */

#ifndef UNICODE_H
#define	UNICODE_H

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

	namespace Internal
	{
		struct UTF8Converter
		{
		};
	}

	#ifdef U8
		#warning "common::unicode -> 'U8' is previously defined, please do not use!"
	#else
		#define U8 (common::unicode::Internal::UTF8Converter)
	#endif

	#define _BM_UTF8_OCT_1 0x7F
	#define _BM_UTF8_OCT_2 0x3F
	#define _BM_UTF8_OCT_3 0x3F
	#define _BM_UTF8_OCT_4 0x3F

	#define _BM_UTF8_HEAD_1 0x7F
	#define _BM_UTF8_HEAD_2 0x1F
	#define _BM_UTF8_HEAD_3 0x0F
	#define _BM_UTF8_HEAD_4 0x07

	class UTF8String
	{
	public:
		UTF8String();
		~UTF8String();
	};

}}

#ifdef __DEBUG__
	#include <iostream>
	#include <bitset>

	#if _UNICODE_CHAR_MAX_SIZE == 2
		#define _DUMP_CHAR_BINARY(c) std::cout << std::bitset<8>((ch) >> 8) << " " << \
			std::bitset<8>(ch) << std::endl;
	#elif _UNICODE_CHAR_MAX_SIZE == 4
		#define _DUMP_CHAR_BINARY(c) std::cout << std::bitset<8>((ch) >> 24) << " " << \
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

