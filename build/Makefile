#****************************************************************************
#
# Makefile for project config.
# by Lee 
#
# This is a GNU make (gmake) makefile
#****************************************************************************
# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := YES
# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := YES
#PLATFORN for can be set YES to windows platform, or LINUX to linux or unix platform
PLATFORN	   := LINUX

SHELL = /bin/sh
CC     := gcc
CXX    := g++

LD 	   := ld
AR 	   := ar
ARFLAGS := cq

DEBUG_CFLAGS     := -Wall -g -DDEBUG
RELEASE_CFLAGS   := -Wall -O3

DEBUG_CXXFLAGS   := ${DEBUG_CFLAGS} 
RELEASE_CXXFLAGS := ${RELEASE_CFLAGS}

DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  :=

ifeq (YES, ${DEBUG})
   CFLAGS       := ${DEBUG_CFLAGS}
   CXXFLAGS     := ${DEBUG_CXXFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
else
   CFLAGS       := ${RELEASE_CFLAGS}
   CXXFLAGS     := ${RELEASE_CXXFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
   CFLAGS   := ${CFLAGS} -pg -O3
   CXXFLAGS := ${CXXFLAGS} -pg -O3
   LDFLAGS  := ${LDFLAGS} -pg
endif

#****************************************************************************
# Preprocessor directives
#****************************************************************************

ifeq (LINUX, ${PLATFORN})
  DEFS := -D__LINUX
else
  DEFS :=
endif

#****************************************************************************
# Makefile code common to all platforms
#****************************************************************************

CFLAGS   := ${CFLAGS}   ${DEFS}
CXXFLAGS := ${CXXFLAGS} ${DEFS}

export CC CXX LD AR ARFLAGS CFLAGS CXXFLAGS LDFLAGS DEFS

SUBDIRS = base ../src/test/cppunit tinyxml

.PHONY : all
all :
	@for subdir in $(SUBDIRS);\
	do\
		(cd $$subdir && $(MAKE)) || exit 1;\
		echo $$subdir; \
	done
.PHONY : clean
clean :
	@for subdir in $(SUBDIRS);\
	do\
		(make clean -C $$subdir);\
	done

