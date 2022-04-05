#ROZSZEZENIA
.SUFFIXES: .c .o .x .h

#KATALOG
DIR = `basename $(CURDIR)`

#NAZWY PLIKOW
NAME1 = main
NAME2 = producent
NAME3 = konsument
NAME4 = libmem
NAME5 = lib

#PROGRAMY WYKONAWCZE
EXEC1s = $(NAME1)s.x
EXEC1d1 = $(NAME1)d1.x
EXEC1d2 = $(NAME1)d2.x
EXEC2s = $(NAME2)s.x
EXEC2d1 = $(NAME2)d1.x
EXEC2d2 = $(NAME2)d2.x
EXEC3s = $(NAME3)s.x
EXEC3d1 = $(NAME3)d1.x
EXEC3d2 = $(NAME3)d2.x

#PLIKI PO KOMPILACJI
OBJS1 = $(NAME1).o
OBJS2 = $(NAME2).o
OBJS3 = $(NAME3).o
OBJS4 = $(NAME4).o
OBJS5 = $(NAME5).o

#PLIKI NAGLOWKOWE
HEADS1 = $(NAME4).h
HEADS2 = $(NAME5).h

#BIBLIOTEKI
LIB_NAME = MojeFunkcje
LIB_DIR = ./lib
LIB_STAT = lib$(LIB_NAME).a
LIB_SHAR = lib$(LIB_NAME).so

LIB_OBJS1 =$(OBJS4)
LIB_OBJS2 =$(OBJS5)


# KOMPILATOR I LINKER 
CO = gcc
LD = $(CO)

#FLAGI KOMPILATORA I LINKERA
CFLAGS = -Wall -std=gnu99 -pedantic -O -fPIC
LFLAGS = -Wall -std=gnu99 -pedantic -O -pthread

AR = ar
ARFLAGS = rsv

#REGULA WZORCOWA
%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
%.o: %.c
	$(CO) $(CFLAGS) -c $<

#REGULY ZALEZNOSCI
.PHONY: all, run1, run2, run3
all: $(EXEC1s) $(EXEC1d1) $(EXEC1d2) $(EXEC2) $(EXEC3)

#BIBLIOTEKA STATYCZNA
$(LIB_STAT): $(LIB_OBJS1) $(LIB_OBJS2)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)

#BIBLIOTEKA DZIELONA
$(LIB_SHAR): $(LIB_OBJS1) $(LIB_OBJS2)
	$(LD) -shared -o $@ $(LFLAGS) $(LIB_OBJS1) $(LIB_OBJS2)


$(EXEC1s): $(EXEC2) $(EXEC3) $(OBJS1) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) -lrt

$(EXEC2s): $(OBJS2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) -lrt

$(EXEC3s): $(OBJS3) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) -lrt

$(EXEC1d1):  $(OBJS1) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) -lrt -Wl,-R $(LIB_DIR)

$(EXEC2d1):  $(OBJS2) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) -lrt -Wl,-R $(LIB_DIR)

$(EXEC3d1):  $(OBJS3) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) -lrt -Wl,-R $(LIB_DIR)

$(EXEC1d2):  $(OBJS1) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) -lrt

$(EXEC2d2):  $(OBJS2) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) -lrt

$(EXEC3d2):  $(OBJS3) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) -lrt

$(OBJS1): $(HEADS1) $(HEADS2)
$(OBJS2): $(HEADS1) $(HEADS2)
$(OBJS3): $(HEADS1) $(HEADS2)


#URUCHAMIANIE PROGRAMU
run1: $(EXEC1s) $(EXEC2s) $(EXEC3s)
	./$(EXEC1s) $(EXEC2s) $(EXEC3s) in.txt out.txt
run2: $(EXEC1d1) $(EXEC2d1) $(EXEC3d1)
	./$(EXEC1d1) $(EXEC2d1) $(EXEC3d1) in.txt out.txt
run3: $(EXEC1d2) $(EXEC2d2) $(EXEC3d2)
	./$(EXEC1d2) $(EXEC2d2) $(EXEC3d2) in.txt out.txt

#POROWNYWANIE ZAWARTOSCI PLIKOW
diff: 
	diff -s in.txt out.txt

#CLEANER
.PHONY: clean tar
clean: 
	rm -f *.o  *~ *.a *.so *.x core core* a.out out.txt; rm -rf ${LIB_DIR}

#ARCHIWIZACJA I KOMPRESJA
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
