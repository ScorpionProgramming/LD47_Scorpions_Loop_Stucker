cmake_minimum_required(VERSION 3.16)
project(myProgramm)
set(CMAKE_GCC_STANDARD 14)

add_subdirectory(lib/glfw)

list(APPEND EXTRA_LIBS
	"-lGL -lX11"
)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

add_executable(myProgramm ${SOURCES})
target_link_libraries(myProgramm glfw ${EXTRA_LIBS})