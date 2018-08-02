cmake_minimum_required(VERSION 3.11)

project(MatrixTests)

add_executable(${PROJECT_NAME} MatrixTests.cpp)
add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

target_link_libraries(${PROJECT_NAME} 
	libmatrix
	${CONAN_LIBS}
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