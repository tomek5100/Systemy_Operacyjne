.SUFFIXES: .c .o .x .h

DIR = `basename $(CURDIR)`

NAME1 = powielacz
NAME2 = zew
NAME3 = lib

EXEC1 = $(NAME1).x
EXEC2 = $(NAME2).x

OBJS1 = $(NAME1).o
OBJS2 = $(NAME2).o

LIBS1 = $(NAME3).c

HEADS1 = $(NAME3).h

CO = gcc
LD = $(CO)

CFLAGS = -Wall -std=c99 -pedantic -O -fPIC -pthread
LFLAGS = -Wall -std=c99 -pedantic -O -pthread

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all, run
all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJS1) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC2): $(OBJS2) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^

run: $(EXEC1) $(EXEC2)

.PHONY: clean tar
clean: 
	rm -f *.o  *~ *.a *.so *.x core core* a.out;

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
