cmake_minimum_required(VERSION 3.11)

project(matrix-task CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(modules/matrix)

if (ENABLE_COVERAGE)
	include(Coverage)
endif ()

if (ENABLE_TESTING)
	include(CTest)
	enable_testing()
	add_subdirectory(tests)
endif()