#include "MiniginPCH.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "PlayerComponent.h"
#include <fstream>
#include  "EnemyComponent.h"
#include <algorithm>
dae::ScoreComponent::ScoreComponent(std::shared_ptr<GameObject> pGameObject, std::shared_ptr<TextComponent> text)
	:BaseComponent(pGameObject),
	m_TextComponent(text),
	m_HightestScore(text),
    m_Score("0"),
m_ShowFinalScore(false)
{
	
}

void dae::ScoreComponent::update(float dt)
{
	//m_pGameObject.lock()->GetComponent<TextComponent>()->SetText("SCORE " + m_Score);
	//auto player = m_pGameObject.lock()->GetComponent<dae::PlayerComponent>();
	//m_Score = m_pGameObject.lock()->GetComponent<PlayerComponent>()->GetScore();
	m_TextComponent->update(dt);
	//m_TextComponent->SetText(m_Text + std::to_string(m_Score));
}

void dae::ScoreComponent::Render() const
{
	if(!m_Score.empty())
	  m_TextComponent->Render();
	
}

void dae::ScoreComponent::SetText()
{
}

void dae::ScoreComponent::setScore(std::string text, std::string score)
{
	m_Score = score;
	m_Text = text;
	m_TextComponent->SetText(text + m_Score);
	std::ifstream input("../Data/best_score.txt");
	int s{ -1 };
	std::string t;
	while (input >> s)
	{
		if (std::find(m_ScoreList.begin(), m_ScoreList.end(), s) == m_ScoreList.end())
		{
			m_ScoreList.push_back(s);
		}

	}

	if (m_ShowFinalScore/*std::find(m_ScoreList.begin(), m_ScoreList.end(), score) == m_ScoreList.end()*/)
	{
		m_ScoreList.push_back(std::stoi(m_Score));

		std::string sc;
		m_ScoreList.push_back(std::stoi(m_Score));
		std::sort(m_ScoreList.begin(), m_ScoreList.end(), std::greater<>());
		m_ScoreList.erase(std::unique(m_ScoreList.begin(), m_ScoreList.end()), m_ScoreList.end());

		for (int i = 0; i < 10; ++i)
		{
			sc += std::to_string(m_ScoreList[i]) + " ";
		}

		//std::cout << score;
		m_Score = "HIGHEST SCORES: " + sc;
		m_TextComponent->SetText(m_Score);
		
		std::ofstream output;
		output.open("../Data/best_score.txt");
		//std::sort(m_ScoreList.begin(), m_ScoreList.end(), std::greater<>());

		// remove duplicates from the list
		for (size_t i = 0; i < m_ScoreList.size(); ++i)
		{
			output << m_ScoreList[i] << '\n';
		}



		output.close();

		

		//ShowFinalScores();
	}
}

void dae::ScoreComponent::OnNotify(const dae::BaseComponent&, dae::EVENT& notifiedEvents)
{
	switch (notifiedEvents)
	{
	case EVENT::PLAYER_SCOREADD:
	{
		m_Score += 50;
		m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->AddScore();
		m_Score = std::to_string(m_pGameObject.lock()->GetComponent<PlayerComponent>()->GetScore());
		m_TextComponent->SetText("SCORE: " + m_Score);

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

std::string  dae::ScoreComponent::ShowFinalScores()
{
	//std::string score;
	//m_ScoreList.push_back(std::stoi(m_Score));
	//std::sort(m_ScoreList.begin(), m_ScoreList.end(), std::greater<>());
	//for(int i =0;i<10;++i)
	//{
	//	score += std::to_string(m_ScoreList[i])+" ";
	//}

	//std::cout << score;
	//m_Score = "HIGHEST SCORES: "+ score;
	//m_TextComponent->SetText(m_Score);
	m_ShowFinalScore = true;
	return m_Score;
	
}


