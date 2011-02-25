
#include "utf8.h"


namespace common{ namespace unicode{

	namespace Decode
	{
		uchar UTF8(unsigned char** buffer, uchar& cache)
		{
			if( IS_UTF8_LEAD_BYTE(__UCONV_CURRENT_BYTE()) )
			{
				uchar ret;
				unsigned char length = UTF8_GET_LENGTH_FROM_HEAD(__UCONV_CURRENT_BYTE());

				if( length == 2 )
					ret = (__UCONV_CURRENT_BYTE() & _UTF8_BM_LEAD_2) << 6;
				else if( length == 3 )
					ret = (__UCONV_CURRENT_BYTE() & _UTF8_BM_LEAD_3) << 12;
				else if( length == 4 )
					ret = (__UCONV_CURRENT_BYTE() & _UTF8_BM_LEAD_4) << 18;

				--length;

				while( length )
					ret |= (__UCONV_GET_NEXT() & _UTF8_BM_FLOW) << (6 * --length);

				__UCONV_SEEK_NEXT();
				return ret;
			}

			return __UCONV_SEEK_NEXT_POST();
		}
	}

	namespace Encode
	{
		unsigned char UTF8(uchar** buffer, uchar& cache)
		{
			if( cache != 0 )
			{
				unsigned char ret = (0x80 | ((__UCONV_CURRENT_BYTE() >> (6 * --cache)) & _UTF8_BM_FLOW ));
				if( cache == 0 )
					__UCONV_SEEK_NEXT();
				return ret;
			}
			else if( __UCONV_CURRENT_BYTE() > 127 )
			{
				if( __UCONV_CURRENT_BYTE() <= 0x7FF )
				{
					cache = 1;
					return (0xC0 | (__UCONV_CURRENT_BYTE() >> 6));
				}
				else if( __UCONV_CURRENT_BYTE() <= 0xFFFF )
				{
					cache = 2;
					return (0xE0 | (__UCONV_CURRENT_BYTE() >> 12));
				}
				else
				{
					cache = 3;
					return (0xF0 | (__UCONV_CURRENT_BYTE() >> 18));
				}
			}

			return __UCONV_SEEK_NEXT_POST();
		}
	}

}}