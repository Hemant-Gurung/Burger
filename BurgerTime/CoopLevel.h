#pragma once
#include "GameScene.h"
#include "LevelComponent.h"
class CoopLevel final: public GameScene
{

public:
	CoopLevel();
	~CoopLevel() = default;
	CoopLevel(const CoopLevel& other) = delete;
	CoopLevel(CoopLevel&& other) noexcept = delete;
	CoopLevel& operator=(const CoopLevel& other) = delete;
	CoopLevel& operator=(CoopLevel&& other) noexcept = delete;

	void Update(float) override;
	void Render() override;
	void Initialize() override;
	void FixedUpdate() override;

private:
	void PlayerOne(std::shared_ptr<LevelComponent> slevel);
	void PlayerTwo(std::shared_ptr<LevelComponent> slevel);

	void UpdateGameLogic();
	void UpdateIfGameIsWon();
	void Enemy(EnemyType&, std::shared_ptr<LevelComponent>);
	void ResetLevel();
	void CheckIfPlayerLost();

	Rectf m_EnemyPos, m_Player1Pos, m_Player2Pos,m_playerBulletPos;
	bool m_RestartGame;
	float m_AccumulatedSec;

};

