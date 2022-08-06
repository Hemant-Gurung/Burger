#pragma once
#include "Base/Observer.h"
#include "Scenegraph/GameObject.h"
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
        void setScore(std::string text, std::string m_score);
        void OnNotify(const dae::BaseComponent&, dae::EVENT&) override;
        void SetPosition(float x, float y, float z);
        std::string ShowFinalScores();
    private:
        std::shared_ptr<TextComponent> m_TextComponent;
        std::shared_ptr<TextComponent> m_HightestScore;

        std::string m_Score{};
        std::string m_Text;
    	std::vector<int> m_ScoreList;
        bool m_ShowFinalScore;

       
    };

}

