// AUTO GENERATED CODE DO NOT EDIT!!!

#ifndef _JSONACTIONTABLE_
#define _JSONACTIONTABLE_

namespace parser { namespace json { 

	enum TokenType
	{

		# UNDEFINED (0 - 0)
		UNDEFINED

		# COMMENT (1 - 2)
		CT_BLOCK
		CT_SINGLE

		# KEYWORDS (3 - 5)
		KW_CATCH
		KW_CASE
		KW_CONTINUE

		# LITERAL (6 - 12)
		LI_HEX
		LI_FLOAT
		LI_TRUE
		LI_INT
		LI_STRING
		LI_FALSE
		LI_NULL

		# OPERATOR (13 - 20)
		OP_MINUS
		OP_RSQRB
		OP_LSQRB
		OP_RCURB
		OP_LCURB
		OP_COMMA
		OP_PLUS
		OP_COLON

		# IDENTIFIER (21 - 21)
		IDENTIFIER

		# WHITESPACE (22 - 26)
		WS_LF
		WS_SPACE
		WS_CRLF
		WS_CR
		WS_TAB
	};

	static const int actionTable[27][128] = {
	};


} /* parser */
} /* json */

#endif