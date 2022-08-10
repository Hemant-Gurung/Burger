#include "pch.h"
#include "SoloLevel.h"

#include <future>
#include <fstream>
#include "Utils.h"
#include "InputManager.h"
#include "BurgerComponent.h"
#include "ResourceManager.h"
#include "Tron.h"
#include "EnemyComponent.h"
#include "imgui.h"
#include "PepperComponent.h"
#include "SoundManager.h"
#include "SServiceLocator.h"


SoloLevel::SoloLevel()
	:GameScene("SoloLevel")
,m_PlayerPos()
,m_Score(nullptr),
m_accumulatedDeathTime(0),
m_ShowScore(false),
m_IsInitialized(false),
m_gotoLevel_2(false)
{
	m_hasOverlapped = false;
	//Initialize();
	
}
//
//SoloLevel::~SoloLevel();


void SoloLevel::Initialize()
{
	
	if (!m_IsInitialized)
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
		auto level = std::make_shared<LevelComponent>(gameObjectLevel);
		level.get()->Initialize(LEVEL_COLLISIONS[0]);
		gameObjectLevel->AddComponent(level);

		//scene.Add(gameObjectLevel);


		//auto level = gameObjectLevel->GetComponent<LevelComponent>();
		//if (m_sLevel == nullptr)
		
		//auto sLevel = std::shared_ptr <LevelComponent>(level);

		auto gameObjecttHighScore = std::make_shared<dae::GameObject>();

		auto fontScore = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
		auto textScore = std::make_shared<dae::TextComponent>(gameObjecttHighScore, " ", fontScore, SDL_Color{ 0,0,255,1 });
		auto transformHighScore = std::make_shared<dae::TransformComponent>(gameObjecttHighScore);

		m_HightestScore = std::make_shared<dae::ScoreComponent>(gameObjecttHighScore, textScore);

		// find highest score
		std::ifstream input("../Data/best_score.txt");
		int s{ -1 };
		std::string t;
		std::vector<int> scores;
		while (input >> s)
		{
			scores.push_back(s);

		}
		m_HightestScore.get()->setScore("HIGHEST SCORE: ", std::to_string(*std::max_element(scores.begin(),scores.end())));
		
		gameObjecttHighScore->AddComponent(transformHighScore);
		m_HightestScore->SetPosition(230, 5, 0);

		gameObjecttHighScore->AddComponent(m_HightestScore);

		AddChild(gameObjecttHighScore);

		AddChild(gameObjectLevel);
		//===PLAYER ONE ===========================================================>>>>>>>>>>>>>>>
		PlayerOne(level);
		//AddObserver(m_sLevel);



		// Enemy Red
		EnemyType enemy = EnemyType::Red;
		Enemy(enemy, level);
		Enemy(enemy, level);
		Enemy(enemy, level);
		Enemy(enemy, level);
		
		if (!SoundManager::GetInstance().IsSoundStreamPlaying("GameSong"))
			SoundManager::GetInstance().PlaySoundStream("GameSong", true);
		

		m_IsInitialized = true;
	}
}


void SoloLevel::Update(float dt)
{
	if (sound != nullptr)
	{
		//sound->Update();
	}
	auto indx = GamePadIndex::playerOne;

	if (dae::InputManager::GetInstance().IsPressed(static_cast<unsigned int>(dae::XBOX360Controller::ControllerButton::ButtonY), indx))
	{
		dae::InputManager::GetInstance().Quit();
	}

	if (!m_ShowScore && gameObjectPlayer!=nullptr)
	{
		auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

		if (player != nullptr && player->GetLives() <= 0)
		{
			m_ShowScore = true;

			m_HightestScore->SetPosition(50, 200, 0);
			m_HightestScore.get()->setScore(m_HightestScore.get()->ShowFinalScores(), std::to_string(player->GetScore()));

			//auto score = gameObjectPlayer->GetComponent<dae::PlayerComponent>()->GetScore();
			gameObjectPlayer->GetComponent<dae::ScoreComponent>()->setScore("SCORE: ", std::to_string(player->GetScore()));
			//m_Score.get()->setScore("SCORE: ",std::to_string(player->GetScore()));

			for (const auto& object : m_sceneObjects)
			{
				object->Update(dt);
			}

			ClearScene();
		}
		
	}
	
	//bool isPlayerOverlappingWithBurger = false;
	if (m_sceneObjects.size() > 0)
	{
		for (const auto& object : m_sceneObjects)
		{
			object->Update(dt);
			
			// get enemy0 pos
			auto enemy0 = object->GetComponent<dae::EnemyComponent>();
			if (enemy0 != nullptr && !enemy0->GetIsDead())
			{
				auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

				m_PlayerPos = player->GetPlayerPos();
				//m_sLevel.get()->SetEnemyPos(enemy0->GetEnemyPos());
				enemy0->UpdatePlayerPosInLevel(m_PlayerPos);

				m_enemyPos = enemy0->GetEnemyPos();

				if (player->CheckPlayerBulletEnemyCollision(m_enemyPos) && enemy0->GetIsDead() == false)
				{
					std::cout << "Killed" << std::endl;
					//	m_sLevel.get()->SetEnemyIsShot(true);
					enemy0->IsDead(true);
				}

			}


		}

		
			int count{};
			//check burger enemy collision
			for (const auto& object : m_sceneObjects)
			{
				auto enemy0 = object->GetComponent<dae::EnemyComponent>();
				if (enemy0 != nullptr && !enemy0->GetIsDead())
				{
					count++;
				}
			}
			if (count == 0 || m_gotoLevel_2)
			{
				//Game won
				std::cout << "Game Won";
				// pass score to the second level
				SoundManager::GetInstance().PlaySoundEffect("End",0);
				m_gotoLevel_2 = false;
				ClearScene();
				dae::InputManager::GetInstance().ResetInput();
				dae::SceneManager::GetInstance().setActive("SecondLevel");
				auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

				dae::SceneManager::GetInstance().GetGameScene("SecondLevel").get()->SetScore(player->GetScore());
				dae::SceneManager::GetInstance().GetGameScene("SecondLevel").get()->SetLives(player->GetLives());
				m_IsInitialized = false;
				//m_sLevel.reset();
				//break;
			}

		UpdateImgui();

	}
}


 void SoloLevel::CheckBurgerDropOnEnemy()
{

}

 void SoloLevel::SetScore(int score)
 {
	 auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

	 player->SetScore(score);
 }

 void SoloLevel::SetLives(int lives)
 {
	 auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

	 player->SetLives(lives);
 }


void SoloLevel::AddObserver(std::shared_ptr<dae::Observer> observer)
{
	m_Observers.push_back(observer);
}

void SoloLevel::Notify(dae::BaseComponent& actor, dae::EVENT e)
{
	for (size_t i = 0; i < m_Observers.size(); ++i)
	{
		m_Observers[i]->OnNotify(actor, e);
	}
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

	if (m_ShowScore)
	{
		m_Score.get()->Render();
		
	}

	if(m_HightestScore!=nullptr)
		m_HightestScore.get()->Render();
	
}

void SoloLevel::Enemy( EnemyType& enemytype, std::shared_ptr<LevelComponent> level)
{
	auto gameObjectEnemy = std::make_shared<dae::GameObject>();
	auto enemy = std::make_shared<dae::EnemyComponent>(gameObjectEnemy, enemytype, std::move(level));
	gameObjectEnemy->SetTag(L"Enemy");
	gameObjectEnemy->AddComponent(enemy);
	enemy->ResetEnemyPos();

	auto fontLives = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 20);
	auto textLives = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontLives, SDL_Color{ 255,255,255,1 });
	auto lives = std::make_shared<dae::LivesCounterComponent>(gameObjectPlayer, textLives);

	enemy->AddObserver(lives);

	//auto fontScore = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
	//auto textScore = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontScore, SDL_Color{ 0,0,255,1 });
	//m_Score = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);

	enemy->AddObserver(m_Score);
	//scene.Add(gameObjectEnemy);
	AddChild(gameObjectEnemy);
}

void SoloLevel::AddBurger(std::shared_ptr<LevelComponent> /*level*/)
{
	//auto gameobjectBurger = std::make_shared<dae::GameObject>();

	//auto fontScore = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
	//auto textScore = std::make_shared<dae::TextComponent>(gameObjectEnemy, " ", fontScore, SDL_Color{ 255,255,255,1 });
	//auto enemyObserver = std::make_shared<dae::ScoreComponent>(gameObjectEnemy, textScore);
	////auto burgerObserver = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);
	//
	//float4 burgerPos;
	////Bottom
	//burgerPos.one = 25.f;
	//burgerPos.two = 123.6f;
	//burgerPos.three = 224.7f;
	//burgerPos.four = 370.78f;

	//Point2f leftPos = Point2f( 55.f,590.f);
	////float lastpos = 590.f;
	//auto burger = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos, burgerPos);
	////burger->AddObserver(burgerObserver);
	//burger->AddObserver(enemyObserver);
	//gameobjectBurger->AddComponent(burger);

	////float4 burgerPos;
	////Bottom
	//burgerPos.one = 173.f;
	//burgerPos.two = 275.2f;
	//burgerPos.three = 369.7f;
	//burgerPos.four = 589.f;

	//Point2f leftPos2 = Point2f(202.2f,590.f);
	//auto burger2 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos2, burgerPos);
	//burger2->AddObserver(enemyObserver);
	////burger2->AddObserver(burgerObserver);
	//gameobjectBurger->AddComponent(burger2);

	//burgerPos.one = 125.f;
	//burgerPos.two = 275.2f;
	//burgerPos.three = 369.7f;
	//burgerPos.four = 471.9f;

	//Point2f leftPos3 = Point2f(348.2f,590.f);
	//auto burger3 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos3, burgerPos);
	//burger3->AddObserver(enemyObserver);
	////burger3->AddObserver(burgerObserver);
	//gameobjectBurger->AddComponent(burger3);


	//burgerPos.one = 125.8f;
	//burgerPos.two = 220.83f;
	//burgerPos.three = 322.225f;
	//burgerPos.four = 471.9f;

	//Point2f leftPos4 = Point2f(505.f,590.f);
	//auto burger4 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos4, burgerPos);
	//burger4->AddObserver(enemyObserver);
	////burger4->AddObserver(burgerObserver);
	//gameobjectBurger->AddComponent(burger4);


	//gameobjectBurger->SetTag(L"Burger");
	////scene.Add(gameobjectBurger);
	//AddChild(gameobjectBurger);
}

void SoloLevel::PlayerOne(std::shared_ptr<LevelComponent> slevel)
{
	gameObjectPlayer = std::make_shared<dae::GameObject>();

	//Do this inside player class
	//player commands\
	//if (dae::Input::GetInstance().m_ConsoleCommands.size() <= 0)
	{
		//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::ShoulderButtonRight), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer,1.f), VK_RIGHT, GamePadIndex::playerOne));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::ShoulderButtonLeft), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer,-1.f), VK_LEFT, GamePadIndex::playerOne));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonX), InputAction(std::make_unique<ShootBullet>(gameObjectPlayer), 'Q', GamePadIndex::playerOne));

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
	//auto player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, slevel);
	auto Player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, std::move(slevel));
	//m_Player = player;

	//add gameobject to scene
	//scene.Add(gameObjectPlayer);


	gameObjectPlayer->AddComponent(Player);
	//gameObjectPlayer->GetComponent<TransformComponent>()->SetPosition(200.f, 10.f, 0.f);
	//score
	// gameObjLivesCounter = std::make_shared<GameObject>();

	//make transform component for the game object
	auto transformLives = std::make_shared<dae::TransformComponent>(gameObjectPlayer);

	//make text component
	auto fontLives = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 20);
	auto textLives = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontLives, SDL_Color{ 255,255,255,1 });
	auto fontScore = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
	auto textScore = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontScore, SDL_Color{ 0,0,255,1 });
	auto textpepper = std::make_shared<dae::TextComponent>(gameObjectPlayer, " ", fontLives, SDL_Color{ 255,255,255,1 });
	// score achievement 
	
	auto lives = std::make_shared<dae::LivesCounterComponent>(gameObjectPlayer, textLives);
	
	m_Score = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);
	

	auto pepperComponent = std::make_shared<PepperComponent>(gameObjectPlayer, textpepper);

	//add observers
	{
		Player->AddObserver(pepperComponent);

		Player->AddObserver(lives);

		//add score observer
		Player->AddObserver(m_Score);
		
	}
	
	//add gameobject scoreAchievement to gameobject
	//gameObjectPlayer->AddComponent(scoreAchievement);
	//add transform component to gameobject
	gameObjectPlayer->AddComponent(transformLives);

	//add counter to gameobject
	gameObjectPlayer->AddComponent(lives);

	//add score component to gameobject
	gameObjectPlayer->AddComponent(m_Score);

	gameObjectPlayer->AddComponent(pepperComponent);

	//add text component for lives
	//gameObjectPlayer->AddComponent(textLives);
	//add textComponent for score
	gameObjectPlayer->AddComponent(textScore);


	gameObjectPlayer->AddComponent(textpepper);

	
	//set score position
	textScore->SetPosition(130, 50, 0);

	//set lives position
	//textLives->SetPosition(570, 5, 0);

	pepperComponent->SetPosition(560, 5, 0);

	gameObjectPlayer->SetTag(L"Player");
	//add to scent
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer);
}


void SoloLevel::GenerateEnemies()
{
	//EnemyType enemy = EnemyType::Red;
	
	//Enemy(enemy, m_sLevel);
	//Enemy(enemy, m_sLevel);
	
}

void SoloLevel::UpdateImgui()
{
	ImGui::Begin("GotoLevel2", NULL);
	
	//ImGui::SetWindowSize(ImVec2((float)10.f, (float)10.f));
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
	ImGui::Checkbox("GotoLevel2", &m_gotoLevel_2);

	ImGui::End();
	
}
