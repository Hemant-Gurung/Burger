#include "MiniginPCH.h"
#include "SceneManager.h"
#include "InputManager.h"

void dae::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_Scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::FixedUpdate()
{
	for (const auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::Render() const
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

std::shared_ptr<GameScene> dae::SceneManager::GetGameScene(const std::string& name)
{
	for(auto scene:m_Scenes)
	{
		if(scene->GetSceneName() == name)
		{
			return scene;
		}
	}

	return nullptr;
}

dae::SceneManager::SceneManager()
{

}


void dae::SceneManager::AddGameScene(std::shared_ptr<GameScene> scene)
{
	const auto it = std::find(std::begin(m_Scenes), std::end(m_Scenes), scene);
	if(it!= m_Scenes.end())
	{
		return;
	}

	m_Scenes.emplace_back(scene);
	if(m_CurrentScene == nullptr)
	{
		m_CurrentScene = scene;
	}
}

void dae::SceneManager::setActive(std::string name)
{
	
	for(auto scene:m_Scenes)
	{
		/*if(m_CurrentScene.get()->GetSceneName() == name )
		{
			m_CurrentScene->Initialize();
			return;
		}*/
		if(scene->GetSceneName() == name)
		{

			m_CurrentScene = scene;
			//InputManager::GetInstance().ResetInput();
			m_CurrentScene->Initialize();
			return;
		}
	}
}
//
//GameScene& dae::SceneManager::CreateScene(const std::string& name)
//{
//	const auto& scene = std::shared_ptr<GameScene>(new GameScene(name));
//	m_Scenes.push_back(scene);
//	return *scene;
//}


