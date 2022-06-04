#pragma once

#include "GameScene.h"
#include "LevelComponent.h"
#include "Scene.h"
#include "PlayerComponent.h"
#include "LevelComponent.h"
#include "ScoreComponent.h"
#include "LivesCounterComponent.h"

class SoloLevel final :
    public GameScene
{
public:
    SoloLevel();
    ~SoloLevel() = default ;

    void Update(float) override;
    void FixedUpdate() override;
    void Initialize() override;
    void Render() override;

    void Enemy( EnemyType&, LevelComponent&) ;
    void AddBurger(LevelComponent&) ;

private:
    void PlayerOne(LevelComponent& slevel) ;

    Rectf m_enemyPos;
    Rectf m_PlayerPos;

    std::shared_ptr<dae::PlayerComponent> m_Player;
    LevelComponent* m_pLevel;
    std::shared_ptr<dae::ScoreComponent> m_Score;
    std::shared_ptr<dae::LivesCounterComponent> m_lives;
    std::shared_ptr<dae::TextComponent> m_Textlives;
    std::shared_ptr<dae::TextComponent> m_TextScore;


    bool m_hasOverlapped;
    bool CheckIfPlayerIsDead(dae::PlayerComponent&);
    void ResetScene();

    float m_accumulatedDeathTime;

};


