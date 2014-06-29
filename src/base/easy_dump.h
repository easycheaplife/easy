#include <execinfo.h>	//	for backtrace backtrace_symbols. reference from: http://linux.die.net/man/3/backtrace_symbols
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void dump(int __signal)
{
	const int __max_stack_flow = 20;
	void* __array[__max_stack_flow];
	char** __strings;
	size_t __size = backtrace(__array,__max_stack_flow);
	printf("backtrace() returned %d addresses\n", __size);
	__strings = backtrace_symbols(__array,__size);
	if(NULL == __strings)
	{
		prrror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}
	fprintf (stderr,"obtained %zd stack frames.nm", __size);
	for (int __i = 0; __i < __size; ++__i)
	{
		printf("%s\n", __strings[i]);
	}
	//	This __strings is malloc(3)ed by backtrace_symbols(), and must be freed here
	free (__strings);
    exit(0);
}
