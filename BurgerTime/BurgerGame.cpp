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
#include "StartScreen.h"
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
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<StartScreen>());
	dae::SceneManager::GetInstance().setActive("startScreen");
}