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

## 🛠 Система команд
```cpp
enum Command {
    HLT, PUSH, POP, ADD, SUB, MUL, DIV, 
    OUT, IN, CALL, RET, JMP, 
    JA, JB, JAE, JBE, JE, JNE
};

project-root/
├── asm/               # Ассемблер
│   ├── include/       # Заголовочные файлы
│   └── source/        # Исходные коды (.cpp)
├── proc/              # Виртуальная машина
│   ├── include/       # Заголовочные файлы
│   ├── source/        # Ядро процессора
|
├──  stack/             # Реализация стека
└── examples/          # Примеры программ
    ├── factorial.txt  # Вычисление факториала
    └── rec_fac.txt    # Рекурсивный пример
