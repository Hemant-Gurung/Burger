#pragma once
#include "Observer.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "BaseComponent.h"
namespace dae
{
    class ScoreComponent :
        public BaseComponent,public Observer
    {
    public:
        ScoreComponent(std::shared_ptr<GameObject>,std::shared_ptr<TextComponent> text);
        ~ScoreComponent() = default;

        ScoreComponent(const ScoreComponent& other) = delete;
        ScoreComponent(ScoreComponent&& other) = delete;
        ScoreComponent& operator=(const ScoreComponent& other) = delete;
        ScoreComponent& operator=(ScoreComponent&& other) = delete;

        void update(float) override;
        void Render() const override;
        void SetText();
        void setScore();
        void OnNotify(const dae::BaseComponent&, dae::EVENT&) override;
        void SetPosition(float x, float y, float z);
        int m_Score{};
    private:
        std::shared_ptr<TextComponent> m_TextComponent;
       
       
    };

}

