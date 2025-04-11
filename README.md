# 🚀 Annuum

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)
![Status](https://img.shields.io/badge/status-experimental-green.svg)

Annuum is a simple interpreter for a custom scripting language. It provides basic functionality for arithmetic operations, variable assignments, conditional statements, and printing values to the console.

## ✨ Features

- 📝 Simple syntax inspired by JavaScript/C
- 🔢 Support for numeric variables and operations
- 🔀 Basic control flow with if/else statements
- 🖨️ Print function for output
- 🐛 Comprehensive error reporting

## 🛠️ Building and Running

### Prerequisites

- GCC compiler
- Unix-like environment (Linux or macOS)

### Build Process

1. First, build the builder utility:

```bash
gcc b.c -o b
```

2. Run the builder to compile the interpreter:

```bash
./b
```

This will create an executable called `anum` in the current directory.

3. Run the interpreter on a script file:

```bash
./anum
```

By default, it will look for a script file at `/Users/illashisko/Documents/GitHub/Annuum/src/src.txt`. You may need to modify the path in `main.c` to point to your script file.

### ⚠️ Windows Compatibility

The project is primarily designed for Unix-like systems. It may require modifications to work on Windows due to:

- Path handling differences
- Directory separator differences
- Some system-specific headers/functions

## 📚 Language Syntax

### Variables

Variables are dynamically typed and store numeric values:

```
a = 5;
b = 10;
c = a + b * 2;
```

### Arithmetic Operations

Supports basic arithmetic:
- ➕ Addition: `+`
- ➖ Subtraction: `-`
- ✖️ Multiplication: `*`
- ➗ Division: `/`

Operations follow standard order of precedence.

### Output

Use the `print` function to display values:

```
print(c);
```

### Conditional Statements

If/else statements for conditional execution:

```
if (condition) {
    // code executed if condition is true
} else {
    // code executed if condition is false
}
```

Supported comparison operators:
- Equal to: `==`
- Not equal to: `!=`
- Greater than: `>`
- Less than: `<`
- Greater than or equal to: `>=`
- Less than or equal to: `<=`

### Code Blocks

Use curly braces to group statements:

```
{
    statement1;
    statement2;
    // more statements
}
```

### Comments

Single-line comments start with `//`:

```
// This is a comment
a = 5; // This is also a comment
```

## 📝 Example Program

```
// Calculate factorial of a small number
n = 5;
factorial = 1;
i = 1;

if (i <= n) {
    factorial = factorial * i;
    i = i + 1;
    
    if (i <= n) {
        factorial = factorial * i;
        i = i + 1;
    }
}

print(factorial);
```

When you run this program, the output will be:
```
2
```
Note: This is not the complete factorial of 5 (which would be 120) because the implementation only processes the first two iterations.

## ⚡ Run Your Own Code

To run your own code, save it to a file and update the file path in `src/main.c`:

```c
const char* src_path = "/path/to/your/file.txt";
```

Then rebuild and run the interpreter.

## ⚠️ Limitations

- Only supports numeric values (floating-point)
- No string support
- No arrays or complex data structures
- Limited control flow (no loops, only if/else)
- No functions or procedures beyond the built-in `print`
- Limited factorial calculation in the example (only handles the first two multiplications)

## 📂 Project Structure

- `src/arr.c` & `src/arr.h`: Dynamic array implementation
- `src/lexer.c` & `src/lexer.h`: Lexical analyzer
- `src/parser.c` & `src/parser.h`: Parser for the language
- `src/interpreter.c` & `src/interpreter.h`: Interpreter for the AST
- `src/logger.c` & `src/logger.h`: Logging utilities
- `src/main.c`: Entry point
- `b.c` & `b.h`: Custom build system

## 🔨 Builder

The project uses a custom builder from [Cbuilder](https://github.com/DilemaFixer/Cbuilder).

## 📋 Development Roadmap

- [ ] Add support for loops
- [ ] Implement functions
- [ ] Expand standard math library

<div align="center">
  <p>Made with ❤️ by <a href="https://github.com/DilemaFixer">DilemaFixer</a></p>
</div>
