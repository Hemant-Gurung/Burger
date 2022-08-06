#include "MiniginPCH.h"
#include "BulletComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "SoundManager.h"
#include "Utils.h"


BulletComponent::BulletComponent(std::shared_ptr<dae::GameObject>& pGameObject,Point2f startPos, Vector2f direction, const EnemyType& enemyType)
	:BaseComponent(pGameObject ),
	m_BulletSpriteHeight(7.f),
	m_BulletSpriteWidth(8.f),
	m_SrcRect(),
	m_DestRect(),
	m_Direction(),
	m_pGameObject(pGameObject),
	m_BulletSpeed(350),
    m_BulletIsDestroyed(false),
    m_BulletBounceCount(0),
	m_IsBulletHittingSomething(false),
    m_enemyType(enemyType)
  , m_BulletToBeBounced(5)
{
	m_DestRect.left = startPos.x;
	m_DestRect.bottom = startPos.y;
	m_Direction = direction;

	//// enemy bullet rect
	//m_DestRectEnemy.left = startPos.x;
	//m_DestRectEnemy.bottom = startPos.y;
	

	m_BulletTexture = std::make_shared<dae::RenderComponent>(m_pGameObject);
	m_BulletTexture->SetTexture("TronBullet.png");

	m_BulletDamageEffect = std::make_shared<dae::RenderComponent>(m_pGameObject);
	m_BulletDamageEffect->SetTexture("DamageEffect.png");

	m_EnemyBulletUp = std::make_shared<dae::RenderComponent>(m_pGameObject);
	m_EnemyBulletUp->SetTexture("EnemyBulletUp.png");

	m_EnemyBulletSide = std::make_shared<dae::RenderComponent>(m_pGameObject);
	m_EnemyBulletSide->SetTexture("EnemyBulletSide.png");

	if(m_enemyType == EnemyType::Red)
	{
		m_BulletTexture = m_EnemyBulletUp;
		m_BulletToBeBounced = 1;
		m_BulletSpeed = 200;
	}

	m_SrcRect.width = m_BulletSpriteWidth;
	m_SrcRect.height = m_BulletSpriteHeight;
	m_SrcRect.left = 0;
	m_SrcRect.bottom = 0;

	m_SrcRectEnemy.width = 9;
	m_SrcRectEnemy.height = 9;
	m_SrcRect.left = 0;
	m_SrcRect.bottom = 0;

	InitializeBulletDestRect();
}

void BulletComponent::Initialize()
{
	
}

void BulletComponent::update(float dt)
{
	if (!m_BulletIsDestroyed)
	{
		/*if (m_enemyType == EnemyType::Red)
		{
			m_DestRectEnemy.left += m_Direction.x * dt * m_BulletSpeed;
			m_DestRectEnemy.bottom += m_Direction.y * dt * m_BulletSpeed;
		}
		else*/
		{
			m_sLevel.lock()->SetBulletPos(m_DestRect);
			m_DestRect.left += m_Direction.x * dt * m_BulletSpeed;
			m_DestRect.bottom += m_Direction.y * dt * m_BulletSpeed;
		}
		UpdateBulletBehavior(dt);
		//SetBulletEffectBool(false);
	}
}

void BulletComponent::SetLevelInformation(std::weak_ptr<LevelComponent>& slevel)
{
	m_sLevel = slevel;
}

void BulletComponent::Render() 
{
	if (!m_BulletIsDestroyed)
	{
		{
			m_BulletTexture->RenderTexture(m_DestRect, m_SrcRect, false, false, 0);
			
		}

		// bullet effect
		if (m_IsBulletHittingSomething)
		{
			RenderBulletEffect();
			SetBulletEffectBool(false);
		}
	}
	else {
		m_DestRect = Rectf{ 0,0,0,0 };
	}
	
	//m_pGameObject->GetComponent<dae::RenderComponent>()->RenderBox(m_DestRect, 5, 5);

}

void BulletComponent::GenerateBullet()
{
	
}

bool BulletComponent::ShootBullet(Point2f startPos,Vector2f direction, float /*elapsedSec*/)
{
	//GenerateBullet();
	m_DestRect.left = startPos.x;
	m_DestRect.bottom = startPos.y;
	m_Direction = direction;
	return true;
}

bool BulletComponent::BulletLife()
{
	return false;
}


Rectf BulletComponent::GetPosition()
{
	return m_DestRect;
}

void BulletComponent::SetBulletEffectBool(bool value)
{
	m_IsBulletHittingSomething = value;
}

void BulletComponent::InitializeBulletDestRect()
{
	m_DestRect.width = m_BulletSpriteWidth;
	m_DestRect.height = m_BulletSpriteHeight;

	m_DestRectEnemy.width = 9;
	m_DestRectEnemy.height = 9;
}

void BulletComponent::UpdateBulletBehavior(float /*elapsed*/)
{
	//r=d−2(d⋅n)n
	auto vertices = m_sLevel.lock()->GetVertices();
	
	for (size_t i = 0; i < vertices.size(); i++)
	{
		auto rect = vertices[i];
		float lowX{FLT_MAX}, lowY{ FLT_MAX }, maxX{ 0 }, maxY{ 0 };
		for (size_t j = 0; j < rect.size();++j)
		{
			if (rect[j].x < lowX) lowX = rect[j].x;
			if (rect[j].y < lowY) lowY = rect[j].y;

			if (rect[j].x > maxX) maxX = rect[j].x;
			if (rect[j].y > maxY) maxY = rect[j].y;

		}

		// create calculatable values from the data
		Rectf tiles{ lowX,lowY,maxX - lowX,maxY - lowY };

		if(m_sLevel.lock()->checkIfEnemyIsShot())
		{
			m_BulletIsDestroyed = true;
		}

		const int collisionBorder = 10;

		//check if bullet count is >=5
		if(m_BulletBounceCount >=m_BulletToBeBounced)
		{
			m_BulletIsDestroyed = true;
			m_BulletBounceCount = 0;
		}
		
		// check the collision of bullet with walls
		Rectf Yincreased;
		Yincreased.left = m_DestRect.left;
		Yincreased.bottom = m_DestRect.bottom + m_Direction.y * collisionBorder;
		Yincreased.width = m_DestRect.width;
		Yincreased.height = m_DestRect.height;
		if (utils::IsOverlapping(Yincreased, tiles))
		{
			SoundManager::GetInstance().PlaySoundEffect("Bullet", 0);
			m_Direction.y *= -1;
			m_BulletBounceCount++;
			//m_Direction = m_Direction - 2 * (m_Direction.DotProduct(Vector2f(1, 0))) * Vector2f(1, 0);
			m_IsBulletHittingSomething = true;
			break;
		}

		Rectf Xincreased;
		Xincreased.left = m_DestRect.left + m_Direction.x * collisionBorder;
		Xincreased.bottom = m_DestRect.bottom;
		Xincreased.width = m_DestRect.width;
		Xincreased.height = m_DestRect.height;
		if(utils::IsOverlapping(Xincreased,tiles))
		{
			SoundManager::GetInstance().PlaySoundEffect("Bullet", 0);
			m_Direction.x *= -1;
			m_BulletBounceCount++;
			m_IsBulletHittingSomething = true;
				//m_Direction = m_Direction - 2 * (m_Direction.DotProduct(Vector2f(1, 0))) * Vector2f(1, 0);
			break;
		}

		

	}

}

bool BulletComponent::CheckCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	return x1 + w1 > x2 && x1 <x2 + w2 && y1 + h1 > y2 && y1 < y2 + h2;
}

void BulletComponent::RenderBulletEffect()
{
	Rectf destREct;
	destREct.left = 0;
	destREct.bottom =0;
	destREct = m_DestRect;
	destREct.width = 30;
	destREct.height = 30;

	Rectf sREct;
	sREct = m_SrcRect;
	sREct.width = 23;
	sREct.height = 21;
	m_BulletDamageEffect->RenderTexture(destREct, sREct, false, false, 0);
}
