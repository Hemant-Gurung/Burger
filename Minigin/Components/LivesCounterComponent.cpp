#include "MiniginPCH.h"
#include "LivesCounterComponent.h"
#include "TextComponent.h"
#include "Scenegraph/GameObject.h"
#include "PlayerComponent.h"

 static int m_TotalLives = 3;

dae::LivesCounterComponent::LivesCounterComponent(std::shared_ptr<GameObject>& pGameObject, std::shared_ptr<TextComponent> text)
	:BaseComponent(pGameObject), Observer(),
	m_TextComponent(text)
{
	
}

void dae::LivesCounterComponent::update(float)
{
	if (m_pGameObject.lock() != nullptr)
	{
		m_TextComponent->SetText(" " + std::to_string(m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->GetLives()));
	}
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
			m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->DestroyLive();
			//m_TotalLives -= 1;
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
