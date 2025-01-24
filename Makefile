CXX = g++
APP_NAME = H2DE
CPP_VERSION = c++17

BIN_DIR = bin
OBJECT_DIR = $(BIN_DIR)\objects
INCLUDE_DIR = include
LIB_DIR = lib
SRC_DIR = src

CXX_FLAGS = -std=$(CPP_VERSION) -m64 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(APP_NAME)
LD_FLAGS = -L$(LIB_DIR) -lmingw32 $(SDL_FLAGS) -lbase64
SDL_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_gfx
DLL_FLAG = -Wl,--out-implib,$(LIB_DIR)/$(APP_NAME).lib

SRC = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SRC))


all: 
	make engine -j
	make test

engine:
	$(CXX) $(CXX_FLAGS) -shared -o $(BIN_DIR)/$(APP_NAME).dll $(SRC_DIR)/*.cpp $(LD_FLAGS) $(SDL_FLAGS) $(DLL_FLAG)

test:
	$(CXX) $(CXX_FLAGS) -o $(BIN_DIR)/test.exe $(SRC_DIR)/test.cpp $(LD_FLAGS) -lH2DE
	make run

run:
	cd $(BIN_DIR) && test.exe

clean:
	for /r $(OBJECT_DIR) %%f in (*.o) do del /q "%%f"
	del /q $(BIN_DIR)\$(APP_NAME).exe 2>nul || true

rebuild: clean all
