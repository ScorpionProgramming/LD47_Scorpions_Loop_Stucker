cmake_minimum_required(VERSION 3.16)
project(LoopStuck)
set(CMAKE_GCC_STANDARD 14)

add_subdirectory(lib/glfw)

list(APPEND EXTRA_LIBS
	"-lGL -lX11"
)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

add_executable(LoopStuck ${SOURCES})
target_link_libraries(LoopStuck glfw ${EXTRA_LIBS})