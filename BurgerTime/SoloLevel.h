#pragma once

#include "GameScene.h"
#include "LevelComponent.h"
#include "Scene.h"
//#include "EnemyComponent.h"

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
    
};

