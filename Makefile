CC:=clang
CFLAGS:=-Wall $(shell sdl2-config --cflags)
.PHONY:=all clean eaux
INCLUDE_EAUX:=./eaux/src
INCLUDE_MOSAIC:=./eaux/mosaic/src
LINK_DIRS:=-L./eaux 
LINK:=-leaux -lm
SRC:=$(wildcard src/*.c)
HEADERS:=$(wildcard src/*.h)
SDL2_STATIC_LIBS:=$(shell sdl2-config --static-libs)


all: zero test eaux/libeaux.a

zero: ${SRC} ${HEADERS} eaux/libeaux.a
	${CC} ${CFLAGS} -I${INCLUDE_EAUX} -I${INCLUDE_MOSAIC} ${SRC} -o zero ${LINK_DIRS} ${LINK} ${SDL2_STATIC_LIBS}

eaux/libeaux.a:
	$(MAKE) -C eaux/

clean:
	$(MAKE) -C eaux/ clean
	-- 	rm *.o
	--	rm zero

test: ${SRC} ${HEADERS} eaux/libeaux.a
	--	${CC} ${CFLAGS} -I${INCLUDE_EAUX} -I${INCLUDE_MOSAIC} test/test.c -o test ${LINK_DIRS} ${LINK}
