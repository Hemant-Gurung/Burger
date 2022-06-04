#include "MiniginPCH.h"
#include "GameScene.h"

GameScene::GameScene(std::string scene)
	:m_sceneName(scene)
{

}

void GameScene::AddChild(std::shared_ptr<dae::GameObject> scene)
{
	//add scene to the list
	m_sceneObjects.emplace_back(scene);
}

void GameScene::RemoveChild(std::shared_ptr<dae::GameObject> scenechild)
{

	// remove the child from the scene objects
	m_sceneObjects.erase(std::remove(std::begin(m_sceneObjects), std::end(m_sceneObjects), scenechild), std::end(m_sceneObjects));
}


void GameScene::RootUpdate(float dt)
{
	for (auto element : m_sceneObjects)
	{
		element->Update(dt);
	}
}

void GameScene::RootFixedUpdate()
{
	for (auto element : m_sceneObjects)
	{
		element->FixedUpdate();
	}
}

void GameScene::RootRender() const
{
	for (auto element : m_sceneObjects)
	{
		element->Render();
	}
}

void GameScene::ClearScene()
{
	m_sceneObjects.clear();
}
