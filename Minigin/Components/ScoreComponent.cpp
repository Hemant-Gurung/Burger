#include "MiniginPCH.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "PlayerComponent.h"
#include  "EnemyComponent.h"
dae::ScoreComponent::ScoreComponent(std::shared_ptr<GameObject> pGameObject, std::shared_ptr<TextComponent> text)
	:BaseComponent(pGameObject),
	m_Score{0},
	m_TextComponent(text)
{
}

void dae::ScoreComponent::update(float)
{
	//m_pGameObject.lock()->GetComponent<TextComponent>()->SetText("SCORE " + m_Score);
	//auto player = m_pGameObject.lock()->GetComponent<dae::PlayerComponent>();
	m_TextComponent->SetText("SCORE: " + m_Score);
}

void dae::ScoreComponent::Render() const
{
	m_TextComponent->Render();
}

void dae::ScoreComponent::SetText()
{
}

void dae::ScoreComponent::setScore()
{
	
}

void dae::ScoreComponent::OnNotify(const dae::BaseComponent&, dae::EVENT& notifiedEvents)
{
	switch (notifiedEvents)
	{
	case EVENT::PLAYER_SCOREADD:
	{
		m_Score += 50;
		//m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->AddScore();
	}
	break;
	case EVENT::ENEMY_DEAD:
	{
		auto a = m_pGameObject.lock()->GetComponent<dae::EnemyComponent>();
		if (a != nullptr)
		{
			a->IsDead(true);
		}
		break;
	}
	default:
		break;
	}
}

void dae::ScoreComponent::SetPosition(float x, float y, float z)
{
	m_TextComponent->SetPosition(x, y, z);
}


