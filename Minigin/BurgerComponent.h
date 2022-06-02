#pragma once
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"
class BurgerComponent :
    public dae::BaseComponent
{
public:
    BurgerComponent(std::shared_ptr<dae::GameObject>&, LevelComponent&);
    void Render() const override;
    void update(float) override;
    void SetPosition(float, float, float) override;
    void SetTexture(const std::string&) override;

private:

    //BURGER SPRITE
    std::shared_ptr<dae::RenderComponent> m_BurgerTop;
    std::shared_ptr<dae::RenderComponent> m_BurgerSalad;
    std::shared_ptr<dae::RenderComponent> m_BurgerMeat;
    std::shared_ptr<dae::RenderComponent> m_BurgerBottom;

    // Burger position
    Rectf m_TopBurgerSize;
    Rectf m_SaladSize;
    Rectf m_MeatSize;
    Rectf m_BottomBurgerSize;

    LevelComponent* m_sLevel;

    void InitializeBurgers();
};

