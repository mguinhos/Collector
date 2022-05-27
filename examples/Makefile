NAME= libcollector

ifeq ($(OS), Windows_NT)
	NAME_EXT=$(NAME).exe
else
	NAME_EXT=$(NAME).elf
endif

CC= clang
CC_INCLUDES= -I../include
CC_LIBS= -L../lib -lcollector

all:
	$(CC) src/libcollector.c -o bin/$(NAME_EXT) $(CC_INCLUDES) $(CC_LIBS)

run:
	./bin/$(NAME_EXT)