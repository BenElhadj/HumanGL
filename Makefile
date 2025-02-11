# Détection de l'OS
UNAME := $(shell uname)

# Compilateur
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
LIB_GLEW = libs/glew
LIB_GLM = libs/glm  # Correction : GLM se trouve dans libs/glm

# Fichiers sources et objets
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Nom de l'exécutable
EXEC = HumanGL

# Définition des bibliothèques et includes selon l'OS
ifeq ($(UNAME), Linux)
    CXXFLAGS += -I$(LIB_GLEW)/include -I$(LIB_GLM)
	LDFLAGS = -L$(LIB_GLEW)/lib -lGLEW -lGL -lglfw -lpthread -ldl -static-libgcc -static-libstdc++

else
    CXXFLAGS += -I$(LIB_GLEW)/include -I$(LIB_GLM)
    LDFLAGS = -lglew32 -lopengl32 -lglfw3
endif

# Compilation de l'exécutable
$(EXEC): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(EXEC)
