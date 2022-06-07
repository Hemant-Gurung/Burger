#include "MiniginPCH.h"
#include "Scenegraph/GameObject.h"
#include "Components/PlayerComponent.h"
#include "Components/RenderComponent.h"
//#include "sdl_sound_system.h"
//#include "SServiceLocator.h"

#include <future>
#include <functional>
#include "SServiceLocator.h"
#include "Texture2D.h"

namespace dae
{
	dae::PlayerComponent::PlayerComponent(std::shared_ptr<GameObject>& pGameObj, std::shared_ptr<LevelComponent> pLevel)
		:BaseComponent(pGameObj),
		m_Velocity(Vector2f(0, 0)),
		m_Acceleration(Vector2f(0, -981.f)),
		eve(EVENT::IDLE),
		m_playerPos{},
		a(nullptr),
		m_MoveSpeed(70.f),
		m_SpriteSheetWidth(240.f),
		m_SpriteSheetHeight(176.f),
		m_InitialDestRectBottom(450),
		m_DestRect(),
		m_SrcRect(),
		m_StartPoint(450),
		m_Colums(15),
		m_Rows(11),
		m_AccumulatedSec(0.f),
		m_FramesPerSecond(60),
		m_CurrFrame(0),
		m_SpriteSheetLeft(0),
		m_SpriteSheetTop(11),
		m_PlayerState(PlayerState::standing),
		IsTextureFlipped(false),
		IsMoving(false),
		m_playerMovement(playerMovement::idle),
		m_sLevel(std::move(pLevel))
		,m_TotalLives(3)
		,m_Pepper(5)
		,m_Score(0)
	,m_ShowDebugLines(false)
	{
		Initialize();
		m_PepperIcon = std::make_shared<RenderComponent>(pGameObj);
		m_SpriteTexture = std::make_shared<RenderComponent>(pGameObj);
		m_SpriteTexture->SetTexture("CharacterSprite.png");

		m_PepperIcon->SetTexture("PepperPng.png");
		m_PlayerIcon = std::make_shared<dae::RenderComponent>(pGameObj);
		m_PlayerIcon->SetTexture("ChefLogo.png");
		//pGameObj->GetComponent<TransformComponent>()->SetPosition(10.f, 400.f, 0.f);
		//m_SpriteTexture->SetPosition(20.f, 100.f, 0.f);
		//pGameObj->AddComponent(m_SpriteTexture);
	}

	PlayerComponent::~PlayerComponent()
	{

		if (a != nullptr)
		{
			//delete a;
			//a = nullptr;
		}
		if (walk.joinable())walk.join();
		if (die.joinable())die.join();
	}


	void PlayerComponent::Initialize()
	{
		m_TotalLives = 3;
		m_FrameTime = 1.f / m_FramesPerSecond;
		//auto level = m_pGameObject.lock()->GetComponent<LevelComponent>();
		InitializeDestRect();
		InitializeSrcRect();
	}

	void PlayerComponent::DeathCall()
	{
		/*std::cout << "Player is Dead\n";
		Notify(*this, EVENT::PLAYER_DEAD);*/
	}

	void PlayerComponent::ScoreCall()
	{
		//std::cout << "Score increased\n";
		Notify(*this, EVENT::PLAYER_SCOREADD);
	}

	void PlayerComponent::ScoreAchievementUnlock()
	{
		//std::cout << "Score increased\n";
		Notify(*this, EVENT::PLAYER_SCORE_ACHIEVEMENT_1);
	}

	void PlayerComponent::ButtonsNotPressed()
	{
		Notify(*this, EVENT::IDLE);
	}

	void PlayerComponent::MoveUp()
	{
		if (m_PlayerState != PlayerState::dead)
		{
		m_Velocity.y = -m_MoveSpeed;
		m_Velocity.x = 0;
		
			m_playerMovement = playerMovement::movingup;
		}
		IsMoving = true;

	}

	void PlayerComponent::MoveDown()
	{
		if (m_PlayerState != PlayerState::dead)
		{
		m_Velocity.y = m_MoveSpeed;
		m_Velocity.x = 0;
		
			m_playerMovement = playerMovement::movinddown;
			IsMoving = true;
		}
		

	}

	void PlayerComponent::MoveLeft()
	{
		if (m_PlayerState != PlayerState::dead)
		{
			IsMoving = true;
			auto f1 = std::async(&SServiceLocator::get_sound_system);
			a = &f1.get();
			a->Play(SoundID::WALK, 50);

			m_Velocity.x = -m_MoveSpeed;
			m_Velocity.y = 0;
			m_playerMovement = playerMovement::movingleft;

			//UpdateSourceRect();
		}
	}

	void PlayerComponent::MoveRight()
	{
		
		if (m_PlayerState != PlayerState::dead)
		{
			IsMoving = true;
		
			m_Velocity.x = m_MoveSpeed;
			m_Velocity.y = 0;
			m_playerMovement = playerMovement::movingright;
		}
	}

	void PlayerComponent::AddObserver(std::shared_ptr<Observer> observer)
	{
		m_Observers.push_back(observer);

	}

	void PlayerComponent::update(float elapsedSec)
	{
		
		if (a != nullptr)
			a->Update();

		//CheckIfPlayerIsDead();
		UpdateSprite(elapsedSec);
		if (m_PlayerState != PlayerState::dead)
		{
			UpdatePlayerMovement(elapsedSec);
			IsMoving = false;
		}
		GuiUpdate();
		
	}

	void PlayerComponent::Render() const
	{
		const int size = 30;

		//get render component
		//auto rendercom = m_pGameObject.lock()->GetComponent<RenderComponent>();

		if (m_ShowDebugLines)
		{
			// draw box using the render box
			m_SpriteTexture->RenderBox(m_DestRect, size, size);
		}
		// draw texture using the render texture function
		m_SpriteTexture->RenderTexture(m_DestRect, m_SrcRect, IsTextureFlipped);

		RenderPlayerLiveCount();
		RenderPepperIcon();
	}

	void PlayerComponent::SetPosition(float /*x*/, float /*x1*/, float /*x2*/)
	{
		m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(450, 450, 0.f);
		//m_TransformComponent->SetPosition(30.f, 30.f, 0.f);
	}

	void PlayerComponent::Notify(BaseComponent& actor, EVENT e)
	{
		for (size_t i = 0; i < m_Observers.size(); ++i)
		{
			m_Observers[i]->OnNotify(actor, e);
		}
	}

	void PlayerComponent::UpdateSprite(float elapsedSec)
	{
		m_AccumulatedSec += elapsedSec / 9.f;
		/*if(m_PlayerState == PlayerState::dead)
		{
			m_FrameTime = 5.f;
		}*/
		if (m_AccumulatedSec > (m_FrameTime))
		{
			++m_CurrFrame %= (m_Colums * m_Rows);

			if (m_CurrFrame >= 2)
			{
				m_CurrFrame = 0;
			}
			m_AccumulatedSec -= m_FrameTime;
			UpdateSourceRect();
		}
	}

	void PlayerComponent::UpdateSourceRect()
	{
		/*if(m_SrcRect.left>48)
		{
			m_SrcRect.left = 0;
		}*/
		m_SrcRect.left = m_SpriteSheetLeft + (int)m_CurrFrame % m_Colums * m_SrcRect.width;
		//m_SrcRect.bottom = 96 /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
		switch (m_PlayerState)
		{
		case PlayerState::standing:
			m_SrcRect.bottom = 0 /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
			break;
		case PlayerState::running:
			m_SrcRect.bottom = 32 /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
			break;
		case PlayerState::climbing:
			m_SrcRect.bottom = 48 /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
			break;
		case PlayerState::throwingPepper:
			m_SrcRect.bottom = 80; /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
			break;
		case PlayerState::dead:
			m_SrcRect.bottom = 64;
			break;
		}
	}

	void PlayerComponent::ChangeState(PlayerState& currentstate)
	{
		m_PlayerState = currentstate;
	}

	void PlayerComponent::FLipTexture(bool flip)
	{
		IsTextureFlipped = flip;
		//Texture2D texture;
		if (IsTextureFlipped)
		{
			//texture = m_pGameObject.lock()->GetComponent<RenderComponent>()->FlipTexture(m_DestRect.left, m_DestRect.bottom,m_DestRect.width,m_DestRect.height,m_SrcRect.left, m_SrcRect.bottom, true);
		}
	}

	void PlayerComponent::InitializeDestRect()
	{
		const int  AvatarHeight = 11;
		const int AvatarWidth = 15;
		const int size = 20;
		m_DestRect.width = m_SpriteSheetWidth / AvatarWidth + size;
		m_DestRect.height = m_SpriteSheetHeight / AvatarHeight + size;
		m_DestRect.left = m_StartPoint;
		m_DestRect.bottom = m_InitialDestRectBottom;
	}

	void PlayerComponent::InitializeSrcRect()
	{
		m_SrcRect.width = m_SpriteSheetWidth / m_Colums;
		m_SrcRect.height = m_SpriteSheetHeight / m_Rows;
		// update src rect
		UpdateSourceRect();
	}

	void PlayerComponent::UpdatePlayerMovement(float elapsedSec)
	{
		//const int size = 0;
		//auto level = m_pGameObject.lock()->GetComponent<LevelComponent>();
		//add player possible movements
		m_sLevel.lock()->HandleCollision(m_DestRect, m_Velocity, m_playerMovement);
		if (IsMoving)
		{
			switch (m_PlayerState)
			{
			case PlayerState::standing:
				m_Velocity = Vector2f(0.f, 0.f);
				m_DestRect.left += m_Velocity.x * elapsedSec;
				m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				break;
			case  PlayerState::running:
				m_DestRect.left += m_Velocity.x * elapsedSec;
				m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				break;
			case PlayerState::climbing:
				m_DestRect.left += m_Velocity.x * elapsedSec;
				m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				break;
			case PlayerState::throwingPepper:
				//m_DestRect.left += m_Velocity.x * elapsedSec;
				//m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				break;

			case PlayerState::dead:
				m_DestRect.left += 0;
				m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				break;
			}
		}
		else
		{
			m_PlayerState = PlayerState::standing;
		}
		//Rectf actors;
		//actors.bottom = m_
		//level->HandleCollision()


	}

	void PlayerComponent::ChangeFrameRate(bool lower)
	{
		if (lower) { --m_FramesPerSecond; }
		else { ++m_FramesPerSecond; }

		m_FrameTime = 1.f / m_FramesPerSecond;

	}

	void PlayerComponent::RenderPlayerLiveCount() const
	{
		Rectf plaerIconPos;
		plaerIconPos.left = 10.f;
		plaerIconPos.bottom = 500.f;
		plaerIconPos.width = 20.f;
		plaerIconPos.height = 20.f;

		Rectf plaerIconSrc;
		plaerIconSrc.left = 0;
		plaerIconSrc.bottom = 0;
		plaerIconSrc.width = 7.f;
		plaerIconSrc.height = 7.f;

		for (int i = 0; i < m_TotalLives; ++i)
		{
			plaerIconPos.bottom +=25;
			m_PlayerIcon->RenderTexture(plaerIconPos, plaerIconSrc);
		}
	}

	void PlayerComponent::RenderPepperIcon() const
	{
		Rectf pepperIconPos;
		pepperIconPos.left = 483.f;
		pepperIconPos.bottom = 15.f;
		pepperIconPos.width = 50.f;
		pepperIconPos.height = 10.f;

		Rectf pepperIconSrc;
		pepperIconSrc.left = 0;
		pepperIconSrc.bottom = 0;
		pepperIconSrc.width = 25.f;
		pepperIconSrc.height = 6.f;

		m_PepperIcon->RenderTexture(pepperIconPos, pepperIconSrc);
	}

	
	void PlayerComponent::CallPlayerIsDead()
	{
		//if (m_sLevel->CheckPlayerEnemyCollision())
		{
			auto f1 = std::async(&SServiceLocator::get_sound_system);
			a = &f1.get();
			a->Play(SoundID::DIE, 50);
			//std::cout << "Player is Dead\n";
			Notify(*this, EVENT::PLAYER_DEAD);
			m_PlayerState = PlayerState::dead;
			m_Velocity.x = 0;
			m_Velocity.y = 0;
		}
	}

	void PlayerComponent::CallScoreNotify()
	{
		ScoreCall();
	}

	void PlayerComponent::ThrowPepper()
	{
		if (m_PlayerState != PlayerState::throwingPepper)
		{
			Notify(*this, EVENT::PLAYER_PEPPERTHROW);
		}
		m_PlayerState = PlayerState::throwingPepper;
	}


	void PlayerComponent::DestroyLive()
	{
		m_TotalLives--;
		if(m_TotalLives<=0)
		{
			m_TotalLives = 0;
		}
		PlayDeadSound();
		
	}

	void PlayerComponent::AddScore()
	{
		m_Score += 50;
	}

	void PlayerComponent::DecreasePepper()
	{
		m_Pepper--;
		if (m_Pepper <= 0)
		{
			m_Pepper = 0;
		}
	}

	void PlayerComponent::PlayDeadSound()
	{
		Mix_Init(MIX_INIT_MP3);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);

		auto dead = Mix_LoadWAV("../Data/Sounds/Lose Life.mp3");
		//	_sample[0] = dae::ResourceManager::GetInstance().LoadSound("Sounds/Start.wav");
		if (!Mix_Playing(3))
		{
			Mix_PlayChannel(3, dead, 0);
		}
	}

	void PlayerComponent::GuiUpdate()
	{

			ImGui::Begin("PlayerDebugLine", NULL);
			//ImGui::SetWindowSize(ImVec2((float)10.f, (float)10.f));
			//ImGui::SetNextWindowPos()
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
			ImGui::Checkbox("ShowPlayerCollider", &m_ShowDebugLines);
			//if (ImGui::Button("Reset Speed")) {
				// This code is executed when the user clicks the button
				//this->speed = 0;
			//}
			//ImGui::SliderFloat("Speed", &this->speed, 0.0f, 10.0f);
			ImGui::End();
		
	}
}
