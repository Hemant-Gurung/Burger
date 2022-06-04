#pragma once
#include <GameScene.h>
class StartScreen :
    public GameScene
{

public:
    StartScreen();
    virtual ~StartScreen() = default;

    void Initialize() override;
    void Update(float) override;
    void FixedUpdate() override;
    void Render() override;

private:

    Vector2f m_Position;
    bool m_showSecondScreen;
   
};

