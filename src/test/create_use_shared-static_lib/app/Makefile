OBJS = test.o
INCLUDE_DIR = ../mylib
CFLAGS += -I$(INCLUDE_DIR)
LFLAGS += -L$(INCLUDE_DIR)
.PHONY : test
test : $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@ -lmy

.PHONY : clean
clean:
	-rm *.o test
