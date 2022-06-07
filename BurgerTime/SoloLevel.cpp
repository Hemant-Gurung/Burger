#include "pch.h"
#include "SoloLevel.h"

#include <future>

#include "Utils.h"
#include "InputManager.h"
#include "BurgerComponent.h"
#include "ResourceManager.h"
#include "BurgerGame.h"
#include "EnemyComponent.h"
#include "PepperComponent.h"
#include "SServiceLocator.h"


SoloLevel::SoloLevel()
	:GameScene("SoloLevel")
,m_PlayerPos(),
m_accumulatedDeathTime(0)
{
	m_hasOverlapped = false;
	//Initialize();
}
//
//SoloLevel::~SoloLevel();


void SoloLevel::Initialize()
{
	//auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	m_hasOverlapped = false;
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


	auto level =gameObjectLevel->GetComponent<LevelComponent>();
	auto m_sLevel = std::shared_ptr<LevelComponent>(level);
	
	AddChild(gameObjectLevel);
	//===PLAYER ONE ===========================================================>>>>>>>>>>>>>>>
	//auto gameObjectEn = std::make_shared<dae::GameObject>();

	PlayerOne(m_sLevel);

	
	auto gameObjectEnemy = std::make_shared<dae::GameObject>();


	// Enemy Red
	EnemyType enemy = EnemyType::Red;
	Enemy(gameObjectEnemy, enemy, m_sLevel);
	Enemy(gameObjectEnemy,enemy,m_sLevel);
	Enemy(gameObjectEnemy,enemy, m_sLevel);
	Enemy(gameObjectEnemy,enemy, m_sLevel);

	//Enemy Egg
	enemy = EnemyType::Egg;
	Enemy(gameObjectEnemy,enemy, m_sLevel);
	Enemy(gameObjectEnemy,enemy, m_sLevel);

	AddBurger(gameObjectEnemy,m_sLevel);

	auto f1 = std::async(&SServiceLocator::get_sound_system);
	sound = &f1.get();
	sound->Play(SoundID::GAMESOUND, 50);
	
}


void SoloLevel::Update(float dt)
{
	if (sound != nullptr)
	{
		//sound->Update();
	}

	//bool isPlayerOverlappingWithBurger = false;
	if(m_sceneObjects.size()>0)
	{
		for (const auto& object : m_sceneObjects)
		{
			// get enemy0 pos
			auto enemy0 = object->GetComponent<dae::EnemyComponent>();
			if (enemy0 != nullptr && !enemy0->GetIsDead())
			{
				
				m_enemyPos = enemy0->GetEnemyPos();
				
			}

			

			if (m_Player.lock() != nullptr)
			{
				if (object->GetTag() == L"Player1")
				{
					m_PlayerPos = m_Player.lock()->GetPlayerPos();
				}
				CheckIfPlayerIsDead(*m_Player.lock());

			}

			// check for game win
			if (object->GetTag() == L"Burger")
			{

				//check burger enemy collision



				int count = 0;
				for (auto burger : object.get()->GetAllcomponents())
				{
					
					if (burger->GetPosition().bottom > 480)
					{
						count++;
					}
				}
				if (count >= 4)
				{
					//Game won
					std::cout << "Game Won";
					ClearScene();
				    dae::InputManager::GetInstance().ResetInput();
					dae::SceneManager::GetInstance().setActive("SecondLevel");
					break;
				}
			}

			//if(player)

			object->Update(dt);
		}
	}


	if (m_hasOverlapped)
	{
		m_accumulatedDeathTime += 10 * dt;
		if (m_accumulatedDeathTime >= 50.f)
		{
			m_accumulatedDeathTime = 0;
			ResetScene();
			m_hasOverlapped = false;
		}
	}
}


 void SoloLevel::CheckBurgerDropOnEnemy()
{

}


bool SoloLevel::CheckIfPlayerIsDead(dae::PlayerComponent& player)
{
	if(m_Player.lock()->GetLives() <=0)
	{
		ClearScene();
		dae::SceneManager::GetInstance().setActive("startScreen");
		
	}
	
	else if (!m_hasOverlapped && m_enemyPos.left!= 0 && utils::IsOverlapping(m_PlayerPos, m_enemyPos))
	{

		m_hasOverlapped = true;
		player.CallPlayerIsDead();
	
		return true;
	}
	return false;
}



void SoloLevel::FixedUpdate()
{
}

void SoloLevel::Render()
{
	for (const auto& object : m_sceneObjects)
	{
		object->Render();
	}
}

void SoloLevel::Enemy(std::shared_ptr<dae::GameObject>& gameObjectEnemy,EnemyType& enemytype, std::shared_ptr<LevelComponent> level)
{
	//gameObjectEnemy = std::make_shared<dae::GameObject>();
	auto enemy = std::make_shared<dae::EnemyComponent>(gameObjectEnemy, enemytype, std::move(level));
	gameObjectEnemy->SetTag(L"Enemy");
	gameObjectEnemy->AddComponent(enemy);
	enemy->ResetEnemyPos();
	//scene.Add(gameObjectEnemy);
	AddChild(gameObjectEnemy);
}

void SoloLevel::AddBurger(std::shared_ptr<dae::GameObject>& gameObjectEnemy,std::shared_ptr<LevelComponent> level)
{
	auto gameobjectBurger = std::make_shared<dae::GameObject>();

	auto fontScore = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
	auto textScore = std::make_shared<dae::TextComponent>(gameObjectEnemy, " ", fontScore, SDL_Color{ 255,255,255,1 });
	auto enemyObserver = std::make_shared<dae::ScoreComponent>(gameObjectEnemy, textScore);
	//auto burgerObserver = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);
	
	float4 burgerPos;
	//Bottom
	burgerPos.one = 25.f;
	burgerPos.two = 123.6f;
	burgerPos.three = 224.7f;
	burgerPos.four = 370.78f;

	Point2f leftPos = Point2f( 55.f,590.f);
	//float lastpos = 590.f;
	auto burger = std::make_shared<BurgerComponent>(gameobjectBurger, std::move(level), leftPos, burgerPos);
	//burger->AddObserver(burgerObserver);
	burger->AddObserver(enemyObserver);
	gameobjectBurger->AddComponent(burger);

	//float4 burgerPos;
	//Bottom
	burgerPos.one = 173.f;
	burgerPos.two = 275.2f;
	burgerPos.three = 369.7f;
	burgerPos.four = 589.f;

	Point2f leftPos2 = Point2f(202.2f,590.f);
	auto burger2 = std::make_shared<BurgerComponent>(gameobjectBurger, std::move(level), leftPos2, burgerPos);
	burger2->AddObserver(enemyObserver);
	//burger2->AddObserver(burgerObserver);
	gameobjectBurger->AddComponent(burger2);

	burgerPos.one = 125.f;
	burgerPos.two = 275.2f;
	burgerPos.three = 369.7f;
	burgerPos.four = 471.9f;

	Point2f leftPos3 = Point2f(348.2f,590.f);
	auto burger3 = std::make_shared<BurgerComponent>(gameobjectBurger, std::move(level), leftPos3, burgerPos);
	burger3->AddObserver(enemyObserver);
	//burger3->AddObserver(burgerObserver);
	gameobjectBurger->AddComponent(burger3);


	burgerPos.one = 125.8f;
	burgerPos.two = 220.83f;
	burgerPos.three = 322.225f;
	burgerPos.four = 471.9f;

	Point2f leftPos4 = Point2f(505.f,590.f);
	auto burger4 = std::make_shared<BurgerComponent>(gameobjectBurger, std::move(level), leftPos4, burgerPos);
	burger4->AddObserver(enemyObserver);
	//burger4->AddObserver(burgerObserver);
	gameobjectBurger->AddComponent(burger4);


	gameobjectBurger->SetTag(L"Burger");
	//scene.Add(gameobjectBurger);
	AddChild(gameobjectBurger);
}

void SoloLevel::PlayerOne( std::shared_ptr<LevelComponent> slevel)
{
	auto gameObjectPlayer = std::make_shared<dae::GameObject>();

	//Do this inside player class
	//player commands
	//if (dae::Input::GetInstance().m_ConsoleCommands.size() <= 0)
	{
		//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonA), InputAction(std::make_unique<ThrowPepperCommand>(gameObjectPlayer), 'O', GamePadIndex::playerOne));
		//dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonB), InputAction(std::make_unique<ScoreCommand>(gameObjectPlayer), 'P', GamePadIndex::playerOne));
		//RIGHT
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadRight), InputAction(std::make_unique<MoveRightCommand>(gameObjectPlayer), 'D', GamePadIndex::playerOne));
		//LEFT
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadLeft), InputAction(std::make_unique<MoveLeftCommand>(gameObjectPlayer), 'A', GamePadIndex::playerOne));
		//UP
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadUp), InputAction(std::make_unique<MoveUpCommand>(gameObjectPlayer), 'W', GamePadIndex::playerOne));
		//DOWN
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadDown), InputAction(std::make_unique<MoveDownCommand>(gameObjectPlayer), 'S', GamePadIndex::playerOne));
	}
	

	auto transformPlayer1 = std::make_shared<dae::TransformComponent>(gameObjectPlayer);
	//transformPlayer1->SetPosition(216, 180, 0);

	//player observer

	gameObjectPlayer->AddComponent(transformPlayer1);
	//player one
	auto player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, std::move(slevel));
	m_Player = player;

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
	auto textpepper = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontLives, SDL_Color{ 255,255,255,1 });
	// score achievement 
	//auto scoreAchievement = std::make_shared<Achievements>(g_Achievements, 4, gameObjectPlayer);

	//make livescounter component/ observer
	//if (m_lives == nullptr)
	//{
		auto lives = std::make_shared<dae::LivesCounterComponent>(gameObjectPlayer, textLives);
		//m_lives = lives;
	//}
	//score counter
	//if (m_Score == nullptr)
	//{
		auto score = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);
		//m_Score = score;
	//}

	auto pepperComponent = std::make_shared<PepperComponent>(gameObjectPlayer, textpepper);

	//add observers
	{
		player->AddObserver(pepperComponent);

		player->AddObserver(lives);

		//add score observer
		player->AddObserver(score);
	}
	//add gameobject scoreAchievement to gameobject
	//gameObjectPlayer->AddComponent(scoreAchievement);
	//add transform component to gameobject
	gameObjectPlayer->AddComponent(transformLives);

	//add counter to gameobject
	gameObjectPlayer->AddComponent(lives);

	//add score component to gameobject
	gameObjectPlayer->AddComponent(score);

	gameObjectPlayer->AddComponent(pepperComponent);

	//add text component for lives
	//gameObjectPlayer->AddComponent(textLives);
	//add textComponent for score
	gameObjectPlayer->AddComponent(textScore);


	gameObjectPlayer->AddComponent(textpepper);

	
	//set score position
	textScore->SetPosition(350, 5, 0);

	//set lives position
	//textLives->SetPosition(570, 5, 0);

	pepperComponent->SetPosition(560, 5, 0);

	gameObjectPlayer->SetTag(L"Player1");
	//add to scent
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer);
}

void SoloLevel::ResetScene()
{
	//ClearScene();
	m_Player.lock()->SetPlayerStartPosition(Point2f{ 450.f,450.f });
	m_PlayerPos = m_Player.lock()->GetPlayerPos();

	auto playerState = dae::PlayerState::standing;
	m_Player.lock()->ChangeState(playerState);

	for (auto obj : m_sceneObjects)
	{
		if(obj->GetTag() == L"Enemy")
		{
			obj->GetComponent<dae::EnemyComponent>()->ResetEnemyPos();
		}
	}

	GenerateEnemies();
	

}

void SoloLevel::GenerateEnemies()
{
	/*EnemyType enemy = EnemyType::Red;
	
	Enemy(enemy, m_sLevel);
	Enemy(enemy, m_sLevel);
	Enemy(enemy, m_sLevel);

	enemy = EnemyType::Egg;
	Enemy(enemy, m_sLevel);
	Enemy(enemy, m_sLevel);*/
	
}
