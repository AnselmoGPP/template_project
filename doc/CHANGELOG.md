- Changes in `main()`: All the original code in `main()` was encapsulated in WordsProcessor and can be called through its interface. Now, code is much more readable, organized, easier to use, and easier to modify. 

- General refactoring: Functions and variables have been encapsulated into class `WordsStorage` (in project `words`). The class definition has been placed in a header file. The definitions of its members are in a cpp file. The header is included (`#include`) in the main file. A good class design provides several advantages. Additionally, tests have been created in a separated project (`words_tests`).

  - It show an easy to use interface (just the methods required by the user) while hiding the implementation (the user does not need to care about it). 
  - Encapsulating functions and variables make our code more organized by grouping together related elements, increasing readability, making the system easier to understand, and also easier to modify.
  - There's no good reason for having static objects here. This class removes them and keeps all the state just inside an instance of such class. Now, multiple instances can be created (if desired), no interference between local an static object can exist, and all required state is encapsulated and hidden.
  - The original functions also suffered refactoring inside them (more details below). 

- Using `std::string` instead of `Word`:

Using `std::string` is more convenient than using C-strings. It provides functionality that is useful for us here (`size()`, `clear()`, `operator==`, `operator!=`, `operator=`) and it's easy to use and manage. The member variable `count` is not needed either (we use key-value pairs).

- `s_wordsArray` (`std::vector<Word*>`) replaced with `WordsStorage::m_wordsArray` (`std::map<std::string, size_t>`):

This is more convenient, easier to use, and avoids pointers. There's no need to use pointers (less readable and more error-prone). A `std::map` is always sorted (automatically), it has efficient lookup (O(log N)), and can store word-count (key-value) pairs. This let us forget about using `std::sort` to sort our array, let us look for words more efficiently (better performance), and saves all the data we need to save (word and count).

- `s_word` (`Word s`) replaced with `WordsStorage::m_words` (`std::queue<std::string>`):

This variable works as a staging variable for storing a single word. Using a string is more convenient due to the reasons previously exposed. Furthermore, using a FIFO queue allows to have more than one word waiting for being stored (this is necessary in case `readInputWords` makes more than one loop-iteration at the beginning while the worker thread still doesn't make one).

- `s_totalFound` (`int`) replaced with `WordsStorage::m_totalFound` (`size_t`):

An int can save negative numbers, which makes no sense here. This type was replaced with `size_t` (a high capacity unsigned type).

- Changes in `workerThread` (now, `WordsStorage::workerThread`): Some changes were applied in order to make this method more readable, more efficient, and faster.

  - Originally, this function was running a loop non-stop. Not a very good practice. It's better to make it wait for new input instead of keep looking for input non-stop. This saves a lot of CPU cycles and prevents this thread from becoming greedy (blocking other threads). This can be achieved using a `std::mutex` (`m_mtxWord`) together with a `std::condition_variable` (`m_condVar`), which are used to make this thread sleep (when there's no new word) or wake it up (when there's a new word).
  - The worker's thread and the main thread share a variable: `m_words`. The access to it was not safe. I made it safe using a `std::mutex` (m_mtxWord) as a `lock_guard` and `unique_lock`, which both are exception safe. Now, both threads cannot access `m_words` simultaneously.
  - Instead of allocating memory dynamically to copy there the new word, and updating `s_word.count[0]`, now everything is much simpler: copy the word using `=`, and call `std::queue::pop()` to clear the first word in the queue.
  - No need for a flag (`endEncountered`) for ending the loop. Instead, now the loop ends immediately once word `end` is encountered. Also, making the comparison now is simpler (`==` Vs. `strcmp`).
  - Originally, looking for a word in `m_wordsArray` was complicated (traverse the array comparing words one by one until it was found or the end of the array was encountered) and not efficient (O(n)). Now, we just use `std::map::find` (simpler and faster).
  - The while-loop was replaced with an infinite for-loop. Since there is no need to wait for the beginning of the loop to exit it, it can be left immediately after discovering that the word is `end`, so we don't need a while-loop for checking that condition at the beginning nor checking the same condition multiple times.

- Changes in `readInputWords` (now, `WordsStorage::readInputWords`):

  - try-catch block introduced here for correctly finishing the worker thread and then propagating the exception again. No other methods require their own try-catch block. The try-catch block at `main()` would handle other exceptions.
  - No need for a flag (`endEncountered`) for ending the loop. Instead, now the loop ends if word `end` is encountered. Before ending, this word (like any word) is sent to the worker, which also considers `end` a signal for ending his own loop. Also, making the comparison now is simpler (`!=` Vs. `strcmp`).
  - Everything is wrong with `std::gets()` (old, deprecated, unsafe: buffer overflow risk). Instead, I use `std::getline()` (modern, safe, readable). It can even detect an EOF automatically (if so, method `handleCinError` is called to handle the resulting error).
  - Now, words containing a non-letter character are discarded. Empty words are also discarded.
  - Originally, the new word was passed to the worker in an unsafe way. Now, a `lock_guard` is used to do it safely.
  - Originally, this thread was waiting for the worker to consume the word by running a loop non-stop. As previously stated, this is bad practice, and can make our thread become a greedy thread. Instead, I use a `std::condition_variable` for telling the worker that he can wake up.
  - The `worker.join()` call has been made safer by adding a check for `worker.joinable()`.
  - The while-loop was replaced with a do-while loop, which is more correct (a first iteration is always performed). Also, the condition just compares the word with `end` (originally, it made the comparison and then updated a flag).

- Multithreading system: 

  1. There are 2 threads: main thread (`readInputWords`) and worker thread (`workerThread`).
  2. The worker thread has 2 possible states: awake and asleep. When awake, it loads a word while keeping the mutex locked. When asleep, waits while keeping the mutex unlocked.
  3. The main thread, when tries to pass a new word to the worker (i.e., when it tries to copy a word onto `m_words`), waits in the mutex until the worker unlocks it (i.e., when it falls asleep). When unlocked, it copies the word and tells the worker to wake up (`std::condition_variable::notice_one`).

- Changes in `lookupWords` (now, `WordsStorage::lookupWords`):

  - `std::printf` (old C-style way of printing text) replaced with `std::cout`.
  - `std::scanf` (old and unsafe: buffer overflow risk) replaced with `std::getline`, which is the modern way to get input (as a line of text).
  - `std::strcpy` (old C-style way of copying a string) was replaced with a copy assignment (`=`) from `std::string`. 
  - Searching for the string was originally done by traversing the entire `s_wordsArray` and checking each word (O(n)). Now, we just find the element fast using `std::map::find`, which also makes the code simpler and more readable.

- STD functions replaced:

  - `std::strdup` (old) was removed together with the `Word` class since it was more convenient to use `std::string` and `std::map<std::string, size_t>`.
  - `std::strcmp` (old) was replaced with the use of the `==` and `!=` overloaded operators of `std::string`.
  - `std::gets` (old, deprecated, unsafe: buffer overflow risk) replaced with `std::getline()` (modern, safe, readable), which can also detect an EOF automatically.
  - `std::scanf` (old and unsafe: buffer overflow risk) replaced with `std::getline`.
  - `std::strcpy` (old) replaced with the copy assignment operator (=) of `std::string`.
  - `std::printf` (old) replaced with `std::cout`.
  - `std::sort` removed because `std::map` already saves elements in sorted order. Thus, there's no need to sort.

- Testing: The class `Test_WordsStorage` makes it easy to test `WordsStorage`'s interface. It redirects STDIN and STDOUT in order to provide a way to perform the tests through the use of strings instead of user input, which is necessary for creating automated tests. The tests are executed in the `tests` function. Further tests can easily be added here (or elsewhere).

- Comments and documentation: Comments are in Doxygen format in order to be able to create automatic documentation (with the script `document.py`). More details in the README file.

- Version control system: The system uses Git. More details in the README file.

- Building: The script `build.py` builds the project with `cmake` and `make`. More details in the README file.

- Automatic formatting: Clang-Format is used for implementing LLVM rules, which are enforced by a Git hook right before making a commit. The hook has already been created with the `set_precommit_hook` script. This includes enforcing a maximum line length of 80 characters (recommended by different style guides: LLVM, Google C++, GNU coding standards, Linux kernel), among other things. More details in the README file.

- Project structure: The project structure has been greatly modified to provide a better organization. More details in the README file.

- Specifications: Small additions have been added to the specifications in order to handle special situations. More details in the README file.
