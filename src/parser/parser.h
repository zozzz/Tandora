/*
 * File:   parser.h
 * Author: Zozzz
 *
 * Created on 2011. február 28., 19:08
 */

#ifndef PARSER_H
#define	PARSER_H

#include "../global.h"
#include "../common/unicode/unicode.h"
#include "../common/File.h"

namespace parser
{
	struct Token
	{
		unsigned short int type;
		unsigned short int value;
		common::unicode::uchar* buffer;

		//INLINE bool operator == (const Token& cmp){ return (type == cmp.type && value == cmp.value); }
	};

	template<class _NextTokenGetter, class _Token = Token>
	class TokenReader
	{
	public:
		typedef _Token Token;

		TokenReader():_nextToken() { trace("Construct::TokenReader"); };
		//~TokenReader();

		void setInput(common::File* file)
		{
			if( _nextToken.input != NULL )
				delete _nextToken.input;
			_nextToken.input = file->reader();
		}

		void setInput(common::unicode::uchar* ch);

		INLINE Token* next()
		{
			return _nextToken();
		}


	protected:
		_NextTokenGetter _nextToken;
	};

}

#endif	/* PARSER_H */

