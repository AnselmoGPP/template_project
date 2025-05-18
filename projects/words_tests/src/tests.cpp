#include "tests.hpp"

InputRedirector::InputRedirector(const std::string &input)
    : originalCin(std::cin.rdbuf()), newCin(input) {
  std::cin.rdbuf(newCin.rdbuf()); // Redirect std::cin
}

void InputRedirector::reset() { std::cin.rdbuf(originalCin); }

std::string InputRedirector::getInput() { return newCin.str(); }

OutputRedirector::OutputRedirector() : originalCout(std::cout.rdbuf()) {
  std::cout.rdbuf(newCout.rdbuf()); // Redirect std::cout
}

void OutputRedirector::reset() { std::cout.rdbuf(originalCout); }

std::string OutputRedirector::inputData() { return newCout.str(); }

Test_WordsStorage::Test_WordsStorage() : testsCount(0) {}

bool Test_WordsStorage::readInputWords(WordsStorage &subject,
                                       std::string input) {
  testsCount++;

  // Arrange (setup)
  InputRedirector inputDir(input);

  // Act (execution)
  try {
    subject.readInputWords();
    inputDir.reset();
  }

  // Assert (verification)
  catch (const std::exception &e) {
    printFail();
    return true;
  }

  printOk();
  return false;
}

bool Test_WordsStorage::lookupWords(WordsStorage &subject, std::string input,
                                    size_t expected) {
  testsCount++;

  // Arrange (setup)
  InputRedirector inputDir(input);
  OutputRedirector outputDir;

  // Act (execution)
  subject.lookupWords();
  inputDir.reset();
  outputDir.reset();

  // Assert (verification)
  std::string expectedOutput;
  if (expected)
    expectedOutput =
        "\nEnter a word for lookup:Success: " + input + " was present " +
        std::to_string(expected) +
        " times in the initial word list\n\nEnter a word for lookup:";
  else if (input.size())
    expectedOutput = "\nEnter a word for lookup:" + input +
                     " was NOT found in the initial word list\n" +
                     "\nEnter a word for lookup:";
  else
    expectedOutput = "\nEnter a word for lookup:";

  if (outputDir.inputData() == expectedOutput) {
    printOk();
    return false;
  } else {
    printFail();
    return true;
  }
}

bool Test_WordsStorage::printWordsList(const WordsStorage &subject,
                                       std::string expected) {
  testsCount++;

  // Arrange (setup)
  OutputRedirector outputDir;

  // Act (execution)
  subject.printWordsList();
  outputDir.reset();

  // Assert (verification)
  if (outputDir.inputData() == expected) {
    printOk();
    return false;
  } else {
    printFail();
    return true;
  }
}

bool Test_WordsStorage::printWordsFound(const WordsStorage &subject,
                                        size_t expected) {
  testsCount++;

  // Arrange (setup)
  OutputRedirector outputDir;

  // Act (execution)
  subject.printWordsFound();
  outputDir.reset();

  // Assert (verification)
  if (outputDir.inputData() ==
      ("\n\n=== Total words found: " + std::to_string(expected) + "\n")) {
    printOk();
    return false;
  } else {
    printFail();
    return true;
  }
}

void Test_WordsStorage::printOk() {
  std::cout << "OK - Test " << testsCount << std::endl;
}

void Test_WordsStorage::printFail() {
  std::cout << "FAIL - Test " << testsCount << std::endl;
}

void tests() {
  Test_WordsStorage test;
  WordsStorage wordsSet, wordsSet_empty;

  std::cout << "Testing WordsStorage::readInputWords():" << std::endl;

  test.readInputWords(wordsSet, "end");
  test.readInputWords(wordsSet, "sword\nend");
  test.readInputWords(wordsSet, "sword\nbow\nhelmet\nend");
  test.readInputWords(wordsSet, "sword2\nbow sword\nshield helmet\nend");
  test.readInputWords(wordsSet, "\nsword\n\nhelmet shie1d\n\n\nend\n\n");

  std::cout << "Testing WordsStorage::printWordsList():" << std::endl;

  test.printWordsList(wordsSet_empty, "\n=== Word list:\n");
  test.printWordsList(wordsSet,
                      "\n=== Word list:\nbow 2\nhelmet 2\nshield 1\nsword 3\n");

  std::cout << "Testing WordsStorage::lookupWords():" << std::endl;

  test.lookupWords(wordsSet, "", 0);
  test.lookupWords(wordsSet, "xyz", 0);
  test.lookupWords(wordsSet, "sword", 3);
  test.lookupWords(wordsSet, "sword2", 0);
  test.lookupWords(wordsSet, "bow sword", 0);

  std::cout << "Testing WordsStorage::printWordsFound():" << std::endl;

  test.printWordsFound(wordsSet_empty, 0);
  test.printWordsFound(wordsSet, 1);

  std::cout << "Integration tests:" << std::endl;

  WordsStorage wordsSet_1;
  test.readInputWords(wordsSet_1, "end");
  test.printWordsList(wordsSet_1, "\n=== Word list:\n");
  test.lookupWords(wordsSet_1, "sword", 0);
  test.lookupWords(wordsSet_1, "bow", 0);
  test.printWordsFound(wordsSet_1, 0);

  WordsStorage wordsSet_2;
  test.readInputWords(wordsSet_2, "sword\nbow\nsword\nbow\nshield\nbow\nend");
  test.printWordsList(wordsSet_2,
                      "\n=== Word list:\nbow 3\nshield 1\nsword 2\n");
  test.lookupWords(wordsSet_2, "bow", 3);
  test.lookupWords(wordsSet_2, "sword", 2);
  test.lookupWords(wordsSet_2, "shield", 1);
  test.lookupWords(wordsSet_2, "hammer", 0);
  test.printWordsFound(wordsSet_2, 3);

  WordsStorage wordsSet_3;
  test.readInputWords(wordsSet_3, "sword2\nbow\nsword\n5bow\nsh1eld\nbow\nend");
  test.printWordsList(wordsSet_3, "\n=== Word list:\nbow 2\nsword 1\n");
  test.lookupWords(wordsSet_3, "bow", 2);
  test.lookupWords(wordsSet_3, "sword2", 0);
  test.printWordsFound(wordsSet_3, 1);

  WordsStorage wordsSet_4;
  test.readInputWords(wordsSet_4,
                      "sword\n\nsword 5\nsword bow\n\n\nbow bow\nbow\nend");
  test.printWordsList(wordsSet_4, "\n=== Word list:\nbow 2\nsword 3\n");
  test.lookupWords(wordsSet_4, "sword bow", 0);
  test.lookupWords(wordsSet_4, "sword 5", 0);
  test.lookupWords(wordsSet_4, "bow bow", 0);
  test.lookupWords(wordsSet_4, "bow", 2);
  test.printWordsFound(wordsSet_4, 1);

  std::cout << "----------------------------------------" << std::endl;
}
