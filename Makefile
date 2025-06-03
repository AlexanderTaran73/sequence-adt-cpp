# Компилятор
CXX = g++
# Флаги компиляции (C++17, все предупреждения, оптимизация)
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Директории
INCLUDE_DIR = include
CLI_DIR = cli
BIN_DIR = bin

# Исходный файл и целевой исполняемый файл
SRC = $(CLI_DIR)/main_cli.cpp
TARGET = $(BIN_DIR)/cli.exe

# Правило по умолчанию — сборка
all: $(TARGET)

# Создание директории bin, если её нет
$(shell mkdir -p $(BIN_DIR))

# Компиляция
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Очистка
clean:
	rm -f $(TARGET)

.PHONY: all clean run