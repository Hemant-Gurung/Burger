#pragma once
#include "BaseComponent.h"

namespace dae
{
	enum class EVENT
	{
		PLAYER_SCOREADD,
		PLAYER_DEAD,
		PLAYER_SCORE_ACHIEVEMENT_1,
		IDLE
	};

	class Observer
	{
	public:
		virtual ~Observer(){};
		virtual void OnNotify(const dae::BaseComponent&, dae::EVENT&)=0;
	};
}

