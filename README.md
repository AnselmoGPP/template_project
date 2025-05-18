# Template project

## Table of Contents
+ [Project structure](#project-structure)
+ [Building the project](#building-the-project)
+ [Specifications](#specifications)
+ [Documentation](#documentation)
+ [Version control](#version-control)
+ [Code formatting](#code-formatting)
+ [Resources](#resources)

## Project structure

The project contains three folders:

- **projects**: It contains the source code (hpp and cpp files), CMakefiles, and doxyfiles of each project. There are two projects:

  - **words**: Original code fixed and greatly improved.
  - **words_tests**: Tests for the Woven project (unit tests and integration tests).

- **scripts**: Useful scripts with different purposes:

  - **build.py**: Build (out-of-source) all the projects easily.
  - **document.py**: Create documentation automatically.
  - **set_precommit_hook.py**: Set a Git precommit hook to enforce automatic formatting. Currently, the hook is active.

- **doc**:

  - **CHANGELOG.md**: Log that documents the changes applied to the original project.
  - **Specifications** of the coding challenge.
  
- **_BUILD**: This folder doesn't exist initially, but it's generated after running the script `build.py`, and contains all the build files (binaries, make files, etc.).

- **.git**: Hidden folder that contains all the Git files used for version control.

## Building the project

Build the projects with the **`build.py`** script. Execute it from `/scripts` with `python3 build.py` or `sudo ./build.py`. It will generate one binary (executable) per project. This script requires `python3`, `cmake`, `make` and `g++`, which can be installed with:

```
sudo apt update
sudo apt install -y python3
sudo apt install -y cmake
sudo apt install -y build-essential
```

Each generated binary is in `_BUILD/<project_name>` and can be executed from there with `./<binary_name>`. Since there are two projects, two binaries will be generated: `words` (main project) and `words_tests` (it performs tests). Tests can be added or modified by modifying the `tests()` function in file "tests.cpp".

## Specifications

Final workflow model:

1. It reads a list of words from STDIN, one per line (i.e., only the first word entered in a line is taken), terminated by the word 'end'. Empty words and words with non-letter characters are discarded. Entering EOF will terminate with an exception.
2. It prints each individual word in alphabetical order, one per line with their number of occurrences. First, uppercase letters; second, lowercase letters.
3. It repeatedly asks the user to enter a word and looks it up in the list of user entries. If it finds the word, it prints its number of occurrences. Otherwise it prints an error message. Empty words and words with non-letter characters are not discarded.
4. It terminates when it encounters EOF. 

## Documentation

Create the documentation automatically with the `document.py` script. Execute it from `/scripts` with `python3 document.py` or `sudo ./document.py`. It will generate documentation in html format in the `_BUILD/docs` directory. This requires `doxygen`, which can be installed with:

```
sudo apt update
sudo apt install -y doxygen
```

## Version control

**Git** is the VCS (Version Control System) used. The git files are in the `.git` folder, except for the `.gitignore` files, which are wherever they're useful. This requires `git`, which can be installed with:

```
sudo apt update
sudo apt install -y git
```

The development process is controlled through the use of commits, which record the most relevant phases of the development process. The `.gitignore` files are used for ignoring certain files and directories, preventing them from being tracked by Git. Some useful commands are:

- `git init`: Create a new Git repository.
- `git log`: Shows commit history of the Git repository.
- `git status`: Check current status of the working directory and staging area.
- `git add --all`: Stage all changes in the working directory. Use `git add filename` for specific files.
- `git commit - m"comment"`: Save staged changes along with descriptive message.
- `git clone`: Copy a remote repository to a local machine.
- `git push`: Send committed changes from a local to a remote Git repository.

## Code formatting

**Clang-Format** is the tool used for automating code formatting and style. The `.clang-format` configuration file (generated with `clang-format -style=llvm -dump-config > .clang-format`) defines the rules (LLVM rules in this case), but can be edited for customization. This requires `clang-format`, which can be installed with:

```
sudo apt update
sudo apt install -y clang-format
```
Clang-Format formatting can be applied directly to the codebase, automatically before committing or during CMake builds, or while integrated with the IDE. This project applies enforced formatting before committing to hpp and cpp files, which blocks commits of code not formatted properly. This maintains clean commits and team consistency. This is achieved with a Git pre-commit hook set with the `set_precommit_hook.py` script.

## Resources

- [Git](https://git-scm.com/doc)
- [CMake](https://cmake.org/documentation/)
- [cppreference](https://en.cppreference.com/w/)
- [Python 3](https://www.python.org/doc/)
- [Doxygen](https://www.doxygen.nl/manual/index.html)
- [Clang-Format 21.0.0](https://clang.llvm.org/docs/index.html)
- [make](https://www.gnu.org/software/make/manual/make.html)
- [GCC](https://gcc.gnu.org/onlinedocs/) (GNU Compiler Collection)
- [Writing and format in GitHub](https://docs.github.com/es/github/writing-on-github/getting-started-with-writing-and-formatting-on-github)
  - [Example 1](https://github.com/SaschaWillems/Vulkan)

