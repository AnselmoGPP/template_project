CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(words_tests)

ADD_EXECUTABLE(${PROJECT_NAME}
	src/main.cpp
	src/tests.cpp
	../words/src/words.cpp

	include/tests.hpp
)

TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC
	include
	../words/include
)











