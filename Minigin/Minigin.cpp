#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FPScomponent.h"


using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void dae::Minigin::Initialize()
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	////auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	////// game object_1
	////auto gameObj = std::make_shared<GameObject>();
	////scene.Add(gameObj);

	//// background
	//auto go = std::make_shared<RenderComponent>();
	//go->SetTexture("background.jpg");
	//gameObj->AddComponent(go);

	// logo
	//auto renderlogo = std::make_shared<RenderComponent>();
	//renderlogo->SetTexture("logo.png");
	//renderlogo->SetPosition(216, 180,0);

	//// game object_2
	//auto gameObj2 = std::make_shared<GameObject>();
	//gameObj2->AddComponent(renderlogo);
	//scene.Add(gameObj2);

	// Text
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<TextComponent>("Programming 4 Assignment", font);
	////gameObj->GetComponent<Transform>()->SetPosition(80, 20, 0);
	//to->SetPosition(80, 20,0);
	//gameObj->AddComponent(to);


	//gameobj3
	/*auto gameObj3 = std::make_shared<GameObject>();
	auto fontFPS = ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);
	auto text = std::make_shared<TextComponent>(" ", fontFPS);*/
	
	//auto fpsComponent = std::make_shared<FPScomponent>(text);
	//gameObj3->AddComponent(text);
	//scene.Add(gameObj3);
	/*gameObj3->AddComponent(fpsComponent);*/
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();
	//ms per update const 
	constexpr float msperupdate = 0.016f;
	{
		
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		auto lastTime = chrono::high_resolution_clock::now();
		


		float lag = 0.0f;
		bool doContinue = true;
		while (doContinue)
		{
			auto currTime = chrono::high_resolution_clock::now();

			//calculate deltaTime
			float deltaTime = chrono::duration<float>(currTime - lastTime).count();
			lastTime = currTime;
			lag += deltaTime;

			doContinue = input.ProcessInput();
			while (lag >= msperupdate)
			{
				//FixedUpdate();
				lag -= msperupdate;
			}

			sceneManager.Update(deltaTime);
			//normal update with deltatime

			renderer.Render();
		}
	}

	Cleanup();
}
