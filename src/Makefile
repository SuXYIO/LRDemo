CC = gcc
CFLAGS = -Wall
CFLAGSEND = -lm
CDBFLAGS = -g

TARG = LR
DBTARG = LR.db

DEP = $(shell find . -name '*.h')
CCODE = $(shell find . -name '*.c')
DSYM = $(shell find . -name '*.dSYM')
OBJ = $(CCODE:%.c=%.o)

$(TARG): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARG) $(OBJ) $(CFLAGSEND)

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGSEND)

debug: $(CCODE) $(DEP)
	$(CC) $(CDBFLAGS) -o $(DBTARG) $(CCODE) $(CFLAGSEND)

clean: 
	rm -rf $(TARG) $(OBJ) $(DSYM)
