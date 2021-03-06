/*
 * settings.h
 *
 *  Created on: 2011.02.15.
 *      Author: Zozzz
 */

#ifndef _SETTINGS_H_2011_Zozzz_
#define _SETTINGS_H_2011_Zozzz_

/**
 * possible compile definitions:
 *      __DEBUG__
 *      __UNITTEST__
 *      __PERFTEST__
 */

#define _TOKEN_ITERATOR_HISTORY_SIZE 5


// in bytes, possible values: 2, 4
// 2 byte: 65,535 char 
// 4 byte: 2,097,152 char (utf8 max)
#define _UNICODE_CHAR_MAX_SIZE 2


// 10 MB
#define _MAX_ALLOWED_FILE_SIZE 0xA00000

#if defined(_CHAR_UNSIGNED) || CHAR_MIN == 0
#define _CHAR_UNSIGNED
#endif

#endif /* _SETTINGS_H_2011_Zozzz_ */
