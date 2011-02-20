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
#include <stdlib.h>


#define _TANDORA_NS_BEGIN namespace tandora{
#define _TANDORA_NS_END }
#define _WAIT_FOR_KEYP char __wfkp__t; std::cout << "\nPress any key to continue... "; std::cin >> __wfkp__t;

// 10 MB
#define _MAX_ALLOWED_FILE_SIZE 0xA00000

#define ALLOC_ARRAY(var, type, size) {var = (type *) malloc(sizeof(type) * (size)); if( var == NULL ) { ex_throw(common::MemoryError::NotEnough, sizeof(type) * (size)); }}
#define RESIZE_ARRAY(var, type, size) { var = (type *) realloc(var, sizeof(type) * (size)); if( var == NULL ) { ex_throw(common::MemoryError::NotEnough, sizeof(type) * (size)); } }
#define FREE_ARRAY(var) free(var)

_TANDORA_NS_BEGIN

typedef unsigned short int SmallInt;
typedef unsigned int Int;
typedef uint64_t LargeInt;

_TANDORA_NS_END

#endif /* _GLOBAL__H_2011_Zozzz_ */
