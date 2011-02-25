/*
 * File:   unicode.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 22:35
 */

// http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/cppdir/RandomStuff.htm

#ifndef UNICODE_H
#define	UNICODE_H

#include "../../settings.h"
#include "../Exception.h"
#include "../Allocator.h"
#include "../debug/debug.h"


#define IS_ASCII_BYTE(byte) ((byte & 0x80) != 0x80)

//#define _UNICODE_STRING_BUFFER_INIT_SIZE_IN_CHAR 250

namespace common { namespace unicode
{

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

	//------------------------------------------------------------------------------
	// Exceptions
	//------------------------------------------------------------------------------

	_EX_PRE_DECL(UnicodeError, common::Exception)
	_EX_PRE_DECL(Malformed, UnicodeError)

	_EX_DECL(UnicodeError, common::Exception, "Invalid char")
		_EX_DECL_SUB_ERR(unicode, Malformed)
	_EX_DECL_END

	_EX_DECL_MSG(UnicodeError, Malformed, "Malformed character \\u%X at %d!")


	namespace Internal
	{
		template<typename _Input, typename _Output>
		class Converter
		{
		public:
			typedef _Input		Input;
			typedef _Input*		InputPtr;
			typedef _Output		Output;
			typedef _Output*	OutputPtr;
			typedef Converter<_Input, _Output> _self;

			/**
			 * Constructor
			 * @param buffer Input data, which is managed by the Converter instance
			 * @param length Input data length
			 * @param cb Converter function (common::unicode::Encode::* / common::unicode::Decode::*)
			 */
			Converter(InputPtr buffer, long int length, Output (*cb)(InputPtr*, uchar&)):
				_bufferLength(length),
				_buffer(buffer),
				_bufferStart(buffer),
				_marked(NULL),
				_mark(false),
				_converterCache(0),
				_convert(cb)
			{
				AssertExit((int)_buffer[length] ,==, 0);
			};

			/**
			 * Destructor, delete: Input buffer, _marked
             */
			~Converter()
			{
				if( _bufferStart != NULL )
					FREE_ARRAY(_bufferStart);

				_buffer = NULL;

				if( _marked != NULL )
					FREE_ARRAY(_marked);
			};


			/**
			 * This function convert input data to output data
			 */
			inline Output next()
			{
				if( _mark )
				{
					if( _markedLength >= _markedBuffLength )
					{
						_markedBuffLength += 100;
						RESIZE_ARRAY(_marked, Output, _markedBuffLength);
					}

					return _marked[_markedLength++] = _convert(&_buffer, _converterCache);
				}

				return _convert(&_buffer, _converterCache);
			}

			/**
			 * Reset buffer position and continue from beginning
             */
			inline void reset()
			{
				_buffer = _bufferStart;
			}

			/**
			 * Mark makes the conversion process, which queried the value of the markedBuffer function
			 */
			inline void startMark()
			{
				_mark = true;
				if( _marked == NULL )
					ALLOC_ARRAY(_marked, Output, (_markedBuffLength = _bufferLength));
				_markedLength = 0;
			};

			/**
			 * Closing marking
			 */
			inline void endMark()
			{
				_mark = false;
			};

			/**
			 * Get marked buffer
			 * @return Pointer array of output type
			 */
			inline OutputPtr markedBuffer()
			{
				OutputPtr ret;
				ALLOC_ARRAY(ret, Output, _markedLength+1);
				ret[_markedLength] = 0;
				while( _markedLength-- )
					ret[_markedLength] = _marked[_markedLength];
				return ret;
			};

		private:
			size_t		_bufferLength;
			size_t		_markedLength;
			InputPtr	_buffer;
			InputPtr	_bufferStart;
			OutputPtr	_marked;
			size_t		_markedBuffLength;
			bool		_mark;
			uchar		_converterCache;

			/**
			 * Converter function pointer
			 */
			Output (*_convert)(InputPtr*, uchar&);
		};

	}

	typedef Internal::Converter<uchar, unsigned char> Writer;
	typedef Internal::Converter<unsigned char, uchar> Reader;

	#define __UCONV_CURRENT_BYTE()	 (**buffer)
	#define __UCONV_SEEK_NEXT()		 (++(*buffer))
	#define __UCONV_SEEK_NEXT_POST() (*(*buffer)++)
	#define __UCONV_GET_NEXT()		 (*++(*buffer))

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

