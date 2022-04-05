.SUFFIXES: .c .o .x .txt
DIR = `basename $(CURDIR)`

NAME1 = fifo
NAME2 = producent
NAME3 = konsument

PLIK1 = in.txt
PLIK2 = out.txt

EXEC1 = $(NAME1).x
EXEC2 = $(NAME2).x
EXEC3 = $(NAME3).x

OBJS1 = $(NAME1).O
OBJS2 = $(NAME2).O
OBJS3 = $(NAME3).O

CFLAGS	:= -Wall -std=c99 -pedantic -O -fPIC
LDFLAGS := -Wall -std=c99 -pedantic -O

CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<
	
%.x: %.o
	$(CO) $(LDFLAGS) -o $@ $^
	
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3)

.PHONY: run
run1: $(EXEC1) $(EXEC2) $(EXEC3)

.PHONY: diff
diff:
	diff -s $(PLIK1) $(PLIK2)

.PHONY: clean tar
clean:                                                     
	rm -f *.o *.a *.x

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	
