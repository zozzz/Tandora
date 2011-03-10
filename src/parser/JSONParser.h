/*
 * File:   JSONParser.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 17:31
 */

#ifndef JSONPARSER_H
#define	JSONPARSER_H

#include "TokenReader.h"
#include "JSONActionTable.h"

namespace parser
{
	typedef TokenReader<json::TT_COUNT, json::actionTable> JSONParser;
}

#endif	/* JSONPARSER_H */

