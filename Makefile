CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Iinclude -std=c++20

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

TARGET = slrparser

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

format:
	@find . -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i

.PHONY: all clean fclean format
