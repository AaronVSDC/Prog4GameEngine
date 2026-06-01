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
	"src/UndyneEngine/ECS/BaseComponent.cpp"
	"src/UndyneEngine/Renderer/Renderer.cpp"
	"src/UndyneEngine/Utils/Texture2D.cpp"
	"src/UndyneEngine/ResourceManager/ResourceManager.cpp" 
	"src/UndyneEngine/Utils/Font.cpp"
	"src/UndyneEngine/Utils/Transform.cpp"
	"src/UndyneEngine/ECS/EngineComponents/TextureComponent.cpp" 
	"src/UndyneEngine/Input/Command.h" 
	"src/UndyneEngine/Audio/SoundServiceLocator.cpp"
	"src/UndyneEngine/Audio/SDLSoundSystem.cpp"
	"src/UndyneEngine/Audio/SoundSystem.h"
 "src/UndyneEngine/ECS/EngineComponents/SpriteComponent.cpp")

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
	GIT_SHALLOW    TRUE

)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(UndyneEngine PUBLIC spdlog::spdlog)

#SDL3
#------
FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG		   release-3.4.8
	GIT_SHALLOW    TRUE

)
FetchContent_MakeAvailable(SDL3)
target_link_libraries(UndyneEngine PUBLIC SDL3::SDL3)

#SDL MIXER
#----------
set(SDLMIXER_VENDORED ON  CACHE BOOL "" FORCE)
set(SDLMIXER_SAMPLES  OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD      OFF CACHE BOOL "" FORCE)  
set(SDLMIXER_MIDI     OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OPUS     OFF CACHE BOOL "" FORCE)
set(SDLMIXER_FLAC     OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3_mixer)
target_link_libraries(UndyneEngine PUBLIC SDL3_mixer::SDL3_mixer)

#SDL TTF
#--------
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)  
set(SDLTTF_SAMPLES  OFF CACHE BOOL "" FORCE)   

FetchContent_Declare(
	SDL3_ttf
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
	GIT_TAG        release-3.2.2
	GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3_ttf)
target_link_libraries(UndyneEngine PUBLIC SDL3_ttf::SDL3_ttf)


#GLM
#-----
FetchContent_Declare(
	glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3  
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glm)
target_link_libraries(UndyneEngine PUBLIC glm::glm-header-only) 

