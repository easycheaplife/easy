#include <vector>
#include <stdio.h>
int main()
{
	std::vector<int>	__vec;
	for(int __i = 0; __i < 10; ++__i)
	{
		__vec.push_back(__i);
	}
	printf("before erase\n");
	for(int __i = 0; __i < __vec.size(); ++__i)
	{
		printf("%d\n",__vec[__i]);
	}
	if(0)
	{
		for(int __i = 0; __i < 10; ++__i)
		{
			if(__i)
			{
				__vec.erase(__vec.begin() + __i);	//	__vec.size() will be changed,it dumps when the __i is large than __vec.size.
			}
		}
	}
	else
	{
		for(std::vector<int>::iterator __it = __vec.begin(); __it != __vec.end();)
		{
			if(*__it)
			{	
				__it = __vec.erase(__it);
			}
			else
			{
				++__it;
			}
		}
	}

	printf("after erase\n");
	for(int __i = 0; __i < __vec.size(); ++__i)
	{
		printf("%d\n",__vec[__i]);
	}
}