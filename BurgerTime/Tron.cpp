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
#include "SServiceLocator.h"
#include "VersusLevel.h"
#include "CoopLevel.h"
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
	SServiceLocator::Init();
	SoundManager::GetInstance().LoadSoundStream("GameSong", "GameMusic.ogg");
	SoundManager::GetInstance().LoadSoundEffect("Loss", "Lose-Life.ogg");
	SoundManager::GetInstance().LoadSoundEffect("Victory", "Victory.ogg");
	SoundManager::GetInstance().LoadSoundEffect("Shoot", "Shoot.ogg");
	SoundManager::GetInstance().LoadSoundEffect("Bullet", "BulletShot.ogg");
	SoundManager::GetInstance().LoadSoundEffect("End", "Level-End.ogg");






	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<SoloLevel>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<StartScreen>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<SecondLevel>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<ThirdLevel>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<VersusLevel>());
	dae::SceneManager::GetInstance().AddGameScene(std::make_shared<CoopLevel>());



	//dae::SceneManager::GetInstance().setActive("CoopLevel");
	dae::SceneManager::GetInstance().setActive("StartScreen");



















	//dae::SceneManager::GetInstance().setActive("SecondLevel");

}