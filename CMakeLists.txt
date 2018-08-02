cmake_minimum_required(VERSION 3.11)

project(matrix-task CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(modules/matrix)

if (ENABLE_TESTING)
	include(CTest)
	enable_testing()
	add_subdirectory(tests)
endif()