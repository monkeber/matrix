cmake_minimum_required(VERSION 3.11)

project(libmatrix)

add_library(${PROJECT_NAME} INTERFACE)

# Disable adding prefix lib- to libraries.
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME}
	INTERFACE
		${PROJECT_SOURCE_DIR}/include
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_compile_options(${PROJECT_NAME}
	PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:
			/MP /W4 /Zf
			$<$<CONFIG:Debug>:/MDd>
			$<$<CONFIG:Release>:/MD>>
		$<$<OR:$<CXX_COMPILER_ID:GNU>>:
			-Wall -Wextra -Wpedantic -pedantic-errors -pipe>
)