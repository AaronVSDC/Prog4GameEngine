#include "UndyneApp.h"
#include "Components/RotatorComponent.h"

void UndyneApp::load()
{
    auto* scene = UndyneEngine::SceneManager::createScene("DemoScene");

    auto* sun = scene->createGameObject("sun");
    sun->getTransform().setLocalPosition(400.0f, 300.0f, 0.0f);
    sun->addComponent<UndyneEngine::TextureComponent>("logo.png");   

    auto* earth = scene->createGameObject("earth");
    earth->setParent(sun);                               
    earth->addComponent<UndyneEngine::TextureComponent>("logo.png");
    earth->addComponent<Digger::RotatorComponent>(120.0f, 1.0f);   

    auto* moon = scene->createGameObject("moon");
    moon->setParent(earth);
    moon->addComponent<UndyneEngine::TextureComponent>("logo.png");
    moon->addComponent<Digger::RotatorComponent>(35.0f, 4.0f);   
}