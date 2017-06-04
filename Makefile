
# GENERICS
CC = gcc

SYSCFLAGS = -Wall -O3
SYSLDFLAGS = -arch x86_64 -ffunction-sections -fdata-sections -Wl -dead_strip

# Fails with Windows targets
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

PLATFORMS = "osx, linux, mingw32, mingw64"

HEADERS_DIR = $(ROOT_DIR)/deps/include
LIBS_DIR = $(ROOT_DIR)/deps/lib

CFLAGS = $(SYSCFLAGS) $(MYCFLAGS) -I$(HEADERS_DIR)
LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS) -L$(LIBS_DIR)

MAIN_SRC := $(wildcard src/*.c)
MAIN_OBJ := $(patsubst src/%.c,tmp/main_%.o,$(MAIN_SRC))

all:
	@echo "Define platform: $(PLATFORMS)"

osx:
	$(MAKE) gpm CFLAGS="-mmacosx-version-min=10.7 -arch x86_64" LDFLAGS="-mmacosx-version-min=10.7 -arch x86_64"

linux:
	$(MAKE) gpm CFLAGS="-m64" LDFLAGS="-m64"

mingw32:
	$(MAKE) gpm CC="mingw32-gcc.exe" CFLAGS="" LDFLAGS=""

mingw64:
	$(MAKE) gpm CC="x86_64-w64-mingw64-gcc.exe" CFLAGS="" LDFLAGS=""

# Link
gpm: $(MAIN_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Core stuff
tmp/main_%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Core stuff
clean:
	rm -f alt docs/* tmp/*
