#---------------------------------
# UNDYNE APP EXE
#---------------------------------


add_executable(UndyneApp 
"src/UndyneApp.cpp"
)

target_link_libraries(UndyneApp PRIVATE UndyneEngine)

if(MSVC)
    target_compile_options(UndyneApp PRIVATE /W4 /WX)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_options(UndyneApp PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    endif()
else()
    target_compile_options(UndyneApp PRIVATE -Wall -Wextra -Werror)
endif()

add_custom_command(TARGET UndyneApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:UndyneApp>/Resources)   