/*
 * File:   ITokenReader.h
 * Author: Zozzz
 *
 * Created on 2011. március 10., 19:22
 */

#ifndef ITOKENREADER_H
#define	ITOKENREADER_H

#include "../common/Exception.h"

namespace parser
{
	struct Token
	{
		unsigned char type;
		unsigned int line;
		unsigned short int col;
		unsigned char* buffer;
	};

	class ITokenReader
	{
	public:
		typedef parser::Token  Token;
		typedef parser::Token* TokenPtr;

		virtual bool next(Token& token) = 0;
	};

	_EX_PRE_DECL(ParserError, common::Exception)
	_EX_PRE_DECL(UnexpectedChar, ParserError)

	_EX_DECL(ParserError, common::Exception, "Parser error")
		_EX_DECL_SUB_ERR(parser, UnexpectedChar)
	_EX_DECL_END

	_EX_DECL_MSG(ParserError, UnexpectedChar, "Unexpected character at %d line, %d column!")
}

#endif	/* ITOKENREADER_H */

