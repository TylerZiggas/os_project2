GCC = gcc -lm
CFLAGS = -Wall -g

MASTER_DEP = master.c
MASTER_OBJ = $(MASTER_DEP:.c=.o)
MASTER = master
BIN_DEP = bin_adder.c
BIN_OBJ = $(BIN_DEP:.c=.o)
BIN_ADDER = bin_adder
HEADER = master.h
OBJ = shared.o

OUTPUT = $(MASTER) $(BIN_ADDER)
all: $(OUTPUT)

$(MASTER): $(MASTER_OBJ) $(OBJ)
	$(GCC) $(CFLAGS) $(MASTER_OBJ) $(OBJ) -o $(MASTER)

$(BIN_ADDER): $(BIN_OBJ) $(OBJ)
	$(GCC) $(CFLAGS) $(BIN_OBJ) $(OBJ) -o $(BIN_ADDER)

%.o: %.c $(HEADER)
	$(GCC) $(CFLAGS) -c $*.c -o $*.o

.PHONY: clean
clean:
	/bin/rm -f $(OUTPUT) *.o  *_log
