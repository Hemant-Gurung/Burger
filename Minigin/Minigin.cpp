#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
//#include "Achievements.h"
#include <SDL_mixer.h>

#include "SoundManager.h"
#include "SServiceLocator.h"


using namespace std;


//achievements 
//Achievement_t g_Achievements[] =
//{
//	_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
//	_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
//	_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
//	_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
//};

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

void dae::Minigin::Instruction()
{
	
	/*std::cout <<std::endl;
	std::cout << "=== GAMEPAD - CONTROLS Player-1 ===\n";
	std::cout << "PLAYER_DEATH =  GAMEPAD_A\n";
	std::cout << "PLAYER_SCORE =  GAMEPAD_B\n";

	std::cout << std::endl;
	std::cout << "=== GAMEPAD - CONTROLS Player-2 ===\n";
	std::cout << "PLAYER_DEATH =  GAMEPAD_X\n";
	std::cout << "PLAYER_SCORE =  GAMEPAD_Y\n";*/

}

void dae::Minigin::Initialize()
{
	//parse svg level
	//m_pLevel = new LevelComponent();
	isOn = false;


	//Mix_Init(MIX_INIT_MP3);
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);
	//auto a = Mix_LoadMUS("prasansa.mp3");
	//Mix_PlayMusic(a, 1);


	
	//register sound
	//ss = new sdl_sound_system();
	//SServiceLocator::register_sound_system(ss);
	// make world
	//b2Vec2 gravity(0.0f, -9.81f);
	//m_World = std::make_unique<b2World>(gravity);
	PrintSDLVersion();
	Instruction();
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	
	
	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		700,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		const std::string error = Mix_GetError();
		throw std::runtime_error(error);
	}
	//m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	InitializeImgui();
	Renderer::GetInstance().Init(m_Window);
}

void dae::Minigin::LoadGame() const
{
}


void dae::Minigin::Cleanup()
{
	//clean up imgui
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();
	SoundManager::GetInstance().Init("../Data/Sounds/");
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
			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplSDL2_NewFrame(m_Window);
			ImGui::NewFrame();

			auto currTime = chrono::high_resolution_clock::now();

			//calculate deltaTime
			float deltaTime = chrono::duration<float>(currTime - lastTime).count();
			lastTime = currTime;
			lag += deltaTime;
			//

			doContinue = input.ProcessInput();
			input.Update();
		
			input.HandleInput();
			
		
			while (lag >= msperupdate)
			{
				sceneManager.FixedUpdate();
				lag -= msperupdate;
			}

			sceneManager.Update(deltaTime);
			//normal update with deltatime
			// 
				//call run steamapi callbacks
			//SteamAPI_RunCallbacks();

			// ImguiUpdate();
			renderer.Render();
			
			//Mix_FreeMusic(gMusic);
			//Mix_Quit();
			
		}
	}
	
	Cleanup();
}

void dae::Minigin::InitializeImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//setup dear imgui style
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
	
	
}


void dae::Minigin::ImguiUpdate()
{
	ImGui::Begin("MyWindow",NULL);
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
	ImGui::Checkbox("Boolean property", &isOn);
	if (ImGui::Button("Reset Speed")) {
		// This code is executed when the user clicks the button
		//this->speed = 0;
	}
	//ImGui::SliderFloat("Speed", &this->speed, 0.0f, 10.0f);
	ImGui::End();
}

