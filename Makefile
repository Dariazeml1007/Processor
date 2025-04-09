# Strict compiler flags (как у вас)
CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef \
         -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations \
         -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ \
         -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual \
         -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security \
         -Wformat=2 -Wignored-qualifiers -Wlogical-op \
         -Wno-missing-field-initializers -Wnon-virtual-dtor \
         -Woverloaded-virtual -Wpointer-arith -Wsign-promo \
         -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel \
         -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG

COMP = g++
BIN_DIR = bin

# Пути для ассемблера
ASM_INCLUDE = asm/include
ASM_SOURCES = asm/source/main_asm.cpp asm/source/label.cpp asm/source/asm.cpp
ASM_SHARED = shared/*.cpp

# Пути для процессора
PROC_INCLUDE = proc/include
PROC_SOURCES = proc/source/main_proc.cpp proc/source/operators.cpp \
               proc/source/process.cpp proc/source/stack.cpp \
               proc/source/read_from_file.cpp

# Основные цели
all: $(BIN_DIR)/asm.exe $(BIN_DIR)/proc.exe

# Сборка ассемблера
$(BIN_DIR)/asm.exe: $(ASM_SOURCES) $(ASM_SHARED)
	@mkdir -p $(BIN_DIR)
	$(COMP) $(CFLAGS) $^ -I $(ASM_INCLUDE) -I shared -o $@

# Сборка процессора
$(BIN_DIR)/proc.exe: $(PROC_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(COMP) $(CFLAGS) $^ -I $(PROC_INCLUDE) -o $@

# Утилиты
obj:
	$(COMP) $(CFLAGS) -c $(ASM_SOURCES) $(PROC_SOURCES) \
	-I $(ASM_INCLUDE) -I $(PROC_INCLUDE) -I shared

clean:
	rm -rf $(BIN_DIR) *.o

run: all
	@if [ -z "$(file)" ]; then \
		echo "Usage: make run file=examples/filename.asm"; \
		exit 1; \
	fi
	$(BIN_DIR)/asm.exe $(file) -o $(BIN_DIR)/program.bin
	$(BIN_DIR)/proc.exe $(BIN_DIR)/program.bin

.PHONY: all obj clean run
