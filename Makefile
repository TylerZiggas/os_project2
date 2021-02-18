GCC = gcc
CFLAGS = -g
DEPS = master 
DEPS2 = bin_adder
OBJ = master.o 
OBJ2 = bin_adder.o
.SUFFIXES: .c .o

all: $(DEPS) $(DEPS2)

$(DEPS):$(OBJ)
	$(GCC) -o $@ $(OBJ)

$(DEPS2):$(OBJ2)
	$(GCC) -o $@ $(OBJ2)

.c.o:
	$(GCC) $(CFLAGS) -c $<

clean: 
	/bin/rm -f *.o $(DEPS) $(DEPS2)
