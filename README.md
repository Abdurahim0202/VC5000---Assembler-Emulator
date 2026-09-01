# Duck2200 Assembler & Emulator

[![C++](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)]()
[![IDE](https://img.shields.io/badge/IDE-Visual%20Studio-purple.svg)]()
[![Status](https://img.shields.io/badge/Status-Work%20in%20Progress-yellow.svg)]()

A **two-pass assembler** and **emulator** for the Duck2200, a hypothetical computer architecture. This project demonstrates fundamental compiler design concepts, including lexical analysis, symbol table management, error handling, and machine code generation.

---

## 📋 Table of Contents
- [Overview](#-overview)
- [Features](#-features)
- [Architecture](#-architecture)
- [Instruction Set](#-instruction-set)
- [Project Structure](#-project-structure)
- [Building the Project](#-building-the-project)
- [Usage](#-usage)
- [Example Program](#-example-program)
- [Project Status](#-project-status)
- [Error Handling](#-error-handling)

---

## 🔍 Overview

The Duck2200 Assembler is an assembler that translates Duck2200 assembly language programs into machine code and executes them on a built-in emulator. The project implements a classic **two-pass assembly** process:

1. **Pass I** — Scans the source code to build a symbol table with label addresses.
2. **Pass II** — Generates machine code using the symbol table and reports errors *(in progress)*.

The emulator then executes the generated machine code, simulating the Duck2200's behavior with a 100,000-word memory and an accumulator-based architecture.

---

## ✨ Features

- **Two-Pass Assembly** — Label resolution using a dedicated symbol table.
- **Symbol Table Management** — Backed by `std::map` for ordered symbol lookup and reporting.
- **Comment & Whitespace Handling** — Strips inline comments (`;`) before parsing each line.
- **Built-in Emulator** — Loads generated instructions into simulated memory and executes them.
- **Modular Architecture** — Clear separation between file access, parsing, symbol table, and emulation.
- **Centralized Error Reporting** — Static `Errors` class for collecting and displaying messages across the assembler.

---

## 🏗️ Architecture

### Duck2200 Machine Specifications

| Component          | Specification                                |
| ------------------- | -------------------------------------------- |
| Memory              | 100,000 words (addresses 0–99,999)           |
| Word Size           | 6 decimal digits                             |
| Accumulator         | Single accumulator for arithmetic            |
| Instruction Format  | OOAAAAA (OO = opcode, AAAAA = operand/address) |

### System Components

```
┌──────────────────────────────────────────────────────────────┐
│                          Assembler                            │
│  ┌──────────────┐  ┌──────────────┐  ┌───────────────────┐    │
│  │  FileAccess  │──│  Instruction │──│    SymbolTable     │   │
│  │   (Reader)   │  │   (Parser)   │  │  (Label Storage)   │   │
│  └──────────────┘  └──────────────┘  └───────────────────┘    │
│          │                │                    │              │
│          └────────────────┼────────────────────┘              │
│                            ▼                                  │
│                     ┌──────────────┐                          │
│                     │   Emulator   │                          │
│                     │  (Executor)  │                          │
│                     └──────────────┘                          │
└──────────────────────────────────────────────────────────────┘
```

---

## 📖 Instruction Set

### Machine Language Instructions

| Opcode | Mnemonic | Description                            |
| ------ | -------- | --------------------------------------- |
| 01     | `ADD`    | Add memory contents to accumulator      |
| 02     | `SUB`    | Subtract memory contents from accumulator |
| 03     | `MULT`   | Multiply accumulator by memory contents |
| 04     | `DIV`    | Divide accumulator by memory contents   |
| 05     | `LOAD`   | Load memory contents into accumulator   |
| 06     | `STORE`  | Store accumulator contents to memory    |
| 07     | `READ`   | Read input into memory location         |
| 08     | `WRITE`  | Write memory contents to output         |
| 09     | `B`      | Unconditional branch                    |
| 12     | `BP`     | Branch if accumulator is positive       |
| 13     | `HALT`   | Terminate program execution             |

### Assembler Directives

| Directive | Description                                       |
| --------- | -------------------------------------------------- |
| `ORG`     | Origin — set the starting location counter          |
| `DC`      | Define Constant — allocate and initialize memory     |
| `DS`      | Define Storage — reserve uninitialized memory        |
| `END`     | Marks the end of the source program                  |

---

## 📁 Project Structure

```
Duck2200-Assembler/
├── Assem.cpp             # Main entry point
├── Assembler.cpp         # Assembler class — drives Pass I & Pass II
├── Assembler.h           # Assembler class definition
├── FileAccess.cpp        # Source file reader
├── FileAccess.h          # File access interface
├── Instruction.h         # Instruction parsing / lexing
├── SymTab.cpp            # Symbol table implementation
├── SymTab.h              # Symbol table interface
├── Emulator.h            # Duck2200 machine emulator
├── Errors.h              # Centralized error reporting
└── stdafx.h / stdafx.cpp # Precompiled header
```

---

## 🔧 Building the Project

### Prerequisites
- **Visual Studio 2019** (or later) with a C++ desktop development workload
- **Windows SDK**

### Build Steps
1. Clone the repository:
   ```
   git clone https://github.com/<your-username>/<your-repo-name>.git
   ```
2. Open the solution in Visual Studio.
3. Build the solution (**F7** or **Build → Build Solution**).

---

## 🚀 Usage

### Command Line
```
Assem.exe <source_file.asm>
```

### Output
The assembler produces:
1. **Symbol Table** — every label and its resolved memory address.
2. **Translation Listing** — machine code paired with the original source line.
3. **Program Output** — results produced by running the assembled program on the emulator.

---

## 📝 Example Program

### Sample Assembly Code
```
;this is a test
        org    100
hi     read    x;comment immediately after statement
        load    x
hay   store   y ; This is the another comment.
          write    x
        bp      hi
        halt
;test
x      dc      5
y      ds      99
b      dc      555
a      dc      100
        end
```

### Sample Symbol Table Output
```
Symbol Table:
Symbol #     Symbol     Location
   0                 a              207
   1                 b              206
   2                 hay           102
   3                 hi              100
   4                 x               106
   5                 y               107
```

### Sample Translation Output
```
Location    Contents    Original Statement
                                    ;this is a test
    0                                        org      100
  100          070106     hi      read     x;comment immediately after statement
  101          050106              load     x
  102          060107     hay   store    y ; This is the another comment.
  103          080106              write    x
  104          120100              bp       hi
  105          130000              halt
                                    ;test
  106          000005     x       dc       5
  107                             y       ds       99
  206          000555     b       dc       555
  207          000100     a       dc       100
                                              end
```

---

## 🚧 Project Status

This project is under active development as part of a software design course. Current state:

- ✅ Pass I (label/location resolution) is implemented.
- 🔲 Pass II (machine code generation) is stubbed out and not yet wired into `main`.
- 🔲 `Instruction::ParseInstruction` needs full lexing/parsing logic for labels, opcodes, and operands.
- 🔲 `SymbolTable::DisplaySymbolTable` needs to print the table shown above.
- 🔲 The emulator's arithmetic instructions (`ADD`, `SUB`, `MULT`, `DIV`) need to actually update the accumulator.

---

## ⚠️ Error Handling

Errors are collected centrally through the `Errors` class so they can be reported anywhere in the assembler:

| Capability              | Description                                   |
| ------------------------ | ---------------------------------------------- |
| `InitErrorReporting()`  | Clears any previously recorded error messages  |
| `RecordError(msg)`      | Records a new error message                    |
| `WasThereErrors()`      | Reports whether any errors have been recorded  |
| `DisplayErrors()`       | Prints and clears the collected error messages |

Planned error categories include invalid opcodes, undefined/duplicate labels, malformed operands, and out-of-range memory references.

---

## 📄 License

No license has been specified yet for this project. Add a `LICENSE` file to make its usage terms clear to others.

---

## 👤 Author

**Your Name**
- GitHub: [@your-username](https://github.com/your-username)

---

## 🙏 Acknowledgments

- Built as part of a Software Design course assignment on assemblers and emulators.
