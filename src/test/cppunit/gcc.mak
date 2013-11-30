#****************************************************************************
#
# Makefile for project config.
# Lee 
#
# This is a GNU make (gmake) makefile
#****************************************************************************
OBJS = easy_main.o
.PHONY : easy_main
easy_main : $(OBJS) 
	g++ easy_main.cc easy_unit_test_template.cc -o easy_main
.PHONY : clean
clean:
	-rm *.o test_main
