#include "pch.h"
#include "VersusLevel.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "ScoreComponent.h"
#include "LivesCounterComponent.h"

VersusLevel::VersusLevel()
	:GameScene("VersusLevel")
	, m_IsP1Damaged(false),
	m_IsP2Damaged(false),
	m_RestartGame(false),
m_AccumulatedSec(0)
{

}

void VersusLevel::Update(float dt)
{
	if (!m_RestartGame)
	{

		for (const auto& object : m_sceneObjects)
		{
			object->Update(dt);

		}
	}
	UpdateGameLogic();

}

void VersusLevel::Render()
{

	for (const auto& object : m_sceneObjects)
	{
		if (object->GetTag() == L"Winner")
		{
			object->Render();
		}
		else
		{
			object->Render();
		}
	}

}

void VersusLevel::Initialize()
{
	dae::ResourceManager::GetInstance().Init("../Data/");

	auto gameObjectLevel = std::make_shared<dae::GameObject>();
	////make render component
	auto Renderlevel = std::make_shared<dae::RenderComponent>(gameObjectLevel);
	auto transformLevel_1 = std::make_shared<dae::TransformComponent>(gameObjectLevel);
	auto level = std::make_shared<LevelComponent>(gameObjectLevel);
	Renderlevel->SetTexture(LEVELS[2]);

	// get level collider information 
	level.get()->Initialize(LEVEL_COLLISIONS[2]);
	gameObjectLevel->AddComponent(transformLevel_1);
	gameObjectLevel->AddComponent(Renderlevel);
	gameObjectLevel->AddComponent(level);
	AddChild(gameObjectLevel);

	PlayerOne(level);
	PlayerTwo(level);

	if (!SoundManager::GetInstance().IsSoundStreamPlaying("GameSong"))
		SoundManager::GetInstance().PlaySoundStream("GameSong", true);
}

void VersusLevel::FixedUpdate()
{
}

void VersusLevel::PlayerOne(std::shared_ptr<LevelComponent> slevel)
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
	//add to scent
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer);
}

void VersusLevel::PlayerTwo(std::shared_ptr<LevelComponent> slevel)
{
	auto gameObjectPlayer = std::make_shared<dae::GameObject>();

	//Do this inside player class
	//player commands\
	//if (dae::Input::GetInstance().m_ConsoleCommands.size() <= 0)
	{
		//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::ShoulderButtonRight1), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer, 1.f), VK_RIGHT, GamePadIndex::playerTwo));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::ShoulderButtonLeft1), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer, -1.f), VK_LEFT, GamePadIndex::playerTwo));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down1, dae::XBOX360Controller::ControllerButton::ButtonX1), InputAction(std::make_unique<ShootBullet>(gameObjectPlayer), 'Q', GamePadIndex::playerTwo));

		//dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonB), InputAction(std::make_unique<ScoreCommand>(gameObjectPlayer), 'P', GamePadIndex::playerOne));
		//RIGHT
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadRight1), InputAction(std::make_unique<MoveRightCommand>(gameObjectPlayer), 'D', GamePadIndex::playerTwo));
		//LEFT
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadLeft1), InputAction(std::make_unique<MoveLeftCommand>(gameObjectPlayer), 'A', GamePadIndex::playerTwo));
		//UP
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadUp1), InputAction(std::make_unique<MoveUpCommand>(gameObjectPlayer), 'W', GamePadIndex::playerTwo));
		//DOWN
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held1, dae::XBOX360Controller::ControllerButton::DpadDown1), InputAction(std::make_unique<MoveDownCommand>(gameObjectPlayer), 'S', GamePadIndex::playerTwo));
	}


	auto transformPlayer1 = std::make_shared<dae::TransformComponent>(gameObjectPlayer);
	//transformPlayer1->SetPosition(216, 180, 0);

	//player observer

	gameObjectPlayer->AddComponent(transformPlayer1);
	//player one
	//auto player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, slevel);
	auto Player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, std::move(slevel), "PlayerEnemyTank.png");
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

	lives->SetPosition(350, 20, 0);

	//set score position
	textScore->SetPosition(330, 50, 0);
	Player->SetTag("Player2");
	//add to scent
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer);
}

void VersusLevel::UpdateGameLogic()
{
	for (auto object : m_sceneObjects)
	{
		auto player1 = object->GetComponentByTag("Player1");
		auto  player2 = object->GetComponentByTag("Player2");
		if (player1 != nullptr)
			m_player1Pos = player1->GetPlayerPos();

		if (player2 != nullptr)
			m_player2Pos = player2->GetPlayerPos();

		if (player1 != nullptr)
		{
			if (player1->CheckPlayerBulletEnemyCollision(m_player2Pos))
			{
				//player1->CheckPlayerBulletEnemyCollision(player2->GetPosition());
				m_IsP2Damaged = true;
				std::cout << "player2Dead";
			}
		}

		if (player2 != nullptr)
		{
			if (player2->CheckPlayerBulletEnemyCollision(m_player1Pos))
			{
				m_IsP1Damaged = true;
				//player1->CheckPlayerBulletEnemyCollision(player2->GetPosition());
				std::cout << "Player1Dead";
			}
		}
		// call damage on player
		if (m_IsP1Damaged && player1)
		{
			player1->CallPlayerIsDead();
			CheckIfPlayerLost(*player1);

			m_IsP1Damaged = false;
		}
		if (m_IsP2Damaged && player2)
		{
			player2->CallPlayerIsDead();
			CheckIfPlayerLost(*player2);

			m_IsP2Damaged = false;
		}
	}

	if(m_RestartGame)
	{
		m_AccumulatedSec += 0.1f;
		if(m_AccumulatedSec >50)
		{
			m_RestartGame = false;
			m_AccumulatedSec = 0;
		}
	}


}

void VersusLevel::CheckIfPlayerLost(const dae::PlayerComponent& comp)
{
	if (comp.GetLives() <= 0)
	{
		ResetLevel();
	}
}

void VersusLevel::ResetLevel()
{	
	m_RestartGame = true;
	SoundManager::GetInstance().PlaySoundEffect("End", 0);

	for (auto object : m_sceneObjects)
	{
		auto player1 = object->GetComponentByTag("Player1");
		auto  player2 = object->GetComponentByTag("Player2");

		if (player1!=nullptr)
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

}


