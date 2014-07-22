#include "easy_exception.h"
#ifndef easy_log_h__
#include "easy_log.h"
#endif //easy_log_h__

namespace easy
{

	EasyException::EasyException( easy_uint32 number, const String& description, const String& source )
		:line_(0),
		number_(number),
		description_(description),
		source_(source)
	{
#ifdef __WINDOWS
		s_log->SaveLog("..\\log\\exception.log");
#elif __LINUX
		s_log->SaveLog("./exception.log");
#endif
		
	}

	EasyException::EasyException( easy_int32 number, const String& description, const String& source, const char* type, const char* file, easy_long line, Excpetionlev level /*= kExceptionLevText */ )
		:line_( line ),
		number_( number ),
		type_name_(type),
		description_( description ),
		source_( source ),
		file_( file )
	{
		switch ( level )
		{
		case kExceptionLevConsole:
			{
				printf( const_cast<char*>(this->GetFullDescription().c_str()) );
				break;
			}
		case kExceptionLevText:
			{
				// Log this error, mask it from debug though since it may be caught and ignored
				s_log->SaveLog( "..\\log\\exception.log", kErrors, const_cast<char*>(this->GetFullDescription().c_str()) );
				break;
			}
		case kExceptionLevMessagebox:
			{
#ifdef __WINDOWS
#ifdef UNICODE
#pragma warning(suppress: 6276)
				::MessageBoxW( NULL, (LPCWSTR)const_cast<char*>(this->GetFullDescription().c_str()), (LPCWSTR)"EXCEPTION", MB_OK );
#elif ASCII
				::MessageBoxA( NULL, const_cast<char*>(this->GetFullDescription().c_str()), "EXCEPTION", MB_OK );
#endif // UNICODE
#endif // __WINDOWS
				break;
			}
		}
	}

	const String& EasyException::GetFullDescription( void ) const
	{
		if (full_desc_.empty())
		{
			_StringStreamBase desc;

			desc <<  "EXCEPTION(" << number_ << ":" << type_name_ << "): "
				<< description_ 
				<< " in " << source_;

			if( line_ > 0 )
			{
				desc << " at " << file_ << " (line " << line_ << ")";
			}

			full_desc_ = desc.str();
		}
		return full_desc_;
	}
}