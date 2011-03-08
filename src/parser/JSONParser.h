/*
 * File:   JSONParser.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 17:31
 */

#ifndef JSONPARSER_H
#define	JSONPARSER_H

#include "parser.h"
#include "JSONActionTable.h"


namespace parser
{
/*
	enum ASCIIConstant
	{
		CNULL=0,// 0
		WHITE,	// space, tab, new line, carraige return
		LCRUB,	// {
		RCRUB,	// }
		LRUDB,	// (
		RRUDB,	// )
		LSQRB,	// [
		RSQRB,	// ]
		LANGB,	// <
		RANGB,	// >

		EXCLM,	// !
		DQUOT,	// "
		SQUOT,	// '
		HASHM,	// #
		DOLLAR,	// $
		PRCNT,	// %
		AND,	// &
		STAR,	// *
		PLUS,	// +
		COMMA,	// ,
		MINUS,	// -
		DOT,	// .
		SLASH,	// /
		BSLASH,	//
		COLON,	// :
		SCOLON,	// ;
		EQUAL,	// =
		QUEST,	// ?
		AT,		// @

		NUM_0,	// 0
		NUM_1,	// 1
		NUM_2,	// 2
		NUM_3,	// 3
		NUM_4,	// 4
		NUM_5,	// 5
		NUM_6,	// 6
		NUM_7,	// 7
		NUM_8,	// 8
		NUM_9,	// 9

		LET_UA, // A
		LET_UB, // B
		LET_UC, // C
		LET_UD, // D
		LET_UE, // E
		LET_UF, // F
		LET_UG, // G
		LET_UH, // H
		LET_UI, // I
		LET_UJ, // J
		LET_UK, // K
		LET_UL, // L
		LET_UM, // M
		LET_UN, // N
		LET_UO, // O
		LET_UP, // P
		LET_UQ, // Q
		LET_UR, // R
		LET_US, // S
		LET_UT, // T
		LET_UU, // U
		LET_UV, // V
		LET_UW, // W
		LET_UX, // X
		LET_UY, // Y
		LET_UZ, // Z

		LET_LA, // a
		LET_LB, // b
		LET_LC, // c
		LET_LD, // d
		LET_LE, // e
		LET_LF, // f
		LET_LG, // g
		LET_LH, // h
		LET_LI, // i
		LET_LJ, // j
		LET_LK, // k
		LET_LL, // l
		LET_LM, // m
		LET_LN, // n
		LET_LO, // o
		LET_LP, // p
		LET_LQ, // q
		LET_LR, // r
		LET_LS, // s
		LET_LT, // t
		LET_LU, // u
		LET_LV, // v
		LET_LW, // w
		LET_LX, // x
		LET_LY, // y
		LET_LZ, // z

		NR_ASCII_CONST
	};

	static const ASCIIConstant LUT_ASCII[128] =
	{
		CNULL	, -1	, -1	, -1	, -1	, -1	, -1	, -1	, // 0-7
		-1		, WHITE	, WHITE	, WHITE	, WHITE	, WHITE	, -1	, -1	, // 8-15
		-1		, -1	, -1	, -1	, -1	, -1	, -1	, -1	, // 16-23
		-1		, -1	, -1	, -1	, -1	, -1	, -1	, -1	, // 24-31
		WHITE	, EXCLM	, DQUOT	, HASHM	, DOLLAR, PRCNT	, AND	, SQUOT	, // 32-39
		LRUDB	, RRUDB	, STAR	, PLUS	, COMMA	, MINUS	, DOT	, SLASH	, // 40-47
		NUM_0	, NUM_1	, NUM_2	, NUM_3	, NUM_4	, NUM_5	, NUM_6	, NUM_7	, // 48-55
		NUM_8	, NUM_9	, COLON	, SCOLON, LANGB , EQUAL	, RANGB	, QUEST	, // 56-62
		AT		, LET_UA, LET_UB, LET_UC, LET_UD, LET_UE, LET_UF, LET_UG,
		LET_UA	, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA,
		LET_UA	, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA,
		LET_UA	, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA, LET_UA,
	};
*/

	/*namespace JSON
	{
		enum TokenType
		{
			Undefined = 0,

			TT_Symbol_LBrace,		// {
			TT_Symbol_RBrace,		// }
			TT_Symbol_LBracket,		// [
			TT_Symbol_RBracket,		// ]
			TT_Symbol_DQuote,		// "
			TT_Symbol_SQuote,		// '
			TT_Symbol_Slash,		// /
			TT_Symbol_BSlash,		//
			TT_Symbol_Colon,		// :
			TT_Symbol_Plus,			// +
			TT_Symbol_Minus,		// -
			TT_Symbol_Comma,		// ,
			TT_Symbol_Dot,			// .

			TT_Literal_Number,		// all number
			TT_Literal_Int,			// 1233454
			TT_Literal_Float,		// 1.23, .2133, 1.33e+10, 1.33e-10, 1.33e10
			TT_Literal_Hex,			// 0x124
			TT_Literal_True,		// true
			TT_Literal_False,		// false
			TT_Literal_Null,		// null
			TT_Literal_String,		// "str", 'str'
			TT_Identifier,			//
			TT_WhiteSpace,			//

			NR_TOKEN_TYPE
		};

		static const int LUT_Action[NR_TOKEN_TYPE][128] = {};


		using namespace common;
		using namespace common::unicode;

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
				return NULL;
			}

		};
	}

	typedef TokenReader<JSON::GetNextToken, Token> JSONParser;*/



	//typedef TokenReader<json::actionTable, Token> JSONParser;
	class JSONParser: public TokenReader<Token, json::TT_COUNT>
	{
	public:
		static JSONParser* create(common::File* file)
		{
			JSONParser* parser = new JSONParser(json::actionTable);
			parser->setInput(file);
			return parser;
		}

	protected:
		JSONParser(ActionTable actionTable[json::TT_COUNT][128]):
			Base(actionTable)
		{
				trace("JSONParser");
		}

	private:
		typedef TokenReader<Token, json::TT_COUNT> Base;
	};
}

#endif	/* JSONPARSER_H */

