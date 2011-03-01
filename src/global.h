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
#define _WAIT_FOR_KEYP char __wfkp__t[1]; std::cout << "\nPress any key to continue... "; std::cin.getline(__wfkp__t,1);

#if defined(__MINGW32__)

	//#define NOTHROW __MINGW_NOTHROW
	#define ATTRIBUTE(...) __attribute__((__VA_ARGS__))
	#define MODIFIERS(...)

#elif defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(__WINDOWS__)

	#define ATTRIBUTE(...)
	#define MODIFIERS(...) __VA_ARGS__

#elif defined(__unix__) || defined(__unix)

	#define ATTRIBUTE(...) __attribute__((__VA_ARGS__))
	#define MODIFIERS(...)

#endif

#define NOTHROW_A ATTRIBUTE(nothrow)
#define NOTHROW_M MODIFIERS(__declspec(nothrow))

#define NORETURN_A ATTRIBUTE(noreturn)
#define NORETURN_M MODIFIERS(__declspec(noreturn))

#define FASTCALL_A ATTRIBUTE(fastcall)
#define FASTCALL_M MODIFIERS(__fastcall)

#ifdef _MSC_VER
	#define INLINE __forceinline // use __forceinline (VC++ specific)
#else
	#define INLINE inline // use standard inline
#endif


typedef unsigned short int SmallInt;
typedef unsigned int Int;
typedef uint64_t LargeInt;


#endif /* _GLOBAL__H_2011_Zozzz_ */
