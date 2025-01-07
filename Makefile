CC = g++
VERSION = -std=c++17
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
SDL_TAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_gfx -lSDL2_ttf

all:
	make engine -j
	make test -j

engine:
	$(CC) $(VERSION) -m64 -shared -o $(BIN_DIR)/H2DE.dll $(SRC_DIR)/H2DE_*.cpp -I$(INCLUDE_DIR) -L$(LIB_DIR) -Wl,--out-implib,$(LIB_DIR)/H2DE.lib $(SDL_TAGS)

test:
	$(CC) $(VERSION) -m64 -o $(BIN_DIR)/test $(SRC_DIR)/test.cpp -I$(INCLUDE_DIR) -L$(LIB_DIR) -lmingw32 -lSDL2main $(SDL_TAGS) -lH2DE
	./$(BIN_DIR)/test.exe

run:
	./$(BIN_DIR)/test.exe
