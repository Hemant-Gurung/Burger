#include "MiniginPCH.h"
#include "BurgerComponent.h"

//using namespace 

	BurgerComponent::BurgerComponent(std::shared_ptr<dae::GameObject>& pGameObject, LevelComponent&)
		:BaseComponent(pGameObject)
	{
		//TOP BUN
		m_BurgerTop = std::make_shared<dae::RenderComponent>(pGameObject);
		m_BurgerTop->SetTexture("Burger/BurgerTop.png");
		//SAAD
		m_BurgerSalad = std::make_shared<dae::RenderComponent>(pGameObject);
		m_BurgerSalad->SetTexture("Burger/BurgerSalad.png");

		m_BurgerMeat = std::make_shared<dae::RenderComponent>(pGameObject);
		m_BurgerMeat->SetTexture("Burger/BurgerMeat.png");

		m_BurgerBottom = std::make_shared<dae::RenderComponent>(pGameObject);
		m_BurgerBottom->SetTexture("Burger/BurgerBottom.png");

		InitializeBurgers();

	}

	void BurgerComponent::Render() const
	{
		BaseComponent::Render();
		Rectf srcrect;
		srcrect.left = 0.f;
		srcrect.bottom = 0.f;
		srcrect.width = 31.f;
		srcrect.height =10.f;


		m_BurgerTop->RenderTexture(m_TopBurgerSize,srcrect, 0);
	}

	void BurgerComponent::update(float x)
	{
		BaseComponent::update(x);
	}

	void BurgerComponent::SetPosition(float x, float x1, float x2)
	{
		BaseComponent::SetPosition(x, x1, x2);
	}

	void BurgerComponent::SetTexture(const std::string& basic_string)
	{
		BaseComponent::SetTexture(basic_string);
	}

void BurgerComponent::InitializeBurgers()
{
		//LEFT POS
	m_TopBurgerSize.left = 40.f;
	m_SaladSize.left = 40.f;
	m_MeatSize.left = 40.f;
	m_BottomBurgerSize.left = 40.f;

	//Bottom
	m_TopBurgerSize.bottom = 30.f;
	m_SaladSize.bottom = 30.f;
	m_MeatSize.bottom = 50.f;
	m_BottomBurgerSize.bottom = 80.f;

		//width
	m_TopBurgerSize.width = 50.f;
	m_SaladSize.width = 35.f;
	m_MeatSize.width = 35.f;
	m_BottomBurgerSize.width = 35.f;

	m_TopBurgerSize.height = 15.f;
	m_SaladSize.height = 7.f;
	m_MeatSize.height = 7.f;
	m_BottomBurgerSize.height = 7.f;
}

