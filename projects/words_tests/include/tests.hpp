#ifndef TESTS_WORDS_HPP
#define TESTS_WORDS_HPP

#include <sstream>
#include <iostream>

#include "words.hpp"

/// Tests for WordsStorage.
void tests();

/**
	@class InputRedirector
	@brief Redirect std::cin to a string.

	Redirect std::cin from STDIN to a string. This allows to pass input with a
	string instead of with STDIN. Redirection is made during construction.
*/
class InputRedirector
{
public:
	InputRedirector(const std::string& input);

	/// Redirect back std::cin to STDIN.
	void reset();

	/// Get data stored in the input string.
	std::string getInput();

private:
	std::streambuf* originalCin; // Original cin
	std::istringstream newCin; // Redirected cin
};

/**
	@class OutputRedirector
	@brief Redirect std::cout to a string.

	Redirect std::cout from STDOUT to a string. This allows to pass output to a
	string instead of to STDOUT. Redirection is made during construction.
*/
class OutputRedirector
{
public:
	OutputRedirector();

	/// Redirect back std::cin to STDOUT.
	void reset();

	/// Get data stored in the output string.
	std::string inputData();

private:
	std::streambuf* originalCout; // Original cout
	std::ostringstream newCout; // Reirected cout
};

/**
	@class Test_WordsStorage
	@brief Tool for testing the WordsStorage interface.

	Class for creating unit tests for the public methods of WordsStorage.
*/
class Test_WordsStorage
{
public:
	Test_WordsStorage();

	/// Test WordsStorage::readInputWords.
	bool readInputWords(WordsStorage& subject, std::string input);

	/// Test WordsStorage::lookupWords.
	bool lookupWords(WordsStorage& subject, std::string input, size_t expected);

	/// Test WordsStorage::printWordsList.
	bool printWordsList(const WordsStorage& subject, std::string expected);

	/// Test WordsStorage::printWordsFound.
	bool printWordsFound(const WordsStorage& subject, size_t expected);

private:
	size_t testsCount;

	void printOk();
	void printFail();
};

#endif