# 🚀 Annuum

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)
![Status](https://img.shields.io/badge/status-experimental-green.svg)
![Platform](https://img.shields.io/badge/platform-UNIX_only-red.svg)

Annuum is a simple interpreter for a custom scripting language with mathematical focus. It provides functionality for arithmetic operations, variable assignments, conditional statements, loops, and printing values to the console.

## ✨ Features

- 📝 Simple syntax inspired by JavaScript/C
- 🔢 Support for numeric variables and operations
- 🔀 Control flow with if/else statements and loops
- 🔄 Loop constructs with `stop` and `next` operators
- 🔒 Constant variable declarations with `const`
- 🖨️ Print function for output
- 🐛 Comprehensive error reporting

## 🛠️ Building and Running

### Prerequisites

- GCC compiler
- Unix-like environment (Linux or macOS)
- **Note**: This project is designed exclusively for Unix-like systems due to Cbuilder dependencies

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

By default, it will look for a script file at `/Users/UserName/Documents/GitHub/Annuum/src/src.txt`. You may need to modify the path in `main.c` to point to your script file.

## 📚 Language Syntax

### Variables

Variables are dynamically typed and store numeric values:

```
a = 5;
b = 10;
c = a + b * 2;
```

### Constants

Declare constants that cannot be modified:

```
const PI = 3.14;
const GRAVITY = 9.8;
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

### Loops

Loop statements for repetitive execution:

```
loop (condition) {
    // code executed while condition is true
}
```

Loop control operators:
- `stop` - breaks out of the loop
- `next` - skips to the next iteration

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

## 📝 Example Programs

### Factorial Calculation

```
// Calculate factorial of 5
n = 5;
factorial = 1;
i = 1;

loop(i <= n) {
    factorial = factorial * i;
    i = i + 1;
}

print(factorial);  // Output: 120
```

### Using Loop Control

```
// Print factorials until they exceed 100
n = 10;
factorial = 1;
i = 1;
loop(i <= n) {
    factorial = factorial * i;
    i = i + 1;
    if(factorial > 100){
        stop;  // Break out of the loop
    }
    print(factorial);
}
```

### Using Constants

```
const PI = 3.14;
radius = 5;
area = PI * radius * radius;
print(area);  // Output: 78.5
```

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
- Limited standard library functions

## 📂 Project Structure

- `src/arr.c` & `src/arr.h`: Dynamic array implementation
- `src/lexer.c` & `src/lexer.h`: Lexical analyzer
- `src/parser.c` & `src/parser.h`: Parser for the language
- `src/interpreter.c` & `src/interpreter.h`: Interpreter for the AST
- `src/logger.c` & `src/logger.h`: Logging utilities
- `src/main.c`: Entry point
- `b.c` & `b.h`: Custom build system (Cbuilder)

## 🔨 Builder

The project uses Cbuilder, a custom build system designed for Unix-like environments only.

## 📋 Development Roadmap

- [ ] Add standard math functions (sin, cos, sqrt, etc.)
- [ ] Implement function definitions
- [ ] Add support for strings
- [ ] Create a standard library
- [ ] Improve error messages and debugging

<div align="center">
  <p>Made with ❤️ by <a href="https://github.com/DilemaFixer">DilemaFixer</a></p>
</div>
