#include "MiniginPCH.h"
#include "Minigin.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameObject.h"

#include "RenderComponent.h"
#include "FPScomponent.h"
#include "TransformComponent.h"
#include "LivesCounterComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ScoreComponent.h"
#include <steam_api.h>
//#include "Achievements.h"
#include <SDL_mixer.h>
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
	
	std::cout <<std::endl;
	std::cout << "=== GAMEPAD - CONTROLS Player-1 ===\n";
	std::cout << "PLAYER_DEATH =  GAMEPAD_A\n";
	std::cout << "PLAYER_SCORE =  GAMEPAD_B\n";

	std::cout << std::endl;
	std::cout << "=== GAMEPAD - CONTROLS Player-2 ===\n";
	std::cout << "PLAYER_DEATH =  GAMEPAD_X\n";
	std::cout << "PLAYER_SCORE =  GAMEPAD_Y\n";

}

void dae::Minigin::Initialize()
{
	//parse svg level
	//m_pLevel = new LevelComponent();



	//Mix_Init(MIX_INIT_MP3);
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);
	//auto a = Mix_LoadMUS("prasansa.mp3");
	//Mix_PlayMusic(a, 1);


	
	//register sound
	ss = new sdl_sound_system();
	SServiceLocator::register_sound_system(ss);
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
		900,
		700,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	//m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);


	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	

	////////// game object_1
	auto gameObj = std::make_shared<GameObject>();
	auto transformLevel = std::make_shared<TransformComponent>(gameObj);
	gameObj->AddComponent(transformLevel);

	
	 //Text
	//
	////gameobj3
	auto gameObj3 = std::make_shared<GameObject>();
	auto transform = std::make_shared<TransformComponent>(gameObj3);
	gameObj3->AddComponent(transform);
	
	scene.Add(gameObj3);

	//fpsComponent
	auto gameObjFPS = std::make_shared<GameObject>();
	auto fontFPS = ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);
	auto text = std::make_shared<TextComponent>( gameObj3," ", fontFPS,SDL_Color(255,255,0));
	
	//make fps component
	auto fpsComponent = std::make_shared<FPScomponent>(gameObj3,text);

	//add textcomponent to game object
	gameObjFPS->AddComponent(text);

	//add fps component to game object
	gameObjFPS->AddComponent(fpsComponent);
	text->SetPosition(10, 450, 0);
	
	scene.Add(gameObjFPS);

	//Addplayer 1
	PlayerOne(scene);
	
	//PlayerTwo(scene);

	
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

			renderer.Render();
			
			
			//Mix_FreeMusic(gMusic);
			//Mix_Quit();
			
		}
	}

	Cleanup();
}

void dae::Minigin::PlayerOne(Scene& scene) const
{
	
		//===PLAYER ONE ===========================================================>>>>>>>>>>>>>>>
		auto gameObjectPlayer = std::make_shared<GameObject>();

		//Do this inside player class
		//player commands
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::down, XBOX360Controller::ControllerButton::ButtonA), std::move(std::make_unique<DeathCommand>(gameObjectPlayer)));
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::down, XBOX360Controller::ControllerButton::ButtonB), std::move(std::make_unique<ScoreCommand>(gameObjectPlayer)));
		//RIGHT
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadRight), std::move(std::make_unique<MoveRightCommand>(gameObjectPlayer)));
		//LEFT
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadLeft), std::move(std::make_unique<MoveLeftCommand>(gameObjectPlayer)));
		//UP
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadUp), std::move(std::make_unique<MoveUpCommand>(gameObjectPlayer)));
		//DOWN
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadDown), std::move(std::make_unique<MoveDownCommand>(gameObjectPlayer)));

		auto transformPlayer1 = std::make_shared<TransformComponent>(gameObjectPlayer);
		//transformPlayer1->SetPosition(216, 180, 0);

		//player observer
		//scene.Add(gameObjectPlayer);
		//auto playerText1 = std::make_shared<RenderComponent>(gameObjectPlayer);
		//playerText1->SetTexture("chef.png");

		//gameObjectPlayer->AddComponent(playerText1);





		gameObjectPlayer->AddComponent(transformPlayer1);
		//player one
		auto player = std::make_shared<PlayerComponent>(gameObjectPlayer);


		//make render component
		auto level = std::make_shared<RenderComponent>(gameObjectPlayer);
		level->SetTexture(LEVELS[0]);

		//add component to gameobject
		//gameObj->AddComponent(RenderComp);
		gameObjectPlayer->AddComponent(level);


		//ADD LEVEL SKELETON
		auto levelVertices = std::make_shared<LevelComponent>(gameObjectPlayer);
		levelVertices.get()->Initialize(LEVEL_COLLISIONS[0]);
		//levelVertices->SetPosition(70, 20, 0);
		gameObjectPlayer->AddComponent(levelVertices);

		//add gameobject to scene
		scene.Add(gameObjectPlayer);

	//observer
		//player->Initialize();
		//Transform component
		
		//transformPlayer1->SetPosition(900, 900, 0);
		//transformPlayer1->SetPosition(900 / 2, 400, 0);
		//renderlogo->SetPosition(216, 180, 0);
		gameObjectPlayer->AddComponent(player);
		gameObjectPlayer->GetComponent<TransformComponent>()->SetPosition(200.f, 10.f, 0.f);
		//score
		// gameObjLivesCounter = std::make_shared<GameObject>();

		//make transform component for the game object
		auto transformLives = std::make_shared<TransformComponent>(gameObjectPlayer);

		//make text component
		auto fontLives = ResourceManager::GetInstance().LoadFont("Burgertime.otf", 16);
		auto textLives = std::make_shared<TextComponent>(gameObjectPlayer, " ", fontLives, SDL_Color(255, 0, 0));
		auto fontScore = ResourceManager::GetInstance().LoadFont("Burgertime.otf", 16);
		auto textScore = std::make_shared<TextComponent>(gameObjectPlayer, " ", fontScore, SDL_Color(0, 255, 0));

		// score achievement 
		//auto scoreAchievement = std::make_shared<Achievements>(g_Achievements, 4, gameObjectPlayer);

		//make livescounter component/ observer
		auto lives = std::make_shared<LivesCounterComponent>(gameObjectPlayer, textLives);

		//score counter
		auto score = std::make_shared<ScoreComponent>(gameObjectPlayer, textScore);
		//score achievement observer
		//player->AddObserver(scoreAchievement);

		//add observers
		player->AddObserver(lives);

		//add score observer
		player->AddObserver(score);

		//add gameobject scoreAchievement to gameobject
		//gameObjectPlayer->AddComponent(scoreAchievement);
		//add transform component to gameobject
		gameObjectPlayer->AddComponent(transformLives);

		//add counter to gameobject
		gameObjectPlayer->AddComponent(lives);

		//add score component to gameobject
		gameObjectPlayer->AddComponent(score);

		//add text component for lives
		gameObjectPlayer->AddComponent(textLives);


		//add textComponent for score
		gameObjectPlayer->AddComponent(textScore);

		gameObjectPlayer->RemoveComponent(textScore);

		//set score position
		textScore->SetPosition(10, 10, 0);

		//set lives position
		textLives->SetPosition(10, 30, 0);

	
		//add to scent
		scene.Add(gameObjectPlayer);


}

void dae::Minigin::PlayerTwo(Scene&) const
{
	//{
	//	//===PLAYER TWO ===========================================================>>>>>>>>>>>>>>>
	//	auto gameObjectPlayerTwo = std::make_shared<GameObject>();
	//	//player two
	//	Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::down, XBOX360Controller::ControllerButton::ButtonX), std::move(std::make_unique<DeathCommand>(gameObjectPlayerTwo)));
	//	Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::down, XBOX360Controller::ControllerButton::ButtonY), std::move(std::make_unique<ScoreCommand>(gameObjectPlayerTwo)));
	//	//RIGHT
	//	Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadRight), std::move(std::make_unique<MoveRightCommand>(gameObjectPlayerTwo)));
	//	//LEFT
	//	Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadLeft), std::move(std::make_unique<MoveLeftCommand>(gameObjectPlayerTwo)));
	//	//UP
	//	Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadUp), std::move(std::make_unique<MoveUpCommand>(gameObjectPlayerTwo)));
	//	//DOWN
	//	Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadDown), std::move(std::make_unique<MoveDownCommand>(gameObjectPlayerTwo)));
	//	//player one
	//	auto playerTwo = std::make_shared<PlayerComponent>(gameObjectPlayerTwo);
	//	//observer

	//	auto fontLives = ResourceManager::GetInstance().LoadFont("Burgertime.otf", 16);
	//	
	//	auto fontScore = ResourceManager::GetInstance().LoadFont("Burgertime.otf", 16);
	//	auto textScore = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontScore, SDL_Color(0, 255, 0));

	//	gameObjectPlayerTwo->AddComponent(playerTwo);
	//	auto textLives2 = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontLives, SDL_Color(255, 0, 0));

	//	auto textScore2 = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontScore, SDL_Color(0, 255, 0));
	//	//make livescounter component/ observer
	//	auto lives2 = std::make_shared<LivesCounterComponent>(gameObjectPlayerTwo, textLives2);

	//	//score counter
	//	auto score2 = std::make_shared<ScoreComponent>(gameObjectPlayerTwo, textScore2);

	//	//add observers
	//	playerTwo->AddObserver(lives2);

	//	//add score observer
	//	playerTwo->AddObserver(score2);

	//	//Transform component
	//	auto transformPlayer2 = std::make_shared<TransformComponent>(gameObjectPlayerTwo);
	//	//transformPlayer1->SetPosition(216, 180, 0);
	//	
	//	//make transform component for the game object
	//	auto transformLives = std::make_shared<TransformComponent>(gameObjectPlayerTwo);
	//	//player observer
	//	//scene.Add(gameObjectPlayer);
	//	auto playerText2 = std::make_shared<RenderComponent>(gameObjectPlayerTwo);
	//	playerText2->SetTexture("chef.png");

	//	gameObjectPlayerTwo->AddComponent(playerText2);
	//	gameObjectPlayerTwo->AddComponent(transformPlayer2);
	//	playerText2->SetPosition(216, 180, 0);
	//	//add transform component to gameobject
	//	gameObjectPlayerTwo->AddComponent(transformLives);

	//	//add counter to gameobject
	//	gameObjectPlayerTwo->AddComponent(lives2);

	//	//add score component to gameobject
	//	gameObjectPlayerTwo->AddComponent(score2);

	//	//add text component for lives
	//	gameObjectPlayerTwo->AddComponent(textLives2);


	//	//add textComponent for score
	//	gameObjectPlayerTwo->AddComponent(textScore2);

	//	//set score position
	//	textScore2->SetPosition(500, 10, 0);

	//	//set lives position
	//	textLives2->SetPosition(500, 30, 0);

	//	//add to scent
	//	scene.Add(gameObjectPlayerTwo);

	//	
	//}
}
