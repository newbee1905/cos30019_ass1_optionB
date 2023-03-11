#include <iostream>

#include "fmt/core.h"
#include "fmt/format.h"

int my_own_func(int a, int b) {
  return a + b;
}

class MyClass {
 private:
  int b;

 public:
  int a;
  MyClass(int a, int b) {
    this->a = a;
    this->b = b;
  }
};

int main(int argc, char** argv) {
  fmt::print("Hello {}\n", "CppTemplate");
  return 0;
}
