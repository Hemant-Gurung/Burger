#include "pch.h"
#include "CoopLevel.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "ScoreComponent.h"
#include "EnemyComponent.h"
#include "LivesCounterComponent.h"

CoopLevel::CoopLevel()
	:GameScene("CoopLevel"), m_RestartGame(false),m_AccumulatedSec(0)

{
}

void CoopLevel::Update(float dt)
{
	if (!m_RestartGame)
	{

		for (const auto& object : m_sceneObjects)
		{
			if (object != nullptr)
			{
				object->Update(dt);

				auto player = object->GetComponent<dae::PlayerComponent>();
				if (player != nullptr && player->GetTag() == "Player1")
				{
					m_Player1Pos = player->GetPlayerPos();
				}
				if (player != nullptr && player->GetTag() == "Player2")
				{
					m_Player2Pos = player->GetPlayerPos();
				}
			}
			UpdateIfGameIsWon();
		}
		
	}
	UpdateGameLogic();

}

void CoopLevel::Render()
{
	for (const auto& object : m_sceneObjects)
	{
		object->Render();
		}
}

void CoopLevel::Initialize()
{
	dae::ResourceManager::GetInstance().Init("../Data/");

	auto gameObjectLevel = std::make_shared<dae::GameObject>();
	////make render component
	auto Renderlevel = std::make_shared<dae::RenderComponent>(gameObjectLevel);
	auto transformLevel_1 = std::make_shared<dae::TransformComponent>(gameObjectLevel);
	auto level = std::make_shared<LevelComponent>(gameObjectLevel);
	Renderlevel->SetTexture(LEVELS[1]);

	// get level collider information 
	level.get()->Initialize(LEVEL_COLLISIONS[1]);
	gameObjectLevel->AddComponent(transformLevel_1);
	gameObjectLevel->AddComponent(Renderlevel);
	gameObjectLevel->AddComponent(level);
	AddChild(gameObjectLevel);

	/*auto gbWinnerPlayer = std::make_shared<dae::GameObject>();
	auto renderWinnerPlayer = std::make_shared<dae::RenderComponent>(gbWinnerPlayer);
	gbWinnerPlayer->AddComponent(renderWinnerPlayer);
	renderWinnerPlayer->SetPosition(256, 240,0);
	gbWinnerPlayer->SetTag(L"Winner");*/

	//AddChild(gbWinnerPlayer);

	PlayerOne(level);
	PlayerTwo(level);


	EnemyType enemy = EnemyType::Red;
	Enemy(enemy, level);
	Enemy(enemy, level);
	Enemy(enemy, level);
	Enemy(enemy, level);

	if (!SoundManager::GetInstance().IsSoundStreamPlaying("GameSong"))
		SoundManager::GetInstance().PlaySoundStream("GameSong", true);

}

void CoopLevel::FixedUpdate()
{
}

void CoopLevel::PlayerOne(std::shared_ptr<LevelComponent> slevel)
{
	auto gameObjectPlayer = std::make_shared<dae::GameObject>();

	//Do this inside player class
	//player commands\
	//if (dae::Input::GetInstance().m_ConsoleCommands.size() <= 0)
	{
		//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::ShoulderButtonRight), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer, 1.f), VK_RIGHT, GamePadIndex::playerOne));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::ShoulderButtonLeft), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer, -1.f), VK_LEFT, GamePadIndex::playerOne));
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

	auto m_Score = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);

	//add observers
	{
		Player->AddObserver(lives);

		//add score observer
		Player->AddObserver(m_Score);

	}
	gameObjectPlayer->AddComponent(transformLives);
	gameObjectPlayer->AddComponent(lives);
	gameObjectPlayer->AddComponent(m_Score);
	gameObjectPlayer->AddComponent(textScore);
	gameObjectPlayer->AddComponent(textpepper);


	//set score position
	textScore->SetPosition(10, 50, 0);
	lives->SetPosition(10, 20, 0);
	Player->SetTag("Player1");
	gameObjectPlayer->SetTag(L"Player1");
	//add to scent
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer);
}

void CoopLevel::PlayerTwo(std::shared_ptr<LevelComponent> slevel)
{
	auto gameObjectPlayer1 = std::make_shared<dae::GameObject>();

	//Do this inside player class
	//player commands\
	//if (dae::Input::GetInstance().m_ConsoleCommands.size() <= 0)
	{
		//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::ShoulderButtonRight1), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer1, 1.f), VK_RIGHT, GamePadIndex::playerTwo));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::ShoulderButtonLeft1), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer1, -1.f), VK_LEFT, GamePadIndex::playerTwo));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down1, dae::XBOX360Controller::ControllerButton::ButtonX1), InputAction(std::make_unique<ShootBullet>(gameObjectPlayer1), 'Q', GamePadIndex::playerTwo));

		//dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonB), InputAction(std::make_unique<ScoreCommand>(gameObjectPlayer), 'P', GamePadIndex::playerOne));
		//RIGHT
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadRight1), InputAction(std::make_unique<MoveRightCommand>(gameObjectPlayer1), 'D', GamePadIndex::playerTwo));
		//LEFT
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadLeft1), InputAction(std::make_unique<MoveLeftCommand>(gameObjectPlayer1), 'A', GamePadIndex::playerTwo));
		//UP
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadUp1), InputAction(std::make_unique<MoveUpCommand>(gameObjectPlayer1), 'W', GamePadIndex::playerTwo));
		//DOWN
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadDown1), InputAction(std::make_unique<MoveDownCommand>(gameObjectPlayer1), 'S', GamePadIndex::playerTwo));
	}


	auto transformPlayer1 = std::make_shared<dae::TransformComponent>(gameObjectPlayer1);
	//transformPlayer1->SetPosition(216, 180, 0);

	//player observer

	gameObjectPlayer1->AddComponent(transformPlayer1);
	//player one
	//auto player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, slevel);
	auto Player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer1, std::move(slevel), "PlayerEnemyTank.png");
	//m_Player = player;

	//add gameobject to scene
	//scene.Add(gameObjectPlayer);


	gameObjectPlayer1->AddComponent(Player);
	//gameObjectPlayer->GetComponent<TransformComponent>()->SetPosition(200.f, 10.f, 0.f);
	//score
	// gameObjLivesCounter = std::make_shared<GameObject>();

	//make transform component for the game object
	auto transformLives = std::make_shared<dae::TransformComponent>(gameObjectPlayer1);

	//make text component
	auto fontLives = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 20);
	auto textLives = std::make_shared<dae::TextComponent>(gameObjectPlayer1, " ", fontLives, SDL_Color{ 255,255,255,1 });
	auto fontScore = dae::ResourceManager::GetInstance().LoadFont("VPPixel-Simplified.otf", 16);
	auto textScore = std::make_shared<dae::TextComponent>(gameObjectPlayer1, " ", fontScore, SDL_Color{ 0,0,255,1 });
	auto textpepper = std::make_shared<dae::TextComponent>(gameObjectPlayer1, " ", fontLives, SDL_Color{ 255,255,255,1 });
	// score achievement 

	auto lives = std::make_shared<dae::LivesCounterComponent>(gameObjectPlayer1, textLives);

	auto m_Score = std::make_shared<dae::ScoreComponent>(gameObjectPlayer1, textScore);

	//add observers
	{
		Player->AddObserver(lives);

		//add score observer
		Player->AddObserver(m_Score);

	}
	gameObjectPlayer1->AddComponent(transformLives);
	gameObjectPlayer1->AddComponent(lives);
	gameObjectPlayer1->AddComponent(m_Score);
	gameObjectPlayer1->AddComponent(textScore);
	gameObjectPlayer1->AddComponent(textpepper);

	lives->SetPosition(350, 20, 0);

	//set score position
	textScore->SetPosition(330, 50, 0);
	Player->SetTag("Player2");
	gameObjectPlayer1->SetTag(L"Player2");

	//add to scent
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer1);
}

void CoopLevel::UpdateGameLogic()
{

	LevelComponent* level =nullptr;
	for (auto& obj:m_sceneObjects)
	{
		if(level==nullptr) level = obj->GetComponent<LevelComponent>();
		if (level != nullptr)
		{
			level->SetPlayerPosInLevel(m_Player1Pos);
			break;
		}
	}

	dae::PlayerComponent* player1 = nullptr;
	dae::PlayerComponent* player2 = nullptr;

	for (auto& obj : m_sceneObjects)
	{
		if (obj->GetTag() == L"Player1")
		{
			player1 = obj->GetComponent<dae::PlayerComponent>();
			
		}
		if (obj->GetTag() == L"Player2")
		{
			player2 = obj->GetComponent<dae::PlayerComponent>();
		}

		
		//

		if (player1 != nullptr && player2 != nullptr)
		{
			if (player1->GetLives() <= 0 || player2->GetLives() <= 0)
			{
				ResetLevel();
			}
			m_Player1Pos = player1->GetPlayerPos();
			level->SetPlayerPosInLevel(m_Player1Pos);
		}

		if (player2 != nullptr && player1 != nullptr)
		{
			m_Player2Pos = player2->GetPlayerPos();
			level->SetPlayer2PosInLevel(m_Player2Pos);
		}
	}



	for(auto& obj:m_sceneObjects)
	{
		auto enemy0 = obj->GetComponent<dae::EnemyComponent>();
		if (enemy0 != nullptr)
		{
			m_EnemyPos = enemy0->GetEnemyPos();
			enemy0->UpdatePlayerPosInLevel(m_Player1Pos);
			bool hit = enemy0->CheckIfPlayerIsHit();
			if(hit)
			{
				player1->CallPlayerIsDead();
			}
			enemy0->UpdatePlayerPosInLevel(m_Player2Pos);
			 hit = enemy0->CheckIfPlayer2IsHit();
			if (hit)
			{
				player2->CallPlayerIsDead();
			}
			if (player1!=nullptr && player1->CheckPlayerBulletEnemyCollision(m_EnemyPos) && enemy0->GetIsDead() == false)
			{
				//	m_sLevel.get()->SetEnemyIsShot(true);
				enemy0->IsDead(true);
				break;
			}
			if (player2 != nullptr && player2->CheckPlayerBulletEnemyCollision(m_EnemyPos) && enemy0->GetIsDead() == false)
			{
				//	m_sLevel.get()->SetEnemyIsShot(true);
				enemy0->IsDead(true);
				break;
			}
		}
	}

	if (m_RestartGame)
	{
		m_AccumulatedSec += 0.1f;
		if (m_AccumulatedSec > 50)
		{
			m_RestartGame = false;
			m_AccumulatedSec = 0;
		}
	}
}

void CoopLevel::UpdateIfGameIsWon()
{
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
	if (count == 0)
	{
		//Game won
		std::cout << "Game Won";
		ResetLevel();
	}
}

void CoopLevel::Enemy(EnemyType& enemytype, std::shared_ptr<LevelComponent> level)
{
	auto gameObjectEnemy = std::make_shared<dae::GameObject>();

	gameObjectEnemy = std::make_shared<dae::GameObject>();
	auto enemy = std::make_shared<dae::EnemyComponent>(gameObjectEnemy, enemytype, std::move(level));
	gameObjectEnemy->SetTag(L"Enemy");
	gameObjectEnemy->AddComponent(enemy);
	enemy->ResetEnemyPos();

	AddChild(gameObjectEnemy);
}

void CoopLevel::ResetLevel()
{
	m_RestartGame = false;
	SoundManager::GetInstance().PlaySoundEffect("End", 0);

	for (auto object : m_sceneObjects)
	{
		auto player1 = object->GetComponentByTag("Player1");
		auto  player2 = object->GetComponentByTag("Player2");

		if (player1 != nullptr)
		{
			player1->SetLives(3);
			player1->InitializeDestRect();
		}
		if (player2 != nullptr)
		{
			player2->SetLives(3);
			player2->InitializeDestRect();
		}
	}

	
	for (auto object : m_sceneObjects)
	{
		auto enemy = object->GetComponent<dae::EnemyComponent>();
		if(enemy!=nullptr)
		{
			enemy->ResetEnemyLife(false);
		}
	}

		
	
}

void CoopLevel::CheckIfPlayerLost()
{

}
