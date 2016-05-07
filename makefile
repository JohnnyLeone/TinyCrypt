CFLAGS=-O2 -Wall -Wno-unused-function
DEV_CFLAGS=-g -Wall -Wno-unused-function

all: tinycrypt microcrypt testARC4
	./testARC4

dev:
	@echo "making development build:"
	@$(MAKE) -f makefile all CFLAGS="$(DEV_CFLAGS)"

tinycrypt: tinycrypt.o minilzo/minilzo.o
	gcc $(CFLAGS) tinycrypt.o minilzo/minilzo.o -o tinycrypt

tinycrypt.o: tinycrypt.c encrypt.c minilzo/minilzo.h
	gcc -c $(CFLAGS) tinycrypt.c -o tinycrypt.o

microcrypt: microcrypt.c encrypt.c
	gcc $(CFLAGS) microcrypt.c encrypt.c -o microcrypt

minilzo/minilzo.o: minilzo/lzoconf.h minilzo/lzodefs.h minilzo/minilzo.c minilzo/minilzo.h
	gcc -c $(CFLAGS) minilzo/minilzo.c -o minilzo/minilzo.o

testARC4: testARC4.o
	gcc $(CFLAGS) testARC4.o -o testARC4

testARC4.o: testARC4.c encrypt.c
	gcc -c $(CFLAGS) testARC4.c -o testARC4.o

clean:
	rm -f *.o
