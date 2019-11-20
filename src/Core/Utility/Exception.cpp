// Exception.cpp

#include <typeinfo>

#include "Exception.h"

namespace dbbenchmark {
namespace utility {

Exception::Exception(int code): _pNested(0), _code(code) {
}

Exception::Exception(const std::string& msg, int code):
	_msg(msg), _pNested(0), _code(code) {
}

Exception::Exception(const std::string& msg, const std::string& arg, int code):
	_msg(msg), _pNested(0), _code(code) {
	if (!arg.empty()) {
		_msg.append(": ");
		_msg.append(arg);
	}
}

Exception::Exception(const std::string& msg, const Exception& nested, int code):
	_msg(msg), _pNested(nested.clone()), _code(code) {
}

Exception::Exception(const Exception& exc):
	std::exception(exc),
	_msg(exc._msg),
	_code(exc._code) {
	_pNested = exc._pNested ? exc._pNested->clone() : 0;
}

Exception::~Exception() throw() {
	delete _pNested;
}

Exception& Exception::operator = (const Exception& exc) {
	if (&exc != this) {
		Exception* newPNested = exc._pNested ? exc._pNested->clone() : 0;
		delete _pNested;
		_msg     = exc._msg;
		_pNested = newPNested;
		_code    = exc._code;
	}
	return *this;
}

const char* Exception::name() const throw() {
	return "Exception";
}

const char* Exception::className() const throw() {
	return typeid(*this).name();
}

const char* Exception::what() const throw() {
	return name();
}

std::string Exception::displayText() const {
	std::string txt = name();
	if (!_msg.empty()) {
		txt.append(": ");
		txt.append(_msg);
	}
	return txt;
}

void Exception::extendedMessage(const std::string& arg) {
	if (!arg.empty()) {
		if (!_msg.empty()) _msg.append(": ");
		_msg.append(arg);
	}
}

Exception* Exception::clone() const {
	return new Exception(*this);
}

void Exception::rethrow() const {
	throw *this;
}

NRF_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
// NRF_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
// NRF_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
// NRF_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
// NRF_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
NRF_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
NRF_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
// NRF_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
NRF_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
// NRF_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
// NRF_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
// NRF_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

NRF_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
NRF_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
// NRF_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
// NRF_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
// NRF_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
// NRF_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
// NRF_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
// NRF_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
// NRF_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
// NRF_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
// NRF_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
// NRF_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
// NRF_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
NRF_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")
NRF_IMPLEMENT_EXCEPTION(HelpCalledException, RuntimeException, "Help called in CLI")

// NRF_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
NRF_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
NRF_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
NRF_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
NRF_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
// NRF_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
NRF_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
NRF_IMPLEMENT_EXCEPTION(UnsupportedFileExtention, FileException, "File extention not supported")
// NRF_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
// NRF_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
// NRF_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
// NRF_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
// NRF_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
// NRF_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
// NRF_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
// NRF_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
// NRF_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
// NRF_IMPLEMENT_EXCEPTION(DirectoryNotEmptyException, FileException, "Directory not empty")
// NRF_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")
// NRF_IMPLEMENT_EXCEPTION(TooManyURIRedirectsException, RuntimeException, "Too many URI redirects")
// NRF_IMPLEMENT_EXCEPTION(URISyntaxException, SyntaxException, "Bad URI syntax")

// NRF_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
// NRF_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

} // namespace dbbenchmark
} // namespace utility
