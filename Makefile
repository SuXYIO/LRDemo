CC = gcc
TARG = LR.out
DEP = $(shell find . -name '*.h')
CCODE = $(shell find . -name '*.c')
OBJ = $(CCODE:%.c=%.o)

$(TARG): $(OBJ)
	$(CC) -o $(TARG) $(OBJ)

%.o: %.c head.h
	$(CC) -c $< -o $@

clean: 
	rm -f $(TARG) $(OBJ)
