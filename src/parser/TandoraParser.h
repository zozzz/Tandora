/*
 * File:   TandoraParser.h
 * Author: Zozzz
 *
 * Created on 2011. február 16., 0:19
 */

#ifndef TANDORAPARSER_H
#define	TANDORAPARSER_H

#include "../global.h"
#include "Token.h"
#include "TandoraTokens.h"

_TANDORA_NS_BEGIN

class TandoraParser: public TokenIterator<Token>
{

public:
	TandoraParser();
	TandoraParser(const TandoraParser& orig);
	virtual ~TandoraParser();

protected:
	virtual TokenPtr _nextToken();

private:

};


_TANDORA_NS_END

#endif	/* TANDORAPARSER_H */
