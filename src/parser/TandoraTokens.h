/*
 * File:   TandoraTokens.h
 * Author: Zozzz
 *
 * Created on 2011. február 16., 0:33
 */

#ifndef TANDORATOKENS_H
#define	TANDORATOKENS_H

#include "../global.h"

_TANDORA_NS_BEGIN

#define _TOKEN_CHAR_MAP_BEGIN(name)
#define _TOKEN_CHAR_MAP_END
#define _TOKEN_CHAR_MAP_ENTRY(char_ptr, value)
#define TCM_FIND(char_ptr, def)

//------------------------------------------------------------------------------
// TOKEN MAIN TYPES
//------------------------------------------------------------------------------

#define T_OPERATOR		0x001
#define T_IDENTIFIER	0x002
#define T_SYMBOL		0x004
#define T_LITERAL		0x008
#define T_PREPROCESSOR	0x010
#define T_WHITESPACE	0x020
#define T_STATEMENT		0x040
#define	T_ATTRIBUTE		0x080
#define T_DEFINITION	0x100
#define T_OTHER_KW		0x200
#define T_COMMENT		0x400

// T_STATEMENT | T_ATTRIBUTE | T_DEFINITION | T_OTHER_KW
#define T_KEYWORD		0x3C0

//------------------------------------------------------------------------------
// KEYWORDS
//------------------------------------------------------------------------------

// statement
#define KW_BREAK		0x00001
#define KW_CASE			0x00002
#define KW_CONTINUE		0x00004
#define KW_DO			0x00008
#define KW_WHILE		0x00010
#define KW_ELSE			0x00020
#define KW_FOR			0x00040
#define KW_IN			0x00080
#define KW_EACH			0x00100
#define KW_IF			0x00200
#define KW_RETURN		0x00400
#define KW_SUPER		0x00800
#define KW_SWITCH		0x01000
#define KW_THROW		0x02000
#define KW_TRY			0x04000
#define KW_CATCH		0x08000
#define KW_FINALLY		0x10000
#define KW_WITH			0x20000
#define KW_DELETE		0x40000

// attribute
#define KW_DYNAMIC		0x001
#define KW_FINAL		0x002
#define KW_INTERNAL		0x004
#define KW_NATIVE		0x008
#define KW_OVERRIDE		0x010
#define KW_PRIVATE		0x020
#define KW_PROTECTED	0x040
#define KW_PUBLIC		0x080
#define KW_STATIC		0x100

// definition
#define KW_CLASS		0x001
#define KW_CONST		0x002
#define KW_EXTENDS		0x004
#define KW_FUNCTION		0x008
#define KW_GET			0x010
#define KW_SET			0x020
#define KW_IMPLEMENTS	0x040
#define KW_INTERFACE	0x080
#define KW_NAMESPACE	0x100
#define KW_PACKAGE		0x200
#define KW_VAR			0x400

// other
#define KW_IMPORT		0x001
#define KW_USE			0x002
#define KW_THIS			0x004
#define KW_NEW			0x008
#define KW_AS			0x010
#define KW_INSTANCEOF	0x020
#define KW_IS			0x040
#define KW_TYPEOF		0x080
#define KW_TYPEDEF		0x100

_TOKEN_CHAR_MAP_BEGIN(TCM_KEYWORD)
	_TOKEN_CHAR_MAP_ENTRY("break", KW_BREAK)
_TOKEN_CHAR_MAP_END

//------------------------------------------------------------------------------
// SYMBOLS
//------------------------------------------------------------------------------

// (
#define S_LPAREN		0x0000001
// )
#define S_RPAREN		0x0000002
// {
#define S_LBRACE		0x0000004
// }
#define S_RBRACE		0x0000008
// [
#define S_LBRACKET		0x0000010
// ]
#define S_RBRACKET		0x0000020
// /
#define S_SLASH			0x0000040
//
#define S_BSLASH		0x0000080
// *
#define S_ASTERIX		0x0000100
// =
#define S_EQUAL			0x0000200
// +
#define S_PLUS			0x0000400
// -
#define S_MINUS			0x0000800
// ?
#define S_QMARK			0x0001000
// %
#define S_PERCENT		0x0002000
// &
#define S_AND			0x0004000
// |
#define S_PIPE			0x0008000
// >
#define S_GT			0x0010000
// <
#define S_LT			0x0020000
// ~
#define S_BNOT			0x0040000
// ^
#define S_BXOR			0x0080000
// !
#define S_NEG			0x0100000
// ,
#define S_COMMA			0x0200000
// ;
#define S_SEMICOLON		0x0400000
// :
#define S_COLON			0x0800000
// .
#define S_DOT			0x1000000
// "
#define S_DQUOTE		0x2000000
// '
#define S_SQUOTE		0x4000000
// @ bit:29
#define S_AT			0x8000000

//------------------------------------------------------------------------------
// OPERATORS
//------------------------------------------------------------------------------

// x + y
#define O_ADD			0x0000001
// ++x ; x++
#define O_INC			0x0000002
// x - y
#define O_SUB			0x0000004
// --x ; x--
#define O_DEC			0x0000008
// x / y
#define O_DIV			0x0000010
// x % y
#define O_MOD			0x0000020
// x * y
#define O_MULTI			0x0000040
// x = y
#define O_ASSIGN		0x0000080
// x << y
#define O_B_LEFT_SHIFT	0x0000100
// x >> y
#define O_B_RIGHT_SHIFT	0x0000200
// x & y
#define O_B_AND			0x0000400
// x | y
#define O_B_OR			0x0000800
// x ^ y
#define O_B_XOR			0x0001000
// ~y
#define O_B_NOT			0x0002000
// x == y
#define O_EQ			0x0004000
// x === y
#define O_SEQ			0x0008000
// x !== y
#define O_SNEQ			0x0010000
// !x
#define O_NEG			0x0020000
// x && y
#define O_L_AND			0x0040000
// x || y
#define O_L_OR			0x0080000
// x.y.z
#define O_MEMBER_ACC	0x0100000
// x < y
#define O_LT			0x0200000
// x > y
#define O_GT			0x0400000
// x:ClassName
#define O_TYPE			0x0800000
// ...rest args
#define O_REST			0x1000000
// **kwargs ; x**y
#define O_DASTERIX		0x2000000

//------------------------------------------------------------------------------
// LITERALS
//------------------------------------------------------------------------------

// "String"
#define L_STRING		0x001
// 12312
#define L_INT			0x002
// 1231.123
#define L_FLOAT			0x004
// 0xABCD234
#define L_HEX			0x008
// 02323
#define L_OCT			0x010
// 0b00101101
#define L_BINARY		0x020
// void
#define L_VOID			0x040
// true
#define L_TRUE			0x080
// false
#define L_FALSE			0x100
// null
#define L_NULL			0x200
// /regexp/flag
#define L_REGEXP		0x400

// L_INT | L_FLOAT | L_HEX | L_OCT | L_BINARY
#define L_NUMBER		0x03E

// L_VOID | L_TRUE | L_FALSE | L_NULL
#define L_CONST			0x3C0

//------------------------------------------------------------------------------
// COMMENTS
//------------------------------------------------------------------------------

#define CT_SINGLE		0x1
#define CT_BLOCK		0x2

_TANDORA_NS_END

#endif	/* TANDORATOKENS_H */
