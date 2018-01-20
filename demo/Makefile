TARGET = sh-demo
SRC = *.c
LIBS=-lSDL2 -framework OpenGL
DLINCLUDE=/usr/local/include/
SDLLIB=/usr/local/lib/
IFLAGS=-I${SDLINCLUDE} -L${SDLLIB}
FLAGS = 
CC = gcc

${TARGET}: ${SRC}
	${CC} ${FLAGS} ${IFLAGS} ${LIBS} $^ -o $@

