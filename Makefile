CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g -O0 -fsanitize=address
LDFLAGS = -fsanitize=address
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Директории
INCLUDE_DIR = include
CLI_DIR = cli
BIN_DIR = bin
TESTS_DIR = tests

# Исходные файлы и цели
SRC = $(CLI_DIR)/main_cli.cpp
TARGET = $(BIN_DIR)/cli.exe

# Тестовые файлы
TEST_SRCS = $(wildcard $(TESTS_DIR)/*.cpp)
TEST_TARGET = $(BIN_DIR)/tests

# Правило по умолчанию — сборка
all: $(TARGET)

# Создание директорий
$(shell mkdir -p $(BIN_DIR))

# Основная сборка
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

# Сборка тестов
$(TEST_TARGET): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Запуск тестов
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Очистка
clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test