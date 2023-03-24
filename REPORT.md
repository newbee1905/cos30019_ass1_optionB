# COS30019 - Assignment 1 Option B

## Instructions

The program has two binary files, one is the program itself and one is for running tests.

You will be able to find search.exe and search_test.exe.

The search.exe file will accept two arguments, which are filename and method, exactly like the requirement.

For the search_test.exe, you only need to run the file. It will print out the number of passed and failed tasks.

## Concept

Instead of making a tree, I use the given grid or maze from the input file. The program surrounds the concept of reusing the grid instead of creating a new array or new grid to contain other information like visited, distance, etc. The only external array I need to create is tracking the parent node, for tracing back the path.

Parent: Contain the direction and where it come from
Cell|Node: A pair of int contain the address in the grid

## Search Algorithms

### DFS

DFS is a search algorithm that does exactly what its name describe. Depth-First Search will go through with the first child until it reaches an end.

Because of that, combined with the given order of directions we must take in the assignment, we can see that the example solution for DFS is super long.

In my submission, I use stack instead of recursion for DFS, so I have to reverse the order of directions to make it similar to the one we must use. To get the same result as the recursive version, I would need to set a cell to visited when I pop it out of the stack. However, in my submission, I optimise the process a bit by setting all the children as visited before pushing into the stack so that it will not need to get to a cell by a roundabout route.

## Research

### Auto convert `enum` to `string_view` at compile time

- Help printing out the result of the directions the robot will take faster.
- Using `constexpr` so all the string is generated at compile time

### `constexpr` map

Based from Cᐩᐩ Weekly With Jason Turner

- Creating a custom map at compile time
- Use linear search since they will be optimised by the compiler and converted to jump table. (Semi tested with gcc)
- Use this with auto generated `string_view` from `enum` for fast checking for the method arguments handler
