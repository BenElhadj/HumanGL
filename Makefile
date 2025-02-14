# Détection OS
UNAME := $(shell uname)

# Compilateur
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -I./libs/glm -I./libs/glew/include

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
EXEC = HumanGL

# Fichiers sources
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Configuration des libs
ifeq ($(UNAME), Linux)
    # Linux/WSL
    LDFLAGS = -lGL -lGLU -lglfw -lGLEW
else
    # Windows (MinGW)
    LDFLAGS = -L./libs/glfw/lib-mingw -lglfw3 -lgdi32 -lglew32 -lopengl32
endif

# Règles de compilation
$(EXEC): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

re: clean $(EXEC)

.PHONY: clean re