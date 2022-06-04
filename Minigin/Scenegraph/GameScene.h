#pragma once
#include "SceneObject.h"
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

    void AddChild(std::shared_ptr<SceneObject>);
    void RemoveChild(std::shared_ptr<SceneObject>);

    virtual void Initialize() = 0;
    virtual void Update(float) = 0;
    virtual void FixedUpdate() = 0;
    virtual void Render() = 0;


    std::string GetSceneName() { return m_sceneName; }


    void RootUpdate(float);
    void RootFixedUpdate();
    void RootRender() const;

protected:
    std::vector<std::shared_ptr<SceneObject>> m_sceneObjects;
    std::string m_sceneName;
};

