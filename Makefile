CC = gcc
CFLAGS =
CDBFLAGS = -g

TARG = LR.out
DBTARG = LR.db.out

DEP = $(shell find . -name '*.h')
SRC = $(shell find . -name '*.c')
OBJ = $(SRC:%.c=%.o)
MAIN_OBJ = ./main.o
OTHER_OBJ = $(filter-out $(MAIN_OBJ), $(OBJ))

$(TARG): $(OTHER_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $(TARG) $(OTHER_OBJ) $(MAIN_OBJ)

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(SRC) $(DEP)
	$(CC) $(CDBFLAGS) -o $(DBTARG) $(SRC)

clean:
	rm -rf $(OBJ) $(shell find . -name '*.dSYM') $(shell find . -name '*.o') $(shell find . -name '*.out')
