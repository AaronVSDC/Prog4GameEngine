#---------------------------------
# UNDYNE APP EXE
#---------------------------------


add_executable(UndyneApp 
"src/UndyneApp.cpp"
"src/Components/RotatorComponent.cpp"
 "src/Commands/MoveCommand.h"
 "src/Level/LevelLoader.cpp" "src/Components/MoveComponent.cpp" "src/Components/DigComponent.cpp" "src/Components/DigField.cpp" "src/Components/DigField.h")

target_link_libraries(UndyneApp PRIVATE UndyneEngine)


add_custom_command(TARGET UndyneApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:UndyneApp>/Resources)   