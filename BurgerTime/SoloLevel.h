#pragma once

#include "GameScene.h"
#include "LevelComponent.h"
#include "Scene.h"
#include "PlayerComponent.h"
#include "LevelComponent.h"


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

    dae::PlayerComponent* m_Player;
    LevelComponent* m_pLevel;
    bool m_hasOverlapped;
    bool CheckIfPlayerIsDead(dae::PlayerComponent&);
    void ResetScene();
  

};


