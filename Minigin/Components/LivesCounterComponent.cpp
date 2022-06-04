#include "MiniginPCH.h"
#include "LivesCounterComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

dae::LivesCounterComponent::LivesCounterComponent(std::shared_ptr<GameObject>& pGameObject, std::shared_ptr<TextComponent> text)
	:BaseComponent(pGameObject), Observer(),
	m_TotalLives{3},
	m_TextComponent(text)
{
	
}

void dae::LivesCounterComponent::update(float)
{
	
	m_TextComponent->SetText(" " + std::to_string(m_TotalLives));
}

void dae::LivesCounterComponent::Render() const
{
	m_TextComponent->Render();
}

void dae::LivesCounterComponent::SetPosition(float x, float y, float z)
{
	m_TextComponent->SetPosition(x, y, z);
}

void dae::LivesCounterComponent::OnNotify(const dae::BaseComponent&, dae::EVENT& notifyEvent)
{
	switch (notifyEvent)
	{
	case EVENT::PLAYER_DEAD:
	{
		if (m_TotalLives > 0)
		{
			m_TotalLives -= 1;
			
		}
	}
	break;
	default:
		break;
	}
}

void dae::LivesCounterComponent::UpdateLives()
{

}
