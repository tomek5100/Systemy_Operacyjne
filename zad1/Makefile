.SUFFIXES: .c .o .x
DIR = `basename $(CURDIR)`

NAME1 = zadA
NAME2 = zadB
NAME3 = zadC
NAME4 = zadD
NAME5 = zadE

EXEC1 = $(NAME1).x
EXEC2 = $(NAME2).x
EXEC3 = $(NAME3).x
EXEC4 = $(NAME4).x
EXEC5 = $(NAME5).x

OBJS1 = $(NAME1).O
OBJS2 = $(NAME2).O
OBJS3 = $(NAME3).O
OBJS4 = $(NAME4).O
OBJS5 = $(NAME5).O

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
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

.PHONY: run1

run1: $(EXEC1)
	./$(EXEC1)
	
.PHONY: run2

run2: $(EXEC2)
	./$(EXEC2)

.PHONY: run3

run3: $(EXEC3)
	./$(EXEC3)

.PHONY: run4

run4: $(EXEC4)
	./$(EXEC4)

.PHONY: run5

run5: $(EXEC5)
	./$(EXEC5)

.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	
