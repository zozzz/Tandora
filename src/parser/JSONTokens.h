/*
 * File:   JSONTokens.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 17:31
 */

#ifndef JSONTOKENS_H
#define	JSONTOKENS_H

namespace parser { namespace JSON
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
		TT_WhiteSpace			// 

	};

}}

#endif	/* JSONTOKENS_H */

