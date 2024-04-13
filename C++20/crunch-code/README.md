# Crunch Code

Crunch Code is a lightweight C++ command-line tool designed to strip comments and unnecessary whitespace from C++ source files. I used this program to create source files to use in videos of compact scrolling code.

## Features

- Remove multiline and single-line comments.
- Condense spaces, skipping unnecessary whitespace.
- Handle multiple files in a single command.

## Getting Started

### Prerequisites

- C++ compiler (GCC or Clang) with C++20 support
- Make (optional for building)

### Building

You can build the project using a C++ compiler such as g++. Here's how you can compile it:

```bash
make
sudo make install
```

### Usage

To use Crunch Code, you can run the executable with the source files as arguments. Here's how to use it from the command line:

```bash
./crunch-code file1.cpp file2.cpp
```

If no files are specified, the program will read from standard input:

```bash
./crunch-code < input.cpp
```

