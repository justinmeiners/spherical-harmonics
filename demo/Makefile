TARGET = sh-demo
SRC = *.c
LIBS=-lSDL2 -framework OpenGL
DLINCLUDE=/usr/local/include/
SDLLIB=/usr/local/lib/
IFLAGS=-I${SDLINCLUDE} -L${SDLLIB}
CC = gcc


all: sh-demo approx-cube
	echo "building"

sh-demo: render.c sdl_main.c texture.c mesh.c lin_math.c sh9.c
	${CC} -Os ${IFLAGS} ${LIBS} $^ -o $@

approx-cube: approx_cube.c texture.c sh9.c
	${CC} -Os $^ -o $@

