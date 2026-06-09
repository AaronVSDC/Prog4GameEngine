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
#include "UndyneEngine/ResourceManager/ResourceManager.h"
#include "UndyneEngine/Renderer/Renderer.h"
#include "UndyneEngine/Utils/Texture2D.h"
#include "UndyneEngine/Observer/Event.h"
#include "UndyneEngine/Observer/Observer.h"
#include "UndyneEngine/Observer/Subject.h"

//engine components
#include "UndyneEngine/ECS/EngineComponents/TextureComponent.h"
#include "UndyneEngine/ECS/EngineComponents/AnimationComponent.h"
#include "UndyneEngine/ECS/EngineComponents/TextComponent.h"

//state machine
#include "UndyneEngine/StateMachine/StateMachineComponent.h"

#ifdef UNDYNE_ENTRY_POINT
#include "UndyneEngine/Core/EntryPoint.h"
#endif

#endif
