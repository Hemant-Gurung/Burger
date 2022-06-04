// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "MiniginPCH.h"

#include "Minigin.h"
//#include <thread>
//#include "InputManager.h"
//#include "SceneManager.h"
//#include "Renderer.h"
//#include "ResourceManager.h"
//#include "TextComponent.h"
//#include "GameObject.h"
//
//#include "RenderComponent.h"
//#include "FPScomponent.h"
//#include "TransformComponent.h"
//#include "LivesCounterComponent.h"
//#include "Observer.h"
//#include "PlayerComponent.h"
//#include "ScoreComponent.h"
//#include "BurgerComponent.h"
////#include <steam_api.h>
////#include "Achievements.h"
//#include <SDL_mixer.h>
//#include "SServiceLocator.h"
//#include "pch.h"
//#include <iostream>
#include "SoloLevel.h"
#include "BurgerGame.h"
//#include "vld.h"


 //namespace dae
//{


BurgerGame::BurgerGame()
{
}

BurgerGame::~BurgerGame()
{
}

void BurgerGame::LoadGame() const
{
	Minigin::LoadGame();

	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<SoloLevel>());
	dae::SceneManager::GetInstance().setActive("SoloLevel");
	//auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	////dae::SceneManager::GetInstance().AddGameScene("Demo");
	//dae::ResourceManager::GetInstance().Init("../Data/");


	//////////// game object_1
	////auto gameObj = std::make_shared<GameObject>();
	////auto transform = std::make_shared<TransformComponent>(gameObj);
	////gameObj->AddComponent(transform);


	// //Text
	////
	//////gameobj3
	//auto gameObj3 = std::make_shared<dae::GameObject>();
	//auto transform1 = std::make_shared<dae::TransformComponent>(gameObj3);
	//gameObj3->AddComponent(transform1);

	//scene.Add(gameObj3);

	////fpsComponent
	//auto gameObjFPS = std::make_shared<dae::GameObject>();
	//auto fontFPS = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);

	//auto text = std::make_shared<dae::TextComponent>(gameObj3, " ", fontFPS, SDL_Color{ 255,255,255,1 });

	////make fps component
	//auto fpsComponent = std::make_shared<dae::FPScomponent>(gameObj3, text);

	////add textcomponent to game object
	//gameObjFPS->AddComponent(text);

	////add fps component to game object
	//gameObjFPS->AddComponent(fpsComponent);
	//text->SetPosition(10, 450, 0);

	//scene.Add(gameObjFPS);

	//auto gameObjectLevel = std::make_shared<dae::GameObject>();
	//////make render component
	//auto Renderlevel = std::make_shared<dae::RenderComponent>(gameObjectLevel);
	//auto transformLevel_1 = std::make_shared<dae::TransformComponent>(gameObjectLevel);
	//gameObjectLevel->AddComponent(transformLevel_1);

	////auto texture = dae::ResourceManager::GetInstance().LoadTexture(LEVELS[0]);
	//Renderlevel->SetTexture(LEVELS[0]);
	//gameObjectLevel->AddComponent(Renderlevel);
	//////ADD LEVEL SKELETON
	//auto levelVertices = std::make_shared<LevelComponent>(gameObjectLevel);
	//levelVertices.get()->Initialize(LEVEL_COLLISIONS[0]);
	//gameObjectLevel->AddComponent(levelVertices);




	////add component to gameobject
	////gameObj->AddComponent(RenderComp);
	////gameObjectLevel->AddComponent(level);




	//scene.Add(gameObjectLevel);

	//auto Level = gameObjectLevel->GetComponent<LevelComponent>();
	//////Addplayer 1
	//PlayerOne(scene, *Level);
	////PlayerTwo(scene,*Level);
	////dae::EnemyType enemy = dae::EnemyType::Red;
	////Enemy(scene, enemy, *Level);

	////enemy = dae::EnemyType::Egg;
	////Enemy(scene, enemy, *Level);
	////////PlayerTwo(scene);


	////BURGER
	//AddBurger(scene, *Level);
}
