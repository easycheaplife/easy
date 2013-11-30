CC = gcc 
LD = ld
AR = ar
ARFLAGS = cq
export CC LD AR ARFLAGS

SUBDIRS = mylib app

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



