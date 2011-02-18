/*
 * File:   Exception.h
 * Author: Zozzz
 *
 * Created on 2011. február 17., 21:09
 */

#ifndef EXCEPTION_H
#define	EXCEPTION_H

#include "../global.h"
#include <deque>
#include <map>
#include <stdarg.h>

/*
namespace common
{
	namespace __
	{
		class ExceptionFactory
		{
		public:
			static std::deque<ExceptionFactory*> Exceptions;
			static ExceptionFactory* _current;

			static void raise()
			{
			};

			static void register_ex(ExceptionFactory* fac)
			{
				_current = fac;
				fac->start = fac->end = Exceptions.size();
				Exceptions.push_back(fac);
			};

			static int nextMsgId()
			{
				if( _current != NULL )
					return (_current->end)++;
				return 0;
			};

			int start;
			int end;

			virtual void _throw() throw() = 0;
		};

		ExceptionFactory* ExceptionFactory::_current = NULL;

	}

	class Exception
	{
	public:

		Exception(const char* msg, int code = 0) throw()
		{

		};

		Exception(int code) throw()
		{
		};
	};

	// new exp
	class ExceptionFactory_UnicodeError;
	class UnicodeError: public Exception
	{
	public:
		static ExceptionFactory_UnicodeError* factory;

		UnicodeError(): Exception(1){};
	};

	class ExceptionFactory_UnicodeError: public __::ExceptionFactory
	{
	public:
		static std::map<int, char*> _msg;

		ExceptionFactory_UnicodeError()
		{
			__::ExceptionFactory::register_ex(this);

			_msg.insert(new std::pair<int, char*>(__::ExceptionFactory::nextMsgId()));

		};

		virtual void _throw() throw()
		{
			throw new UnicodeError();
		};
	};


	//__::ExceptionFactory.register_ex(new ExceptionFactory_UnicodeError());

	ExceptionFactory_UnicodeError* UnicodeError::factory = new ExceptionFactory_UnicodeError();

#define _DECL_EXCEPTION(name)
#define _DECL_EXCEPTION_END
#define _DECL_EXCEPTION_CONST
#define _DECL_EXCEPTION_CONST_END
#define _DECL_EXCEPTION_MSG
#define _DECL_EXCEPTION_MSG_END
#define _EXCEPTION_CONST(name)
#define _EXCEPTION_MSG(msg)
#define _RAISE(...) common::__::ExceptionFactory.raise(__LINE__, __FILE__, __VA_ARGS__)

	_DECL_EXCEPTION(UnicodeError)

		_DECL_EXCEPTION_CONST
			_EXCEPTION_CONST(WrongChar)
		_DECL_EXCEPTION_CONST_END


		_DECL_EXCEPTION_MSG
			_EXCEPTION_MSG("blabla")
		_DECL_EXCEPTION_MSG_END

	_DECL_EXCEPTION_END


		_RAISE(UnicodeError::WrongChar);
}
	 * */

#endif	/* EXCEPTION_H */

