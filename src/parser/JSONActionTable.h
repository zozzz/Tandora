// AUTO GENERATED CODE, DO NOT EDIT!!!

#ifndef _JSONACTIONTABLE_
#define _JSONACTIONTABLE_

namespace parser { namespace json { 

	enum TokenType
	{
		// UNDEFINED (0 - 0)
		UNDEFINED = 0,

		// COMMENT (1 - 2)
		CT_BLOCK,
		CT_SINGLE,

		// KEYWORDS (3 - 8)
		KW_IS,
		KW_CATCH,
		KW_ASSERT,
		KW_CONTINUE,
		KW_CASE,
		KW_THIS,

		// LITERAL (9 - 15)
		LI_HEX,
		LI_FLOAT,
		LI_TRUE,
		LI_INT,
		LI_STRING,
		LI_FALSE,
		LI_NULL,

		// OPERATOR (16 - 23)
		OP_MINUS,
		OP_RSQRB,
		OP_LSQRB,
		OP_RCURB,
		OP_LCURB,
		OP_COMMA,
		OP_PLUS,
		OP_COLON,

		// IDENTIFIER (24 - 24)
		IDENTIFIER,

		// WHITESPACE (25 - 29)
		WS_LF,
		WS_SPACE,
		WS_CRLF,
		WS_CR,
		WS_TAB,

		TT_COUNT
	};

	extern unsigned int actionTable[30][128];

} /* parser */
} /* json */

#endif