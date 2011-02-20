
#include "unicode.h"

namespace common { namespace unicode
{
	int UTF8Converter::BM_HEADER[] = {0, 0, _BM_UTF8_HEAD_2, _BM_UTF8_HEAD_3, _BM_UTF8_HEAD_4};
	int UTF8Converter::LS_OCT[] = {0, 0, 6, 12, 18};

	UTF8Converter::UTF8Converter()
	{
	}

	UTF8Converter::~UTF8Converter()
	{
	}

}}