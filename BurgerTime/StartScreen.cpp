#include "pch.h"
#include "StartScreen.h"

#include <future>

#include "Managers/InputManager.h"
#include <memory>
#include <string>
#include <vector>

#include "ResourceManager.h"
#include "SoundManager.h"
#include "SServiceLocator.h"


StartScreen::StartScreen()
	:GameScene("startScreen")
{
}

void StartScreen::Initialize()
{

	m_showSecondScreen = false;
	dae::ResourceManager::GetInstance().Init("../Data/");

	auto gameObjectStart = std::make_shared<dae::GameObject>();
	//RIGHT
	dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::down, dae::XBOX360Controller::ControllerButton::ButtonX), InputAction(std::make_unique<Button>(gameObjectStart), VK_SPACE, GamePadIndex::playerOne));
	//LEFT

	//dae::Input::GetInstance().MapEvent(std::make_pair(dae::XBOX360Controller::ButtonState::held, dae::XBOX360Controller::ControllerButton::DpadLeft), InputAction(std::make_unique<MoveLeftCommand>(gameObjectStart), 'A', GamePadIndex::playerOne));
	//UP


	auto Renderlevel = std::make_shared<dae::RenderComponent>(gameObjectStart);
	
	auto transformLevel_1 = std::make_shared<dae::TransformComponent>(gameObjectStart);
	gameObjectStart->AddComponent(transformLevel_1);

	//auto texture = dae::ResourceManager::GetInstance().LoadTexture(LEVELS[0]);
	m_Position = Vector2f(0, 500);
	Renderlevel->SetPosition(m_Position.x,m_Position.y,0);
	Renderlevel->SetTexture("StartScreen_made.png");
	
	gameObjectStart->AddComponent(Renderlevel);
	gameObjectStart->SetTag(L"Blank");

	AddChild(gameObjectStart);

	auto gameObjectStart_1 = std::make_shared<dae::GameObject>();
	
	auto transformLevel_2 = std::make_shared<dae::TransformComponent>(gameObjectStart);
	gameObjectStart->AddComponent(transformLevel_2);
	auto Renderlevel1 = std::make_shared<dae::RenderComponent>(gameObjectStart_1);
	Renderlevel1->SetTexture("StartScreen_made_1.png");
	gameObjectStart_1->AddComponent(Renderlevel1);
	gameObjectStart_1->SetTag(L"Blank1");

	AddChild(gameObjectStart_1);


	//auto f1 = std::async(&SServiceLocator::get_sound_system);
	//sound = &f1.get();
	//sound->Play(SoundID::STARTSCREEN, 50);

	if (!SoundManager::GetInstance().IsSoundStreamPlaying("Start"))
		SoundManager::GetInstance().PlaySoundStream("Start", true);
	
}

void StartScreen::Update(float dt)
{
	auto indx = GamePadIndex::playerOne;
	if (dae::InputManager::GetInstance().IsDown(static_cast<unsigned int>(dae::XBOX360Controller::ControllerButton::ButtonX), indx))
	{
		ClearScene();
		//dae::SceneManager::GetInstance().setActive("SoloLevel");

	}
	for (auto obj : m_sceneObjects)
	{
		if (obj->GetTag() == L"Blank")
		{
			if (m_Position.y >= 0)
			{
				m_Position.y -= 50 * dt;
				obj->GetComponent<dae::RenderComponent>()->SetPosition(m_Position.x, m_Position.y, 0);
			}
			else
			{
				m_showSecondScreen = true;
			}
		}
		obj->Update(dt);
	}
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
		if (obj->GetTag() != L"Blank")
		{
			if(m_showSecondScreen)
			{
				obj->Render();
			}
		}
		else
		{
			obj->Render();
		}
	}

}
