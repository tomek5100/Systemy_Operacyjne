.SUFFIXES: .c .o .x .h

DIR = `basename $(CURDIR)`

NAME1 = serwer
NAME2 = klient
NAME3 = libqueue

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

.PHONY: all, run, xterm
all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJS1) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^ -lrt
$(EXEC2): $(OBJS2) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^ -lrt

xterm:
	xterm -hold -title SERWER -bg red -e ./serwer.x &
	sleep 1;
	xterm -hold -title KLIENT1 -bg green -e ./klient.x &
	xterm -hold -title KLIENT2 -bg green -e ./klient.x &

run: $(EXEC1) $(EXEC2) xterm

.PHONY: clean tar
clean: 
	rm -f *.o *.a *.x

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
