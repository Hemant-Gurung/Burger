
#include "MiniginPCH.h"
#include "EnemyComponent.h"
#include "GameObject.h"

namespace dae
{
	EnemyComponent::EnemyComponent(std::shared_ptr<GameObject>& pGameobject, EnemyType& enemytype, LevelComponent& pLevel)
		:BaseComponent(pGameobject),
		m_Velocity(0.f, 0.f),
		m_MoveSpeed(50.f),
		m_SpriteSheetWidth(105.f),
		m_SpriteSheetHeight(105.f),
		m_InitialDestRectBottom(10),
		m_StartPoint(15),
		m_SpriteSheetLeft(0),
		m_AccumulatedSec(0.f),
		m_FramesPerSecond(60),
		m_CurrFrame(0),
		m_Colums(7),
		m_Rows(7),
		m_enemyType(enemytype),
		m_sLevel(&pLevel),
		m_enemyState(EnemyStates::movingRight)

	{
		Initialize();
		m_EnemySprite = std::make_shared<RenderComponent>(pGameobject);
		m_EnemySprite->SetTexture("EnemySprite.png");
		//pGameobject->AddComponent(m_EnemySprite);


	}

	dae::EnemyComponent::~EnemyComponent()
	{

	}


	void dae::EnemyComponent::Initialize()
	{
		m_FrameTime = 1.f / m_FramesPerSecond;
		InitializeDstRect();
		InitializeSrcRect();
	}

	void dae::EnemyComponent::InitializeDstRect()
	{
		const int enemyHeight = 7;
		const int enemyWidth = 7;
		const int size = 15;
		m_DestRect.width = m_SpriteSheetWidth / enemyWidth + size;
		m_DestRect.height = m_SpriteSheetHeight / enemyHeight + size;
		switch (m_enemyType)
		{
		case EnemyType::Red:
			m_DestRect.left = m_StartPoint;
			m_DestRect.bottom = m_InitialDestRectBottom;

			break;

		case EnemyType::Egg:
			m_DestRect.left = m_StartPoint + 100.f;
			m_DestRect.bottom = m_InitialDestRectBottom;

			break;
		}

	}

	void dae::EnemyComponent::InitializeSrcRect()
	{
		//const int columns = 7;
		//const int rows = 7;
		m_SrcRect.width = m_SpriteSheetWidth / m_Colums;
		m_SrcRect.height = m_SpriteSheetHeight / m_Rows;
	}

	void dae::EnemyComponent::Render() const
	{
		m_EnemySprite->RenderTexture(m_DestRect, m_SrcRect, false);
	}

	void dae::EnemyComponent::update(float elapsed)
	{
		//BaseComponent::update(elapsed);
		UpdateSprite(elapsed);
		UpdateEnemyMovementState(elapsed);
		UpdateEnemyMovementusingState(elapsed);

	}

	void dae::EnemyComponent::UpdateSprite(float elapsedSec)
	{
		m_AccumulatedSec += elapsedSec / 9.f;
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

	void dae::EnemyComponent::UpdateSourceRect()
	{
		m_SrcRect.left = m_SpriteSheetLeft + (int)m_CurrFrame % m_Colums * m_SrcRect.width;

		switch (m_enemyType)
		{
		case EnemyType::Red:
			m_SrcRect.bottom = 0;
			break;

		case EnemyType::Egg:
			m_SrcRect.bottom = 48;
			break;
		}

	}

	void dae::EnemyComponent::UpdateEnemyMovementState(float /*elapsedSec*/)
	{

		//m_DestRect.left += 10 * elapsedSec;
		auto playerPos = m_sLevel->GetPlayerPositionInTheLevel();
		auto playerCenter = Point2f(playerPos.left + playerPos.width / 2, playerPos.bottom + playerPos.height / 2);

		// make a line from enemyPos to playerPOs
		auto enemyCenter = Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2);

		auto previousstate = m_enemyState;
		switch (m_enemyState)
		{
		case EnemyStates::movingRight:
			//if (previousstate != EnemyStates::movingLeft)
		{
			if (!checkIfMovementIsPossible(m_enemyState))
			{
				if (playerCenter.y - enemyCenter.y >= 3.f)
				{
					m_enemyState = EnemyStates::movingUp;
					break;
				}
				else
				{
					m_enemyState = EnemyStates::movingDown;
					break;
				}
				break;
			}
			else
			{
				// when going right check if moving down or up is possible
				auto tempstateDown = EnemyStates::movingDown;
				auto tempStateUp = EnemyStates::movingUp;
				if (checkIfMovementIsPossible(tempstateDown))
				{
					if (enemyCenter.y - playerCenter.y >= 1.f)
					{
						m_enemyState = tempstateDown;
						break;
					}
				}
				if (checkIfMovementIsPossible(tempStateUp))
				{
					if (playerCenter.y - enemyCenter.y >= 1.f)
					{
						m_enemyState = tempStateUp;
						break;
					}
				}
			}
		}
		break;

		case EnemyStates::movingLeft:

			//if (previousstate != EnemyStates::movingRight)
		{
			if (!checkIfMovementIsPossible(m_enemyState))
			{
				if (playerCenter.y - enemyCenter.y >= 1.f)
				{
					m_enemyState = EnemyStates::movingUp;
				}
				else
				{
					m_enemyState = EnemyStates::movingDown;
				}
			}
			else
			{
				// when going right check if moving down or up is possible
				auto tempstateDown = EnemyStates::movingDown;
				auto tempStateUp = EnemyStates::movingUp;
				if (checkIfMovementIsPossible(tempstateDown))
				{
					if (enemyCenter.y - playerCenter.y >= 1.f)
					{
						m_enemyState = tempstateDown;
						break;
					}
				}
				if (checkIfMovementIsPossible(tempStateUp))
				{
					if (playerCenter.y - enemyCenter.y >= 1.f)
					{

						m_enemyState = tempStateUp;
						break;
					}
				}
			}
		}
		break;
		case EnemyStates::movingDown:
			if (!checkIfMovementIsPossible(m_enemyState))
			{
				if (playerCenter.x - enemyCenter.x >= 3.f)
				{
					if (m_enemyState != EnemyStates::movingLeft)
						m_enemyState = EnemyStates::movingRight;
				}
				else
				{
					if (m_enemyState != EnemyStates::movingRight)
						m_enemyState = EnemyStates::movingLeft;
				}
			}
			else
			{
				// when going right check if moving down or up is possible
				auto tempstateRight = EnemyStates::movingRight;
				auto tempStateLeft = EnemyStates::movingLeft;
				if (checkIfMovementIsPossible(tempstateRight))
				{
					if (playerCenter.x - enemyCenter.y >= 1.f)
					{
						//if (m_enemyState != EnemyStates::movingLeft)
						m_enemyState = tempstateRight;
						break;
					}
				}
				else if (checkIfMovementIsPossible(tempStateLeft))
				{
					if (enemyCenter.y - playerCenter.y >= 1.f)
					{
						//if (m_enemyState != EnemyStates::movingRight)
						m_enemyState = tempStateLeft;
						break;
					}
				}
			}

			break;
		case EnemyStates::movingUp:
			if (!checkIfMovementIsPossible(m_enemyState))
			{
				if (playerCenter.x - enemyCenter.x >= 7.f)
				{
					//if(m_enemyState != EnemyStates::movingLeft)
					m_enemyState = EnemyStates::movingRight;
					break;
				}
				//if(enemyCenter.x-playerCenter.x >=)
				if (enemyCenter.x - playerCenter.x >= 1.f)
				{
					if (previousstate != EnemyStates::movingRight)
					{
						//if (m_enemyState != EnemyStates::movingRight)
						m_enemyState = EnemyStates::movingLeft;
					}
					break;
				}
			}
			else
			{
				// when going right check if moving down or up is possible
				auto tempstateRight = EnemyStates::movingRight;
				auto tempStateLeft = EnemyStates::movingLeft;
				if (checkIfMovementIsPossible(tempstateRight))
				{
					if (playerCenter.x - enemyCenter.y >= 1.f)
					{
						//if (m_enemyState != EnemyStates::movingLeft)
						m_enemyState = tempstateRight;
						break;
					}
				}
				else if (checkIfMovementIsPossible(tempStateLeft))
				{
					if (enemyCenter.y - playerCenter.y >= 1.f)
					{
						//if (m_enemyState != EnemyStates::movingRight)
						m_enemyState = tempStateLeft;
						break;
					}
				}
			}
			break;

		default:
			break;
		}

		//if playerpos x is greater than enemy pos move right
		//if playerpos x is less than enemy pos move left

		//if playerpos y is less then move down
		// if player pos y is more move up


	}

	void dae::EnemyComponent::UpdateEnemyMovementusingState(float elapsedSec)
	{
		auto playerPos = m_sLevel->GetPlayerPositionInTheLevel();
		auto playerCenter = Point2f(playerPos.left + playerPos.width / 2, playerPos.bottom + playerPos.height / 2);

		// make a line from enemyPos to playerPOs
		auto enemyCenter = Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2);

		//HandleEnemyCollision();
		switch (m_enemyState)
		{
		case EnemyStates::movingRight:
			m_Velocity.x = m_MoveSpeed;
			m_Velocity.y = 0;
			break;
		case EnemyStates::movingLeft:
			m_Velocity.x = -m_MoveSpeed;
			m_Velocity.y = 0;

			break;
		case EnemyStates::movingDown:
			m_Velocity.y = -m_MoveSpeed;
			m_Velocity.x = 0;
			break;
		case EnemyStates::movingUp:
			m_Velocity.y = m_MoveSpeed;
			m_Velocity.x = 0;
			break;

		default:
			break;
		}


		m_DestRect.left += m_Velocity.x * elapsedSec;;
		m_DestRect.bottom += m_Velocity.y * elapsedSec;

	}

	void dae::EnemyComponent::HandleEnemyCollision()
	{
		m_sLevel->HandleCollision(m_DestRect, m_Velocity, m_enemyState);
	}

	bool dae::EnemyComponent::checkIfMovementIsPossible(EnemyStates& state)
	{
		//auto tempCurrentState = state;
		//auto tempCurrentState = m_enemyState;
		m_sLevel->HandleCollision(m_DestRect, m_Velocity, state);

		if (m_Velocity == Vector2f(0, 0))
		{
			// if not go back to previous state
			return false;
		}
		return true;
		//m_DestRect.bottom += 20 * elapsedSec;
	}

	Rectf& dae::EnemyComponent::GetEnemyPos()
	{
		return m_DestRect;
	}

}
