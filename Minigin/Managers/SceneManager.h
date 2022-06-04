#pragma once
#include "Singleton.h"
#include "GameScene.h"
#include <memory>
#include <string>
#include <vector>
namespace dae
{
	//class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager() = default;

		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

		void AddGameScene(std::shared_ptr<GameScene> scene);
		void setActive(std::string name);

		//GameScene& CreateScene(const std::string& name);

		void Update(float);
		void FixedUpdate();
		void Render() const;

		std::shared_ptr<GameScene> GetGameScene(const std::string& name);
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		
		std::vector<std::shared_ptr<GameScene>> m_Scenes;
		std::shared_ptr<GameScene> m_CurrentScene;
	};
}
