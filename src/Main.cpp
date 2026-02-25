#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <glm/glm.hpp>

#include <filesystem>
#include <RotatorComponent.h>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto daeLogo = std::make_unique<dae::GameObject>();
	daeLogo->AddComponent(std::make_unique<dae::TextureComponent>("logo.png")); 
	daeLogo->SetLocalPosition(glm::vec3(200, 200,0));
	daeLogo->AddComponent(std::make_unique<dae::RotatorComponent>(200.f)); 


	auto daeLogo2 = std::make_unique<dae::GameObject>();
	daeLogo2->AddComponent(std::make_unique<dae::TextureComponent>("logo.png"));
	daeLogo2->AddComponent(std::make_unique<dae::RotatorComponent>(50.f, 2.f));

	daeLogo2->SetParent(daeLogo.get(), false); 



	scene.Add(std::move(daeLogo));
	scene.Add(std::move(daeLogo2)); 

}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
