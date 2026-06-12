#---------------------------------
# UNDYNE APP EXE
#---------------------------------


add_executable(UndyneApp 
"src/UndyneApp.cpp"
"src/Components/RotatorComponent.cpp"
 "src/Commands/MoveCommand.h"
 "src/Level/LevelLoader.cpp" "src/Components/MoveComponent.cpp" "src/Components/DigComponent.cpp" "src/Components/LevelGridComponent.cpp" "src/Components/DigTrailComponent.cpp" "src/Components/ScoreComponent.cpp" "src/Components/ScoreDisplayComponent.cpp" "src/Components/PickupComponent.cpp" "src/Components/GoldBagComponent.cpp" "src/Components/LivesComponent.cpp" "src/Components/LivesDisplayComponent.cpp" "src/Components/GridMovementComponent.cpp" "src/Components/EnemyComponent.cpp" "src/Components/ShootComponent.cpp" "src/Components/ProjectileComponent.cpp" "src/Components/MonsterSpawnerComponent.cpp" "src/Components/LevelController.cpp" "src/Factory/EnemyFactory.cpp" "src/Factory/PlayerFactory.cpp" "src/Commands/ActionCommand.h" "src/Commands/StartModeCommand.h" "src/Commands/SecondPlayerMoveCommand.h" "src/Commands/FireCommand.h" "src/GameState.cpp" "src/AI/ChaseState.cpp" "src/AI/NobbinChaseState.cpp" "src/AI/HobbinChaseState.cpp" "src/AI/DeadState.cpp" "src/AI/CrushedState.cpp" "src/AI/ManualControlState.cpp")

target_link_libraries(UndyneApp PRIVATE UndyneEngine)

if(MSVC) 
    target_compile_options(UndyneApp PRIVATE /W4 /WX)
else()
    target_compile_options(UndyneApp PRIVATE -Wall -Wextra -Werror)
endif()

add_custom_command(TARGET UndyneApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:UndyneApp>/Resources)   