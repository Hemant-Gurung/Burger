#include "pch.h"
#include "SecondLevel.h"
#include "BurgerComponent.h"
#include "EnemyComponent.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include <algorithm>
#include "Utils.h"
#include <fstream>

#include "imgui.h"
#include "SoundManager.h"

SecondLevel::SecondLevel()
	:GameScene("SecondLevel")
	, m_PlayerPos()
	, m_Player(nullptr)
	, m_lives(nullptr)
	, m_Score(nullptr),
	m_accumulatedDeathTime(0),
	m_IsInitialized(false),
	m_ShowScore(false)
	, m_GotoLevel3(false)
{
	m_hasOverlapped = false;
}

void SecondLevel::Initialize()
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
		Renderlevel->SetTexture(LEVELS[1]);
		gameObjectLevel->AddComponent(Renderlevel);
		////ADD LEVEL SKELETON
		auto level = std::make_shared<LevelComponent>(gameObjectLevel);
		level.get()->Initialize(LEVEL_COLLISIONS[1]);
		gameObjectLevel->AddComponent(level);

		//scene.Add(gameObjectLevel);
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
		m_HightestScore.get()->setScore("HIGHEST SCORE: ", std::to_string(*std::max_element(scores.begin(), scores.end())));

		gameObjecttHighScore->AddComponent(transformHighScore);
		m_HightestScore->SetPosition(260, 5, 0);

		gameObjecttHighScore->AddComponent(m_HightestScore);

		AddChild(gameObjecttHighScore);



		//auto Level = gameObjectLevel->GetComponent<LevelComponent>();
		//m_sLevel = std::shared_ptr<LevelComponent>(Level);
		AddChild(gameObjectLevel);
		//===PLAYER ONE ===========================================================>>>>>>>>>>>>>>>
		PlayerOne(level);

		//AddBurger(m_sLevel);


		// Enemy Red
		EnemyType enemy = EnemyType::Egg;
		Enemy(enemy, level);
		Enemy(enemy, level);
		Enemy(enemy, level);


		//Enemy Egg
		/*enemy = EnemyType::Egg;
		Enemy(enemy, m_sLevel);*/

	}
}

void SecondLevel::Update(float dt)
{
	

	if (!m_ShowScore && gameObjectPlayer != nullptr)
	{
		auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

		if (player != nullptr && player->GetLives() <= 0)
		{
			m_ShowScore = true;
			m_HightestScore->SetPosition(50, 200, 0);
			m_HightestScore.get()->setScore(m_HightestScore.get()->ShowFinalScores(), std::to_string(player->GetScore()));
			//m_Score.get()->setScore("SCORE: ", std::to_string(m_Player.get()->GetScore()));

			gameObjectPlayer->GetComponent<dae::ScoreComponent>()->setScore("SCORE: ", std::to_string(player->GetScore()));
			//m_HightestScore.get()->setScore(m_HightestScore.get()->ShowFinalScores(), "0");

			for (const auto& object : m_sceneObjects)
			{
				object->Update(dt);
			}
			ClearScene();
		}
		//dae::SceneManager::GetInstance().GetGameScene("ScoreScene")->SetScore(m_Player->GetScore());
		//dae::SceneManager::GetInstance().setActive("ScoreScene");
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
				// get player component
				auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

				//m_sLevel.get()->SetEnemyPos(enemy0->GetEnemyPos());
				enemy0->UpdatePlayerPosInLevel(player->GetPlayerPos());

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
			if (enemy0!=nullptr && !enemy0->GetIsDead())
			{
				count++;
			}
		}
		if (count == 0 || m_GotoLevel3)
		{
			//Game won
			m_GotoLevel3 = false;
			SoundManager::GetInstance().PlaySoundEffect("End", 0);

			m_GotoLevel3 = false;
			ClearScene();
			dae::InputManager::GetInstance().ResetInput();
			dae::SceneManager::GetInstance().setActive("ThirdLevel");
			auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

			dae::SceneManager::GetInstance().GetGameScene("ThirdLevel").get()->SetScore(player->GetScore());
			dae::SceneManager::GetInstance().GetGameScene("ThirdLevel").get()->SetLives(player->GetLives());
			//break;
		}
		UpdateImgui();
	}

}

void SecondLevel::FixedUpdate()
{
}

void SecondLevel::Render()
{
	if (m_ShowScore)
	{
		m_Score.get()->Render();
	}

	for (const auto& object : m_sceneObjects)
	{
		object->Render();
	}

	if (m_HightestScore != nullptr)
		m_HightestScore.get()->Render();
}

void SecondLevel::SetScore(int score)
{

	auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();

	player->SetScore(score);
}

void SecondLevel::SetLives(int lives)
{
	auto player = gameObjectPlayer->GetComponent<dae::PlayerComponent>();
	player->SetLives(lives);
}

void SecondLevel::Enemy(EnemyType& enemytype, std::shared_ptr<LevelComponent>level)
{
	auto gameObjectEnemy = std::make_shared<dae::GameObject>();
	
	gameObjectEnemy = std::make_shared<dae::GameObject>();
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

void SecondLevel::AddBurger(std::shared_ptr<LevelComponent> level)
{
	auto gameobjectBurger = std::make_shared<dae::GameObject>();
	float4 burgerPos;
	//Bottom
	burgerPos.one = 31.461f;
	burgerPos.two = 125.84f;
	burgerPos.three = 173.03f;
	burgerPos.four = 228.18f;

	Point2f leftPos = Point2f(56.180f, 430.f);
	//float lastpos = 590.f;

	auto burger = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos, burgerPos);
	burger->AddObserver(m_Score);
	gameobjectBurger->AddComponent(burger);

	//float4 burgerPos;
	//Bottom
	burgerPos.one = 31.461f;
	burgerPos.two = 275.2f;
	burgerPos.three = 369.7f;
	burgerPos.four = 424.f;

	Point2f leftPos2 = Point2f(202.2f, 590.f);
	auto burger2 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos2, burgerPos);
	burger2->AddObserver(m_Score);
	gameobjectBurger->AddComponent(burger2);

	burgerPos.one = 31.461f;
	burgerPos.two = 228.2f;
	burgerPos.three = 322.7f;
	burgerPos.four = 424.7f;

	Point2f leftPos3 = Point2f(348.f, 590.f);
	auto burger3 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos3, burgerPos);
	burger3->AddObserver(m_Score);
	gameobjectBurger->AddComponent(burger3);


	burgerPos.one = 31.461f;
	burgerPos.two = 78.6f;
	burgerPos.three = 135.8f;
	burgerPos.four = 228.090f;

	Point2f leftPos4 = Point2f(505.6f, 430.f);
	auto burger4 = std::make_shared<BurgerComponent>(gameobjectBurger, level, leftPos4, burgerPos);
	burger4->AddObserver(m_Score);
	gameobjectBurger->AddComponent(burger4);


	gameobjectBurger->SetTag(L"Burger");
	//scene.Add(gameobjectBurger);
	AddChild(gameobjectBurger);
}

void SecondLevel::PlayerOne(std::shared_ptr<LevelComponent> slevel)
{
	gameObjectPlayer = std::make_shared<dae::GameObject>();

	//Do this inside player class
	//player commands
	//if (dae::Input::GetInstance().m_ConsoleCommands.size() <= 0)
	{
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::ShoulderButtonRight), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer, 1.f), VK_RIGHT, GamePadIndex::playerOne));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::ShoulderButtonLeft), InputAction(std::make_unique<AimTankTurret>(gameObjectPlayer, -1.f), VK_LEFT, GamePadIndex::playerOne));
		dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonX), InputAction(std::make_unique<ShootBullet>(gameObjectPlayer), 'Q', GamePadIndex::playerOne));

		//InputAction a = InputAction(std::make_unique<DeathCommand>(gameObjectPlayer), GamePadIndex::playerOne);
		//dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonA), InputAction(std::make_unique<ThrowPepperCommand>(gameObjectPlayer), 'O', GamePadIndex::playerOne));
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
	m_Player = std::make_shared<dae::PlayerComponent>(gameObjectPlayer, slevel);


	//add gameobject to scene
	//scene.Add(gameObjectPlayer);
	m_Player->SetPlayerStartPosition(Point2f{ 303.f,459.f });

	gameObjectPlayer->AddComponent(m_Player);
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

	
	m_lives = std::make_shared<dae::LivesCounterComponent>(gameObjectPlayer, textLives);
		//m_lives = lives;
	
	//score counter
	
	
	m_Score = std::make_shared<dae::ScoreComponent>(gameObjectPlayer, textScore);
		//m_Score = score;
	
	//score achievement observer
	//player->AddObserver(scoreAchievement);

	//add observers
	m_Player->AddObserver(m_lives);

	//add score observer
	m_Player->AddObserver(m_Score);

	//add gameobject scoreAchievement to gameobject
	//gameObjectPlayer->AddComponent(scoreAchievement);
	//add transform component to gameobject
	gameObjectPlayer->AddComponent(transformLives);

	//add counter to gameobject
	gameObjectPlayer->AddComponent(m_lives);

	//add score component to gameobject
	gameObjectPlayer->AddComponent(m_Score);

	//add text component for lives
	gameObjectPlayer->AddComponent(textLives);


	//add textComponent for score
	gameObjectPlayer->AddComponent(textScore);

	//set score position
	textScore->SetPosition(130, 50, 0);

	//set lives position
	textLives->SetPosition(570, 5, 0);

	gameObjectPlayer->SetTag(L"Player1");
	//add to scent
	//scene.Add(gameObjectPlayer);
	AddChild(gameObjectPlayer);
}

void SecondLevel::ResetScene()
{
	//ClearScene();
	m_Player->SetPlayerStartPosition(Point2f{ 303.f,459.f });
	m_PlayerPos = m_Player->GetPlayerPos();

	auto playerState = dae::PlayerState::standing;
	m_Player->ChangeState(playerState);

	for (auto obj : m_sceneObjects)
	{
		if (obj->GetTag() == L"Enemy")
		{
			obj->GetComponent<dae::EnemyComponent>()->ResetEnemyPos();
		}
	}


}

bool SecondLevel::CheckIfPlayerIsDead(dae::PlayerComponent& player)
{
	//if (m_Player->GetLives() <= 0)
	//{
	//	//ClearScene();
	//	//dae::SceneManager::GetInstance().setActive("startScreen");

	//}

	//else if (!m_hasOverlapped && m_enemyPos.left != 0 && utils::IsOverlapping(m_PlayerPos, m_enemyPos))
	//{

	//	m_hasOverlapped = true;
	//	player.CallPlayerIsDead();
	//	//m_Player = nullptr;
	//	//m_PlayerPos = Rectf(0, 0, 0, 0);
	//	return true;
	//}
	//return false;
	if (m_Player->GetLives() <= 0)
	{
		ClearScene();
		dae::SceneManager::GetInstance().setActive("StartScreen");

	}
	else if (!m_hasOverlapped && m_enemyPos.left != 0 && utils::IsOverlapping(m_PlayerPos, m_enemyPos))
	{

		m_hasOverlapped = true;
		player.CallPlayerIsDead();

		return true;
	}
	//Rectf playerpos = m_sLevel.get()->GetPlayerPositionInTheLevel();
	return false;
}

void SecondLevel::UpdateImgui()
{

	ImGui::Begin("GotoLevel3", NULL);
	//ImGui::SetWindowSize(ImVec2((float)10.f, (float)10.f));
	//ImGui::SetNextWindowPos()
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
	ImGui::Checkbox("GotoLevel2", &m_GotoLevel3);

	ImGui::End();

}