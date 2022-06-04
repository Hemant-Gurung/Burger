#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include "Observer.h"
namespace dae
{
    static int m_TotalLives = 3;
    class LivesCounterComponent :
        public BaseComponent,public Observer
    {
    public:
        LivesCounterComponent(std::shared_ptr<GameObject>&, std::shared_ptr<TextComponent> text);
        ~LivesCounterComponent() = default;
         
        LivesCounterComponent(const LivesCounterComponent& other) = delete;
        LivesCounterComponent(LivesCounterComponent&& other) = delete;
        LivesCounterComponent& operator=(const LivesCounterComponent& other) = delete;
        LivesCounterComponent& operator=(LivesCounterComponent&& other) = delete;

        void update(float) override;
        void Render() const override;
        void SetPosition(float x,float y,float z);
        //observer
         void OnNotify(const dae::BaseComponent&, dae::EVENT&);
    private:
        std::shared_ptr<TextComponent> m_TextComponent;
        void UpdateLives();
       
    };
}

