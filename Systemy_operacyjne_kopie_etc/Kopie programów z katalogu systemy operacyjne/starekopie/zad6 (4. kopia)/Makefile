.SUFFIXES: .c .o .x .h .txt
DIR = `basename $(CURDIR)`

CFLAGS	:= -Wall -std=c99 -pedantic -o -fPIC -pthread
LDFLAGS := -Wall -std=c99 -pedantic -o -pthread
CC = gcc -std=c99 -pedantic
LDLIBS := -L. -lrt -lsem

.PHONY: all
all: powielacz.x zew.x

lib.o: lib.c lib.h
libsem.a: lib.o
	ar rv libsem.a lib.o

zew.x: zew.o libsem.a
zew.o: zew.x lib.h
powielacz.x: powielacz.o libsem.a
powielacz.o: powielacz.c lib.h

%.x: %.o
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
.PHONY: clean tar
clean:                                                     
	rm -f *.o *.a *.x

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	
