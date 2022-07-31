#pragma once
#include "Scenegraph/GameObject.h"
#include "Components/PlayerComponent.h"
#include <iostream>
#include "SceneManager.h"




class Command
{
public:
	Command(std::shared_ptr<dae::GameObject> pGameObject)
		:m_pGameObject(pGameObject)
	{};
	virtual ~Command() = default;
	virtual void Execute() = 0;
	std::weak_ptr<dae::GameObject> m_pGameObject;
};

//jump

class JumpCommand: public Command
{
public:
	JumpCommand(std::shared_ptr<dae::GameObject> pGameobject)
		:Command(pGameobject)
	{};
	void  Execute() override
	{
		//jump call player
		std::cout << "Jumping\n"; 
	};
};

class DeathCommand : public Command
{
public:
	DeathCommand(std::shared_ptr<dae::GameObject> pGameobject)
		:Command(pGameobject)
	{};
	void  Execute() override
	{
		m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->DeathCall();
		
	};
};

class ShootBullet : public Command
{
public:
	ShootBullet(std::shared_ptr<dae::GameObject> pGameobject)
		:Command(pGameobject)
	{};
	void  Execute() override
	{
		m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->GenerateBullet();

	};
};

class AimTankTurret : public Command
{
public:
	AimTankTurret(std::shared_ptr<dae::GameObject> pGameobject,float direction)
		:Command(pGameobject), m_Direction(direction)
	{};
	void  Execute() override
	{
		if (m_Direction > 0)
		{
			m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->RotateTankTurret(1);
		}
		else
		{
			m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->RotateTankTurret(-1);

		}

	};
private:
	float m_Direction;
};



class ScoreCommand : public Command
{
public:
	ScoreCommand(std::shared_ptr<dae::GameObject> pGameobject)
		:Command(pGameobject)
	{};
	void  Execute() override
	{
		m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->ScoreCall();

	};
};


//FireGun 

class FireGunCommand : public Command
{
public:
	FireGunCommand(std::shared_ptr<dae::GameObject> pGameobject)
		:Command(pGameobject)
	{}
	void  Execute() override { std::cout << "Firing Gun\n"; }
};

//Duck

class DuckCommand : public Command
{  
public:
	DuckCommand(std::shared_ptr<dae::GameObject> pGameobject)
		:Command(pGameobject)
	{

	}
	void  Execute() override { std::cout << "Ducking\n"; }
};

//Fart

class FartCommand : public Command
{
public:
	FartCommand(std::shared_ptr<dae::GameObject> pGameobject)
		:Command(pGameobject)
	{

	}
	void  Execute() override { std::cout << "Farting \n"; }
};

//MOVEMENT COMMANDS
class MoveLeftCommand : public Command
{
public:
	MoveLeftCommand(std::shared_ptr<dae::GameObject> pGameobject) :
		Command(pGameobject)
	{

	}

	void Execute() override
	{
		auto playerObj = m_pGameObject.lock()->GetComponent<dae::PlayerComponent>();
		playerObj->MoveLeft();
		dae::PlayerState state = dae::PlayerState::running;
		playerObj->ChangeState(state);
		//m_pGameObject.lock()->GetComponent<dae::PlayerComponent>()->SetVelocity();
		bool checkTexture = playerObj->GetIsFlippedHorizontal();
		if (checkTexture)
		{
			playerObj->FLipTextureHorozontal(false);
		}
	}
};

class MoveRightCommand : public Command
{
public:
	MoveRightCommand(std::shared_ptr<dae::GameObject> pGameobject) :
		Command(pGameobject)
	{
		
	}

	void Execute() override
	{
		auto playerObj = m_pGameObject.lock()->GetComponent<dae::PlayerComponent>();
		dae::PlayerState state = dae::PlayerState::running;
		playerObj->ChangeState(state);
		playerObj->MoveRight();
		bool checkTexture = playerObj->GetIsFlippedHorizontal();
		if (!checkTexture)
		{
			playerObj->FLipTextureHorozontal(true);
		}

	}
};
class MoveUpCommand : public Command
{
public:
	MoveUpCommand(std::shared_ptr<dae::GameObject> pGameobject) :
		Command(pGameobject)
	{

	}

	void Execute() override
	{
		auto playerObj = m_pGameObject.lock()->GetComponent<dae::PlayerComponent>();
		playerObj->MoveUp();
		dae::PlayerState state = dae::PlayerState::climbing;
		playerObj->ChangeState(state);
		bool checkTexture = playerObj->GetIsFlippedVertical();
		if (!checkTexture)
		{
			playerObj->FLipTextureVertical(true);
		}
	}
};

class MoveDownCommand : public Command
{
public:
	MoveDownCommand(std::shared_ptr<dae::GameObject> pGameobject) :
		Command(pGameobject)
	{

	}

	void Execute() override
	{
		auto playerObj =  m_pGameObject.lock()->GetComponent<dae::PlayerComponent>();
		playerObj->MoveDown();
		dae::PlayerState state = dae::PlayerState::climbing;
		playerObj->ChangeState(state);
		bool checkTexture = playerObj->GetIsFlippedVertical();
		if (!checkTexture)
		{
			playerObj->FLipTextureVertical(true);
		}
	}
};

class Button : public Command
{
public:
	Button(std::shared_ptr<dae::GameObject> pGameobject) :
		Command(pGameobject)
	{
		
		
	}

	void Execute() override
	{
		
		dae::SceneManager::GetInstance().setActive("SoloLevel");
		/*auto playerObj = m_pGameObject.lock()->GetComponent<dae::PlayerComponent>();
		playerObj->MoveDown();
		dae::PlayerState state = dae::PlayerState::climbing;
		playerObj->ChangeState(state);*/
	}
};






struct InputAction
{
	InputAction() :
		playerIndex(GamePadIndex::playerOne)
		, command(nullptr),
		keyboardCode(-1) {}

	InputAction(std::unique_ptr<Command> command = nullptr,int keyboardCode =-1, GamePadIndex playerIndex = GamePadIndex::playerOne) :
		command(std::move(command)),
		playerIndex(playerIndex),
		keyboardCode(keyboardCode)

	{}
	std::unique_ptr<Command> command;
	GamePadIndex playerIndex;
	int keyboardCode; //VK 0x07 to 0xFE
};