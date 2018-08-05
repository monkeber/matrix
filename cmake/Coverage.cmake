if (ENABLE_COVERAGE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
		-g \
		-O0 \
		-fprofile-arcs \
		-ftest-coverage"
	)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
		--coverage"
	)
endif ()