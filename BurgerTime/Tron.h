#pragma once
#include "pch.h"
#include "Minigin.h"



class Tron: public dae::Minigin
{
public:
	Tron();
	~Tron();

	void LoadGame() const override;

};