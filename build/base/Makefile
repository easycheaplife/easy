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
INCS := ../../src/base

#****************************************************************************
# Source files
#****************************************************************************
OBJS = myadd.o \
mysub.o \
easy_big_int.o \
easy_log.o \
easy_exception.o \
easy_util.o \
easy_threading.o \
easy_atomic_op.o

SRCS = $(INCS)/myadd.cc \
$(INCS)/mysub.cc \
$(INCS)/easy_big_int.cc \
$(INCS)/easy_log.cc \
$(INCS)/easy_exception.cc \
$(INCS)/easy_util.cc \
$(INCS)/easy_threading.cc \
$(INCS)/easy_atomic_op.cc \

#	OBJS := $(addsuffix .o,$(basename ${SRCS}))

#****************************************************************************
# Targets of the build
#****************************************************************************
OUTPUT := ../../lib/libeasy.a
.PHONY: all
all: ${OUTPUT}

$(OUTPUT): $(OBJS)
	$(AR) -$(ARFLAGS) $@ $^
%.o: $(INCS)/%.cc $(INCS)/%.h

	$(CXX) -c ${CXXFLAGS} -v $(SRCS);

.PHONY: clean
clean:
	-rm *.o *.a
	
