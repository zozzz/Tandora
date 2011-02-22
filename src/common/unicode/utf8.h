/*
 * File:   utf8.h
 * Author: Zozzz
 *
 * Created on 2011. február 22., 12:27
 */

/*****************************************************************************
 * INCLUDE ONLY FOR UNICODE.H (DO NOT INCLUDE ANY OTHER FILE)
 ****************************************************************************/

#ifndef UTF8_H
#define	UTF8_H


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


#ifdef utf8
	#warning "common::unicode -> 'utf8' is previously defined, please do not use!"
#else
	#define utf8 (common::unicode::Internal::UTF8Converter)
#endif


namespace Internal
{
	class UTF8Converter
	{
	public:
		// constructors for: char, char*, wchar_t, wchar_t*, uchar, uchar*
		// operators for: uchar, uchar*, ustring, ustring*
	};


	template<typename _SourceType>
	class UTF8Iterator: public Iterator {};

	template<>
	class UTF8Iterator<char>: public Iterator
	{
	public:
		typedef char			Source;
		typedef char*			SourcePtr;
		typedef const char		CSource;
		typedef const char*		CSourcePtr;

		UTF8Iterator(CSourcePtr buffer, bool selfHandled = false):
			_selfHandledBuffer(selfHandled),
			_buffer(buffer),
			Iterator()
		{
#ifndef _CHAR_UNSIGNED
			// TODO: x
#endif
		}

		virtual ~UTF8Iterator()
		{
			if( _selfHandledBuffer && _buffer != NULL )
				delete[] _buffer;
		}

		virtual Value current()
		{
			trace("UTF8Iterator<char>::current()");
		}

	private:
		bool _selfHandledBuffer;
		CSourcePtr _buffer;
	};

}

#endif	/* UTF8_H */

