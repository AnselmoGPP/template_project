#ifndef WORDS_HPP
#define WORDS_HPP

#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <condition_variable>

/**
	@class WordsStorage 
	@brief Store words an their occurrences.

	This class stores words provided by the user through the STDIN. It also
	provides related functionality: The user can check if a certain word exists
	in storage and its number of occurrences. It can print the number of words
	found by the user. It can also print all the words in storage and their
	occurrences.
*/
class WordsStorage
{
public:
	WordsStorage();
	~WordsStorage();

	/**
		@brief Stores words passed by the user through STDIN.

		Read input words from STDIN and pass them to the worker thread for
		inclusion in the word list (storage). Passing the word 'end' will
		terminate the process. Only the first word entered in a line is taken.
		Empty words and words with non-letter characters are discarded. Entering
		EOF will terminate with an exception.
	*/
	void readInputWords();

	/**
		@brief Allows the user search for words in storage and its occurrences.

		Repeatedly ask the user for a word and check whether it is present in
		storage. Entering EOF (Ctrl+D on Linux, Ctrl+Z on Windows) will
		terminate the process.
	*/
	void lookupWords();
        
        /**
          @brief Print all words sorted together with the number of occurrences.
        
          Print all words sorted together with the number of occurrences. Sorting is done
          alphabetically. First, uppercase letters; second, lowercase letters.
        */
	void printWordsList() const;

	/// Print the number of words found in lookupWords().
	void printWordsFound() const;

private:
	std::map<std::string, size_t> m_wordsArray; /// Words and occurrences
	std::queue<std::string> m_words; /// Shared staging variable for new words
	size_t m_totalFound; /// Total number of words found in 'lookupWords()'

	std::mutex m_mtxWord; /// Controls access to 'm_word'
	std::condition_variable m_condVar; /// Wake up/sleep the worker thread

	/**
		@brief Worker thread for saving words in storage.

		Thread that gets a word (temporarily stored in 'm_word') and saves it
		in storage ('m_wordsArray'). Encountering the word 'end' will terminate
		the process.
	*/
	void workerThread();

	/**
		@brief Handle cin errors like EOF.
	
		When std::cin reaches EOF it enters a fail state, which has to be
		cleared; otherwise, subsequent input operations won't work. If it
		doesn't return an exception, it's an EOF error.
	*/
	void handleCinError() const;

	/// Finish worker thread safely.
	void finishWorker(std::thread& worker);
	
	/// Check whether a string only contains letters (alpha characters).
	bool isAllAlpha(const std::string& str);
};

#endif
