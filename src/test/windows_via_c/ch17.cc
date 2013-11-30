#include "easy_cppunit_proxy.h"
#include <WindowsX.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <stdio.h>

//
// TestCase class
//
class TestCh17 : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestCh17);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test_shared);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test_shared();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCh17);

//
// tests implementation
//

#define USE_SHARE_MEMORY
std::wstring share_memory_name = L"lee";
std::wstring share_memory_context = L"how do you do!";

// Tell the linker to make the Shared section readable, writable, and shared.
#pragma comment(linker, "/Section:Shared,RWS")

// Tell the compiler to put this initialized variable in its own Shared 
// section so it is shared by all instances of this application.
#pragma data_seg("Shared")
volatile LONG app_inst = 0;
#pragma data_seg()

TCHAR szName[]=TEXT("LARGEPAGE");
typedef int (*GETLARGEPAGEMINIMUM)(void);
#define BUF_SIZE 65536


void DisplayError(TCHAR* pszAPI, DWORD dwError)
{
	LPVOID lpvMessageBuffer;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpvMessageBuffer, 0, NULL);

	//... now display this string
	_tprintf(TEXT("ERROR: API        = %s\n"), pszAPI);
	_tprintf(TEXT("       error code = %d\n"), dwError);
	_tprintf(TEXT("       message    = %s\n"), lpvMessageBuffer);

	// Free the buffer allocated by the system
	LocalFree(lpvMessageBuffer);

	ExitProcess(GetLastError());
}

void Privilege(TCHAR* pszPrivilege, BOOL bEnable)
{
	HANDLE           hToken;
	TOKEN_PRIVILEGES tp;

	// open process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		DisplayError(TEXT("OpenProcessToken"), GetLastError());

	// get the luid
	if (!LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid))
		DisplayError(TEXT("LookupPrivilegeValue"), GetLastError());

	tp.PrivilegeCount = 1;

	// enable or disable privilege
	if (bEnable)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// enable or disable privilege
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		DisplayError(TEXT("AdjustTokenPrivileges"), GetLastError());

	// close the handle
	if (!CloseHandle(hToken))
		DisplayError(TEXT("CloseHandle"), GetLastError());
}

void TestCh17::test_shared()
{
	// There is another instance of this application running
	InterlockedExchangeAdd((PLONG) & app_inst,1);
	std::cout << "number of instance running " << app_inst << std::endl;
#if 0
	system("pause");
#endif
	// This instance of the application is terminating
	InterlockedExchangeAdd((PLONG) &app_inst, -1);

#ifdef USE_SHARE_MEMORY
	HANDLE handle = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUFSIZ,share_memory_name.c_str());
	if (NULL == handle)
	{
		std::cout << "Could not create file mapping object." <<  GetLastError() << std::endl;
		return;
	}
	LPCTSTR buf = static_cast<LPTSTR>(MapViewOfFile(handle,FILE_MAP_ALL_ACCESS,0,0,BUFSIZ));
	if (buf == NULL) 
	{
		std::cout << "Could not map view of file." <<  GetLastError() << std::endl;
		return ;
	}
	memcpy((PVOID)buf, share_memory_context.c_str(), share_memory_context.size());
#else
	HANDLE handle = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,share_memory_name.c_str());
	if (NULL == handle)
	{
		std::cout << "Could not open file mapping object." <<  GetLastError() << std::endl;
		return;
	}
	LPCTSTR buf = static_cast<LPTSTR>(MapViewOfFile(handle,FILE_MAP_ALL_ACCESS,0,0,BUFSIZ));
	if (buf == NULL) 
	{
		std::cout << "Could not map view of file." <<  GetLastError() << std::endl;
		return ;
	}
	std::wstring contex;
	wmemcpy(const_cast<wchar_t*>(contex.c_str()),buf,BUFSIZ);
#endif
	system("pause");
	UnmapViewOfFile(buf);
	CloseHandle(handle);
#if 0
	//	map big file
	// call succeeds only on Windows Server 2003 SP1 or later
	HINSTANCE  hDll = LoadLibrary(TEXT("kernel32.dll"));
	if (hDll == NULL)
	{
		DisplayError(TEXT("LoadLibrary"), GetLastError());
	}
	GETLARGEPAGEMINIMUM pGetLargePageMinimum = (GETLARGEPAGEMINIMUM)GetProcAddress(hDll,"GetLargePageMinimum");
	if (pGetLargePageMinimum == NULL)
	{
		DisplayError(TEXT("GetProcAddress"), GetLastError());
	}
	SIZE_T size = (*pGetLargePageMinimum)();
	FreeLibrary(hDll);
	printf("Page Size: %u\n", size);
	Privilege(TEXT("SeLockMemoryPrivilege"), TRUE);

	HANDLE map_file = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE | SEC_COMMIT | SEC_LARGE_PAGES,
		0,                       // max. object size 
		size,                    // buffer size  
		szName);                 // name of mapping object

	if (map_file == NULL) 
		DisplayError(TEXT("CreateFileMapping"), GetLastError());
	else
		printf("File mapping object successfully  created.\n");

	Privilege(TEXT("SeLockMemoryPrivilege"), FALSE);

	LPCTSTR pBuf = (LPTSTR) MapViewOfFile(map_file,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		BUF_SIZE);           

	if (pBuf == NULL) 
		DisplayError(TEXT("MapViewOfFile"), GetLastError());
	else
		printf("View of file successfully mapped.\n");

	// do nothing, clean up an exit
	UnmapViewOfFile(pBuf);
	CloseHandle(map_file);
#endif
}
