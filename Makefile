NAME= collector

ifeq ($(OS), Windows_NT)
	NAME_EXT=$(NAME).lib
else
	NAME_EXT=lib$(NAME).a
endif

CC= clang
CC_INCLUDES= -Iinclude
CC_FLAGS= -c -static -DDEBUG

AR= llvm-ar
AR_FLAGS= --format=gnu

all:
	$(CC) src/lib.c -o lib/$(NAME).o $(CC_INCLUDES) $(CC_FLAGS)
	$(AR) rcs lib/$(NAME_EXT) lib/$(NAME).o $(AR_FLAGS)