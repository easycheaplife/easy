SHELL = /bin/sh
CC = gcc
AR = ar

CFLAG = -O2 -W -Wall

INCLUDE_PATH = -I.
TEST_TARGET = ./libpuls.a
TEST_OBJ = puls.o
TEST_SRC = ./puls.cc

all : $(TEST_TARGET)
$(TEST_OBJ): $(TEST_SRC)
	$(CC) $(CFLAG) $(INCLUDE_PATH) -c $(TEST_SRC)

$(TEST_TARGET) : $(TEST_OBJ)
	$(AR) cq $@ $^

clean:
	rm -f *.o $(TEST_TARGET) 
