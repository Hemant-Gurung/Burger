#pragma once

#include "Scenegraph/GameScene.h"
#include "Components/LevelComponent.h"
#include "PlayerComponent.h"
#include "ScoreComponent.h"
#include "LivesCounterComponent.h"

class SoloLevel final :
    public GameScene
{
public:
    SoloLevel();
    ~SoloLevel()= default;

    SoloLevel(const SoloLevel& other) = delete;
    SoloLevel(SoloLevel&& other) noexcept = delete;
    SoloLevel& operator=(const SoloLevel& other) = delete;
    SoloLevel& operator=(SoloLevel&& other) noexcept = delete;

    void Update(float) override;
    void FixedUpdate() override;
    void Initialize() override;
    void Render() override;

    void Enemy( EnemyType&, std::shared_ptr<LevelComponent>) ;
    void AddBurger(std::shared_ptr<LevelComponent>) ;
    void AddObserver(std::shared_ptr<dae::Observer> observer);
    void Notify(dae::BaseComponent&, dae::EVENT);
private:
    void PlayerOne(std::shared_ptr<LevelComponent> slevel) ;
    void CheckBurgerDropOnEnemy();
    Rectf m_enemyPos;
    Rectf m_PlayerPos;

    std::shared_ptr<dae::PlayerComponent> m_Player;
    std::shared_ptr<LevelComponent> m_sLevel;
    std::shared_ptr<dae::ScoreComponent> m_Score;
    std::shared_ptr<dae::LivesCounterComponent> m_lives;


    bool m_hasOverlapped;
    bool CheckIfPlayerIsDead(dae::PlayerComponent&);
    void ResetScene();
    void GenerateEnemies();

    float m_accumulatedDeathTime;

    Sound_System* sound;
    std::shared_ptr<dae::GameObject> gameObjectEnemy;
    std::shared_ptr<dae::GameObject> gameObjectPlayer;
    std::vector<std::shared_ptr<dae::Observer>> m_Observers;


};






