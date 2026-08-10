#vibecoding makefiles day 1
CXX = g++
CXXFLAGS = -Wall -O2 -Iinclude
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
TARGET = supercraft

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)