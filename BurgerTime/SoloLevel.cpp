#include "pch.h"
#include "SoloLevel.h"
#include "InputManager.h"
#include "LivesCounterComponent.h"
#include "BurgerComponent.h"
#include "ResourceManager.h"
#include "ScoreComponent.h"
#include "BurgerGame.h"


SoloLevel::SoloLevel()
	:GameScene("SoloLevel")
{
	Initialize();
}

void SoloLevel::Update(float dt)
{
	for (const auto& object : m_sceneObjects)
	{
		object->Update(dt);
	}
}

void SoloLevel::FixedUpdate()
{
}

void SoloLevel::Initialize()
{
	//auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	
	//dae::SceneManager::GetInstance().AddGameScene("Demo");
	dae::ResourceManager::GetInstance().Init("../Data/");

	auto gameObjectLevel = std::make_shared<dae::GameObject>();
	////make render component
	auto Renderlevel = std::make_shared<dae::RenderComponent>(gameObjectLevel);
	auto transformLevel_1 = std::make_shared<dae::TransformComponent>(gameObjectLevel);
	gameObjectLevel->AddComponent(transformLevel_1);

	//auto texture = dae::ResourceManager::GetInstance().LoadTexture(LEVELS[0]);
	Renderlevel->SetTexture(LEVELS[0]);
	gameObjectLevel->AddComponent(Renderlevel);
	////ADD LEVEL SKELETON
	auto levelVertices = std::make_shared<LevelComponent>(gameObjectLevel);
	levelVertices.get()->Initialize(LEVEL_COLLISIONS[0]);
	gameObjectLevel->AddComponent(levelVertices);

	//scene.Add(gameObjectLevel);

	auto Level = gameObjectLevel->GetComponent<LevelComponent>();

	AddChild(gameObjectLevel);
	//===PLAYER ONE ===========================================================>>>>>>>>>>>>>>>
	PlayerOne( *Level);

	AddBurger(*Level);


	// Enemy Red
	EnemyType enemy = EnemyType::Red;
	Enemy( enemy, *Level);


	//Enemy Egg
	enemy = EnemyType::Egg;
	Enemy(enemy, *Level);

	
}

void SoloLevel::Render()
{
	for (const auto& object : m_sceneObjects)
	{
		object->Render();
	}
}

void SoloLevel::Enemy( EnemyType& enemytype, LevelComponent& level) 
{
	auto gameObjectEnemy = std::make_shared<dae::GameObject>();

	

	auto enemy = std::make_shared<dae::EnemyComponent>(gameObjectEnemy, enemytype, level);

	gameObjectEnemy->AddComponent(enemy);
	//scene.Add(gameObjectEnemy);
	AddChild(gameObjectEnemy);
}

void SoloLevel::AddBurger( LevelComponent& level) 
{
	auto gameobjectBurger = std::make_shared<dae::GameObject>();
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



	//scene.Add(gameobjectBurger);
	AddChild(gameobjectBurger);
}

void SoloLevel::PlayerOne( LevelComponent& slevel) 
{
	auto gameObjectPlayer = std::make_shared<dae::GameObject>();

	//Do this inside player class
	//player commands
	//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
	dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonA), InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne));
	dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonB), InputAction(std::make_unique<ScoreCommand>(gameObjectPlayer), GamePadIndex::playerOne));
	//RIGHT
	dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadRight), InputAction(std::make_unique<MoveRightCommand>(gameObjectPlayer), GamePadIndex::playerOne));
	//LEFT
	dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadLeft), InputAction(std::make_unique<MoveLeftCommand>(gameObjectPlayer), GamePadIndex::playerOne));
	//UP
	dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadUp), InputAction(std::make_unique<MoveUpCommand>(gameObjectPlayer), GamePadIndex::playerOne));
	//DOWN
	dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadDown), InputAction(std::make_unique<MoveDownCommand>(gameObjectPlayer), GamePadIndex::playerOne));



	auto transformPlayer1 = std::make_shared<dae::TransformComponent>(gameObjectPlayer);
	//transformPlayer1->SetPosition(216, 180, 0);

	//player observer

	gameObjectPlayer->AddComponent(transformPlayer1);
	//player one
	auto player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, slevel);


	//add gameobject to scene
	//scene.Add(gameObjectPlayer);


	gameObjectPlayer->AddComponent(player);
	//gameObjectPlayer->GetComponent<TransformComponent>()->SetPosition(200.f, 10.f, 0.f);
	//score
	// gameObjLivesCounter = std::make_shared<GameObject>();

	//make transform component for the game object
	auto transformLives = std::make_shared<dae::TransformComponent>(gameObjectPlayer);

	//make text component
	auto fontLives = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 20);
	auto textLives = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontLives, SDL_Color{ 255,255,255,1 });
	auto fontScore = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
	auto textScore = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontScore, SDL_Color{ 255,255,255,1 });

	// score achievement 
	//auto scoreAchievement = std::make_shared<Achievements>(g_Achievements, 4, gameObjectPlayer);

	//make livescounter component/ observer
	auto lives = std::make_shared<dae::LivesCounterComponent>(gameObjectPlayer, textLives);

	//score counter
	auto score = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);
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
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer);
}
