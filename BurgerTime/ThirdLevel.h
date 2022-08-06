#pragma once
#include "Scenegraph/GameScene.h"
#include "Components/LevelComponent.h"
#include "PlayerComponent.h"

#include "ScoreComponent.h"
#include "LivesCounterComponent.h"

class ThirdLevel :
    public GameScene
{

public:
    ThirdLevel();
    ~ThirdLevel();

    ThirdLevel(const ThirdLevel & other) = delete;
    ThirdLevel(ThirdLevel && other) noexcept = delete;
    ThirdLevel& operator=(const ThirdLevel & other) = delete;
    ThirdLevel& operator=(ThirdLevel && other) noexcept = delete;

    void Initialize() override;
    void Update(float) override;
    void FixedUpdate() override;
    void Render() override;

    void SetScore(int score);
    void SetLives(int lives);
private:
    //std::shared_ptr<LevelComponent> m_sLevel;
   // LevelComponent* m_pLevel;
   // std::shared_ptr<LevelComponent> m_sLevel;

    std::shared_ptr<dae::ScoreComponent> m_Score;
    std::shared_ptr<dae::ScoreComponent>m_HightestScore;
    std::shared_ptr<dae::GameObject> gameObjectPlayer;
    bool m_ShowScore;
    bool m_IsInitialized;

    bool m_hasOverlapped;
    //bool CheckIfPlayerIsDead(dae::PlayerComponent&);
    //void ResetScene();

    float m_accumulatedDeathTime;


    Rectf m_enemyPos;
    Rectf m_PlayerPos;

    void Enemy(EnemyType&, std::shared_ptr<LevelComponent>);
    void AddBurger(std::shared_ptr<LevelComponent>);
    void PlayerOne(std::shared_ptr<LevelComponent> slevel);
    bool CheckIfPlayerIsDead(dae::PlayerComponent&);
    //void ResetScene();
};

