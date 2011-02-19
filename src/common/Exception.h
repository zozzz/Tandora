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

/*

 NEW PLAN

class Exception
{
public:
	Exception(const char* fn, const char* file, const int line, const int code, const char* msg):
		_fn(fn),
		_file(file),
		_line(line),
		_code(code),
		_msg(msg)
	  {
		  cout << fn << " " << file << " " << line << " " << code << " " << msg << endl;
	  }

	operator char* ()
	{
		size_t length = strlen(_fn);
		length += strlen(_file);
		length += strlen(_msg);

		// sprintf
	}

protected:
	//static std::deque<Exception> _stack;

	const char* _fn;
	const char* _file;
	const char* _msg;
	const int _line;
	const int _code;
};

#define _EX_DECL(name) class name : public Exception{ \
	public:\
	name(const char* fn, const char* file, const int line, const int code, const char* msg): \
		Exception(fn, file, line, code, msg) \
	{} \
	protected: \
		static void _throw(const char* fn, const char* file, const int line, const int code, const char* msg){ throw new name (fn, file, line, code, msg); } \
	public:

#define _EX_END };
#define _EX_MSG(id, msg) static void id (const char* fn, const char* file, const int line){ _throw(fn, file, line, 0, #msg); }
#define EX_RAISE(ex) ex (__FUNCTION__, __FILE__, __LINE__);
#define EX_TRY
#define EX_CATCH_NO_THROW(UnicodeError)
#define EX_CATCH(UnicodeError)
#define EX_FINALLY
#define EX_END

_EX_DECL(UnicodeError)

	_EX_MSG(InvalidCharacter, "Invalid char")

_EX_END

*/

#include <map>
#include <deque>

namespace common
{
	namespace __
	{
		struct EX_CallStackItem
		{
			const char* func;
			const char* file;
			const int line;

			EX_CallStackItem(const char* _func, const char* _file, const int _line):
				func(_func),
				file(_file),
				line(_line) { };
		};
	}

	class Exception
	{
	public:
		const char* func;
		const char* file;
		const char* msg;

		const int line;

		Exception(const char* _fn, const char* _file, const int _line, const char* _msg):
			func(_fn),
			file(_file),
			line(_line),
			msg(_msg)
		{
			_addStack(_fn, _file, _line);
		}

		operator char*()
		{
			// sprintf
			return NULL;
		}

		static void _addStack(const char* _fn, const char* _file, const int _line)
		{
			//_stack.push_back(new __::EX_CallStackItem(_fn, _file, _line));
		}

		static void _clearStack(){ _stack.clear(); }

	protected:
		static std::map<char*, int> _msgId;

	public:
		static std::deque<__::EX_CallStackItem*> _stack;

		char* formatMsg(const char* msg, ...)
		{
		}
	};

}

#define _EX_DECL(name) class name : public common::Exception { \
	private: \
		typedef name _self; \
	public: \
		name (const char* _fn, const char* _file, const int _line, const char* _msg): \
		common::Exception(_fn, _file, _line, _msg){}

#define _EX_DECL_END };
#define _EX_DECL_ERR(name, msg) class name { \
	static void _throw (const char* _fn, const char* _file, const int _line){ throw new _self(_fn, _file, _line, #msg); } };

#define ex_try try
#define ex_catch(x) catch( x EX )
#define ex_final finally
#define ex_rethrow common::Exception::_addStack(__FUNCTION__, __FILE__, __LINE__); throw EX;
#define ex_rethrowc catch( Exception EX ){ ex_rethrow }
#define ex_throw(ex) ex ::_throw(__FUNCTION__, __FILE__, __LINE__)

_EX_DECL(IOError)

	_EX_DECL_ERR(FileNotFound, "File not found: %s")

_EX_DECL_END

#endif	/* EXCEPTION_H */

