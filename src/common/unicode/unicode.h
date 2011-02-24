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

#define IS_ASCII_BYTE(byte) ((byte & 0x80) != 0x80)

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

namespace common
{

	typedef enum
	{
		AUTO = 0, // try to determine encoding from BOM
		UTF_8 = 1
	} Encoding;

namespace unicode
{
	// http://bytes.com/topic/c/answers/138989-class-template-parameter-function
	// http://www.newty.de/fpt/fpt.html

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
	// Unicode Iterator
	//------------------------------------------------------------------------------
	#define __CALL_DECODE() _decode(&_buffer, _converted[_convLength++])
	template<typename _Source>
	class Iterator
	{
	public:
		typedef uchar			Value;
		typedef uchar*			ValuePtr;
		typedef _Source			Source;
		typedef _Source*		SourcePtr;

		Iterator(unsigned char* buff, size_t size, void (*decoderFunction)(SourcePtr*, uchar&)):
			_convLength(0),
			_position(0),
			_decode(decoderFunction)
		{
			_buffer = buff;

			ALLOC_ARRAY(_converted, Value, size+1);
			_converted[size] = '\0';
		};

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
					if( offset < 0 )
						ex_throw(ValueError::OutOfRange, 0, 0, offset);

					while( _convLength <= offset )
					{
						__CALL_DECODE();
						if( _converted[_convLength-1] == 0 )
							ex_throw(ValueError::OutOfRange, 0, _convLength-1, offset);
					}
					_position = offset;
				break;

				case SEEK_CUR:

					if( offset < 0 && abs(offset) > _position )
						ex_throw(ValueError::OutOfRange, 0, _position, ((int)_position) + offset);

					while( _convLength <= _position + offset )
					{
						__CALL_DECODE();
						if( _converted[_convLength-1] == 0 )
							ex_throw(ValueError::OutOfRange, 0, _convLength-1, _position + offset);
					}

					_position += offset;
				break;

				case SEEK_END:
					while( _converted[_convLength-1] != 0 )
						__CALL_DECODE();

					if( _convLength - offset < 0 )
						ex_throw(ValueError::OutOfRange, 0, _convLength-1, offset);

					_position = _convLength - offset;
				break;

				default:
					ex_throwm(Exception, "Undefined origin: %d", origin);
				break;
			}
		}

		inline void reset() { _position = 0; }

		inline Value* next()
		{
			if( _convLength <= _position )
				__CALL_DECODE();

			return _converted + _position++;
		}

		inline void markStart()
		{
			if( _convLength == 0 )
				ex_throwm(Exception, "This function can only be invoked after the Iterator::next() function!", NULL);

			_markStart = _position-1;
		}

		inline void markEnd()
		{
			if( _convLength == 0 )
				ex_throwm(Exception, "This function can only be invoked after the Iterator::next() function!", NULL);

			_markEnd = _position;
		}

		inline Value* getMarkedBuffer()
		{
			size_t size = _markEnd - _markStart;
			if( size <= 0 )
				ex_throwm(ValueError::OutOfRange, "Invalid mark range (%d - %d)!", _markStart, _markEnd);

			trace(size);

			Value* ret = new Value[size+1];

			for(size_t i=0 ; i<size ; i++)
				ret[i] = _converted[_markStart+i];

			_markStart = 0;
			_markEnd = 0;

			ret[size] = '\0';
			return ret;
		}

	public:
		ValuePtr	_converted;
		SourcePtr	_buffer;

		size_t		_convLength;
		size_t		_position;
		size_t		_markStart;
		size_t		_markEnd;

		void (*_decode)(SourcePtr*, uchar&);

		//virtual inline void decode(){};
	};




	namespace Encoder
	{
		inline int UTF8(unsigned char* buff)
		{

		}
	}




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

	namespace Decoder
	{
		#define __GET_NEXT_BYTE() (*(++*_buffer))
		#define __SEEK_NEXT_BYTE() ++*_buffer
		#define _CURRENT_ (**_buffer)

		inline void UTF8(unsigned char** _buffer, uchar& _converted)
		{
			if( IS_ASCII_BYTE( _CURRENT_ ) )
			{
				_converted = _CURRENT_;
				__SEEK_NEXT_BYTE();
			}
			else if( IS_UTF8_LEAD_BYTE( _CURRENT_ ) )
			{
				unsigned char length = UTF8_GET_LENGTH_FROM_HEAD( _CURRENT_ );

				if( length == 2 )
					_converted = (_CURRENT_ & _UTF8_BM_LEAD_2) << 6;
				else if( length == 3 )
					_converted = (_CURRENT_ & _UTF8_BM_LEAD_3) << 12;
				else if( length == 4 )
					_converted = (_CURRENT_ & _UTF8_BM_LEAD_4) << 18;

				while( --length )
				{
					_converted |= (__GET_NEXT_BYTE() & _UTF8_BM_FLOW) << (length * 6);
				}

				__SEEK_NEXT_BYTE();
			}
			else if( IS_UTF8_BYTE( _CURRENT_ ) )
			{
				ex_throw(UnicodeError::Malformed, _CURRENT_, 0);
			}

		}
	}
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

