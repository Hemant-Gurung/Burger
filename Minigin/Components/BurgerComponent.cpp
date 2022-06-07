#include "MiniginPCH.h"
#include "BurgerComponent.h"
#include "utils.h"

//using namespace 

BurgerComponent::BurgerComponent(std::shared_ptr<dae::GameObject>& pGameObject, std::shared_ptr<LevelComponent> level, const Point2f leftLast,const float4 burgerBottom)
	:BaseComponent(pGameObject),
		m_sLevel(std::move(level))
		,m_LowestPos(leftLast.y)
{
	// Initialize Burger Position
	m_TopBurgerSize.left = leftLast.x;
	m_SaladSize.left = leftLast.x;
	m_MeatSize.left = leftLast.x;
	m_BottomBurgerSize.left = leftLast.x;

	//Bottom
	m_TopBurgerSize.bottom = burgerBottom.one;
	m_SaladSize.bottom = burgerBottom.two;
	m_MeatSize.bottom = burgerBottom.three;
	m_BottomBurgerSize.bottom = burgerBottom.four;



	m_IsTopOverlappingWithPlayer = false;
	m_IsSaladOverlappingWithPlayer = false;
	m_IsMeatOverlappingWithPlayer = false;
	m_IsBottomOverlappingWithPlayer = false;

	//initialize burger-burger collision
	m_Top_SaladCollision = false;
	m_Salad_MeatCollision = false;
	m_Meat_BottomCollision = false;

	m_ShowColliders = false;
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


	//BURGER ICON
	m_BurgerIcon = std::make_shared<dae::RenderComponent>(pGameObject);
	m_BurgerIcon->SetTexture("Burger/BurgerLogo.png");
	


	InitializeBurgers();

}

void BurgerComponent::Render() const
{
	BaseComponent::Render();
	Rectf srcrect;
	srcrect.left = 0.f;
	srcrect.bottom = 0.f;
	srcrect.width = 31.f;
	srcrect.height = 10.f;


	if (m_ShowColliders)
	{
		m_BurgerTop->RenderBox(m_TopBurgerSize, static_cast<int>(m_TopBurgerSize.width), static_cast<int>(m_TopBurgerSize.height));
		m_BurgerSalad->RenderBox(m_SaladSize, static_cast<int>(m_SaladSize.width), static_cast<int>(m_SaladSize.height));
		m_BurgerMeat->RenderBox(m_MeatSize, static_cast<int>(m_MeatSize.width), static_cast<int>(m_MeatSize.height));
		m_BurgerBottom->RenderBox(m_BottomBurgerSize, static_cast<int>(m_BottomBurgerSize.width), static_cast<int>(m_BottomBurgerSize.height));
	}
	// BTopBurger
	m_BurgerTop->RenderTexture(m_TopBurgerSize, srcrect, 0);

	// SALAD

	m_BurgerSalad->RenderTexture(m_SaladSize, srcrect, 0);

	// MEAT

	m_BurgerMeat->RenderTexture(m_MeatSize, srcrect, 0);

	// BOTTOM


	m_BurgerBottom->RenderTexture(m_BottomBurgerSize, srcrect, 0);



	//REnder burger icon
	//RenderBurgerCount();
}

void BurgerComponent::update(float /*x*/)
{
	//BaseComponent::update(x);
	OnGuiUpdate();
	IsPlayerOverlappingWithPlayer();
	Handle_Burger_Burger_Collision();
	HandleBurgerDropLogicWithPlayer();
	HandleBurgerDorpLogicWithBurger();
	HandleBurgerEnemyCollision();

	
}

void BurgerComponent::SetPosition(float x, float x1, float x2)
{
	BaseComponent::SetPosition(x, x1, x2);
}

void BurgerComponent::SetTexture(const std::string& basic_string)
{
	BaseComponent::SetTexture(basic_string);
}

bool BurgerComponent::IsPlayerOverlappingWithPlayer()
{
	if (m_sLevel.lock() != nullptr)
	{
		return Check_Player_Burger_Collision(m_sLevel.lock()->GetPlayerPositionInTheLevel());
	}
	return false;
}

void BurgerComponent::AddObserver(std::shared_ptr<dae::Observer> observer)
{
	m_Observers.push_back(observer);
}

void BurgerComponent::Notify(BaseComponent& actor, dae::EVENT e)
{
	for (size_t i = 0; i < m_Observers.size(); ++i)
	{
		m_Observers[i]->OnNotify(actor, e);
	}
}

bool BurgerComponent::Check_Player_Burger_Collision(Rectf& playerPos)
{
	if (utils::IsOverlapping(m_TopBurgerSize, playerPos))
	{
		//Notify(*this, dae::EVENT::PLAYER_SCOREADD);
		m_IsTopOverlappingWithPlayer = true;
		return true;
	}
	
	if (utils::IsOverlapping(m_SaladSize, playerPos))
	{
		//Notify(*this, dae::EVENT::PLAYER_SCOREADD);
		m_IsSaladOverlappingWithPlayer = true;
		return true;
	}
	
	if (utils::IsOverlapping(m_MeatSize, playerPos))
	{
		//Notify(*this, dae::EVENT::PLAYER_SCOREADD);
		m_IsMeatOverlappingWithPlayer = true;
		return true;
	}
	
	if (utils::IsOverlapping(m_BottomBurgerSize, playerPos))
	{
		//Notify(*this, dae::EVENT::PLAYER_SCOREADD);
		m_IsBottomOverlappingWithPlayer = true;
		return true;
	}
	
	return false;

}

void BurgerComponent::InitializeBurgers()
{
	
	//width
	m_TopBurgerSize.width = 80.f;
	m_SaladSize.width = 80.f;
	m_MeatSize.width = 80.f;
	m_BottomBurgerSize.width = 80.f;

	m_TopBurgerSize.height = 20.f;
	m_SaladSize.height = 20.f;
	m_MeatSize.height = 20.f;
	m_BottomBurgerSize.height = 20.f;
}

void BurgerComponent::OnGuiUpdate()
{
	//ImGui::Begin("BurgerInfo", NULL);
	////ImGui::SetWindowSize(ImVec2((float)10.f, (float)10.f));
	////ImGui::SetNextWindowPos()
	//ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
	//ImGui::Checkbox("ShowColliders", &m_ShowColliders);
	////if (ImGui::Button("Reset Speed")) {
	//	// This code is executed when the user clicks the button
	//	//this->speed = 0;
	////}
	//ImGui::Checkbox("IsOverlapping", &m_IsTopOverlappingWithPlayer);
	//ImGui::SliderFloat("BurgerTopPosition", &m_TopBurgerSize.bottom, 0.0f, 700.0f);
	//ImGui::SliderFloat("BurgerTopLeft", &m_TopBurgerSize.left, 0.0f, 1000.0f);
	////ImGui::SliderFloat("BurgerTopPosition", &m_TopBurgerSize.bottom, 0.0f, 500.0f);

	//ImGui::End();
}

void BurgerComponent::Handle_Burger_Burger_Collision()
{
	//check if top is overlapping with the salad
	if (utils::IsOverlapping(m_TopBurgerSize, m_SaladSize))
	{
		m_IsTopOverlappingWithPlayer = false;
		m_Top_SaladCollision = true;
		//m_SaladSize.bottom += 10;
		
		//m_IsTopOverlappingWithPlayer = false;
	}
	//check if salad is overlapping with the meat

	if (utils::IsOverlapping(m_SaladSize, m_MeatSize))
	{
		m_IsSaladOverlappingWithPlayer = false;
		m_Salad_MeatCollision = true;
		//m_MeatSize.bottom += 10;
	}

	//check if meat is overlapping with the bottom burger

	if (utils::IsOverlapping(m_MeatSize, m_BottomBurgerSize))
	{
		m_IsMeatOverlappingWithPlayer = false;
		m_Meat_BottomCollision = true;

	}
}

void BurgerComponent::HandleBurgerDropLogicWithPlayer()
{
	// Logic with player collision

	// TOP BURGER
	if (m_IsTopOverlappingWithPlayer)
	{
		m_TopBurgerSize.bottom += 10;
	}
	// SALAD
	if (m_IsSaladOverlappingWithPlayer)
	{
		m_SaladSize.bottom += 10;
	}
	//MEAT
	if (m_IsMeatOverlappingWithPlayer)
	{
		m_MeatSize.bottom += 10;
	}

	// BOTTOM BURGER PLAYER COLLISION LOGIC
	if (m_IsBottomOverlappingWithPlayer)
	{
		if (m_BottomBurgerSize.bottom < m_LowestPos)
		{
			m_BottomBurgerSize.bottom += 10;
		}
	}

}

void BurgerComponent::HandleBurgerDorpLogicWithBurger()
{

	if(m_Top_SaladCollision)
	{
		//if(m_IsTopOverlappingWithPlayer)
		{
			//m_TopBurgerSize.bottom += 10;
			
			m_IsTopOverlappingWithPlayer = false;
		}
		
		if (m_SaladSize.bottom < m_LowestPos - m_BottomBurgerSize.height - m_MeatSize.height)
		{
			m_SaladSize.bottom += 10;
		}
	}
	if (m_Salad_MeatCollision)
	{
		m_IsSaladOverlappingWithPlayer = false;
		m_Top_SaladCollision = false;
		if (m_MeatSize.bottom < m_LowestPos - m_BottomBurgerSize.height)
		{
			m_MeatSize.bottom += 10;
		}
	}
	if (m_Meat_BottomCollision)
	{
		m_IsMeatOverlappingWithPlayer = false;
		m_Salad_MeatCollision = false;
		if (m_BottomBurgerSize.bottom < m_LowestPos)
		{
			
			m_BottomBurgerSize.bottom += 10;
		}
		else
		{
			m_Meat_BottomCollision = false;
		}
	}
}

void BurgerComponent::RenderBurgerCount() const
{
	Rectf burgerpos;
	burgerpos.left = 540.f;
	burgerpos.bottom = 5.f;
	burgerpos.width = 20.f;
	burgerpos.height = 20.f;

	Rectf burgerpossrc;
	burgerpossrc.left = 0;
	burgerpossrc.bottom = 0;
	burgerpossrc.width = 7.f;
	burgerpossrc.height = 7.f;
	m_BurgerIcon->RenderTexture(burgerpos, burgerpossrc);
}

void BurgerComponent::HandleBurgerEnemyCollision()
{
	if (m_IsTopOverlappingWithPlayer)
	{
		if (utils::IsOverlapping(m_TopBurgerSize, m_sLevel.lock()->GetEnemyPosInTheLevel()))
		{
			Notify(*this, dae::EVENT::ENEMY_DEAD);
		
		}
	}
	if (m_IsSaladOverlappingWithPlayer)
	{
		if (utils::IsOverlapping(m_SaladSize, m_sLevel.lock()->GetEnemyPosInTheLevel()))
		{
			Notify(*this, dae::EVENT::ENEMY_DEAD);
			
		}
	}
	if (m_IsMeatOverlappingWithPlayer)
	{
		if (utils::IsOverlapping(m_MeatSize, m_sLevel.lock()->GetEnemyPosInTheLevel()))
		{
			Notify(*this, dae::EVENT::ENEMY_DEAD);
			
		}
	}
	if (m_IsBottomOverlappingWithPlayer)
	{
		if (utils::IsOverlapping(m_BottomBurgerSize, m_sLevel.lock()->GetEnemyPosInTheLevel()))
		{
			Notify(*this, dae::EVENT::ENEMY_DEAD);
			
		}
	}
}

void BurgerComponent::PlayBurgerSound()
{

}

//BOTTOM BURFER BURGER COLLISION LOGINC
