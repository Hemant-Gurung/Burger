#pragma once
#include "pch.h"
#include "Minigin.h"



class BurgerGame: public dae::Minigin
{
public:
	BurgerGame();
	~BurgerGame();

	void LoadGame() const override;

};