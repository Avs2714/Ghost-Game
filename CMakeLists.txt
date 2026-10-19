cmake_minimum_required(VERSION 3.16)
project(GhostGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(PLATFORM Web CACHE STRING "" FORCE)

add_subdirectory(/home/dell/raylib raylib)

add_executable(GhostGame src/main.cpp)
target_link_libraries(GhostGame PRIVATE raylib)

if(EMSCRIPTEN)
    set_target_properties(GhostGame PROPERTIES SUFFIX ".html")
    target_link_options(GhostGame PRIVATE
        "-sUSE_GLFW=3"
        "-sASYNCIFY"
        "--preload-file"
        "${CMAKE_SOURCE_DIR}/src/sprites@/sprites"
	"-sALLOW_MEMORY_GROWTH=1"
    )
endif()
