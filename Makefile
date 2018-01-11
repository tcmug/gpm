
# GENERICS
ROOT_DIR := $(ROOT)

SDL2_CONFIG = $(ROOT_DIR)/bin/sdl2-config

CC = g++

SYSCFLAGS = -Wall -O3 -pedantic -W -g -Wignored-qualifiers -std=c++11 -arch x86_64
SYSLDFLAGS = -ffunction-sections -fdata-sections -dead_strip -std=c++11 -arch x86_64 -rpath ./dependencies/lib

HEADERS_DIR = $(ROOT_DIR)/include
LIBS_DIR = $(ROOT_DIR)/lib

# LUA CONFIG
LUA_LDFLAGS = -llua -lBulletCollision -lBullet3Common -lBulletDynamics -lLinearMath
LUA_CFLAGS =

# SDL2
SDL2_LDFLAGS = $(shell $(SDL2_CONFIG) --static-libs) $(ROOT)/lib/libSDL2_ttf.a $(ROOT)/lib/libSDL2_mixer.a $(ROOT)/lib/libSDL2_image.a -lfreetype -Wl,-framework,OpenGL
SDL2_CFLAGS =


################################################################################
## BUILD PROCESS
################################################################################

_CFLAGS = $(SYSCFLAGS) $(MYCFLAGS) -I$(HEADERS_DIR) $(SDL2_CFLAGS) -I$(HEADERS_DIR)/bullet $(LUA_CFLAGS)
_LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS) -L$(LIBS_DIR)  $(SDL2_LDFLAGS) -I$(HEADERS_DIR) -I$(HEADERS_DIR)/bullet $(SDL_LDFLAGS) $(LUA_LDFLAGS)

MAIN_SRC := $(wildcard src/*.cpp)
MAIN_OBJ := $(patsubst src/%.cpp,tmp/main_%.o,$(MAIN_SRC))

GFX_SRC := $(wildcard src/gfx/*.cpp)
GFX_OBJ := $(patsubst src/gfx/%.cpp,tmp/gfx_%.o,$(GFX_SRC))

OBJ_SRC := $(wildcard src/obj/*.cpp)
OBJ_OBJ := $(patsubst src/obj/%.cpp,tmp/obj_%.o,$(OBJ_SRC))

LUA_SRC := $(wildcard src/lua/*.cpp)
LUA_OBJ := $(patsubst src/lua/%.cpp,tmp/lua_%.o,$(LUA_SRC))

all: $(MAIN_OBJ) $(GFX_OBJ) $(OBJ_OBJ) $(LUA_OBJ)
	$(CC) -o $@ $^ $(_LDFLAGS)

# Objects.
tmp/main_%.o: src/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

# Objects.
tmp/gfx_%.o: src/gfx/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

# Objects.
tmp/obj_%.o: src/obj/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

# Lua.
tmp/lua_%.o: src/lua/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

# Clean.
clean:
	rm -f gpm docs/* tmp/*


