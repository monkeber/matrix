cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MatrixTask)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(modules/matrix)

if (ENABLE_TESTING)
	include(CTest)
	enable_testing()
	add_subdirectory(tests)
endif()