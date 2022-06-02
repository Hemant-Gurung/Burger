#include "MiniginPCH.h"
#include "EnemyComponent.h"

#include "GameObject.h"

dae::EnemyComponent::EnemyComponent(std::shared_ptr<GameObject>& pGameobject, EnemyType& enemytype, LevelComponent& pLevel)
	:BaseComponent(pGameobject),
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
	m_sLevel(&pLevel)
	
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
		m_DestRect.left = m_StartPoint+100.f;
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
	m_EnemySprite->RenderTexture(m_DestRect, m_SrcRect,false);
}

void dae::EnemyComponent::update(float elapsed)
{
	//BaseComponent::update(elapsed);
	UpdateSprite(elapsed);
	UpdateEnemyMovement(elapsed);
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

void dae::EnemyComponent::UpdateEnemyMovement(float elapsedSec)
{
	m_DestRect.left += 10 * elapsedSec;
}
