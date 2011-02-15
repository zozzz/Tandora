/*
 * Token.h
 *
 *  Created on: 2011.02.15.
 *      Author: Zozzz
 */

#ifndef _TOKEN_H_2011_Zozzz_
#define _TOKEN_H_2011_Zozzz_

#include "../global..h"

_TANDORA_NS_BEGIN

struct Token
{
	Token(SmallInt _type, Int _stype, SmallInt _line, SmallInt _col):
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
	}

	SmallInt type;
	Int stype;

	SmallInt line;
	SmallInt col;

	char* buffer;
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

_TANDORA_NS_END

#endif /* _TOKEN_H_2011_Zozzz_ */
