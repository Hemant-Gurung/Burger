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
#include "BurgerComponent.h"
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
	isOn = false;


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
		1280,
		700,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	//m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	InitializeImgui();
	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	

	////////// game object_1
	//auto gameObj = std::make_shared<GameObject>();
	//auto transform = std::make_shared<TransformComponent>(gameObj);
	//gameObj->AddComponent(transform);

	
	 //Text
	//
	////gameobj3
	auto gameObj3 = std::make_shared<GameObject>();
	auto transform1 = std::make_shared<TransformComponent>(gameObj3);
	gameObj3->AddComponent(transform1);
	
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

	auto gameObjectLevel = std::make_shared<GameObject>();
	////make render component
	auto Renderlevel = std::make_shared<RenderComponent>(gameObjectLevel);
	auto transformLevel_1 = std::make_shared<TransformComponent>(gameObjectLevel);
	gameObjectLevel->AddComponent(transformLevel_1);

	Renderlevel->SetTexture(LEVELS[0]);
	gameObjectLevel->AddComponent(Renderlevel);
	////ADD LEVEL SKELETON
	auto levelVertices = std::make_shared<LevelComponent>(gameObjectLevel);
	levelVertices.get()->Initialize(LEVEL_COLLISIONS[0]);
	gameObjectLevel->AddComponent(levelVertices);

	


	//add component to gameobject
	//gameObj->AddComponent(RenderComp);
	//gameObjectLevel->AddComponent(level);


	

	scene.Add(gameObjectLevel);

	auto Level = gameObjectLevel->GetComponent<LevelComponent>();
	////Addplayer 1
	PlayerOne(scene, *Level);
	//PlayerTwo(scene,*Level);
	EnemyType enemy = EnemyType::Red;
	Enemy(scene, enemy, *Level);

	enemy = EnemyType::Egg;
	Enemy(scene, enemy, *Level);
	////PlayerTwo(scene);


	//BURGER
	AddBurger(scene, *Level);
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

	

	//scenemanager

	//ImGui::Render();
	//ImGui_ImplSDL2_InitForSDLRenderer(m_Window);
	
	//ImGui::GetDrawData();


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
			
			/*ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplSDL2_NewFrame(m_Window);
			ImGui::NewFrame();*/

			
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

void dae::Minigin::PlayerOne(Scene& scene,  LevelComponent& slevel) const
{
	
		//===PLAYER ONE ===========================================================>>>>>>>>>>>>>>>
		auto gameObjectPlayer = std::make_shared<GameObject>();

		//Do this inside player class
		//player commands
		//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
		Input::GetInstance().MapEvent(std::make_pair( XBOX360Controller::ButtonState::down, XBOX360Controller::ControllerButton::ButtonA), InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne));
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::down, XBOX360Controller::ControllerButton::ButtonB), InputAction(std::make_unique<ScoreCommand>(gameObjectPlayer), GamePadIndex::playerOne));
		//RIGHT
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadRight), InputAction(std::make_unique<MoveRightCommand>(gameObjectPlayer), GamePadIndex::playerOne));
		//LEFT
		Input::GetInstance().MapEvent(std::make_pair( XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadLeft), InputAction(std::make_unique<MoveLeftCommand>(gameObjectPlayer), GamePadIndex::playerOne));
		//UP
		Input::GetInstance().MapEvent(std::make_pair( XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadUp), InputAction(std::make_unique<MoveUpCommand>(gameObjectPlayer), GamePadIndex::playerOne));
		//DOWN
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::held, XBOX360Controller::ControllerButton::DpadDown), InputAction(std::make_unique<MoveDownCommand>(gameObjectPlayer), GamePadIndex::playerOne));


		auto transformPlayer1 = std::make_shared<TransformComponent>(gameObjectPlayer);
		//transformPlayer1->SetPosition(216, 180, 0);

		//player observer

		gameObjectPlayer->AddComponent(transformPlayer1);
		//player one
		auto player = std::make_shared<PlayerComponent>(gameObjectPlayer,slevel);

		
		//add gameobject to scene
		scene.Add(gameObjectPlayer);


		gameObjectPlayer->AddComponent(player);
		//gameObjectPlayer->GetComponent<TransformComponent>()->SetPosition(200.f, 10.f, 0.f);
		//score
		// gameObjLivesCounter = std::make_shared<GameObject>();

		//make transform component for the game object
		auto transformLives = std::make_shared<TransformComponent>(gameObjectPlayer);

		//make text component
		auto fontLives = ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 20);
		auto textLives = std::make_shared<TextComponent>(gameObjectPlayer, " ", fontLives, SDL_Color(255, 255, 255));
		auto fontScore = ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
		auto textScore = std::make_shared<TextComponent>(gameObjectPlayer, " ", fontScore, SDL_Color(255, 255, 255));

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

		//set score position
		textScore->SetPosition(350, 5, 0);

		//set lives position
		textLives->SetPosition(570, 5, 0);

	
		//add to scent
		scene.Add(gameObjectPlayer);


}

void dae::Minigin::PlayerTwo(Scene& scene, LevelComponent& level) const
{
	{
		//===PLAYER TWO ===========================================================>>>>>>>>>>>>>>>
		auto gameObjectPlayerTwo = std::make_shared<GameObject>();
		//player two
		Input::GetInstance().MapEvent(std::make_pair( XBOX360Controller::ButtonState::down1, XBOX360Controller::ControllerButton::ButtonA), InputAction(std::make_unique<DeathCommand>(gameObjectPlayerTwo), GamePadIndex::playerTwo));
		Input::GetInstance().MapEvent(std::make_pair(XBOX360Controller::ButtonState::down1, XBOX360Controller::ControllerButton::ButtonB), InputAction(std::make_unique<ScoreCommand>(gameObjectPlayerTwo), GamePadIndex::playerTwo));
		//RIGHT
		Input::GetInstance().MapEvent(std::make_pair( XBOX360Controller::ButtonState::held1, XBOX360Controller::ControllerButton::DpadRight), InputAction(std::make_unique<MoveRightCommand>(gameObjectPlayerTwo), GamePadIndex::playerTwo));
		//LEFT
		Input::GetInstance().MapEvent(std::make_pair(  XBOX360Controller::ButtonState::held1, XBOX360Controller::ControllerButton::DpadLeft ), InputAction(std::make_unique<MoveLeftCommand>(gameObjectPlayerTwo), GamePadIndex::playerTwo));
		//UP
		Input::GetInstance().MapEvent(std::make_pair( XBOX360Controller::ButtonState::held1, XBOX360Controller::ControllerButton::DpadUp), InputAction(std::make_unique<MoveUpCommand>(gameObjectPlayerTwo), GamePadIndex::playerTwo));
		//DOWN
		Input::GetInstance().MapEvent(std::make_pair(  XBOX360Controller::ButtonState::held1, XBOX360Controller::ControllerButton::DpadDown), InputAction(std::make_unique<MoveDownCommand>(gameObjectPlayerTwo), GamePadIndex::playerTwo));


		auto transformPlayer1 = std::make_shared<TransformComponent>(gameObjectPlayerTwo);
		//transformPlayer1->SetPosition(216, 180, 0);

		//player observer

		gameObjectPlayerTwo->AddComponent(transformPlayer1);
		//player one
		auto player = std::make_shared<PlayerComponent>(gameObjectPlayerTwo, level);


		//add gameobject to scene
		scene.Add(gameObjectPlayerTwo);


		gameObjectPlayerTwo->AddComponent(player);
		//gameObjectPlayer->GetComponent<TransformComponent>()->SetPosition(200.f, 10.f, 0.f);
		//score
		// gameObjLivesCounter = std::make_shared<GameObject>();

		//make transform component for the game object
		auto transformLives = std::make_shared<TransformComponent>(gameObjectPlayerTwo);

		//make text component
		auto fontLives = ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 20);
		auto textLives = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontLives, SDL_Color(255, 255, 255));
		auto fontScore = ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
		auto textScore = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontScore, SDL_Color(255, 255, 255));

		// score achievement 
		//auto scoreAchievement = std::make_shared<Achievements>(g_Achievements, 4, gameObjectPlayer);

		//make livescounter component/ observer
		auto lives = std::make_shared<LivesCounterComponent>(gameObjectPlayerTwo, textLives);

		//score counter
		auto score = std::make_shared<ScoreComponent>(gameObjectPlayerTwo, textScore);
		//score achievement observer
		//player->AddObserver(scoreAchievement);

		//add observers
		player->AddObserver(lives);

		//add score observer
		player->AddObserver(score);

		//add gameobject scoreAchievement to gameobject
		//gameObjectPlayer->AddComponent(scoreAchievement);
		//add transform component to gameobject
		gameObjectPlayerTwo->AddComponent(transformLives);

		//add counter to gameobject
		gameObjectPlayerTwo->AddComponent(lives);

		//add score component to gameobject
		gameObjectPlayerTwo->AddComponent(score);

		//add text component for lives
		gameObjectPlayerTwo->AddComponent(textLives);


		//add textComponent for score
		gameObjectPlayerTwo->AddComponent(textScore);

		//set score position
		textScore->SetPosition(350, 5, 0);

		//set lives position
		textLives->SetPosition(570, 5, 0);


		//add to scent
		scene.Add(gameObjectPlayerTwo);
		//auto playerTwo = std::make_shared<PlayerComponent>(gameObjectPlayerTwo,level);
		////observer

		//auto fontLives = ResourceManager::GetInstance().LoadFont("Burgertime.otf", 16);
		//
		//auto fontScore = ResourceManager::GetInstance().LoadFont("Burgertime.otf", 16);
		//auto textScore = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontScore, SDL_Color(0, 255, 0));

		//gameObjectPlayerTwo->AddComponent(playerTwo);
		//auto textLives2 = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontLives, SDL_Color(255, 0, 0));

		//auto textScore2 = std::make_shared<TextComponent>(gameObjectPlayerTwo, " ", fontScore, SDL_Color(0, 255, 0));
		////make livescounter component/ observer
		//auto lives2 = std::make_shared<LivesCounterComponent>(gameObjectPlayerTwo, textLives2);

		////score counter
		//auto score2 = std::make_shared<ScoreComponent>(gameObjectPlayerTwo, textScore2);

		////add observers
		//playerTwo->AddObserver(lives2);

		////add score observer
		//playerTwo->AddObserver(score2);

		////Transform component
		//auto transformPlayer2 = std::make_shared<TransformComponent>(gameObjectPlayerTwo);
		////transformPlayer1->SetPosition(216, 180, 0);
		//
		////make transform component for the game object
		//auto transformLives = std::make_shared<TransformComponent>(gameObjectPlayerTwo);
		////player observer
		////scene.Add(gameObjectPlayer);
		////auto playerText2 = std::make_shared<RenderComponent>(gameObjectPlayerTwo);
		////playerText2->SetTexture("chef.png");

		////gameObjectPlayerTwo->AddComponent(playerText2);
		//gameObjectPlayerTwo->AddComponent(transformPlayer2);
		////playerText2->SetPosition(216, 180, 0);
		////add transform component to gameobject
		//gameObjectPlayerTwo->AddComponent(transformLives);

		////add counter to gameobject
		//gameObjectPlayerTwo->AddComponent(lives2);

		////add score component to gameobject
		//gameObjectPlayerTwo->AddComponent(score2);

		////add text component for lives
		//gameObjectPlayerTwo->AddComponent(textLives2);


		////add textComponent for score
		//gameObjectPlayerTwo->AddComponent(textScore2);

		////set score position
		//textScore2->SetPosition(500, 10, 0);

		////set lives position
		//textLives2->SetPosition(500, 30, 0);

		////add to scent
		//scene.Add(gameObjectPlayerTwo);

		
	}
}

void dae::Minigin::Enemy(Scene& scene,EnemyType& enemyType,  LevelComponent& level) const
{
	auto gameObjectEnemy = std::make_shared<GameObject>();

	auto enemy = std::make_shared<EnemyComponent>(gameObjectEnemy,enemyType,level);
	
	gameObjectEnemy->AddComponent(enemy);
	scene.Add(gameObjectEnemy);

}

void dae::Minigin::AddBurger(Scene& scene, LevelComponent& level) const
{
	auto gameobjectBurger = std::make_shared<GameObject>();
	float4 burgerPos;
	//Bottom
	burgerPos.one = 25.f;
	burgerPos.two = 123.6f;
	burgerPos.three = 224.7f;
	burgerPos.four = 370.78f;

	float leftPos = 55.f;
	auto burger = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos, burgerPos);
	gameobjectBurger->AddComponent(burger);

	//float4 burgerPos;
	//Bottom
	burgerPos.one = 173.f;
	burgerPos.two = 275.2f;
	burgerPos.three = 369.7f;
	burgerPos.four = 589.f;

	float leftPos2 = 202.2f;
	auto burger2 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos2, burgerPos);
	gameobjectBurger->AddComponent(burger2);

	burgerPos.one = 125.f;
	burgerPos.two = 275.2f;
	burgerPos.three = 369.7f;
	burgerPos.four = 471.9f;

	float leftPos3 = 348.2f;
	auto burger3 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos3, burgerPos);
	gameobjectBurger->AddComponent(burger3);


	burgerPos.one = 125.8f;
	burgerPos.two = 220.83f;
	burgerPos.three = 322.225f;
	burgerPos.four = 471.9f;

	float leftPos4 = 505.f;
	auto burger4 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos4, burgerPos);
	gameobjectBurger->AddComponent(burger4);



	scene.Add(gameobjectBurger);
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

