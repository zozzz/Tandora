
#include "unicode.h"


namespace common { namespace unicode
{
	/*
	int UTF8Converter::BM_HEADER[] = {0, 0, _BM_UTF8_HEAD_2, _BM_UTF8_HEAD_3, _BM_UTF8_HEAD_4};
	int UTF8Converter::LS_OCT[] = {0, 0, 6, 12, 18};

	UTF8Converter::UTF8Converter(const char* ch)
	{
		_conv(ch, strlen(ch));
	}

	UTF8Converter::UTF8Converter(const char* ch, size_t length)
	{
		_conv(ch, length);
	}

	UTF8Converter::UTF8Converter(const wchar_t* ch)
	{
		_conv(ch, wcslen(ch));
	}

	UTF8Converter::UTF8Converter(const wchar_t* ch, size_t length)
	{
		_conv(ch, length);
	}

	UTF8Converter::UTF8Converter(const int number)
	{
	}

	UTF8Converter::UTF8Converter(const double number)
	{
	}

	UTF8Converter::UTF8Converter(const float number)
	{
	}


	void UTF8Converter::_conv(const char* buffer, size_t length)
	{
		_length = 0;
		ALLOC_ARRAY(_buffer, uchar, length);
		uchar ch = (unsigned char)*buffer++;

		short int ucharOctLength = 0;
		do
		{
			if( _UTF8_OCT_IS_UTF8(ch) )
			{
				if( _UTF8_OCT_IS_HEAD(ch) )
				{
					ucharOctLength = _UTF8_GET_LENGTH_FROM_HEAD(ch);
					_buffer[_length] = (ch & BM_HEADER[ucharOctLength]) << LS_OCT[--ucharOctLength];

					parseUnicodeFlow:
					ch = (unsigned char)*buffer++;

					_buffer[_length] |= (ch & _BM_UTF8_FLOW) << LS_OCT[--ucharOctLength];

					if( ucharOctLength == 0 )
							_length++;
					else
						goto parseUnicodeFlow;

				}
				else
				{
					parseUnicodeFlow:

						ch = (unsigned char)*buffer++;
						if( ucharOctLength == 0 )
						{
							ex_throw(UnicodeError::Malformed, ch, _length);
						}

						_buffer[_length] |= (ch & _BM_UTF8_FLOW) << (6 * --ucharOctLength);

						if( ucharOctLength == 0 )
							_length++;
						else
							goto parseUnicodeFlow;
				}
			}
			else
			{
				_buffer[_length++] = ch;
			}

			ch = (unsigned char)*buffer++;

		}while( ch );

		if( _length != length )
			RESIZE_ARRAY(_buffer, uchar, _length+1);

		_buffer[_length] = '\0';

		//trace(_buffer[0]);
	}

	void UTF8Converter::_conv(const wchar_t* buffer, size_t length)
	{
		if( sizeof(wchar_t) == _UNICODE_CHAR_MAX_SIZE )
		{
			wchar_t* _buff;
			ALLOC_ARRAY(_buff, wchar_t, length);
			memcpy(_buff, buffer, length);
			_buffer = reinterpret_cast<uchar*>(_buff);
			_length = length;
		}
		else // TODO: ...
		{
		}
	}

	 **/

}}