project(MatrixTests)

add_executable(${PROJECT_NAME} MatrixTests.cpp)
add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

target_link_libraries(${PROJECT_NAME} 
	matrix
	${CONAN_LIBS}
)