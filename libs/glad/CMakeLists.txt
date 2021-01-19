set(SOURCE_LIST "src/glad.c")

add_library(glad
	${SOURCE_LIST}
)

target_include_directories(glad PUBLIC include)

#this is apparently important for IDEs to find the header files.
source_group(
	TREE "${PROJECT_SOURCE_DIR}/libs/glad/include"
	PREFIX "Header Files"
	FILES ${HEADER_LIST}
)
