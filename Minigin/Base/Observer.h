#pragma once
#include "Components/BaseComponent.h"

namespace dae
{
	enum class EVENT
	{
		PLAYER_SCOREADD,
		PLAYER_DEAD,
		PLAYER_PEPPERTHROW,
		PLAYER_SCORE_ACHIEVEMENT_1,
		ENEMY_DEAD,
		IDLE
	};

	class Observer
	{
	public:
		virtual ~Observer(){};
		virtual void OnNotify(const dae::BaseComponent&, dae::EVENT&)=0;
	};
}

