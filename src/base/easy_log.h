/********************************************************************
	created:	2011/11/17
	created:	17:11:2011   18:49
	filename: 	E:\lee-private-project\trunk\easy\src\base\easy_log.h
	file path:	E:\lee-private-project\trunk\easy\src\base
	file base:	easy_log
	file ext:	h
	author:		Lee
	
	purpose:	log class to record information by special level
*********************************************************************/
#ifndef easy_log_h__
#define easy_log_h__

#include <stdarg.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#ifndef easy_copy_disabled_h__
#include "easy_copy_disabled.h"
#endif //easy_copy_disabled_h__

#ifndef easy_singleton_h__
#include "easy_singleton.h"
#endif //easy_singleton_h__

#define LOG_FILE_EASY	"..\\log\\easy.log"
#define LOG_FILE_DATABASE	"..\\log\\db.log"


namespace easy
{
	/*
	 *	Enumeration of logging levels
	 */
	enum LoggingLevel
	{
		kErrors,			//	Only actual error conditions will be logged.the color is red
		kWarnings,			//	Warnings will be logged as well.the color is gray
		kStandard,			//	Basic events will be logged (default level).the color is white
		kInformative,		//	Useful tracing (object creations etc) information will be logged,this level do not write to text file
	};
	class EasyLog : public EasyCopyDisabled
	{
		friend class EasySingleton<EasyLog>;
	public:
		/*
		 *	save log to special file, this a static function,open the file when write something.
		 */
		static void SaveLog(const char* file_name,LoggingLevel level,const char* log_context,...);

		static void SetConsoleColour(LoggingLevel level);

		/*
		 *	open a log file,clear exist log file
		 */
		void OpenLogFile(const char* file_name);

		/*
		 *	write log to file
		 */
		void SaveLog(const char* log_context,...);

		void set_level(LoggingLevel level){ level_ = level;}

	private:
		EasyLog() { }

		~EasyLog(); 

	private:
		std::string   log_file_;

		std::ofstream ofstream_;

		LoggingLevel	level_;
	};
	#define s_log easy::EasySingleton<EasyLog>::Instance()
}
	
#endif // easy_log_h__

