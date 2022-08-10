#include "MiniginPCH.h"
#include "TextComponent.h"
#include "Scenegraph/GameObject.h"
#include "PlayerComponent.h"
#include "PepperComponent.h"


PepperComponent::PepperComponent(std::shared_ptr<dae::GameObject>& pGameObject, std::shared_ptr<dae::TextComponent> text)
	:BaseComponent(pGameObject), Observer(),
	m_TextComponent(text)
{

}

void PepperComponent::update(float)
{
	if (m_pGameObject.lock() != nullptr)
	{
		//m_TextComponent->SetText(" " + std::to_string(m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->GetRemainingPepper()));
	}
}

void PepperComponent::Render() const
{
	m_TextComponent->Render();
}

void PepperComponent::SetPosition(float x, float y, float z)
{
	m_TextComponent->SetPosition(x, y, z);
}

void PepperComponent::OnNotify(const dae::BaseComponent&, dae::EVENT& notifyEvent)
{
	switch (notifyEvent)
	{
	case dae::EVENT::PLAYER_PEPPERTHROW:
	{
		{
			
			//m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->DecreasePepper();
			
		}
	}
	break;
	default:
		break;
	}
}

