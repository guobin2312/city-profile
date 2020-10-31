NAME=$(notdir $(PWD))

#CFLAGS=-g -Wall -Wa,-ahdlcs=$@.lst $(EXTRA_CFLAGS)
CFLAGS=-g -Wall $(EXTRA_CFLAGS)
LDFLAGS=$(EXTRA_LDFLAGS)

all: $(NAME)

$(NAME): main.o svg.o solve.o
	$(CC) -o $@ $(LDFLAGS) $+ $(LOADLIBES) $(LDLIBS)

main.o: Makefile main.c common.h
svg.o: Makefile svg.c common.h
solve.o: Makefile solve.c common.h

clean:
	-rm -f *.o *.lst *~ $(NAME)

.PHONEY: all clean
