/*
 * File:   JSONParser.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 17:31
 */

#ifndef JSONPARSER_H
#define	JSONPARSER_H

#include "parser.h"
#include "JSONTokens.h"

namespace parser
{


	namespace JSON
	{
		using namespace common;
		using namespace common::unicode;

		static const TokenType TokenCharMap[128] = {
			Undefined, Undefined, Undefined, Undefined, Undefined, Undefined, Undefined, // 6
			Undefined, Undefined, TT_WhiteSpace, TT_WhiteSpace, Undefined, Undefined, TT_WhiteSpace, // 13
			Undefined, Undefined, Undefined, Undefined, Undefined, Undefined, Undefined, // 20
			Undefined, Undefined, Undefined, Undefined, Undefined, Undefined, Undefined, // 27
			Undefined, Undefined, Undefined, Undefined, TT_WhiteSpace, Undefined, TT_Symbol_DQuote, // 34
			Undefined, Undefined, Undefined, Undefined, TT_Symbol_SQuote, Undefined, Undefined, // 41
			Undefined, TT_Symbol_Plus, TT_Symbol_Comma, TT_Symbol_Minus, TT_Symbol_Dot, TT_Symbol_Slash, TT_Literal_Number, //48
			TT_Literal_Number, TT_Literal_Number, TT_Literal_Number, TT_Literal_Number, TT_Literal_Number, TT_Literal_Number, TT_Literal_Number, // 55
			TT_Literal_Number, TT_Literal_Number, TT_Symbol_Colon, Undefined, Undefined, Undefined, Undefined, // 62
			Undefined, Undefined, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, // 69
			TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier,	// 76
			TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier,	// 83
			TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier,	// 90
			TT_Symbol_LBracket, TT_Symbol_BSlash, TT_Symbol_RBracket, Undefined, TT_Identifier, Undefined, TT_Identifier, // 97
			TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier,	// 104
			TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier,	TT_Identifier, TT_Identifier, TT_Identifier,	// 113
			TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier,	TT_Identifier, TT_Identifier, TT_Identifier,	// 120
			TT_Identifier, TT_Identifier, TT_Identifier, TT_Identifier, TT_Symbol_LBrace, Undefined, TT_Symbol_RBrace, Undefined, Undefined // 127
		};

		#define _JSON_IS_IDENTIFIER(ch)
		#define _JSON_IS_WHITESPACE(ch)
		#define _JSON_IS_SYMBOL(ch)

		struct GetNextToken
		{
			//GetNextToken(){  }

			Reader* input;
			TokenType token;

			Token* operator() ()
			{
				uchar ch;
				token = Undefined;

				while( (ch = input->next()) )
				{
					if( token == Undefined )
					{
						token = TokenCharMap[ch];

						if( token == TT_Identifier )
							input->startMark(true);
					}
					else if( token != TokenCharMap[ch] )
					{
						input->endMark();
						break;
					}

				}

				if( token )
				{
					Token* tok = new Token();
					tok->type = token;
					if( input->hasMarkedData() )
						tok->buffer = input->markedBuffer(-1);
					else
						tok->buffer = NULL;
					return tok;
				}
				else
					return NULL;
			}

		};
	}

	typedef TokenReader<JSON::GetNextToken, Token> JSONParser;
}

#endif	/* JSONPARSER_H */

