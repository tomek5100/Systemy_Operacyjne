.SUFFIXES: .c .o .x .txt
DIR = `basename $(CURDIR)`

NAME1 = zad4
PLIK1 = in.txt
PLIK2 = out.txt

EXEC1 = $(NAME1).x

OBJS1 = $(NAME1).O

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
all: $(EXEC1)

.PHONY: run
run: $(EXEC1)

.PHONY: diff
diff:
	diff -s $(PLIK1) $(PLIK2)

.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
	
