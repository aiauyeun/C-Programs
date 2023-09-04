CC              = clang
CFLAGS          = -Wall -Wextra -Werror -Wstrict-prototypes -pedantic
LFLAGS          = 
EXEC            = colorb
COLORB_OBJS      = io.o bmp.o colorb.o
TEST_OBJS = iotest.o io.o

all: colorb iotest

colorb: $(COLORB_OBJS)
	$(CC) $(LFLAGS) -o colorb $(COLORB_OBJS)

iotest: $(TEST_OBJS)
	$(CC) $(LFLAGS) -o iotest $(TEST_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f colorb iotest $(COLORB_OBJS) $(TEST_OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make


