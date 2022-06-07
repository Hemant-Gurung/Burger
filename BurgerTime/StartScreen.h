#pragma once
#include "Scenegraph/GameScene.h"
#include "Utils/Sound_System.h"
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
    Sound_System* sound;
    Vector2f m_Position;
    bool m_showSecondScreen;
   
};

