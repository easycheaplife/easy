/********************************************************************
	created:	2011/11/18
	created:	18:11:2011   16:27
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_exception.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_exception
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_exception_h__
#define easy_exception_h__
#include <string>
#include <sstream>
#include <exception>

#ifndef easy_base_type_h__
#include "easy_base_type.h"
#endif //easy_base_type_h__

typedef std::basic_stringstream<char,std::char_traits<char>,std::allocator<char> > _StringStreamBase;
typedef std::string String;

namespace easy {
//	exception level
enum Excpetionlev {
    //	only output from console
    kExceptionLevConsole = 0,
    //	only output from text file
    kExceptionLevText,
    //	only output from message box
    kExceptionLevMessagebox,
};

class EasyException : public std::exception {
  protected:
    easy_long		line_;
    easy_uint32		number_;
    String			type_name_;
    String			description_;
    String			source_;
    String			file_;
    mutable String	full_desc_;
  public:
    //	Static definitions of error codes.
    //	Add many more exception codes, since we want the user to be able
    //	to catch most of them.
    enum ExceptionCodes {
        kErrorInvalidParams,
        kErrorFileNotFound,
        kErrorArrayIndexOutOfBounds,
        kErrorMemoryAllocaFailed,
    };

    //	Default constructor.
    EasyException( easy_uint32 number, const String& description, const String& source );

    //	Advanced constructor.
    EasyException( easy_int32 number, const String& description, const String& source, const char* type, const char* file, easy_long line, Excpetionlev level = kExceptionLevText );

    /*
    Returns a string with the full description of this error.
    The description contains the error number, the description
    supplied by the thrower, what routine threw the exception,
    and will also supply extra platform-specific information
    where applicable.
    */
    const String& GetFullDescription(void) const;

    //	Gets the error code.
    easy_uint32 GetNumber(void) const throw() {
        return number_;
    }

    //	Gets the source function.
    const String& GetSource() const {
        return source_;
    }

    //	Gets source file name.
    const String& GetFile() const {
        return file_;
    }

    //	Gets line number.
    long GetLine() const {
        return line_;
    }

    //	Override std::exception::what
    const char* What() const throw() {
        return GetFullDescription().c_str();
    }

    ~EasyException() throw() { }
};

/*
Template struct which creates a distinct type for each exception code.
This is useful because it allows us to create an overloaded method
for returning different exception types by value without ambiguity.
From 'Modern C++ Design' (Alexandrescu 2001).
*/
template <easy_uint32 num>
struct ExceptionCodeType {
    enum { number = num };
};

class  EasyInvalidParamException : public EasyException {
  public:
    EasyInvalidParamException(easy_uint32 in_number, const String& in_description, const String& in_source, const char* in_file, easy_long in_line, Excpetionlev level = kExceptionLevText)
        : EasyException(in_number, in_description, in_source, "EasyInvalidParamException", in_file, in_line, level) {}
};

class  EasyFileNotFoundException : public EasyException {
  public:
    EasyFileNotFoundException(easy_uint32 in_number, const String& in_description, const String& in_source, const char* in_file, easy_long in_line, Excpetionlev level = kExceptionLevText)
        : EasyException(in_number, in_description, in_source, "EasyFileNotFoundException", in_file, in_line, level) {}
};

class  EasyErrorArrayIndexOutOfBoundsException : public EasyException {
  public:
    EasyErrorArrayIndexOutOfBoundsException(easy_uint32 in_number, const String& in_description, const String& in_source, const char* in_file, easy_long in_line, Excpetionlev level = kExceptionLevText)
        : EasyException(in_number, in_description, in_source, "EasyErrorArrayIndexOutOfBoundsException", in_file, in_line, level) {}
};

class  EasyMemoryAllocaFailed : public EasyException {
  public:
    EasyMemoryAllocaFailed(easy_uint32 in_number, const String& in_description, const String& in_source, const char* in_file, easy_long in_line, Excpetionlev level = kExceptionLevText)
        : EasyException(in_number, in_description, in_source, "EasyMemoryAllocaFailed", in_file, in_line, level) {}
};

/*
Class implementing dispatch methods in order to construct by-value
exceptions of a derived type based just on an exception code.
This nicely handles construction of derived Exceptions by value (needed
for throwing) without suffering from ambiguity - each code is turned into
a distinct type so that methods can be overloaded. This allows EXCEPTION
to stay small in implementation (desirable since it is embedded) whilst
still performing rich code-to-type mapping.
*/
class EasyExceptionFactory {
  private:
    /// Private constructor, no construction
    EasyExceptionFactory() {}
  public:
    static EasyInvalidParamException Create(
        ExceptionCodeType<EasyException::kErrorInvalidParams> code,
        const String& desc,
        const String& src, const char* file, long line, Excpetionlev level = kExceptionLevText) {
        return EasyInvalidParamException(code.number, desc, src, file, line,level);
    }

    static EasyFileNotFoundException Create(
        ExceptionCodeType<EasyException::kErrorFileNotFound> code,
        const String& desc,
        const String& src, const char* file, long line, Excpetionlev level = kExceptionLevText) {
        return EasyFileNotFoundException(code.number, desc, src, file, line,level);
    }

    static EasyErrorArrayIndexOutOfBoundsException Create(
        ExceptionCodeType<EasyException::kErrorArrayIndexOutOfBounds> code,
        const String& desc,
        const String& src, const char* file, long line, Excpetionlev level = kExceptionLevText) {
        return EasyErrorArrayIndexOutOfBoundsException(code.number, desc, src, file, line,level);
    }

    static EasyMemoryAllocaFailed Create(
        ExceptionCodeType<EasyException::kErrorMemoryAllocaFailed> code,
        const String& desc,
        const String& src, const char* file, long line, Excpetionlev level = kExceptionLevText) {
        return EasyMemoryAllocaFailed(code.number, desc, src, file, line,level);
    }
};

#ifndef EASY_EXCEPTION
#define EASY_EXCEPTION( num,desc, src, level) throw EasyExceptionFactory::Create( ExceptionCodeType<num>(),desc, src, __FILE__, __LINE__, level );
#endif
}
#endif // easy_exception_h__