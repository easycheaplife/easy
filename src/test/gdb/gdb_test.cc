#include<stdio.h>
int sum(int __n)
{
	int __sum = 0;
	for(int __i = 0; __i < __n; ++__i)
	{
		__sum += __i;
	}
	return __sum;
}

int main()
{
	int __result = 0;
	for(int __i = 0; __i < 100; ++__i)
	{
		__result += __i;
	}
	printf("rslult[1~100] = %d\n",__result);
	printf("rslult[1~250] = %d\n",sum(250));
	getchar();
	return 0;
}