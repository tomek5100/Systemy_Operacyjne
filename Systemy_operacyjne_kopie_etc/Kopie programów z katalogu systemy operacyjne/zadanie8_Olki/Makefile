#ROZSZEZENIA
.SUFFIXES: .c .o .x .h

#KATALOG
DIR = `basename $(CURDIR)`

#NAZWY PLIKOW
NAME1 = serwer
NAME2 = klient
NAME3 = funkcjek

#PROGRAMY WYKONAWCZE
EXEC1 = $(NAME1).x
EXEC2 = $(NAME2).x

#PLIKI PO KOMPILACJI
OBJS1 = $(NAME1).o
OBJS2 = $(NAME2).o

#BIBLIOTEKI
LIBS1 = $(NAME3).c

#PLIKI NAGLOWKOWE
HEADS1 = $(NAME3).h

# KOMPILATOR I LINKER 
CO = gcc
LD = $(CO)

#FLAGI KOMPILATORA I LINKERA
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC 
LFLAGS = -Wall -std=c99 -pedantic -O

#REGULA WZORCOWA
%.o: %.c
	$(CO) $(CFLAGS) -c $<

#REGULY ZALEZNOSCI
.PHONY: all, run
all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJS1) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^ -lrt
$(EXEC2): $(OBJS2) $(LIBS1)
	$(LD) -o $@ $(LFLAGS) $^ -lrt


#URUCHAMIANIE PROGRAMU
run: $(EXEC1) $(EXEC2)
	xterm -hold -title SERWER -bg Red -e ./$(EXEC1) &
	xterm -hold -title KLIENT1 -bg DodgerBlue1 -e ./$(EXEC2) &
	xterm -hold -title KLIENT2 -bg DodgerBlue1 -e ./$(EXEC2) 

#CLEANER
.PHONY: clean tar
clean: 
	rm -f *.o  *~ *.a *.so *.x core core* a.out;

#ARCHIWIZACJA I KOMPRESJA
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
