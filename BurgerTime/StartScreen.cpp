#include "pch.h"
#include "StartScreen.h"

#include <future>

#include "Managers/InputManager.h"
#include <memory>
#include <string>
#include <vector>

#include "ResourceManager.h"
#include "SoundManager.h"
//#include "SServiceLocator.h"


StartScreen::StartScreen()
	:GameScene("StartScreen"),
	m_IsStartButtonPressed(false),m_AccumulatedSeconds(0),m_IsGameStarted(false),m_IsVsLevelChosen(false),m_IsCoOpLevelChosen(false)
{
	
}

void StartScreen::Initialize()
{
	m_IsGameStarted = false;
	m_showSecondScreen = false;
	dae::ResourceManager::GetInstance().Init("../Data/");

	auto gameObjectStart = std::make_shared<dae::GameObject>();
	auto Renderlevel = std::make_shared<dae::RenderComponent>(gameObjectStart);
	auto transformLevel_1 = std::make_shared<dae::TransformComponent>(gameObjectStart);
	gameObjectStart->AddComponent(transformLevel_1);
	Renderlevel->SetPosition(m_Position.x,m_Position.y,0);
	Renderlevel->SetTexture("StartScreenTron.jpeg");
	gameObjectStart->AddComponent(Renderlevel);
	gameObjectStart->SetTag(L"Blank");

	AddChild(gameObjectStart);

	auto gameobjectBullet = std::make_shared<dae::GameObject>();
	auto BulletStartScreen = std::make_shared<dae::RenderComponent>(gameobjectBullet);
	BulletStartScreen->SetTexture("TronBullet.png");
	gameobjectBullet->AddComponent(BulletStartScreen);
	gameobjectBullet->SetTag(L"bullet");
	AddChild(gameobjectBullet);

	m_DestRect = Rectf{ 230,320,20,20 };
	m_SrcRect = Rectf{ 0,0,8,7 };
}

void StartScreen::Update(float dt)
{
	
	for (auto obj : m_sceneObjects)
	{
		obj->Update(dt);
	}
	UpdatePlayerChoice();
	UpdateChosenLevel(dt);
}

void StartScreen::FixedUpdate()
{
	for (auto obj : m_sceneObjects)
	{
		obj->FixedUpdate();
	}
}

void StartScreen::Render()
{
	for (auto obj : m_sceneObjects)
	{
		auto bullet = obj->GetComponent<dae::RenderComponent>();
		if (obj->GetTag() == L"bullet" && bullet != nullptr )
		{
			bullet->RenderTexture(m_DestRect, m_SrcRect, false, false, 0);
		}
		else
		{
			obj->Render();
		}
	}
}

void StartScreen::ResetLevel()
{
	ClearScene();
	m_AccumulatedSeconds = 0;
	m_IsStartButtonPressed = false;
	m_IsVsLevelChosen = false;
	m_IsCoOpLevelChosen = false;
	m_IsGameStarted = true;
	m_DestRect = Rectf{ 230,320,20,20 };
}

void StartScreen::UpdateChosenLevel(float dt)
{

	if(m_IsVsLevelChosen)
	{
		//dae::InputManager::GetInstance().Quit();
		m_DestRect.bottom -= 100 * dt;
		m_AccumulatedSeconds += 0.1f;
		if (m_AccumulatedSeconds >= 10)
		{
			ResetLevel();
			dae::SceneManager::GetInstance().setActive("VersusLevel");
		}
	}

	if (m_IsStartButtonPressed)
	{
		m_DestRect.bottom -= 100 * dt;
		m_AccumulatedSeconds += 0.1f;
		if (m_AccumulatedSeconds >= 10)
		{
			ResetLevel();
			dae::SceneManager::GetInstance().setActive("SoloLevel");
		}
	}

	if (m_IsCoOpLevelChosen)
	{
		m_DestRect.bottom -= 100 * dt;
		m_AccumulatedSeconds += 0.1f;
		if (m_AccumulatedSeconds >= 10)
		{
			ResetLevel();
			dae::SceneManager::GetInstance().setActive("CoopLevel");
		}
	}
}

void StartScreen::UpdatePlayerChoice()
{
	if (!m_IsGameStarted)
	{
		auto indx = GamePadIndex::playerOne;
		/*static_cast<unsigned int>(dae::XBOX360Controller::ControllerButton::ButtonX*/
		if (dae::InputManager::GetInstance().IsPressed(static_cast<unsigned int>(dae::XBOX360Controller::ControllerButton::ButtonA), indx))
		{
			m_IsStartButtonPressed = true;
			m_AccumulatedSeconds = 0;
		}
		if (dae::InputManager::GetInstance().IsPressed(static_cast<unsigned int>(dae::XBOX360Controller::ControllerButton::ButtonX), indx))
		{
			m_IsVsLevelChosen = true;
			m_AccumulatedSeconds = 0;
		}
		if (dae::InputManager::GetInstance().IsPressed(static_cast<unsigned int>(dae::XBOX360Controller::ControllerButton::ButtonB), indx))
		{
			m_IsCoOpLevelChosen = true;
			m_AccumulatedSeconds = 0;
		}
		if (dae::InputManager::GetInstance().IsPressed(static_cast<unsigned int>(dae::XBOX360Controller::ControllerButton::ButtonY), indx))
		{
			dae::InputManager::GetInstance().Quit();
		}
	}
}
