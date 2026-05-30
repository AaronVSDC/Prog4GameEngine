#ifndef UNDYNE_ENGINE_H
#define UNDYNE_ENGINE_H 


//for use of Undyne applications
#include "UndyneEngine/Core/Application.h"
#include "UndyneEngine/Log/Log.h"
#include "UndyneEngine/ECS/GameObject.h"
#include "UndyneEngine/ECS/Scene/SceneManager.h"
#include "UndyneEngine/Input/Command.h"
#include "UndyneEngine/Input/InputManager.h"
#include "UndyneEngine/Audio/SoundServiceLocator.h"
#include "UndyneEngine/Audio/SDLSoundSystem.h"
//engine components
#include "UndyneEngine/ECS/EngineComponents/TextureComponent.h"


#ifdef UNDYNE_ENTRY_POINT
#include "UndyneEngine/Core/EntryPoint.h"
#endif

#endif
