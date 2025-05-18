#include <iostream>

#include "words.hpp"

void fixEOF()
{
  if (std::cin.eof()) {
    clearerr(stdin);
    std::cin.clear();
    //std::cin.ignore();
  }
}

int main2() {
  std::string linebuf;

  do {
      if (!std::getline(std::cin, linebuf))
          fixEOF();
         std::cout << "Received (" << linebuf << ")" << std::endl;
  } while (linebuf != "end");

  std::cin.get();
  return 0;
}

int main() {
  try {
    WordsStorage wordsSet;
    wordsSet.readInputWords();
    wordsSet.printWordsList();
    wordsSet.lookupWords();
    wordsSet.printWordsFound();
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  std::cin.get();
  return 0;
}
