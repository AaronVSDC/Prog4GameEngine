#--------------------------------------------
# UNDYNE ENGINE DLL
#--------------------------------------------


add_library(UndyneEngine SHARED
	"src/UndyneEngine/Core/EntryPoint.h"
	"src/UndyneEngine/Core/Application.cpp" 
	"src/UndyneEngine/Core/Core.h"
	"src/UndyneEngine.h"
	"src/UndyneEngine/Log/Log.cpp" 
	"src/UndyneEngine/Core/Window.h" 
	"src/UndyneEngine/Core/Window.cpp"
	"src/UndyneEngine/Input/InputManager.cpp" 
	"src/UndyneEngine/ECS/GameObject.h"
	"src/UndyneEngine/ECS/BaseComponent.h"
	"src/UndyneEngine/ECS/Scene/SceneManager.cpp"
	"src/UndyneEngine/ECS/Scene/Scene.cpp"
	"src/UndyneEngine/ECS/GameObject.cpp" 
	"src/UndyneEngine/ECS/BaseComponent.cpp")

target_include_directories(UndyneEngine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_definitions(UndyneEngine PRIVATE UDE_BUILD_DLL) 



#LIBRARIES
#---------------
include(FetchContent)

#SDPLOG
#-------
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.3
)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(UndyneEngine PUBLIC spdlog::spdlog)

#SDL3
#------
FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG		   release-3.4.8
)
FetchContent_MakeAvailable(SDL3)
target_link_libraries(UndyneEngine PUBLIC SDL3::SDL3)
