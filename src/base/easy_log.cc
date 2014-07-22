#include "easy_log.h"

#ifdef __WINDOWS
#ifndef easy_console_h__
#include "easy_console.h"
#endif //easy_console_h__
#endif //__WINDOWS

#ifndef easy_time_h__
#include "easy_time.h"
#endif //easy_time_h__

#define BUFFER_LENGTH		1024

namespace easy
{
	void EasyLog::SaveLog( const char* file_name,LoggingLevel level,const char* log_context ,... )
	{
		char buf[BUFFER_LENGTH];
		char* p = buf;
		va_list args;
		int n;
		va_start(args, log_context);
		// buf-3 is room for CR/LF/NUL
#ifdef  __WINDOWS
		n = _vsnprintf_s(p, BUFFER_LENGTH, sizeof buf - 3, log_context, args); 
#elif defined __LINUX
		n = vsnprintf(p, sizeof buf - 3, log_context, args); 
#endif //__WINDOWS
		va_end(args);

		p += (n < 0) ? sizeof buf - 3 : n;
		while ( p > buf  &&  isspace(p[-1]) )
		{
			*--p = '\0';
		}
		*p++ = '\n';
		*p   = '\0';
#ifdef __WINDOWS
#ifdef UNICODE
		OutputDebugStringW((LPCWSTR)buf);
#elif defined ASCII
		OutputDebugStringA(buf);
#endif // UNICODE
		SetConsoleColour(level);
#endif //__WINDOWS
		std::cout << buf;
		std::ofstream ofs(file_name,std::ios::app);
		if (!ofs.bad())
		{
			ofs << buf ;
			ofs.close();
		}
	}

	void EasyLog::SaveLog( const char* log_context,... )
	{
		char buf[BUFFER_LENGTH];
		char* p = buf;
		va_list args;
		int n;
		va_start(args, log_context);
		// buf-3 is room for CR/LF/NUL
#ifdef  __WINDOWS
		n = _vsnprintf_s(p, BUFFER_LENGTH, sizeof buf - 3, log_context, args); 
#elif defined __LINUX
		n = vsnprintf(p, sizeof buf - 3, log_context, args); 
#endif //__WINDOWS
		va_end(args);

		p += (n < 0) ? sizeof buf - 3 : n;
		while ( p > buf  &&  isspace(p[-1]) )
		{
			*--p = '\0';
		}
		*p++ = '\n';
		*p   = '\0';
#ifdef __WINDOWS
#ifdef UNICODE
		OutputDebugStringW((LPCWSTR)buf);
#elif defined ASCII
		OutputDebugStringA(buf);
#endif // UNICODE
		SetConsoleColour(level_);
#endif
		std::cout << buf;
		if (!ofstream_.bad())
		{
			ofstream_ << buf ;
		}
	}

	void EasyLog::OpenLogFile( const char* file_name )
	{
		if (!file_name)
		{
			return;
		}
		ofstream_.open(file_name,std::ios::trunc);
		level_ = kStandard;
		log_file_ = file_name;
	}

	void EasyLog::SetConsoleColour(LoggingLevel level)
	{
#ifdef __WINDOWS
		std::cout << easy::bg_black << easy::fg_white << std::setiosflags( std::ios::left );
		switch (level)
		{
		case kErrors:
			{
				std::cout <<  easy::fg_red 
					<<"[Errors:]  ";
				break;
			}
		case kWarnings:
			{
				std::cout <<  easy::fg_yellow 
					<<"[Warnings:]  ";
				break;
			}
		case kStandard:
			{
				std::cout <<  easy::fg_white 
					<<"[Standard:]  ";
				break;
			}
		case kInformative:
			{
				std::cout <<  easy::fg_cyan 
					<<"[Informative:]  ";
					break;
			}
		}
#elif defined __LINUX

#endif //__WINDOWS
	}

	EasyLog::~EasyLog()
	{
		ofstream_.close();
		easy_ulong32 time_to_num = EasyTime::time_to_number();
		std::stringstream log_file_backup;

		std::basic_string <char>::size_type pos = log_file_.find_last_of('.');
		std::string new_log_file;
		for (int i = 0; i < (int)pos; ++i)
		{
			new_log_file += log_file_[i];
		}

		log_file_backup  << new_log_file << "_" << time_to_num << ".log";
#ifdef __WINDOWS
#pragma warning(suppress: 6031)
		rename(log_file_.c_str(),log_file_backup.str().c_str());
#elif defined __LINUX
		rename(log_file_.c_str(),log_file_backup.str().c_str());
#endif
	}
}