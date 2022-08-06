#pragma once
#include "GameScene.h"
#include "LevelComponent.h"
class VersusLevel final :public GameScene
{
public:
	VersusLevel() ;
	~VersusLevel() = default;
	VersusLevel(const VersusLevel& other) = delete;
	VersusLevel(VersusLevel&& other) noexcept = delete;
	VersusLevel& operator=(const VersusLevel& other) = delete;
	VersusLevel& operator=(VersusLevel&& other) noexcept = delete;	

	void Update(float) override;
	void Render() override;
	void Initialize() override;
	void FixedUpdate() override;
private:
	void PlayerOne(std::shared_ptr<LevelComponent> slevel);
	void PlayerTwo(std::shared_ptr<LevelComponent> slevel);

	void UpdateGameLogic();
	void CheckIfPlayerLost(const dae::PlayerComponent&);
	void ResetLevel();
	Rectf m_player1Pos;
	Rectf m_player2Pos;

	bool m_IsP1Damaged;
	bool m_IsP2Damaged;

	bool m_RestartGame;
	float m_AccumulatedSec;
};

