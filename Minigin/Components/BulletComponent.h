#pragma once
#include "BaseComponent.h"
#include "LevelComponent.h"
#include "RenderComponent.h"
class BulletComponent :
    public dae::BaseComponent
{
public:
	BulletComponent(std::shared_ptr<dae::GameObject>&,Point2f,Vector2f direction,const EnemyType&) ;
	~BulletComponent() = default;
	BulletComponent(const BulletComponent& other) = delete;
	BulletComponent(BulletComponent&& other) noexcept = delete;
	BulletComponent& operator=(const BulletComponent& other) = delete;
	BulletComponent& operator=(BulletComponent&& other) noexcept = delete;

	void Initialize();
	void update(float dt) override;
	void SetLevelInformation(std::weak_ptr<LevelComponent>&);
	void Render();

	void GenerateBullet();
	bool ShootBullet(Point2f startPos,Vector2f direction, float elapsedSec);
	bool BulletLife();
	void SetPlayerPos(Rectf playerPos) { m_PlayerPos = playerPos; }
	Rectf GetPosition() override;
	void SetBulletEffectBool(bool);
	void SetIsDestroyed(bool value) { m_BulletIsDestroyed = value; }
	bool GetIsDestroyed() { return m_BulletIsDestroyed; }
private:
	float m_BulletSpriteWidth;
	float m_BulletSpriteHeight;
	bool m_IsBulletHittingSomething;

	std::shared_ptr<dae::RenderComponent> m_BulletTexture;
	std::shared_ptr<dae::RenderComponent> m_BulletDamageEffect;

	std::shared_ptr<dae::GameObject> m_pGameObject;
	std::weak_ptr<LevelComponent> m_sLevel;

	Vector2f m_Velocity;
	Vector2f m_Direction;
	float m_StartPosition;
	Rectf m_DestRect;
	Rectf m_SrcRect;

	Rectf m_DestRectEnemy;
	Rectf m_SrcRectEnemy;

	 int m_BulletSpeed;
	Rectf m_PlayerPos;
	int m_BulletBounceCount;
	bool m_BulletIsDestroyed;
	EnemyType m_enemyType;
	int m_BulletToBeBounced;

	std::shared_ptr<dae::RenderComponent> m_EnemyBulletUp;
	std::shared_ptr<dae::RenderComponent> m_EnemyBulletSide;


	void InitializeBulletDestRect();
	void UpdateBulletBehavior(float elapsed);
	bool CheckCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
	void RenderBulletEffect();
};

