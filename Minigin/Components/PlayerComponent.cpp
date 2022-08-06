#include "MiniginPCH.h"
#include "Scenegraph/GameObject.h"
#include "Components/PlayerComponent.h"
#include "Components/RenderComponent.h"
#include "BulletComponent.h"
#include "LivesCounterComponent.h"
//#include "sdl_sound_system.h"
//#include "SServiceLocator.h"


#include <future>
#include <functional>

#include "SoundManager.h"
#include "SServiceLocator.h"
#include "Texture2D.h"
#include "Utils.h"

namespace dae
{
	dae::PlayerComponent::PlayerComponent(std::shared_ptr<GameObject>& pGameObj, std::shared_ptr<LevelComponent> pLevel, std::string playerTexture)
		:BaseComponent(pGameObj),
		m_Velocity(Vector2f(0, 0)),
		m_Acceleration(Vector2f(0, -981.f)),
		eve(EVENT::IDLE),
		m_playerPos{},
		a(nullptr),
		m_MoveSpeed(70.f),
		m_SpriteSheetWidth(35.f),
		m_SpriteSheetHeight(32.f),
		m_InitialDestRectBottom(264),
		m_DestRect(),
		m_SrcRect(),
		m_StartPoint(429),
		m_Colums(1),
		m_Rows(1),
		m_AccumulatedSec(0.f),
		m_FramesPerSecond(60),
		m_CurrFrame(0),
		m_SpriteSheetLeft(0),
		m_SpriteSheetTop(0),
		m_PlayerState(PlayerState::standing),
		IsTextureFlippedHorizontal(false),
		IsTextureFlippedVertical(false),
		IsMoving(false),
		m_playerMovement(playerMovement::idle),
		m_sLevel(pLevel)
		, m_TotalLives(3)
		, m_Pepper(5)
		, m_Score(0)
		, m_ShowDebugLines(false),
		m_RotateTurret(0),
		m_shootBullet(false),
		m_sGameObject(pGameObj),
		m_ExecuteBullet(false),
		m_AccumulatedTime(2),
		timetoWaitBeforeShooting(3),
		m_TimeToWaitBeforeEntering(0),
		m_IsPlayerShot(false)
	{
		Initialize();
		m_PepperIcon = std::make_unique<RenderComponent>(pGameObj);
		m_PepperIcon->SetTexture("PepperPng.png");

		m_SpriteTexture = std::make_unique<RenderComponent>(pGameObj);
		//m_SpriteTexture->SetTexture("CharacterSprite.png");
		m_SpriteTexture->SetTexture(playerTexture);

		m_SpriteVerticalTexture = std::make_unique<RenderComponent>(pGameObj);
		m_TurretTexture = std::make_unique<RenderComponent>(pGameObj);

		if (playerTexture == "PlayerTank.png")
		{
			m_SpriteVerticalTexture->SetTexture("TronTankVertical.png");
			m_TurretTexture->SetTexture("TankTurret.png");

		}
		else
		{
			m_SpriteVerticalTexture->SetTexture("PlayerEnemyTankVertical.png");
			m_TurretTexture->SetTexture("PlayerEnemyTurret.png");

		}
		//turret

		m_PlayerIcon = std::make_unique<dae::RenderComponent>(pGameObj);
		m_PlayerIcon->SetTexture("TankLive.png");

		// Explosion
		m_DeathExplosion = std::make_unique<dae::RenderComponent>(pGameObj);
		m_DeathExplosion->SetTexture("PlayerExplosion.png");

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

		//for(int i=0;i<m_Bullets.size();++i)
	//	{
			//delete [] m_Bullets;
			//m_Bullets[i] = nullptr;
		//}
	}


	void PlayerComponent::Initialize()
	{
		m_Bullets.resize(0);
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

	void PlayerComponent::ShootBullet()
	{

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
			IsTextureFlippedVertical = false;
			IsMoving = true;
			//auto f1 = std::async(&SServiceLocator::get_sound_system);
			//a = &f1.get();
			//a->Play(SoundID::WALK, 50);

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
		
		if (!m_IsPlayerShot)
		{
			if (m_ExecuteBullet)
			{
				m_shootBullet = true;
				m_ExecuteBullet = false;
			}

			for (auto& bullet : m_Bullets)
			{
				//m_bullet->update(elapsedSec);
				bullet->SetLevelInformation(m_sLevel);

				bullet->update(elapsedSec);
			}


		/*	if (a != nullptr)
				a->Update();*/

			//CheckIfPlayerIsDead();
			UpdateSprite(elapsedSec);
			if (m_PlayerState != PlayerState::dead)
			{
				UpdatePlayerMovement(elapsedSec);
				IsMoving = false;
			}
		}
		else
		{
			WaitBeforeEnteringTheArena();
		}
		GuiUpdate();
		

	}

	void PlayerComponent::Render() const
	{
		if (m_IsPlayerShot)
		{
			Rectf exp = Rectf{ m_DestRect.left,m_DestRect.bottom,50,50 };
			Rectf srcExp = Rectf{ 0,0,30,32 };
			m_DeathExplosion->RenderTexture(exp, srcExp, false, false);
		}
		else
		{
			const int size = 30;

			if (m_shootBullet)
			{
				/*m_bullet->Render();*/
				for (auto& bullet : m_Bullets)
				{
					bullet->Render();
				}
			}
			//get render component
			//auto rendercom = m_pGameObject.lock()->GetComponent<RenderComponent>();

			if (m_ShowDebugLines)
			{
				// draw box using the render box
				m_SpriteTexture->RenderBox(m_DestRect, size, size);
			}
			// draw texture using the render texture function
			if (IsTextureFlippedVertical)
			{
				//IsTextureFlippedHorizontal = false;
				m_SpriteVerticalTexture->RenderTexture(m_DestRect, m_SrcRect, IsTextureFlippedHorizontal, IsTextureFlippedVertical);

			}
			else
			{
				m_SpriteTexture->RenderTexture(m_DestRect, m_SrcRect, IsTextureFlippedHorizontal, IsTextureFlippedVertical);
			}

			RenderTankTurret();
			RenderPlayerLiveCount();
			RenderPepperIcon();
		}
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
			m_SrcRect.bottom = 0 /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
			break;
		case PlayerState::climbing:
			m_SrcRect.bottom = 0 /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
			break;
		case PlayerState::throwingPepper:
			m_SrcRect.bottom = 0; /*m_SpriteSheetTop + (m_Colums + 1) * m_SrcRect.height*/;
			break;
		case PlayerState::dead:
			m_SrcRect.bottom = 0;
			break;
		}
	}

	void PlayerComponent::ChangeState(PlayerState& currentstate)
	{
		m_PlayerState = currentstate;
	}

	void PlayerComponent::FLipTextureHorozontal(bool flip)
	{
		IsTextureFlippedVertical = false;
		IsTextureFlippedHorizontal = flip;
		//Texture2D texture;
	}
	void PlayerComponent::FLipTextureVertical(bool flip)
	{
		IsTextureFlippedHorizontal = false;
		IsTextureFlippedVertical = flip;
		//Texture2D texture;
	}

	void PlayerComponent::InitializeDestRect()
	{
		const int  AvatarHeight = 1;
		const int AvatarWidth = 1;
		//const int size = 20;
		m_DestRect.width = m_SpriteSheetWidth / AvatarWidth;
		m_DestRect.height = m_SpriteSheetHeight / AvatarHeight;

		int index = rand() % (3 - 0 + 1) + 0;
		int times = rand() % (67 - 12 + 1) - 12;
		for (int i = 0; i < times; ++i)
		{
			index = rand() % (3 - 0 + 1) + 0;
		}
		m_DestRect.left = PlayerStartPosition[index].x; // 
		m_DestRect.bottom = PlayerStartPosition[index].y;
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
				m_TurretTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);

				break;
			case  PlayerState::running:
				m_DestRect.left += m_Velocity.x * elapsedSec;
				m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_TurretTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);

				break;
			case PlayerState::climbing:
				m_DestRect.left += m_Velocity.x * elapsedSec;
				m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_TurretTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);

				break;
			case PlayerState::throwingPepper:
				//m_DestRect.left += m_Velocity.x * elapsedSec;
				//m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_TurretTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);

				break;

			case PlayerState::dead:
				m_DestRect.left += 0;
				m_DestRect.bottom += m_Velocity.y * elapsedSec;
				m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_SpriteTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);
				m_TurretTexture->SetPosition(m_DestRect.left, m_DestRect.bottom, 0);

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
		plaerIconPos.left = m_pGameObject.lock()->GetComponent<dae::LivesCounterComponent>()->GetPosition().left;
		plaerIconPos.bottom = 20.f;
		plaerIconPos.width = 20.f;
		plaerIconPos.height = 25.f;

		Rectf plaerIconSrc;
		plaerIconSrc.left = 0;
		plaerIconSrc.bottom = 0;
		plaerIconSrc.width = 12.f;
		plaerIconSrc.height = 16.f;

		for (int i = 0; i < m_TotalLives; ++i)
		{
			plaerIconPos.left += 25;
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

	void PlayerComponent::RenderTankTurret() const
	{
		Rectf pepperIconSrc;
		pepperIconSrc.left = 0;
		pepperIconSrc.bottom = 0;
		pepperIconSrc.width = 35.f;
		pepperIconSrc.height = 32.f;

		m_TurretTexture->RenderTexture(m_DestRect, pepperIconSrc, false, false, m_RotateTurret);
	}

	void PlayerComponent::GenerateBullet()
	{
		
		m_AccumulatedTime++;

		if (m_AccumulatedTime >= timetoWaitBeforeShooting)
		{
			SoundManager::GetInstance().PlaySoundEffect("Shoot", 0);
			Vector2f direction = Vector2f(cos(m_RotateTurret * float(M_PI / 180.f)), sin(m_RotateTurret * float(M_PI / 180.f)));

			//m_Bullets.push_back(new BulletComponent(m_sGameObject, Point2f((m_DestRect.left + m_DestRect.width / 2), (m_DestRect.bottom + m_DestRect.height / 2)), direction));
			m_Bullets.push_back(std::move(std::make_unique<BulletComponent>(m_sGameObject, Point2f((m_DestRect.left + m_DestRect.width / 2), (m_DestRect.bottom + m_DestRect.height / 2)), direction, EnemyType::Egg)));
			//std::make_shared<BulletComponent>(m_sGameObject)
			m_ExecuteBullet = true;
			m_AccumulatedTime = 0;
		}
	}

	bool PlayerComponent::CheckPlayerBulletEnemyCollision(Rectf& enemyPos)
	{
		if (m_shootBullet)
		{
			

			for (auto& bullet : m_Bullets)
			{
				// first check if the bullet is already destroyed
				if (utils::IsOverlapping(bullet->GetPosition(), enemyPos))
				{
					bullet->SetIsDestroyed(true);
					return true;
				}
			}
		}
		return false;
	}

	void PlayerComponent::RotateTankTurret(float direction)
	{
		if (direction > 0)
		{
			m_RotateTurret += 5;
		}
		else
		{
			m_RotateTurret -= 5;
		}
	}


	void PlayerComponent::CallPlayerIsDead()
	{
		{
			SoundManager::GetInstance().PlaySoundEffect("Loss", 0);

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
		if (!m_IsPlayerShot)
		{
			m_TotalLives--;
			if (m_TotalLives <= 0)
			{
				m_TotalLives = 0;
			}
			SoundManager::GetInstance().PlaySoundEffect("Loss", 0);

			//PlayDeadSound();
			m_IsPlayerShot = true;
		}
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

	void PlayerComponent::WaitBeforeEnteringTheArena()
	{
		m_TimeToWaitBeforeEntering++;
		if (m_TimeToWaitBeforeEntering >= 100)
		{
			m_IsPlayerShot = false;
			m_TimeToWaitBeforeEntering = 0;
			InitializeDestRect();
		}

	}

	void PlayerComponent::GuiUpdate()
	{

		ImGui::Begin("PlayerDebugLine", NULL);
		//ImGui::SetWindowSize(ImVec2((float)10.f, (float)10.f));
		//ImGui::SetNextWindowPos()
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
		ImGui::Checkbox("ShowPlayerCollider", &m_ShowDebugLines);
		ImGui::SliderFloat("BurgerLeftPosition", &m_DestRect.left, 0.0f, 700.0f);
		ImGui::SliderFloat("BurgerbottomPosition", &m_DestRect.bottom, 0.0f, 700.0f);
		//ImGui::SliderFloat("BurgerbottomPosition", &, 0.0f, 700.0f);
		ImGui::SliderInt("BulletShootDuration", &timetoWaitBeforeShooting, 0, 10);


		//if (ImGui::Button("Reset Speed")) {
			// This code is executed when the user clicks the button
			//this->speed = 0;
		//}
		//ImGui::SliderFloat("Speed", &this->speed, 0.0f, 10.0f);
		ImGui::End();

	}
}
