SHELL = /bin/sh
CC = gcc

INCPATH = -I.
TEST_TARGET = ./puls_lib_test
TEST_OBJ = puls.o
TEST_SRC = ./puls_lib_test.cc
LDFLAGS := ./libpuls.a
all : 
	$(CC) -g $(TEST_SRC) $(LDFLAGS) -o $(TEST_TARGET) $(INCPATH)
	#gcc -g ./puls_lib_test.cc ./libpuls.a -o ./lib_puls_test -I.

clean:
	rm -rf *.o $(TEST_TARGET) 
