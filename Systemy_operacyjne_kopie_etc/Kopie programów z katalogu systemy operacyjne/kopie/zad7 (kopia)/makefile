.SUFFIXES = .c .o .h .x

DIR = `basename $(CURDIR)`

NAME1 = main
NAME2 = producer
NAME3 = consumer
LIB1 = shared
LIB2 = semaphores
MEM = shared
SEM1 = semp
SEM2 = semc
TXT1 = initial
TXT2 = target

OBJ1 = $(NAME1).o
OBJ2 = $(NAME2).o
OBJ3 = $(NAME3).o
LIBO1 = $(LIB1).o
LIBO2 = $(LIB2).o


EXECs =$(NAME1)s.x
EXECd1 =$(NAME1)d1.x
EXECd2 =$(NAME1)d2.x
EXECPR =$(NAME2).x
EXECCO =$(NAME3).x
EXECPR2 =$(NAME2)2.x
EXECCO2 =$(NAME3)2.x


ARG = $(MEM) $(EXECPR) $(EXECCO) $(TXT1) $(SEM1) $(TXT2) $(SEM2)
ARG2 = $(MEM) $(EXECPR2) $(EXECCO2) $(TXT1) $(SEM1) $(TXT2) $(SEM2)


CO=gcc
LD=$(CO)
AR=ar

LIB_NAME = MyFunctions
LIB_DIR = ./lib
LIB_STAT = lib$(LIB_NAME).a
LIB_SHAR = lib$(LIB_NAME).so
LIB_OBJS = $(LIB1) $(LIB2)

HEADS = $(LIB1).h $(LIB2).h

CFLAGS = -pthread -Wall -pedantic -std=c99 -O -fPIC
LFLAGS = -pthread -Wall -std=c99 -pedantic -O
AFLAGS = rsv

%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXECs) $(EXECd1) $(EXECd2)  

$(LIB_STAT): $(LIBO1) $(LIBO2)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)

$(LIB_SHAR): $(LIBO1) $(LIBO2) 
	$(LD) -shared -o $@ $(LFLAGS) $^
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR) $(LIB_DIR)
	
$(EXECs): $(OBJ1) $(LIB_STAT) 
	$(LD) -o $@ $(LFLAGS) $(OBJ1) -L$(LIB_DIR) -l$(LIB_NAME) -lrt
$(EXECd1): $(OBJ1) $(LIB_SHAR) 
	$(LD) -o $@ $(LFLAGS) $(OBJ1) -L$(LIB_DIR) -l$(LIB_NAME) -lrt -Wl,-R $(LIB_DIR)	
$(EXECd2): $(OBJ1) $(LIB_SHAR) 
	$(LD) -o $@ $(LFLAGS) $(OBJ1) -L$(LIB_DIR) -l$(LIB_NAME) -lrt
$(EXECPR): $(OBJ2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJ2) -L$(LIB_DIR) -l$(LIB_NAME) -lrt
$(EXECCO): $(OBJ3) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJ3) -L$(LIB_DIR) -l$(LIB_NAME) -lrt
$(EXECPR2): $(OBJ2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJ2) -L$(LIB_DIR) -l$(LIB_NAME) -lrt -Wl,-R $(LIB_DIR)
$(EXECCO2): $(OBJ3) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJ3) -L$(LIB_DIR) -l$(LIB_NAME) -lrt -Wl,-R $(LIB_DIR)
	
.PHONY: runs rund1 rund2

runs: $(EXECs) $(EXECPR) $(EXECCO)
	./$(EXECs) $(ARG)
	diff $(TXT1) $(TXT2) -s
rund1:$(EXECd1) $(EXECPR2) $(EXECCO2)
	./$(EXECd1) $(ARG2)
	diff $(TXT1) $(TXT2) -s
rund2:$(EXECd2) $(EXECPR) $(EXECCO)
	( export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXECd2) $(ARG))
	diff $(TXT1) $(TXT2) -s

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}


