// iterator_logging_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iterator_logging_server.h"
#include "ace/Task.h"

class my_task : public ACE_Task_Base
{
	virtual int svc (void)
	{
		while(1)
		{
			::Sleep(1);
			printf("my_task running\n");
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
#if 1
	iterator_logging_server __server;
	__server.run(9876);
#else
	my_task __task;
	__task.activate();
	__task.wait();
#endif
	return 0;
}

