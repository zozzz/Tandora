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
	//------------------------------------------------------------------------------
	// All action is 32 bit wide:
	//------------------------------------------------------------------------------
	enum Action
	{
		//------------------------------------------------------------------------------
		// Throw unexpected char error
		// 0000 0000 0000 0000 0000 0000 0000 0000
		//									  \__/
		//									    |
		//									 Action Type
		//------------------------------------------------------------------------------
		ERROR = 0,

		//------------------------------------------------------------------------------
		// Everything is ok
		// 0000 0000 0000 0000 0000 0000 0000 0001
		//									  \__/
		//									    |
		//									 Action Type
		//------------------------------------------------------------------------------
		CONTINUE,

		//------------------------------------------------------------------------------
		// Close token
		// Params:
		//		- token type
		//		- offset: how many chars track back or forward
		// 0000 0000 0000 0000 0000 0000 0000 0010
		//					   \_______/ \__/ \__/
		//						 |		  |     |
		//				 Token type	 Offset	 Action Type
		//
		//------------------------------------------------------------------------------
		CLOSE,

		//------------------------------------------------------------------------------
		// Test char position inside token
		// Params:
		//		- token type
		//		- position[5]: max 5 position test available (0-15)
		// 0000 0000 0000 0000 0000 0000 0000 0011
		// \_______/ \__/ \__/ \__/ \__/ \__/ \__/
		//	   |	   \____|___||___|____/    |
		//	Token type		 Positions		 Action Type
		//------------------------------------------------------------------------------
		CHAR_AT,

		//------------------------------------------------------------------------------
		// Change token type
		// Params:
		//		- token type
		// 0000 0000 0000 0000 0000 0000 0000 0100
		//						    \_______/ \__/
		//							    |       |
		//						Token type	 Action Type
		//------------------------------------------------------------------------------
		CHNG_TYPE
	};

	struct Token
	{
		unsigned char type;
		unsigned char* buffer;
	};

	template<class _Token, int _TokenNumber>
	class TokenReader
	{
	public:
		typedef _Token Token;

		void setInput(common::File* file)
		{
			AssertExit(file ,!=, NULL);

			ALLOC_ARRAY(buffer, unsigned char, file->size());

			file->read(buffer, file->size());
		}

		//void setInput(common::unicode::uchar* ch);

		Token next()
		{
			trace(buffer[0]);
			trace(_actionTable[0][buffer[0]]);
		}


	protected:
		typedef unsigned int ActionTable;
		unsigned char* buffer;
		ActionTable (*_actionTable)[128];

		TokenReader(ActionTable actionTable[_TokenNumber][128]):
			_actionTable(actionTable)
		{
			trace("TokenReader");
		}
	};

}

#endif	/* PARSER_H */

