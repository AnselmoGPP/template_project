#include "tests.hpp"
#include "words.hpp"

int main() {
  try {
    tests();
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  std::cin.get();
  return 0;
}
