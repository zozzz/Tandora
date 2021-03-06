/*
 * Token.h
 *
 *  Created on: 2011.02.15.
 *      Author: Zozzz
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "../global.h"
#include "../common/unicode/unicode.h"

namespace parser{

	#define _TOKEN_CHAR_MAP_BEGIN(name)
	#define _TOKEN_CHAR_MAP_END
	#define _TOKEN_CHAR_MAP_ENTRY(char_ptr, value)
	#define TCM_FIND(char_ptr, def)

	struct Token
	{
		/*Token(SmallInt _type, Int _stype, SmallInt _line, SmallInt _col):
			type(_type),
			stype(_stype),
			line(_line),
			col(_col),
			buffer(NULL)
		{

		}

		Token(SmallInt _type, Int _stype, SmallInt _line, SmallInt _col, char* _buffer):
			type(_type),
			stype(_stype),
			line(_line),
			col(_col),
			buffer(_buffer)
		{

		}

		~Token()
		{
			if( buffer )
				delete[] buffer;
		}*/

		SmallInt type;
		Int stype;

		SmallInt line;
		SmallInt col;

		common::unicode::uchar* buffer;
	};

	template<class token_t>
	class TokenIterator
	{
	public:
		typedef token_t Token;
		typedef token_t* TokenPtr;

		TokenIterator();
		virtual ~TokenIterator();

		void inputFile(char* path);
		void inputString(char* path);



	protected:
	#if _TOKEN_ITERATOR_HISTORY_SIZE > 0
		TokenPtr* _history;
	#endif

		virtual TokenPtr _nextToken() = 0;
		TokenPtr _prevToken();
	};

}

#endif /* TOKEN_H */
