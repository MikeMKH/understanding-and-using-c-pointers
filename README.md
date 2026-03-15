# understanding-and-using-c-pointers

Unit test examples from "Understanding and Using C Pointers"

## Prerequisites

Before setting up this project, ensure you have the following installed:

```bash
clang --version
cmake --version
brew --version

brew install cmake ninja criterion
```

## Project Layout

```
pointers/
├── CMakeLists.txt
├── src/
│   └── chapter02/
│       ├── dynamic_alloc.c
│       └── dynamic_alloc.h
└── tests/
    └── chapter02/
        └── test_dynamic_alloc.c
```

## Building and Running Tests

From the `build` folder, build the project and run the unit tests with verbose output:

```bash
ninja && ./test_dynamic_alloc --verbose
```
