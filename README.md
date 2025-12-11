# STM32 Calculator with Keypad & I2C LCD 🧮

This project implements a functional calculator on an **STM32F103 (Bluepill)** microcontroller using the HAL library. It interfaces with a 4x4 Matrix Keypad for input and a 16x2 LCD (via I2C) for output.

## 🌟 Key Features
* **Hardware Interfacing:** Direct GPIO manipulation for Matrix Keypad scanning.
* **Communication Protocol:** I2C communication implementation for LCD display (PCF8574 expander).
* **Expression Parsing:** Supports input buffering strings and parsing multi-digit numbers (Integers & Floats).
* **Error Handling:** Includes protection against "Division by Zero" errors.

## 🛠 Hardware Used
1.  **Microcontroller:** STM32F103C8T6
2.  **Display:** 16x2 LCD with I2C Module (Address: `0x27` or `0x3F`)
3.  **Input:** 4x4 Matrix Keypad
4.  **Debugger:** ST-Link V2

## 🔌 Pin Configuration (Wiring)
| Component | Pin Name | STM32 Pin |
| :--- | :--- | :--- |
| **Keypad Rows** | R1 - R4 | PBx (Check `main.h`) |
| **Keypad Cols** | C1 - C4 | PAx (Check `main.h`) |
| **LCD** | SDA / SCL | PB11 / PB10 (I2C2) |

## 💻 Code Structure
The core logic is located in `Core/Src/main.c`:
* `read_key()`: Scans the keypad rows and columns to detect key presses using GPIO logic.
* `get_next_number()`: Parses the input string buffer to extract operands.
* **Main Loop:** Handles string construction, detects operators (`+`, `-`, `*`, `/`), performs calculations, and updates the LCD.

## 🚀 Future Improvements
* Adding support for parentheses `()` and operator precedence.
* Refactoring the monolithic `main.c` logic into a separate `calculator_logic.c` library.

---
*Developed using STM32CubeIDE & HAL Library.*
