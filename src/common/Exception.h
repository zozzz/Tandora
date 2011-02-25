/*
 * File:   Exception.h
 * Author: Zozzz
 *
 * Created on 2011. február 17., 21:09
 */

#ifndef EXCEPTION_H
#define	EXCEPTION_H

#include <stdarg.h>
#include <map>
#include <deque>
#include <exception>

namespace common
{
	namespace Internal
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

		const char* formatMsg(const char* msg, ...);

		void _addStack(const char* _fn, const char* _file, const int _line);

		void _clearStack();
	}

	class Exception : public std::exception
	{
		friend void Internal::_addStack(const char* _fn, const char* _file, const int _line);
		friend void Internal::_clearStack();
	public:
		const char* func;
		const char* file;
		const char* msg;
		const int line;

		Exception(const char* _fn, const char* _file, const int _line, const char* _msg) throw();

		virtual ~Exception() throw();

		virtual const char* what() const throw() { return msg; }

		static const char* defaultMessage(const char* msg){ if(msg == NULL) return "Exception base"; else return msg; };

		operator const char*() { return what(); }

	private:
		static std::deque<Internal::EX_CallStackItem*> _stack;
	};


	#define _EX_PRE_DECL(name, base) class name;
	#define _EX_DECL(name, base, defMsg) class name : public base \
		{ public: \
			name (const char* _fn, const char* _file, const int _line, const char* _msg): \
			base (_fn, _file, _line, _msg) {}; \
			static const char* defaultMessage(const char* msg){ if(msg == NULL) return "["#base"::"#name"] "defMsg; else return msg; };

	#define _EX_DECL_END };
	#define _EX_DECL_MSG(base, name, msg) _EX_DECL(name, base, msg) _EX_DECL_END
	#define _EX_DECL_SUB_ERR(ns, name) typedef ns :: name name;


	#define ex_try try
	#define ex_catch(x) catch( x )
	#define ex_final finally
	#define ex_rethrow common::Internal::_addStack(__FUNCTION__, __FILE__, __LINE__); throw
	#define ex_rethrowc catch( ... ){ ex_rethrow }
	#define ex_throw(ex, ...) throw ex (__FUNCTION__, __FILE__, __LINE__, common::Internal::formatMsg(ex :: defaultMessage(NULL), __VA_ARGS__))
	#define ex_throwm(ex, msg, ...) throw ex (__FUNCTION__, __FILE__, __LINE__, common::Internal::formatMsg(ex :: defaultMessage( msg ), __VA_ARGS__))

	//------------------------------------------------------------------------------
	// IOError
	//------------------------------------------------------------------------------

	_EX_PRE_DECL(IOError, Exception)
	_EX_PRE_DECL(FileNotFound, IOError)
	_EX_PRE_DECL(FileIsTooLarge, IOError)


	_EX_DECL(IOError, Exception, "Undefined error")
		_EX_DECL_SUB_ERR(common, FileNotFound)
		_EX_DECL_SUB_ERR(common, FileIsTooLarge)
	_EX_DECL_END

	_EX_DECL_MSG(IOError, FileNotFound, "File not found: '%s'!")
	_EX_DECL_MSG(IOError, FileIsTooLarge, "File is too large: '%s' %s!")


	//------------------------------------------------------------------------------
	// ValueError
	//------------------------------------------------------------------------------

	_EX_PRE_DECL(ValueError, Exception)
	_EX_PRE_DECL(OutOfRange, ValueError)

	_EX_DECL(ValueError, Exception, "Undefined error")
		_EX_DECL_SUB_ERR(common, OutOfRange)
	_EX_DECL_END

	_EX_DECL_MSG(ValueError, OutOfRange, "The specified value is out of range (%d - %d): %d")


	//------------------------------------------------------------------------------
	// MemoryError
	//------------------------------------------------------------------------------

	_EX_PRE_DECL(MemoryError, Exception)
	_EX_PRE_DECL(NotEnough, MemoryError)

	_EX_DECL(MemoryError, Exception, "Undefined error")
		_EX_DECL_SUB_ERR(common, NotEnough)
	_EX_DECL_END

	_EX_DECL_MSG(MemoryError, NotEnough, "Not enough memory, try to allocate: %d byte!")


	//------------------------------------------------------------------------------
	// StopIteration
	//------------------------------------------------------------------------------

	_EX_DECL_MSG(Exception, StopIteration, "Iterator reach end of stream!")
}

#endif	/* EXCEPTION_H */

