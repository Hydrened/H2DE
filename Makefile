CXX = g++
APP_NAME = H2DE
CPP_VERSION = c++17

BIN_DIR = bin
OBJECT_DIR = $(BIN_DIR)/objects
INCLUDE_DIR = include
LIB_DIR = lib
SRC_DIR = src

CXX_FLAGS = -std=$(CPP_VERSION) -m64 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/$(APP_NAME)
SDL_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_gfx
LD_FLAGS = -L$(LIB_DIR) -lmingw32 $(SDL_FLAGS) -lbase64 -linih
DLL_FLAG = -Wl,--out-implib,$(LIB_DIR)/$(APP_NAME).lib

SRC = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
SRC := $(filter-out $(SRC_DIR)/test.cpp, $(SRC))
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SRC))

all:
	make engine -j
	make test
	make run

engine: $(OBJ)
	$(CXX) $(CXX_FLAGS) -shared -o $(BIN_DIR)/$(APP_NAME).dll $^ $(LD_FLAGS) $(SDL_FLAGS) $(DLL_FLAG)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

test:
	$(CXX) $(CXX_FLAGS) -o $(BIN_DIR)/test.exe $(SRC_DIR)/test.cpp $(LD_FLAGS) -l$(APP_NAME)

run:
	cd $(BIN_DIR) && test.exe

clean:
	if exist "$(OBJECT_DIR)" rmdir /s /q "$(OBJECT_DIR)"
	for /d %%d in ($(SRC_DIR)\*) do if exist %%d mkdir "$(OBJECT_DIR)"/%%~nxd
	if exist $(BIN_DIR)\settings.ini del /f /q $(BIN_DIR)\settings.ini
	if exist $(BIN_DIR)\$(APP_NAME).dll del /f /q $(BIN_DIR)\$(APP_NAME).dll
	if exist $(LIB_DIR)\$(APP_NAME).lib del /f /q $(LIB_DIR)\$(APP_NAME).lib

rebuild: clean all
