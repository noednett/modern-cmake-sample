#required minimum
cmake_minimum_required(VERSION 3.19) #just the version i am using, as far as i have read, this should work on 3.11...3.19

#basic setup
project(
	modern-cmake-sample
	VERSION 1.0
	DESCRIPTION "An example of a modern cmake project"
	LANGUAGES CXX C) #we need C for glad

###############################################
############# Change this #####################
###############################################


#insert required packages needed for building this project
set(PACKAGES
	GLUT
	OpenGL
)

#insert files from src/ to be compiled
set(SOURCES
	src/Shader.cpp
)

set(CORE_LIB core) #i call it core for now

#insert requested libraries from libs/
set(LOCAL_LIBS
	glad
)

#insert requested external libraries
set(EXT_LIBS
	glfw
)

###############################################

#some compiler stuff
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

	#idk what this does
	#~"Lets ensure -std=c++xx instead of -std=g++xx" 
	set(CMAKE_CXX_EXTENSIONS OFF)

	#This is apparently useful for IDEs
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	#possible testing
	#include(CTest)

	#we could include Doxygen here for generating a doc but that doesnt make sense for now.
endif()

#setting some apparently useful compiler flags
set(CMAKE_CXX_FLAGS "-g -Wall")

#finding packages
foreach(PACKAGE ${PACKAGES})
	find_package(${PACKAGE})
endforeach(PACKAGE)

#add libs
foreach(LIBRARY ${LOCAL_LIBS})
	add_subdirectory("libs/${LIBRARY}")
endforeach(LIBRARY)

#compiling src/ as a library
add_library(${CORE_LIB} OBJECT ${SOURCES})

#adding required headers
target_include_directories(${CORE_LIB} PUBLIC ${PROJECT_SOURCE_DIR}/include)

#target_link_libraries(${CORE_LIB} PUBLIC ${LOCAL_LIBS} ${EXT_LIBS})

#add main app
add_executable(app app/main.cpp)

target_link_libraries(app PRIVATE ${CORE_LIB} PUBLIC ${EXT_LIBS} ${LOCAL_LIBS} ${CMAKE_DL_LIBS})
