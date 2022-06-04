#pragma once
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"
#include "PlayerComponent.h"
class BurgerComponent :
    public dae::BaseComponent
{
public:
    BurgerComponent(std::shared_ptr<dae::GameObject>&, LevelComponent&,const float left,const float4& burgerBottom);
    void Render() const override;
    void update(float) override;
    void SetPosition(float, float, float) override;
    void SetTexture(const std::string&) override;
    void Check_Player_Burger_Collision(Rectf& );
private:

    //BURGER SPRITE
    std::shared_ptr<dae::RenderComponent> m_BurgerTop;
    std::shared_ptr<dae::RenderComponent> m_BurgerSalad;
    std::shared_ptr<dae::RenderComponent> m_BurgerMeat;
    std::shared_ptr<dae::RenderComponent> m_BurgerBottom;

    std::shared_ptr<dae::RenderComponent> m_BurgerIcon;

    // Burger position
    Rectf m_TopBurgerSize;
    Rectf m_SaladSize;
    Rectf m_MeatSize;
    Rectf m_BottomBurgerSize;

    LevelComponent* m_sLevel;

    void InitializeBurgers();
    void OnGuiUpdate();
    void Handle_Burger_Burger_Collision();
    void HandleBurgerDropLogicWithPlayer();
    void HandleBurgerDorpLogicWithBurger();
    void RenderBurgerCount() const;

    bool m_ShowColliders;

    // burger player collision
    bool m_IsTopOverlappingWithPlayer;
    bool m_IsSaladOverlappingWithPlayer;
    bool m_IsMeatOverlappingWithPlayer;
    bool m_IsBottomOverlappingWithPlayer;
    float m_LowestPos = 590.f;


    // BURGER- BURGER COLLISION
    bool m_Top_SaladCollision;
    bool m_Salad_MeatCollision;
    bool m_Meat_BottomCollision;
};

