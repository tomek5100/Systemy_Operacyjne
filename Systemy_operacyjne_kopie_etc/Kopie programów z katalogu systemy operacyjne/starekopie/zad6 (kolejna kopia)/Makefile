.SUFFIXES: .c .o .x .h .txt
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

CFLAGS	:= -Wall -std=c99 -pedantic -o -fPIC -pthread
LDFLAGS := -Wall -std=c99 -pedantic -o -pthread

CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<
	
%.x: %.o
	$(CO) $(LDFLAGS) -o $@ $^
	
.PHONY: all
all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJS1) $(LIBS1)
	$(LD) -o $@ $(LDFLAGS) $^
$(EXEC2): $(OBJS2) $(LIBS2)
	$(LD) -o $@ $(LDFLAGS) $^

.PHONY: run
run: $(EXEC1) $(EXEC2)
	
.PHONY: clean tar
clean:                                                     
	rm -f *.o *.a *.x

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	
