#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

#include "words.hpp"

WordsStorage::WordsStorage() : m_totalFound(0) {}

WordsStorage::~WordsStorage() {}

void WordsStorage::readInputWords() {
  std::string linebuf;
  std::thread worker(std::thread(&WordsStorage::workerThread, this));

  try {
    do {
      // Get input line. Ignore EOF.
      if (!std::getline(std::cin, linebuf))
        handleCinError();

      // Get first word of the line.
      std::istringstream iss(linebuf);
      iss >> linebuf;

      // Discard empty word or words containing a non-letter character.
      if (linebuf == "" || isAllAlpha(linebuf) == false)
        continue;

      // Wait for idle worker and pass word to worker safely.
      std::lock_guard<std::mutex> lock(m_mtxWord);
      m_words.push(linebuf);
      m_condVar.notify_one(); // Wake up the worker thread

    } while (linebuf != "end");

    // Wait for worker to terminate
    if (worker.joinable())
      worker.join();
  } catch (const std::exception &e) {
    finishWorker(worker);
    throw;
  }
}

void WordsStorage::workerThread() {
  // Keep m_word locked except when worker sleeps.
  std::unique_lock<std::mutex> lock(m_mtxWord);

  std::string newWord;
  std::map<std::string, size_t>::iterator iter;

  for (;;) {
    // Wait for new words
    m_condVar.wait(lock, [this] { return m_words.size(); });

    // Get new word.
    newWord = m_words.front();
    m_words.pop();

    // Exit loop condition.
    if (newWord == "end")
      break;

    // Save new word.
    iter = m_wordsArray.find(newWord);
    if (iter != m_wordsArray.end())
      iter->second++;
    else
      m_wordsArray[newWord] = 1U;
  }
}

void WordsStorage::lookupWords() {
  std::string linebuf;
  std::map<std::string, size_t>::iterator iter;

  for (;;) {
    std::cout << "\nEnter a word for lookup:" << std::flush;
    if (!std::getline(std::cin, linebuf)) { // Get line & check for EOF/error
      handleCinError();
      return;
    }

    // Search for the word
    iter = m_wordsArray.find(linebuf);
    if (iter != m_wordsArray.end()) // word found
    {
      std::cout << "Success: " << linebuf << " was present " << iter->second
                << " times in the initial word list" << std::endl;
      ++m_totalFound;
    } else // word not found
      std::cout << linebuf << " was NOT found in the initial word list"
                << std::endl;
  }
}

void WordsStorage::printWordsList() const {
  std::cout << "\n=== Word list:" << std::endl;
  for (const auto &p : m_wordsArray)
    std::cout << p.first << " " << p.second << std::endl;
}

void WordsStorage::printWordsFound() const {
  std::cout << "\n\n=== Total words found: " << m_totalFound << std::endl;
}

void WordsStorage::handleCinError() const {
  if (std::cin.eof()) {
    clearerr(stdin);
    std::cin.clear();
    //std::cin.ignore();
  } else
    throw std::runtime_error("Failure when reading from std::cin.");
}

void WordsStorage::finishWorker(std::thread &worker) {
  // Pass word "end" to worker safely and wake it up.
  {
    std::lock_guard<std::mutex> lock(m_mtxWord); // Wait for idle worker
    m_words.push("end");
    m_condVar.notify_one(); // Wake up the worker thread
  }

  // Wait for worker to terminate
  if (worker.joinable())
    worker.join();
}

bool WordsStorage::isAllAlpha(const std::string &str) {
  return std::all_of(str.begin(), str.end(),
                     [](unsigned char c) { return std::isalpha(c); });
}
