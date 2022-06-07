#pragma once
#include "Scenegraph/GameScene.h"
#include "Components/LevelComponent.h"
#include "PlayerComponent.h"
#include "ScoreComponent.h"
#include "LivesCounterComponent.h"

class SecondLevel final :
    public GameScene
{
public:
    SecondLevel();
    ~SecondLevel() = default;

    SecondLevel(const SecondLevel& other) = delete;
    SecondLevel(SecondLevel&& other) noexcept = delete;
    SecondLevel& operator=(const SecondLevel& other) = delete;
    SecondLevel& operator=(SecondLevel&& other) noexcept = delete;

    void Initialize() override;
    void Update(float) override;
    void FixedUpdate() override;
    void Render() override;

private:
    std::shared_ptr<dae::PlayerComponent> m_Player;
    std::shared_ptr<LevelComponent> m_sLevel;
    LevelComponent* m_pLevel;
    std::shared_ptr<dae::ScoreComponent> m_Score;
    std::shared_ptr<dae::LivesCounterComponent> m_lives;
    std::shared_ptr<dae::TextComponent> m_Textlives;
    std::shared_ptr<dae::TextComponent> m_TextScore;

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
    void ResetScene();

};

