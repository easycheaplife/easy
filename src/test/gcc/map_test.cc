#include <map>
#include <stdio.h>
int main()
{
	std::map<int,int>	__map;
	for(int __i = 0; __i < 10; ++__i)
	{
		__map[__i] = __i;
	}
	for(int __i = 0; __i < 10; ++__i)
	{
		printf("before erase:\n");
		printf("%d\n",__map[__i]);
	}
	
	for(std::map<int,int>::iterator __it = __map.begin(); __it != __map.end();)
	{
		if(__it->first)
		{
			/*
			map_test.cc:19:27: error: no match for ¡®operator=¡¯ in ¡®__it = __map.std::map<_Key, _Tp, _Compare, _Alloc>::erase [with _Key = int, _Tp = int, 
			_Compare = std::less<int>, _Alloc = std::allocator<std::pair<const int, int> >, std::map<_Key, _Tp, _Compare, _Alloc>::iterator = std::_Rb_tree_iterator<std::pair<const int, int> >](__it)¡¯
			/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/bits/stl_tree.h:154:5: note: candidate is: 
			std::_Rb_tree_iterator<std::pair<const int, int> >& std::_Rb_tree_iterator<std::pair<const int, int> >::operator=(const std::_Rb_tree_iterator<std::pair<const int, int> >&)
			*/
			//__it = __map.erase(__it);	//	error at linux gcc, return a iterator is not stl standard
			__map.erase(__it++);		//	this is ok?
		}
		else
		{
			++__it;
		}
	}
	
	for(int __i = 0; __i < 10; ++__i)
	{
		printf("after erase:\n");
		printf("%d\n",__map[__i]);
	}
	return 0;
}