// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "MiniginPCH.h"

#include "Minigin.h"
#include "SoloLevel.h"
#include "BurgerGame.h"
#include "StartScreen.h"
#include "SecondLevel.h"
#include "ThirdLevel.h"
#include "ScoreScene.h"
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
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<SecondLevel>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<ThirdLevel>());
	//dae::SceneManager::GetInstance().AddGameScene(std::make_shared<ScoreScene>());

	dae::SceneManager::GetInstance().setActive("startScreen");
	//dae::SceneManager::GetInstance().setActive("SecondLevel");

}