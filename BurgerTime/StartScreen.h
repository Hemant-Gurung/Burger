#pragma once
#include "Scenegraph/GameScene.h"
#include "Utils/Sound_System.h"
#include "RenderComponent.h"
//#include "ButtonManager.h"
class StartScreen :
    public GameScene
{

public:
    StartScreen();
    virtual ~StartScreen() = default;

    StartScreen(const StartScreen& other) = delete;
    StartScreen(StartScreen&& other) noexcept = delete;
    StartScreen& operator=(const StartScreen& other) = delete;
    StartScreen& operator=(StartScreen&& other) noexcept = delete;

    void Initialize() override;
    void Update(float) override;
    void FixedUpdate() override;
    void Render() override;

private:
    Vector2f m_Position;
    Rectf m_DestRect;
    Rectf m_SrcRect;
    bool m_showSecondScreen;

    float m_AccumulatedSeconds;
    bool m_IsStartButtonPressed;
    bool m_IsVsLevelChosen;
    bool m_IsCoOpLevelChosen;

    bool m_IsGameStarted;
    //std::shared_ptr<ButtonManager> m_ButtonManager;
    void ResetLevel();
    void UpdateChosenLevel(float);
    void UpdatePlayerChoice();

};

