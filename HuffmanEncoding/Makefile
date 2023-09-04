CC              = clang
CFLAGS          = -Wall -Wextra -Werror -Wstrict-prototypes -pedantic
LFLAGS          = 
EXEC            = huff
HUFF_OBJS      = bitwriter.o node.o pq.o huff.o
PQTEST_OBJS = pq.o node.o pqtest.o 
NODETEST_OBJS = node.o nodetest.o 
BWTEST_OBJS = bwtest.o bitwriter.o
LIBS1= io-$(shell uname -m).a


all: huff pqtest nodetest bwtest

huff: $(HUFF_OBJS) $(LIBS1)
	$(CC) $(LFLAGS) -o huff $(HUFF_OBJS) $(LIBS1)

nodetest: $(NODETEST_OBJS)
	$(CC) $(LFLAGS) -o nodetest $(NODETEST_OBJS)

pqtest: $(PQTEST_OBJS)
	$(CC) $(LFLAGS) -o pqtest $(PQTEST_OBJS)

bwtest: $(BWTEST_OBJS) $(LIBS1)
	$(CC) $(LFLAGS) -o bwtest $(BWTEST_OBJS) $(LIBS1)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f huff bwtest pqtest nodetest $(HUFF_OBJS) $(PQTEST_OBJS) $(NODETEST_OBJS) $(BWTEST_OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make

