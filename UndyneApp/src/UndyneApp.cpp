#include "UndyneApp.h"


void UndyneApp::load()
{
	//all game objects get loaded in here
	auto* scene = UndyneEngine::SceneManager::createScene("DemoScene");
	auto* logo = scene->createGameObject();

	logo->addComponent<UndyneEngine::TransformComponent>(200,100,50);
	logo->addComponent<UndyneEngine::TextureComponent>("logo.png"); 


}