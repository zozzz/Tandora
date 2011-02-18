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
#define _UNICODE_CHAR_SIZE 2

#endif /* _SETTINGS_H_2011_Zozzz_ */
