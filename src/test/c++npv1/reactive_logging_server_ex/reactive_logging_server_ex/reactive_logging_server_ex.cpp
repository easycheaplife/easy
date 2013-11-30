// reactive_logging_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "reactive_logging_server_ex.h"

int _tmain(int argc, _TCHAR* argv[])
{
	reactive_logging_server_ex __server;
	__server.run(9876);
	return 0;
}

