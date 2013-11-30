/********************************************************************
	created:	2011/10/25
	created:	25:10:2011   11:59
	filename: 	E:\lee-private-project\trunk\easy\src\test\cppunit\easy_file_reporter.h
	file path:	E:\lee-private-project\trunk\easy\src\test\cppunit
	file base:	easy_file_reporter
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_file_reporter_h__
#define easy_file_reporter_h__

#include <stdio.h>

#include "easy_cppunit_timer.h"

#pragma warning (disable : 4996)
//
// CppUnit mini file(stream) reporter
//
class FileReporter : public CPPUNIT_NS::Reporter 
{
private:
	FileReporter(const FileReporter&);
	FileReporter& operator=(const FileReporter&);
public:
	// reporting to stderr
	explicit FileReporter(bool doMonitor = false):
	m_numErrors(0), m_numIgnored(0), m_numExplicit(0), m_numTests(0), _myStream(false),
		m_failed(false), m_doMonitor(doMonitor)
	{ _file = stderr; }

	// reporting to the file with the given name
	explicit FileReporter(const char* file, bool doMonitor = false):
	m_numErrors(0), m_numIgnored(0), m_numExplicit(0), m_numTests(0), _myStream(true),
		m_failed(false), m_doMonitor(doMonitor)
	{
#ifndef _STLP_USE_SAFE_STRING_FUNCTIONS
		_file = fopen(file, "w");
#else
		fopen_s(&_file, file, "w");
#endif
	}

	// reporting to the given file
	explicit FileReporter(FILE* stream, bool doMonitor = false):
	m_numErrors(0), m_numIgnored(0), m_numExplicit(0), m_numTests(0), _myStream(false),
		m_failed(false), m_doMonitor(doMonitor)
	{ _file = stream; }

	virtual ~FileReporter() {
		if (_myStream)
			fclose(_file);
		else
			fflush(_file);
	}

	virtual void error(const char *in_macroName, const char *in_macro, const char *in_file, int in_line) {
		// Error might be called several times between 2 progress calls, we shouldn't however consider
		// that a test failed twice so we simply keep the info that test failed, number of failed tests
		// is computed later in end method.
		m_failed = true;
		fprintf(_file, "\n\n%s(%d) : %s(%s);", in_file, in_line, in_macroName, in_macro);
	}

	virtual void message( const char *msg )
	{ fprintf(_file, "\n\t%s", msg ); }

	virtual void progress(const char *in_className, const char *in_shortTestName, bool ignored, bool explicitTest) {
		if (m_doMonitor) {
			m_globalTimer.restart();
			m_testTimer.start();
		}
		++m_numTests;
		m_failed = false;
		if (ignored)
			++m_numIgnored;
		fprintf(_file, "%s::%s", in_className, in_shortTestName);
		if (ignored) {
			const char *ignoredReason;
			if (explicitTest) {
				++m_numExplicit;
				ignoredReason = " EXPLICIT";
			}
			else
				ignoredReason = " IGNORED";

			fprintf(_file, "%s", ignoredReason);
		}
	}

	virtual void end() {
		if (m_doMonitor) {
			m_globalTimer.stop();
			m_testTimer.stop();
			fprintf(_file, " %f msec", m_testTimer.elapsedMilliseconds());
		}
		if (m_failed) {
			++m_numErrors;
		}
		fprintf(_file, "\n");
	}

	virtual void printSummary() {
		if (m_numErrors > 0) {
			fprintf(_file, "\nThere were errors! %d of %d tests", m_numErrors, m_numTests);
		}
		else {
			fprintf(_file, "\nOK %d tests", m_numTests);
		}

		if (m_numIgnored > 0) {
			fprintf(_file, ", %d ignored", m_numIgnored);
		}

		if (m_numExplicit > 0) {
			fprintf(_file, " (%d explicit)", m_numExplicit);
		}

		if (m_doMonitor) {
			fprintf(_file, " %f msec", m_globalTimer.elapsedMilliseconds());
		}

		fprintf(_file, "\n\n");
	}
private:
	int m_numErrors;
	int m_numIgnored;
	int m_numExplicit;
	int m_numTests;
	// flag whether we own '_file' and are thus responsible for releasing it in the destructor
	bool  _myStream;
	bool m_failed;
	bool m_doMonitor;
	Timer m_globalTimer, m_testTimer;
	FILE* _file;
};
#endif // easy_file_reporter_h__