OBJS = easy_test_linux_static_lib.o
INCLUDE_DIR = ../base
LIB_DIR = ../../lib
CFLAGS += -I$(INCLUDE_DIR)
LFLAGS += -L$(LIB_DIR)
CPPFLAGS = $(CFLAGS)
.PHONY : easy_test_linux_static_lib
easy_test_linux_static_lib : $(OBJS) 
#g++ easy_test_linux_static_lib.cc -leasy $(CPPFLAGS) $(LFLAGS)  -static -o easy_test_linux_static_lib
	$(CXX) -g -Wall -v $(CPPFLAGS) $(LFLAGS) $^ -o $@ -leasy -pthread -Wunused-variable

.PHONY : clean
clean:
	-rm *.o easy_test_linux_static_lib
