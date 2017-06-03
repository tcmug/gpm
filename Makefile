
# GENERICS
CC = gcc

SYSCFLAGS = -Wall -O3 -mmacosx-version-min=10.7 -arch x86_64
SYSLDFLAGS = -mmacosx-version-min=10.7 -arch x86_64 -ffunction-sections -fdata-sections -Wl -dead_strip
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

# LUA CONFIG
HEADERS_DIR = $(ROOT_DIR)/deps/include
LIBS_DIR = $(ROOT_DIR)/deps/lib

# SPECIFICS
CFLAGS = $(SYSCFLAGS) $(MYCFLAGS) -I$(HEADERS_DIR)
LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS) -L$(LIBS_DIR)

MAIN_SRC := $(wildcard src/*.c)
MAIN_OBJ := $(patsubst src/%.c,tmp/main_%.o,$(MAIN_SRC))

all: mini

# Link
gpm: $(MAIN_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Core stuff
tmp/main_%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Core stuff
clean:
	rm -f alt docs/* tmp/*

mini: gpm
	strip gpm ; SetFile -t APPL gpm

dist: mini
	upx alt


