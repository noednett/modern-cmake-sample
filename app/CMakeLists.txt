add_executable(app src/app.cpp)

#potentially we can differentiate here between public, private and interface libraries.

target_link_libraries(app glad glfw ${CMAKE_DL_LIBS}) #the last one is required to compile correctly if things use dlopen or dlclose, it is also crucial that it comes last.

#idk if this needs to be private but it is for now
target_include_directories(app PRIVATE include)
