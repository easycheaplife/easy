OBJS = myadd.o mysub.o
libmy.a: $(OBJS)
	$(AR) -$(ARFLAGS) $@ $^
%.o: %.c %.h
	$(CC) -c myadd.c mysub.c;
.PHONY: clean
clean:
	-rm *.o *.a
	
