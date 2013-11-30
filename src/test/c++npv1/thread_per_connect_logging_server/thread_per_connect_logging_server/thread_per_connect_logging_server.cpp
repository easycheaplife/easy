// reactive_logging_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "thread_per_connect_logging_server.h"
#include "ace/Signal.h"
#include "ace/Time_Value.h"

namespace {
	extern "C" void sigterm_handler (int /* signum */) { /* No-op. */ }
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Register to receive the <SIGTERM> signal.
	ACE_Sig_Action sa ((ACE_SignalHandler)sigterm_handler,SIGTERM);

	thread_per_connect_logging_server __server;
	__server.run(9876);
	ACE_Thread_Manager::instance()->cancel_all();
	ACE_Time_Value __time_out(60);
	ACE_Thread_Manager::instance()->wait(&__time_out);
	return 0;
}

