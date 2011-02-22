/* 
 * File:   utf8.h
 * Author: Zozzz
 *
 * Created on 2011. február 22., 12:27
 */

#ifndef UTF8_H
#define	UTF8_H

	
#define _BM_UTF8_FLOW 0x3F

#define _UTF8_BM_HEAD_1 0x7F
#define _UTF8_BM_HEAD_2 0x1F
#define _UTF8_BM_HEAD_3 0x0F
#define _UTF8_BM_HEAD_4 0x07

#define _UTF8_OCT_NULL	0x1
#define _UTF8_OCT_HEAD	0x2
#define _UTF8_OCT_FLOW	0x4
#define _UTF8_OCT_ASCII	0x8

//#define UTF8_OCT_IS_UTF8(oct) ( ((oct) & 0x80) == 0x80 )
//#define UTF8_OCT_IS_HEAD(oct) ( ((oct) & 0xC0) == 0xC0 )
#define IS_UTF8_BYTE(byte)


#define UTF8_DETERMINE_OCT_TYPE(oct) (\
	((oct) & 0x80) == 0x80 ? ( ((oct) & 0xC0) == 0xC0 ? _UTF8_OCT_HEAD : _UTF8_OCT_FLOW ) : \
	((oct) & 0x7F) != 0 ? _UTF8_OCT_ASCII : _UTF8_OCT_NULL)

#define _UTF8_BOM_0 0xEF
#define _UTF8_BOM_1 0xBB
#define _UTF8_BOM_2 0xBF
#define UTF8_DETECT_BOM(buff) ((buff[0]) == _UTF8_BOM_0 && (buff[1]) == _UTF8_BOM_1 && (buff[2]) == _UTF8_BOM_2)

#define UTF8_GET_LENGTH_FROM_HEAD(oct) ( oct < 0xE0 ? 2 : ( oct < 0xF0 ? 3 : 4) )

#ifdef U8
	#warning "common::unicode -> 'U8' is previously defined, please do not use!"
#else
	#define U8 (common::unicode::UTF8Converter)
#endif
	

#endif	/* UTF8_H */

