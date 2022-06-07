#include "pch.h"
#include "ScoreScene.h"

ScoreScene::ScoreScene(int player, int score)
	:GameScene("ScoreScene")
	,m_Score(score)
	,m_Player(player)
{
	
}


void ScoreScene::Initialize()
{
}

void ScoreScene::Update(float)
{
	m_SText.get()->SetText("Player "+std::to_string(m_Player)+ " " + std::to_string(m_Score));
}

void ScoreScene::FixedUpdate()
{
}

void ScoreScene::Render()
{
	m_SText.get()->Render();
}
