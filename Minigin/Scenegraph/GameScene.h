#pragma once
#include "GameObject.h"
#include <memory>
#include <string>
#include <vector>

class GameScene

{
public:
    GameScene(std::string scene);
    virtual ~GameScene() = default;

    GameScene(const GameScene& other) = delete;
    GameScene(GameScene&& other) = delete;
    GameScene& operator=(const GameScene& other) = delete;
    GameScene& operator=(GameScene&& other) = delete;

    void AddChild(std::shared_ptr<dae::GameObject>);
    void RemoveChild(std::shared_ptr<dae::GameObject>);

    virtual void Initialize() = 0;
    virtual void Update(float) = 0;
    virtual void FixedUpdate() = 0;
    virtual void Render() = 0;
    virtual void SetScore(int /*Score*/){};
    virtual void SetLives(int) {};

    std::string GetSceneName() { return m_sceneName; }


    void RootUpdate(float);
    void RootFixedUpdate();
    void RootRender() const;

protected:

    void ClearScene();
    std::vector<std::shared_ptr<dae::GameObject>> m_sceneObjects;
    std::string m_sceneName;
};

