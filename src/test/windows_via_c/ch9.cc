#include "easy_cppunit_proxy.h"
#include "easy_threading.h"
#include <tchar.h>
//
// TestCase class
//
class TestCh9 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestCh9);
#if 0
	CPPUNIT_IGNORE;
#endif
#if 0
	CPPUNIT_TEST(test_event);
	CPPUNIT_TEST(test_multi_event);
#else
	CPPUNIT_TEST(test_semaphore);
#endif
	CPPUNIT_TEST_SUITE_END();

protected:
	void test_event();
	void test_multi_event();
	void test_semaphore();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCh9);

//
// tests implementation
//

TCHAR __share_memory_block[BUFSIZ];
TCHAR __read_data_block[BUFSIZ];
HANDLE __server_event;
HANDLE __client_event;

class server_thread : easy::EasyThread
{
public:
	easy_int32 _Run( void* p )
	{
		wait_res_ = ::WaitForSingleObject(__client_event,INFINITE);
		switch (wait_res_)
		{
			// All read-event objects were signaled
		case WAIT_OBJECT_0: 
			{
				int __len = wcslen(__share_memory_block);
				wmemcpy_s(__read_data_block,BUFSIZ,__share_memory_block,sizeof(TCHAR)*__len);
				wmemcpy_s(__share_memory_block,BUFSIZ,_T("nice to meet you, too!"),wcslen(_T("nice to meet you, too!")));
				::SetEvent(__server_event);
				wprintf(_T("server process data: %s\n"), __share_memory_block); 
			}
			break;

			// An error occurred
		default: 
			printf("Wait error: %d\n", GetLastError()); 
			ExitProcess(0); 
		}
		return 0;
	}
private:
	DWORD	wait_res_;
};
//	main thread
void TestCh9::test_event()
{
	memset(__share_memory_block,0,sizeof(TCHAR)*BUFSIZ);
	memset(__read_data_block,0,sizeof(TCHAR)*BUFSIZ);
	__server_event = CreateEvent(NULL,TRUE,FALSE,_T("server_event"));
	__client_event = CreateEvent(NULL,TRUE,FALSE,_T("client_event"));
	server_thread* __server_thread = new server_thread();

	int __len = wcslen(_T("hello,nice to meet you!"));
	wmemcpy_s(__share_memory_block,BUFSIZ,_T("hello,nice to meet you!"),__len*sizeof(TCHAR));
	SetEvent(__client_event);

	DWORD wait_res_ = ::WaitForSingleObject(__server_event,INFINITE);
	switch (wait_res_)
	{
		// All read-event objects were signaled
	case WAIT_OBJECT_0: 
		{
			wprintf(_T("client recv data: %s\n"), __share_memory_block); 
		}
		break;

		// An error occurred
	default: 
		printf("Wait error: %d\n", GetLastError()); 
		delete __server_thread;
		ExitProcess(0); 
	}
	while(1)
	{
		::Sleep(100);
	}
	delete __server_thread;
}

#define THREADCOUNT 4 

HANDLE ghGlobalWriteEvent; 
HANDLE ghReadEvents[THREADCOUNT];
DWORD WINAPI ThreadProc(LPVOID lpParam) 
{
	DWORD dwWaitResult;
	HANDLE hEvents[2]; 

	hEvents[0] = *(HANDLE*)lpParam;  // thread's read event
	hEvents[1] = ghGlobalWriteEvent; // global write event

	dwWaitResult = WaitForMultipleObjects( 
		2,            // number of handles in array
		hEvents,      // array of event handles
		TRUE,         // wait till all are signaled
		INFINITE);    // indefinite wait

	switch (dwWaitResult) 
	{
		// Both event objects were signaled
	case WAIT_OBJECT_0: 
		// TODO: Read from the shared buffer
		printf("Thread %d reading from buffer...\n", 
			GetCurrentThreadId());
		break; 

		// An error occurred
	default: 
		printf("Wait error: %d\n", GetLastError()); 
		ExitThread(0); 
	}

	// Set the read event to signaled

	if (! SetEvent(hEvents[0]) ) 
	{ 
		printf("SetEvent failed (%d)\n", GetLastError());
		ExitThread(0);
	} 

	return 1;
}

void CreateEventsAndThreads(void) 
{
	HANDLE hThread; 
	DWORD i, dwThreadID; 

	// Create a manual-reset event object. The master thread sets 
	// this to nonsignaled when it writes to the shared buffer. 

	ghGlobalWriteEvent = CreateEvent( 
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		TRUE,               // initial state is signaled
		TEXT("WriteEvent")  // object name
		); 

	if (ghGlobalWriteEvent == NULL) 
	{ 
		printf("CreateEvent failed (%d)\n", GetLastError());
		return;
	}
	else if ( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		printf("Named event already exists.\n");
		return;
	}

	// Create multiple threads and an auto-reset event object 
	// for each thread. Each thread sets its event object to 
	// signaled when it is not reading from the shared buffer. 

	for(i = 0; i < THREADCOUNT; i++) 
	{
		// Create the auto-reset event
		ghReadEvents[i] = CreateEvent( 
			NULL,     // no security attributes
			FALSE,    // auto-reset event
			TRUE,     // initial state is signaled
			NULL);    // object not named

		if (ghReadEvents[i] == NULL) 
		{
			printf("CreateEvent failed (%d)\n", GetLastError());
			return;
		}

		hThread = CreateThread(NULL, 
			0, 
			ThreadProc, 
			&ghReadEvents[i],  // pass event handle
			0, 
			&dwThreadID); 

		if (hThread == NULL) 
		{
			printf("CreateThread failed (%d)\n", GetLastError());
			return;
		}
	}
}

void WriteToBuffer(VOID) 
{
	DWORD dwWaitResult, i; 

	// Reset ghGlobalWriteEvent to nonsignaled, to block readers

	if (! ResetEvent(ghGlobalWriteEvent) ) 
	{ 
		printf("ResetEvent failed (%d)\n", GetLastError());
		return;
	} 

	// Wait for all reading threads to finish reading

	dwWaitResult = WaitForMultipleObjects( 
		THREADCOUNT,   // number of handles in array
		ghReadEvents,  // array of read-event handles
		TRUE,          // wait until all are signaled
		INFINITE);     // indefinite wait

	switch (dwWaitResult) 
	{
		// All read-event objects were signaled
	case WAIT_OBJECT_0: 
		// TODO: Write to the shared buffer
		printf("Main thread writing to the shared buffer...\n");
		break;

		// An error occurred
	default: 
		printf("Wait error: %d\n", GetLastError()); 
		ExitProcess(0); 
	} 

	// Set ghGlobalWriteEvent to signaled

	if (! SetEvent(ghGlobalWriteEvent) ) 
	{
		printf("SetEvent failed (%d)\n", GetLastError());
		ExitProcess(0);
	}

	// Set all read events to signaled
	for(i = 0; i < THREADCOUNT; i++) 
		if (! SetEvent(ghReadEvents[i]) ) 
		{ 
			printf("SetEvent failed (%d)\n", GetLastError());
			return;
		} 
}

void CloseEvents()
{
	int i;

	for( i=0; i < THREADCOUNT; i++ )
		CloseHandle(ghReadEvents[i]);

	CloseHandle(ghGlobalWriteEvent);
}

void TestCh9::test_multi_event()
{
	int i;

	// TODO: Create the shared buffer

	// Create the events and THREADCOUNT threads to read from the buffer

	CreateEventsAndThreads();

	// Write to the buffer three times, just for test purposes

	for(i=0; i < 3; i++)
		WriteToBuffer();

	// Close the events

	CloseEvents();

}

#define MAX_SEM_COUNT 10
#define THREADCOUNT2 12

HANDLE ghSemaphore;
DWORD WINAPI ThreadProc2( LPVOID lpParam )
{
	DWORD dwWaitResult; 
	BOOL bContinue=TRUE;

	while(bContinue)
	{
		// Try to enter the semaphore gate.

		dwWaitResult = WaitForSingleObject( 
			ghSemaphore,   // handle to semaphore
			0L);           // zero-second time-out interval

		switch (dwWaitResult) 
		{ 
			// The semaphore object was signaled.
		case WAIT_OBJECT_0: 
			{
				// TODO: Perform task
				printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
				bContinue=FALSE;            

				// Simulate thread spending time on task
				Sleep(5);

				// Relase the semaphore when task is finished

				if (!ReleaseSemaphore( 
					ghSemaphore,  // handle to semaphore
					1,            // increase count by one
					NULL) )       // not interested in previous count
				{
					printf("ReleaseSemaphore error: %d\n", GetLastError());
				}
				HANDLE __open_semaphone = OpenSemaphore(EVENT_ALL_ACCESS,TRUE,_T("lee"));
				if (NULL == __open_semaphone)
				{

				}
			}
			break; 

			// The semaphore was nonsignaled, so a time-out occurred.
		case WAIT_TIMEOUT: 
			printf("Thread %d: wait timed out\n", GetCurrentThreadId());
			break; 
		}
	}
	return TRUE;
}
void TestCh9::test_semaphore()
{
	HANDLE aThread[THREADCOUNT2];
	DWORD ThreadID;
	int i;

	// Create a semaphore with initial and max counts of MAX_SEM_COUNT

	ghSemaphore = CreateSemaphore( 
		NULL,           // default security attributes
		MAX_SEM_COUNT,  // initial count
		MAX_SEM_COUNT,  // maximum count
		_T("lee"));          // unnamed semaphore

	if (ghSemaphore == NULL) 
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return;
	}

	// Create worker threads

	for( i=0; i < THREADCOUNT2; i++ )
	{
		aThread[i] = CreateThread( 
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE) ThreadProc2, 
			NULL,       // no thread function arguments
			0,          // default creation flags
			&ThreadID); // receive thread identifier

		if( aThread[i] == NULL )
		{
			printf("CreateThread error: %d\n", GetLastError());
			return;
		}
	}

	// Wait for all threads to terminate

	WaitForMultipleObjects(THREADCOUNT2, aThread, TRUE, INFINITE);

	// Close thread and semaphore handles

	for( i=0; i < THREADCOUNT2; i++ )
		CloseHandle(aThread[i]);

	CloseHandle(ghSemaphore);

}
