#pragma once
#include "BaseComponent.h"
#include "LevelComponent.h"
#include "RenderComponent.h"
class BulletComponent :
    public dae::BaseComponent
{
public:
	BulletComponent(std::shared_ptr<dae::GameObject>&,Point2f,Vector2f direction) ;
	~BulletComponent() = default;
	BulletComponent(const BulletComponent& other) = delete;
	BulletComponent(BulletComponent&& other) noexcept = delete;
	BulletComponent& operator=(const BulletComponent& other) = delete;
	BulletComponent& operator=(BulletComponent&& other) noexcept = delete;

	void Initialize();
	void update(float dt) override;
	void SetLevelInformation(std::weak_ptr<LevelComponent>&);
	void Render() const override;

	void GenerateBullet();
	bool ShootBullet(Point2f startPos,Vector2f direction, float elapsedSec);
	bool BulletLife();
	void SetPlayerPos(Rectf playerPos) { m_PlayerPos = playerPos; }
	Rectf& GetBulletPos() { return m_DestRect; }
private:
	float m_BulletSpriteWidth;
	float m_BulletSpriteHeight;

	std::shared_ptr<dae::RenderComponent> m_BulletTexture;
	std::shared_ptr<dae::GameObject> m_pGameObject;
	std::weak_ptr<LevelComponent> m_sLevel;

	Vector2f m_Velocity;
	Vector2f m_Direction;
	float m_StartPosition;
	Rectf m_DestRect;
	Rectf m_SrcRect;
	const int m_BulletSpeed;
	Rectf m_PlayerPos;
	int m_BulletBounceCount;
	bool m_BulletIsDestroyed;

	void InitializeBulletDestRect();
	void UpdateBulletBehavior(float elapsed);
	bool CheckCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
};

