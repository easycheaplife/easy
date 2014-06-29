#****************************************************************************
#
# Makefile for project config.
# Lee 
#
# This is a GNU make (gmake) makefile
#****************************************************************************

#****************************************************************************
# Include paths
#****************************************************************************
INCS := ../../base 
LIBS = ../../../lib
CFLAGS += -I$(INCS)
LFLAGS += -L$(LIBS)
CPPFLAGS = $(CFLAGS)

#****************************************************************************
# Source files
# test file shold be the first line, you should not be a empty line
#****************************************************************************
OBJS = easy_main.o
SRCS = easy_main.cc \
../easy_test_dump.cc \
#../stl/stl_advance_test.cc \
#../easy_unit_test_template.cc \
#../easy_test_log.cc \
#../easy_test_singleton.cc \
#../easy_test_type_traits.cc \
#../easy_test_allocator.cc\
#../easy_test_template.cpp\
#../easy_test_list.cc\
#../easy_test_locked_queue.cc\
#../easy_test_byte_buffer.cc \
#../easy_test_ring_buffer.cc \
#../easy_test_threading.cc \
#../easy_test_stdandard_allocator.cc \


.PHONY : easy_main
easy_main : $(OBJS) 
	$(CXX) $(SRCS) -I./ -D__LINUX $(CPPFLAGS) $(LFLAGS) -o $^ -o $@ -leasy -pthread -rdynamic
.PHONY : clean
clean:
	-rm *.o test_main
