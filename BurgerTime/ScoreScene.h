#pragma once
#include "Scenegraph/GameScene.h"
#include "Components/TextComponent.h"
class ScoreScene :
    public GameScene
{
public:

    ScoreScene(int player =0, int score =-1);
    virtual ~ScoreScene() = default;

    ScoreScene(const ScoreScene& other) = delete;
    ScoreScene(ScoreScene&& other) noexcept = delete;
    ScoreScene& operator=(const ScoreScene& other) = delete;
    ScoreScene& operator=(ScoreScene&& other) noexcept = delete;

    void Initialize() override;
    void Update(float) override;
    void FixedUpdate() override;
    void Render() override;

private:

  /*  Vector2f m_Position;
    bool m_showSecondScreen;*/
    float m_countSecondsToShowScore;
    std::shared_ptr<dae::TextComponent> m_SText;
    int m_Score;
    int m_Player;
};

