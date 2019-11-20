// Exception.h

#if !defined(Foundation_API)
#define Foundation_API
#endif

#ifndef _DBBENCHMARK_EXCEPTION_H_
#define _DBBENCHMARK_EXCEPTION_H_

#include <stdexcept>

namespace dbbenchmark {
namespace utility {
/**
* @brief This is the base class for all exceptions defined
* in the NRF project.
* @details
* @author Poco / Modified by Ozgun AY
* @version 1.0
* @date 09/03/2019
* @bug None so far
*/
class Foundation_API Exception: public std::exception
{
public:
	/** Creates an exception.
	* @param msg Message that exception defined with. */
	Exception(const std::string& msg, int code = 0);
	/** Creates an exception.
	* @param msg Message that exception defined with.
	* @param arg Detailed information about exception.
	* @param code Exception code if defined. */
	Exception(const std::string& msg, const std::string& arg, int code = 0);
	/** Creates an exception and stores a clone
	* of the nested exception.
	* @param msg Message that exception defined with.
	* @param nested Exception to be cloned.
	* @param code Exception code if defined. */
	Exception(const std::string& msg, const Exception& nested, int code = 0);
	/** Copy constructor. */
	Exception(const Exception& exc);
	/** Destroys the exception and deletes the nested exception. */
	~Exception() throw();
	/** Assignment operator. */
	Exception& operator = (const Exception& exc);
	/** Returns a static string describing the exception. */
	virtual const char* name() const throw();
	/** Returns the name of the exception class. */
	virtual const char* className() const throw();
	/** Returns a static string describing the exception.
	* Same as name(), but for compatibility with std::exception. */
	virtual const char* what() const throw();
	/** Returns a pointer to the nested exception, or
	* null if no nested exception exists. */
	const Exception* nested() const;
	/** Returns the message text. */
	const std::string& message() const;
	/** Returns the exception code if defined. */
	int code() const;
	/** Returns a string consisting of the
	* message name and the message text. */
	std::string displayText() const;
	/** Creates an exact copy of the exception.
	* The copy can later be thrown again by
	* invoking rethrow() on it. */
	virtual Exception* clone() const;
	/** (Re)Throws the exception.
	*
	* This is useful for temporarily storing a
	* copy of an exception (see clone()), then
	* throwing it again. */
	virtual void rethrow() const;

protected:
	/** Standard constructor. */
	Exception(int code = 0);
	/** Sets the message for the exception. */
	void message(const std::string& msg);
	/** Sets the extended message for the exception. */
	void extendedMessage(const std::string& arg);

private:
	std::string _msg;
	Exception*  _pNested;
	int			_code;
};
//
// inlines
//
inline const Exception* Exception::nested() const {
	return _pNested;
}

inline const std::string& Exception::message() const {
	return _msg;
}

inline void Exception::message(const std::string& msg) {
	_msg = msg;
}

inline int Exception::code() const {
	return _code;
}
//
// Macros for quickly declaring and implementing exception classes.
// Unfortunately, we cannot use a template here because character
// pointers (which we need for specifying the exception name)
// are not allowed as template arguments.
//
#define NRF_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE) \
	class API CLS: public BASE														\
	{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const std::string& msg, int code = CODE);								\
		CLS(const std::string& msg, const std::string& arg, int code = CODE);		\
		CLS(const std::string& msg, const utility::Exception& exc, int code = CODE);	\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		utility::Exception* clone() const;												\
		void rethrow() const;														\
	};

#define NRF_DECLARE_EXCEPTION(API, CLS, BASE) \
	NRF_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define NRF_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int code): BASE(code)																	\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)		\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const utility::Exception& exc, int code): BASE(msg, exc, code)	\
	{																								\
	}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& exc)															\
	{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
	}																								\
	const char* CLS::name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::className() const throw()														\
	{																								\
		return typeid(*this).name();																\
	}																								\
	utility::Exception* CLS::clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::rethrow() const																		\
	{																								\
		throw *this;																				\
	}

//
// Standard exception classes
//
NRF_DECLARE_EXCEPTION(Foundation_API, LogicException, Exception)
// NRF_DECLARE_EXCEPTION(Foundation_API, AssertionViolationException, LogicException)
// NRF_DECLARE_EXCEPTION(Foundation_API, NullPointerException, LogicException)
// NRF_DECLARE_EXCEPTION(Foundation_API, NullValueException, LogicException)
// NRF_DECLARE_EXCEPTION(Foundation_API, BugcheckException, LogicException)
NRF_DECLARE_EXCEPTION(Foundation_API, InvalidArgumentException, LogicException)
NRF_DECLARE_EXCEPTION(Foundation_API, NotImplementedException, LogicException)
// NRF_DECLARE_EXCEPTION(Foundation_API, RangeException, LogicException)
NRF_DECLARE_EXCEPTION(Foundation_API, IllegalStateException, LogicException)
// NRF_DECLARE_EXCEPTION(Foundation_API, InvalidAccessException, LogicException)
// NRF_DECLARE_EXCEPTION(Foundation_API, SignalException, LogicException)
// NRF_DECLARE_EXCEPTION(Foundation_API, UnhandledException, LogicException)

NRF_DECLARE_EXCEPTION(Foundation_API, RuntimeException, Exception)
NRF_DECLARE_EXCEPTION(Foundation_API, NotFoundException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, ExistsException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, TimeoutException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, SystemException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, RegularExpressionException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, LibraryLoadException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, LibraryAlreadyLoadedException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, NoThreadAvailableException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, PropertyNotSupportedException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, PoolOverflowException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, NoPermissionException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, OutOfMemoryException, RuntimeException)
NRF_DECLARE_EXCEPTION(Foundation_API, DataException, RuntimeException)
NRF_DECLARE_EXCEPTION(Foundation_API, HelpCalledException, RuntimeException)

//NRF_DECLARE_EXCEPTION(Foundation_API, DataFormatException, DataException)
NRF_DECLARE_EXCEPTION(Foundation_API, SyntaxException, DataException)
NRF_DECLARE_EXCEPTION(Foundation_API, CircularReferenceException, DataException)
NRF_DECLARE_EXCEPTION(Foundation_API, PathSyntaxException, SyntaxException)
NRF_DECLARE_EXCEPTION(Foundation_API, IOException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, ProtocolException, IOException)
NRF_DECLARE_EXCEPTION(Foundation_API, FileException, IOException)
NRF_DECLARE_EXCEPTION(Foundation_API, UnsupportedFileExtention, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, FileExistsException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, FileNotFoundException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, PathNotFoundException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, FileReadOnlyException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, FileAccessDeniedException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, CreateFileException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, OpenFileException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, WriteFileException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, ReadFileException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, DirectoryNotEmptyException, FileException)
// NRF_DECLARE_EXCEPTION(Foundation_API, UnknownURISchemeException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, TooManyURIRedirectsException, RuntimeException)
// NRF_DECLARE_EXCEPTION(Foundation_API, URISyntaxException, SyntaxException)

// NRF_DECLARE_EXCEPTION(Foundation_API, ApplicationException, Exception)
// NRF_DECLARE_EXCEPTION(Foundation_API, BadCastException, RuntimeException)

} // namespace dbbenchmark
} // namespace utility


#endif // _DBBENCHMARK_EXCEPTION_H_
