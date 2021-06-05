CC:=gcc
CFLAGS:=-Wall -g -O0 -fsanitize=address
.PHONY:=all clean eaux
INCLUDE_EAUX:=./eaux/src
INCLUDE_MOSAIC:=./eaux/mosaic/src
INCLUDE:=-I./eaux/src -I./eaux/mosaic/src -I./quo/quo
LINK_DIRS_DEBUG:=-L./eaux -L./eaux/mosaic
LINK:=-leaux -lmosaic -lm -lasan

DEBUG_FLAGS:=-g -O0 -fsanitize=address

SRC:=$(wildcard src/*.c)
HEADERS:=$(wildcard src/*.h)


all: zero test eaux/libeaux.a

zero: ${SRC} ${HEADERS} eaux/libeaux.a
	echo "Building zero"
	${CC} ${CFLAGS} ${INCLUDE} ${SRC} -o zero ${LINK_DIRS} ${LINK}

zero_quo: $(SRC) $(HEADERS) eaux/libeaux.a
	echo "Building zero_quo"
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o zero -L./eaux -L./eaux/mosaic -lasan -leaux -lmosaic -lm -lX11 -lGL

eaux/libeaux.a:
	$(MAKE) -C eaux/

clean:
	$(MAKE) -C eaux/ clean
	-- 	rm *.o
	--	rm zero

test: ${SRC} ${HEADERS} eaux/libeaux.a
	--	${CC} ${CFLAGS} -I${INCLUDE_EAUX} -I${INCLUDE_MOSAIC} test/test.c -o test ${LINK_DIRS} ${LINK}
