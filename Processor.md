# 🖥️ Гибридный процессор (стек + регистры + RAM)

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus)
![Architecture](https://img.shields.io/badge/Architecture-Hybrid-FF6600)

Проект включает **ассемблер** и **виртуальную машину** для процессора с гибридной архитектурой (стек + регистры + RAM). Поддерживает 18 команд, метки и рекурсию.

## 🔥 Особенности
- **Гибридная модель**: стек (`PUSH/POP`) + регистры (`AX, BX, CX, DX`) + RAM
- **18 инструкций**: арифметика, ветвления, вызовы функций
- **Полный цикл**: ассемблер → бинарный код → исполнение
- **Примеры программ**: факториал, рекурсия, работа с памятью

```cpp
enum Command {
    HLT, PUSH, POP, ADD, SUB, MUL, DIV,
    OUT, IN, CALL, RET, JMP,
    JA, JB, JAE, JBE, JE, JNE
};
```
## 📂 Структура проекта (реальная реализация)

```text
Processor/
├── asm/               # Ассемблер (транслятор)
│   ├── source/        # Исходники (.cpp)
│   │   ├── asm.cpp    # Основная логика
│   │   ├── label.cpp  # Обработка меток
│   │   └── main.cpp   # Точка входа
│   └── Makefile       # Сборка asm.exe
│
├── proc/              # Виртуальная машина
│   ├── source/        # Ядро процессора
│   │   ├── stack.cpp  # Реализация стека
│   │   ├── ops.cpp    # Арифметические операции
│   │   └── main.cpp   # Исполнение программ
│   └── Makefile       # Сборка proc.exe
│
├── stack/             # Отдельный модуль стека
│   ├── stack.cpp      # Реализация стека
│   └── stack.h        # Заголовки
│
└── examples/          # Тестовые программы
    ├── fact.asm       # Пример факториала
    └── rec_fac.txt    # Рекурсивный факториал
