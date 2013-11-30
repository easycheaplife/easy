/********************************************************************
	created:	2011/10/25
	created:	25:10:2011   11:40
	filename: 	E:\lee-private-project\trunk\easy\src\test\cppunit\easy_cppunit_timer.h
	file path:	E:\lee-private-project\trunk\easy\src\test\cppunit
	file base:	easy_cppunit_timer
	file ext:	h
	author:		Lee
	
	purpose:	
*********************************************************************/
#ifndef easy_cppunit_timer_h__
#define easy_cppunit_timer_h__

#if defined (_WIN32)
#  define CPPUNIT_WIN32_TIMER
#  include <windows.h>
#endif

class Timer {
public:
	Timer() 
	{
#if defined (CPPUNIT_WIN32_TIMER)
		m_start.LowPart = m_restart.LowPart = m_stop.LowPart = 0;
		m_start.HighPart = m_restart.HighPart = m_stop.HighPart = 0;
		QueryPerformanceFrequency(&m_frequency);
#endif
	}

	void start() 
	{
#if defined (CPPUNIT_WIN32_TIMER)
		QueryPerformanceCounter(&m_start);
#endif
	}

	void restart() 
	{
#if defined (CPPUNIT_WIN32_TIMER)
		QueryPerformanceCounter(&m_restart);
		if (m_start.HighPart == 0 && m_start.LowPart == 0) 
		{
			m_start = m_restart;
		}
#endif
	}

	void stop() 
	{
#if defined (CPPUNIT_WIN32_TIMER)
		LARGE_INTEGER stop;
		QueryPerformanceCounter(&stop);
		if ((m_stop.HighPart != 0 || m_stop.LowPart != 0) &&
			m_restart.HighPart != 0 && m_restart.LowPart != 0) 
		{
				m_stop.HighPart += (stop.HighPart - m_restart.HighPart);
				if (stop.LowPart < m_restart.LowPart) {
					if (m_restart.LowPart - stop.LowPart > m_stop.LowPart) 
					{
						m_stop.HighPart -= 1;
					}
					m_stop.LowPart -= m_restart.LowPart - stop.LowPart;
				}
				else 
				{
					if (stop.LowPart - m_restart.LowPart > 0xFFFFFFFF - m_stop.LowPart) 
					{
						m_stop.HighPart += 1;
					}
					m_stop.LowPart += stop.LowPart - m_restart.LowPart;
				}
		}
		else 
		{
			m_stop = stop;
		}
#endif
	}

	double elapsedMilliseconds() const 
	{
#if defined (CPPUNIT_WIN32_TIMER)
		LARGE_INTEGER elapsed;
		elapsed.HighPart = m_stop.HighPart - m_start.HighPart;
		elapsed.LowPart = m_stop.LowPart - m_start.LowPart;
		return (double)elapsed.QuadPart / (double)m_frequency.QuadPart * 1000;
#else
		return 0;
#endif
	}

	static bool supported() 
	{
#if defined (CPPUNIT_WIN32_TIMER)
		return true;
#else
		return false;
#endif
	}

private:
#if defined (CPPUNIT_WIN32_TIMER)
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_start, m_stop, m_restart;
#endif
};

#endif // easy_cppunit_timer_h__