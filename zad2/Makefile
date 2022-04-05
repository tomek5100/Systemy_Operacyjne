.SUFFIXES: .c .o .x
DIR = `basename $(CURDIR)`

NAME1 = zad2
NAME2 = zadA

EXEC1 = $(NAME1).x
EXEC2 = $(NAME2).x

OBJS1 = $(NAME1).O
OBJS2 = $(NAME2).O

CFLAGS	:= -Wall -std=c99 -pedantic -O -fPIC
LDFLAGS := -Wall -std=c99 -pedantic -O

CO = gcc
LD = $(CO)

AR = ar
ARFLAGS = rsv

%.o: %.c
	$(CO) $(CFLAGS) -c $<
	
%.x: %.o
	$(CO) $(LDFLAGS) -o $@ $^
	
.PHONY: all
all: $(EXEC1) $(EXEC2)

.PHONY: run

run: $(EXEC1) $(EXEC2)
	./$(EXEC1) $(EXEC2)
	

.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	
