/*
 * global..h
 *
 *  Created on: 2011.02.15.
 *      Author: Zozzz
 */

#ifndef _GLOBAL__H_2011_Zozzz_
#define _GLOBAL__H_2011_Zozzz_

#include "settings.h"
#include <stdint.h>
#include <wchar.h>


#define _TANDORA_NS_BEGIN namespace tandora{
#define _TANDORA_NS_END }
//#define _WAIT_FOR_KEYP char __wfkp__t; std::cout << "\nPress any key to continue... "; std::cin >> __wfkp__t;

_TANDORA_NS_BEGIN

typedef unsigned short int SmallInt;
typedef unsigned int Int;
typedef uint64_t LargeInt;

_TANDORA_NS_END

#endif /* _GLOBAL__H_2011_Zozzz_ */
