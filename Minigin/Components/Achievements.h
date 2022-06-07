#pragma once
#include <glm/fwd.hpp>
#include "BaseComponent.h"
#include "Scenegraph/GameObject.h"
#include "Base/Observer.h"
#include <steam_api.h>

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }


struct Achievement_t
{
	int m_eAchievementID;
	const char* m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

// Defining our achievements
enum EAchievements
{
	ACH_WIN_ONE_GAME = 0,
	ACH_WIN_100_GAMES = 1,
	ACH_TRAVEL_FAR_ACCUM = 2,
	ACH_TRAVEL_FAR_SINGLE = 3,
};

// Global access to Achievements object
//Achievements* g_SteamAchievements = NULL;

// Achievement array which will hold data about the achievements and their state
//Achievement_t g_Achievements[] =
//{
//	_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
//	_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
//	_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
//	_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
//};

class Achievements : public dae::BaseComponent,public dae::Observer
{

private:
	int64 m_iAppID; // Our current AppID
	Achievement_t* m_pAchievements; // Achievements data
	int m_iNumAchievements; // The number of Achievements
	bool m_bInitialized; // Have we called Request stats and received the callback?

public:
	Achievements(Achievement_t* Achievements, int NumAchievements, std::shared_ptr<dae::GameObject> pGameObject);
	~Achievements();

	bool RequestStats();
	bool SetAchievement(const char* ID);


	STEAM_CALLBACK(Achievements, OnUserStatsReceived, UserStatsReceived_t,
		m_CallbackUserStatsReceived);
	STEAM_CALLBACK(Achievements, OnUserStatsStored, UserStatsStored_t,
		m_CallbackUserStatsStored);
	STEAM_CALLBACK(Achievements, OnAchievementStored,
		UserAchievementStored_t, m_CallbackAchievementStored);

	void OnNotify(const dae::BaseComponent&, dae::EVENT&) override;
};
