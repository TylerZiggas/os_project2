GCC = gcc
CFLAGS = -g
DEPS = master
OBJ = master.o
.SUFFIXES: .c .o

$(DEPS):$(OBJ)
	$(GCC) -o $@ $(OBJ)

.c.o:
	$(GCC) $(CFLAGS) -c $<

clean: 
	/bin/rm -f *.o $(DEPS)
