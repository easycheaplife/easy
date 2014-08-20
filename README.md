easy,cross platform c++ class lib (makefile version) which include database,network,thread,lock.buffer,and so on. at the directory of src/test you will find some test code,you can use at windows or linux to do unit test.
winodws:
	use vistual studio 2012 and just add the test code to you project.
	
linux:
	change the src/test/cppunit/Makefile,some thing like this:
	#****************************************************************************
	# Source files
	# test file shold be the first line, you should not be a empty line
	#****************************************************************************
	OBJS = easy_main.o
	SRCS = easy_main.cc \
	../easy_test_lock.cc \
	../easy_test_allocator.cc \
	../easy_test_threading.cc \
	../stl/stl_map_test.cc \
	../easy_test_dump.cc \
	../stl/stl_advance_test.cc \
	../easy_unit_test_template.cc \
	../easy_test_log.cc \
	../easy_test_singleton.cc \
	../easy_test_type_traits.cc \
	../easy_test_template.cpp\
	../easy_test_list.cc\
	../easy_test_locked_queue.cc\
	../easy_test_byte_buffer.cc \
	../easy_test_ring_buffer.cc \
	../easy_test_stdandard_allocator.cc \
	and compile at src/build/Makefile,it will find all sub directory and compile everyone.
	
	enjoy it !
	
