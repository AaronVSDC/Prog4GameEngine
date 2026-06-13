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

# VISUAL LEAK DETECTOR (Debug only)
#--------------------------------- 
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VLD_ROOT "C:/Program Files (x86)/Visual Leak Detector")
    if(EXISTS "${VLD_ROOT}/include/vld.h")
        target_include_directories(UndyneApp PRIVATE "${VLD_ROOT}/include")
        target_link_libraries(UndyneApp     PRIVATE "${VLD_ROOT}/lib/Win64/vld.lib")
        target_compile_definitions(UndyneApp PRIVATE UDE_USE_VLD)

        file(COPY
                "${VLD_ROOT}/bin/Win64/vld_x64.dll"
                "${VLD_ROOT}/bin/Win64/dbghelp.dll"
                "${VLD_ROOT}/bin/Win64/Microsoft.DTfW.DHL.manifest"
            DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    else()
        message(WARNING "Visual Leak Detector not found at ${VLD_ROOT} — leak detection disabled.")
    endif()
endif() 