# Compiler et options
CXX = g++
# CXXFLAGS = -std=c++11 -Wall -Wextra -O3
CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -I/usr/include
LDFLAGS = -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Fichiers sources et objets
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
TARGET = $(BIN_DIR)/HumanGL

# Règles
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ && echo "Compiled $<"

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re