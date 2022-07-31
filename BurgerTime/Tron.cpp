// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "MiniginPCH.h"

#include "Minigin.h"
#include "SoloLevel.h"
#include "Tron.h"
#include "StartScreen.h"
#include "SecondLevel.h"
#include "ThirdLevel.h"
#include "ScoreScene.h"
#include "SoundManager.h"
//#include "vld.h"


 //namespace dae
//{


Tron::Tron()
{

}

Tron::~Tron()
{
}

void Tron::LoadGame() const
{
	Minigin::LoadGame();

	SoundManager::GetInstance().LoadSoundStream("GameSong", "GameMusic.ogg");
	SoundManager::GetInstance().LoadSoundStream("Start", "Start.ogg");
	SoundManager::GetInstance().LoadSoundEffect("Loss", "Lose-Life.ogg");

	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<SoloLevel>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<StartScreen>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<SecondLevel>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<ThirdLevel>());
	//dae::SceneManager::GetInstance().AddGameScene(std::make_shared<ScoreScene>());

	dae::SceneManager::GetInstance().setActive("SoloLevel");
	//dae::SceneManager::GetInstance().setActive("SecondLevel");

}