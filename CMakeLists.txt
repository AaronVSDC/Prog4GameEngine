#--------------------------
# ROOT CMAKE FILE
#--------------------------

cmake_minimum_required(VERSION 3.20)
project("Undyne")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()


#COMPILE DEFINITIONS
#----------------------
add_compile_definitions(UDE_PLATFORM_WINDOWS)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(UDE_DEBUG)
endif() 

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(UDE_RELEASE)
endif()


add_subdirectory(UndyneEngine)
add_subdirectory(UndyneApp)

