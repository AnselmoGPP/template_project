CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(calculator)
MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})
ADD_EXECUTABLE(${PROJECT_NAME}
	src/main.cpp
	src/calculator.cpp

	include/calculator.hpp
)

TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC
	include
)











