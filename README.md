# SIMPLEX Assembler & Virtual Machine Emulator

A custom two-pass assembler and interactive CPU emulator for the 32-bit SIMPLEX architecture. Built entirely in C++, this project translates human-readable assembly language into raw machine code and executes it within a simulated memory environment.

##  Features

### The Assembler (`asm.cpp`)
* **Optimized Internal Form:** Uses a highly efficient "One Read" architecture. It parses the source file once into a C++ `std::map` (Internal Form), saving massive File I/O overhead during Pass 2.
* **Robust Error Handling:** Detects syntax errors, duplicate labels, missing/unexpected operands, and strictly enforces alphanumeric label constraints. Logs all errors to a `.log` file.
* **Flexible Number Parsing:** Supports decimal, hexadecimal (`0x`), and octal (`0`) operand inputs seamlessly via `strtol`.
* **Binary & Listing Generation:** Outputs a pure binary object file (`.o`) for the emulator and a beautifully aligned, human-readable listing file (`.lst`) for debugging.

### The Emulator (`emu.cpp`)
* **Interactive CLI:** A fully interactive command-line interface to control the virtual CPU state.
* **Lightning-Fast Decoding:** Uses bitwise shifting (`>> 8` and `& 0xFF`) rather than string slicing to decode the 32-bit instruction formats instantly.
* **Advanced Memory Tracing:** Features `-read` and `-write` trace modes to actively monitor and log real-time array sorting and memory state changes.
* **Safe Memory Management:** Simulates a full 16MB (24-bit) address space securely on the heap using `std::vector` to prevent segmentation faults.

---

##  Build Instructions

This project requires a standard C++ compiler (like `g++`).

**1. Compile the Assembler:**
`g++ asm.cpp -o asm`

**2. Compile the Emulator:**
`g++ emu.cpp -o emu`

---

##  Usage Guide

### Step 1: Assemble the Code
Run the assembler on your `.asm` source file. 
`./asm program.asm`

**Outputs Generated:**
* `program.o` : The raw binary machine code.
* `program.lst` : The formatted text file showing memory addresses, hex machine code, and original instructions.
* `program.log` : The error log (if compilation fails) or a success message.

### Step 2: Run the Emulator
Launch the virtual machine and load your `.o` file:
`./emu`
*(When prompted, enter your object file name: program.o)*

**Emulator Commands:**
* `-run` : Execute the entire program silently until a `HALT` instruction.
* `-t` : Step through the program one single instruction at a time (Trace mode).
* `-reg` : Display the current state of the A, B, SP, and PC registers.
* `-dump` : Dump the entire active memory state to the terminal in formatted hexadecimal.
* `-read` : Run the program and specifically log every time the CPU loads data from memory.
* `-write` : Run the program and specifically log every time the CPU stores/overwrites data in memory.
* `-isa` : Display the instruction set architecture reference.
* `-quit` : Power off the virtual machine.

---

##  SIMPLEX Instruction Set Architecture (ISA)

The SIMPLEX CPU is an accumulator-based machine featuring two primary registers (`A` and `B`), a Program Counter (`PC`), and a Stack Pointer (`SP`). 

Instructions are 32-bit: the lowest 8 bits are the opcode, and the upper 24 bits are the operand.

| Opcode | Mnemonic | Operand | Description |
|--------|----------|---------|-------------|
| 0      | `ldc`    | value   | Load constant into A |
| 1      | `adc`    | value   | Add constant to A |
| 2      | `ldl`    | offset  | Load local (from SP + offset) into A |
| 3      | `stl`    | offset  | Store local (A to SP + offset) |
| 4      | `ldnl`   | offset  | Load non-local (from A + offset) into A |
| 5      | `stnl`   | offset  | Store non-local (B to A + offset) |
| 6      | `add`    | none    | A = B + A |
| 7      | `sub`    | none    | A = B - A |
| 8      | `shl`    | none    | A = B << A |
| 9      | `shr`    | none    | A = B >> A |
| 10     | `adj`    | value   | Adjust Stack Pointer (SP = SP + value) |
| 11     | `a2sp`   | none    | SP = A |
| 12     | `sp2a`   | none    | A = SP |
| 13     | `call`   | offset  | PC = PC + offset, saves return address to A |
| 14     | `return` | none    | PC = A |
| 15     | `brz`    | offset  | Branch to PC + offset if A == 0 |
| 16     | `brlz`   | offset  | Branch to PC + offset if A < 0 |
| 17     | `br`     | offset  | Unconditional branch to PC + offset |
| 18     | `HALT`   | none    | Stop execution |

> **Note on Syntax:** Labels must be strictly alphanumeric and begin with a letter. Directives include `data` (to reserve/initialize memory) and `SET` (to force a label to a specific value).

---

##  Example Programs

Included in this repository are sample `.asm` files to test the system:
* **`bubble_sort.asm`**: An in-place Bubble Sort algorithm that sorts an array of integers. Best viewed using the `-write` emulator command to watch the array swap in real-time.

* **`test4.asm`**: A deep recursive algorithm that calculates the Binomial Coefficients (a row of Pascal's Triangle) and stores them sequentially in memory. Tests the stack pointer and `call`/`return` mechanics extensively.# Two-Pass-Assembler
