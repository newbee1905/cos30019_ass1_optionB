# COS30019 - Assignment 1 Option B

## Research

### Auto convert `enum` to `string_view` at compile time

- Help printing out the result of the directions the robot will take faster.
- Using `constexpr` so all the string is generated at compile time

### `constexpr` map

Based from Cᐩᐩ Weekly With Jason Turner

- Creating a custom map at compile time
- Use linear search since they will be optimised by the compiler and converted to jump table. (Semi tested with gcc)
- Use this with auto generated `string_view` from `enum` for fast checking for the method arguments handler
