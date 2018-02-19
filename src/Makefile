CC=gcc
CFLAGS=-std=c99
SRC=main/main.c backend/*.c core/*.c
SRC_FULL=main/main.c backend/*.c core/*.c debugger/*.c
INCLUDE=-Iinclude
LIBS=-lSDL2

BUILD_EXEC= ../cdmg
BUILD_FLAGS= -DCDMG_BUILD -O2 -finline-limit=1024

DEBUG_EXEC= ../cdmg_debug
DEBUG_FLAGS= -DCDMG_DEBUG -Wall -Wextra -pedantic -g

all:
	${CC} ${CFLAGS} ${INCLUDE} ${DEBUG_FLAGS} ${SRC} -o ${DEBUG_EXEC} ${LIBS}

debug:
	${CC} ${CFLAGS} ${INCLUDE} ${DEBUG_FLAGS} ${SRC} -o ${DEBUG_EXEC}

build:
	${CC} ${CFLAGS} ${INCLUDE} ${BUILD_FLAGS} ${SRC} -o ${BUILD_EXEC}

clean:
	rm ${BUILD_EXEC} ${DEBUG_EXEC}
.PHONY: clean
