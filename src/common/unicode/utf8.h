/*
 * File:   utf8.h
 * Author: Zozzz
 *
 * Created on 2011. február 22., 12:27
 */

#ifndef UTF8_H
#define	UTF8_H

#include "unicode.h"
#include "../../global.h"

#define _UTF8_BM_LEAD_2 0x1F
#define _UTF8_BM_LEAD_3 0x0F
#define _UTF8_BM_LEAD_4 0x07
#define _UTF8_BM_FLOW 0x3F

#define IS_UTF8_BYTE(byte) ( ((byte) & 0x80) == 0x80 )
#define IS_UTF8_LEAD_BYTE(oct) ( ((oct) & 0xC0) == 0xC0 )

#define UTF8_DETERMINE_OCT_TYPE(oct) (\
	((oct) & 0x80) == 0x80 ? ( ((oct) & 0xC0) == 0xC0 ? _UTF8_OCT_HEAD : _UTF8_OCT_FLOW ) : 0)

#define _UTF8_BOM_0 0xEF
#define _UTF8_BOM_1 0xBB
#define _UTF8_BOM_2 0xBF
#define UTF8_DETECT_BOM(buff) ((buff[0]) == _UTF8_BOM_0 && (buff[1]) == _UTF8_BOM_1 && (buff[2]) == _UTF8_BOM_2)

#define UTF8_GET_LENGTH_FROM_HEAD(oct) ( oct < 0xE0 ? 2 : ( oct < 0xF0 ? 3 : 4) )
#define UTF8_GET_LENGTH_FROM_CH(ch) ( (ch) < 0x800 ? 2 : ((ch) < 0x10000 ? 3 : 4) )


#ifdef utf8
	#warning "common::unicode -> 'utf8' is previously defined, please do not use!"
#else
	#define utf8 (common::unicode::Internal::UTF8Converter)
#endif

namespace common { namespace unicode
{
	namespace Decode
	{
		uchar UTF8(unsigned char** buffer, uchar& cache);
	}

	namespace Encode
	{
		unsigned char UTF8(uchar** buffer, uchar& cache);
	}
}}



	/*class UTF8Converter : public UConverter
	{
	public:
		inline static uchar* decode(const char* array){}
		inline static uchar* decode(const wchar_t* array){}

		inline static char* encode(const uchar* array){}
		inline static char* encode(const ustring* str){}

		// constructors for: char*, wchar_t*, uchar*
		// operators for: uchar*, ustring*
	};*/


	/*template<typename _SourceType>
	class UTF8Iterator: public Iterator {};

	template<>
	class UTF8Iterator<unsigned char>: public Iterator
	{
	public:
		typedef unsigned char	Source;
		typedef unsigned char*	SourcePtr;

		UTF8Iterator(SourcePtr buffer, size_t length):
			Iterator()
		{
			_buffer = _start = buffer;

			ALLOC_ARRAY(_converted, Value, length+1);
			_converted[length] = '\0';
		}

		virtual ~UTF8Iterator()
		{
			if( _start != NULL )
				delete[] _start;
		}

	protected:

		#define __GET_NEXT_BYTE() (*_buffer++)
		#define __SEEK_NEXT_BYTE() _buffer++
		#define _CURRENT_ (*_buffer)

		// 0.622 / 100
		virtual inline void decode()
		{
			if( IS_ASCII_BYTE( _CURRENT_ ) )
			{
				_converted[_convLength++] = _CURRENT_;
				__SEEK_NEXT_BYTE();
				return;
			}
			else if( IS_UTF8_LEAD_BYTE( _CURRENT_ ) )
			{
				Value ch;
				unsigned char length = UTF8_GET_LENGTH_FROM_HEAD( _CURRENT_ );

				if( length == 2 )
					ch = (_CURRENT_ & _UTF8_BM_LEAD_2) << 6;
				else if( length == 3 )
					ch = (_CURRENT_ & _UTF8_BM_LEAD_3) << 12;
				else if( length == 4 )
					ch = (_CURRENT_ & _UTF8_BM_LEAD_4) << 18;

				while( --length )
				{
					ch |= (__GET_NEXT_BYTE() & _UTF8_BM_FLOW) << (length * 6);
				}

				_converted[_convLength++] = ch;
				__SEEK_NEXT_BYTE();
			}
			else if( IS_UTF8_BYTE( _CURRENT_ ) )
			{
				ex_throw(UnicodeError::Malformed, _CURRENT_, _convLength);
			}
		}

	private:
		SourcePtr _start;
		SourcePtr _buffer;
	};*/



#endif	/* UTF8_H */

