#pragma once
#define WIN32_LEAN_AND_MEAN
#include  <iostream>
#include <map>
#include <Windows.h>
#include <Xinput.h>
#include "Utils/Singleton.h"
#include "Command.h"
#include "structs.h"
#include <map>

#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"XInput9_1_0.lib")

namespace dae
{
	
	class InputManager :public Singleton<InputManager>
	{
		class InputManagetImpl;

		//std::unique_ptr<InputManagetImpl> pImpl;
		InputManagetImpl* pImpl;

	public:
		InputManager();
		~InputManager();

		void Initialize();
		void Update();
		bool ProcessInput();
		void HandleInput();

		void Quit();
		bool IsDown(unsigned int button,GamePadIndex&) const;
		bool IsUp(unsigned int button, GamePadIndex&) const;
		bool IsPressed(unsigned int button, GamePadIndex&) const;
		void ResetInput();
	private:
		bool m_quit;

	};

	

	class XBOX360Controller
	{

	public:
		enum class ControllerButton
		{
			ButtonA = XINPUT_GAMEPAD_A,
			ButtonB = XINPUT_GAMEPAD_B,
			ButtonX = XINPUT_GAMEPAD_X,
			ButtonY = XINPUT_GAMEPAD_Y,
			DpadUp = XINPUT_GAMEPAD_DPAD_UP,
			DpadDown = XINPUT_GAMEPAD_DPAD_DOWN,
			DpadLeft = XINPUT_GAMEPAD_DPAD_LEFT,
			DpadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
			ShoulderButtonRight = XINPUT_GAMEPAD_RIGHT_SHOULDER,
			ShoulderButtonLeft = XINPUT_GAMEPAD_LEFT_SHOULDER,


			ButtonA1 = XINPUT_GAMEPAD_A,
			ButtonB1 = XINPUT_GAMEPAD_B,
			ButtonX1 = XINPUT_GAMEPAD_X,
			ButtonY1 = XINPUT_GAMEPAD_Y,
			DpadUp1 = XINPUT_GAMEPAD_DPAD_UP,
			DpadDown1 = XINPUT_GAMEPAD_DPAD_DOWN,
			DpadLeft1 = XINPUT_GAMEPAD_DPAD_LEFT,
			DpadRight1 = XINPUT_GAMEPAD_DPAD_RIGHT,
			ShoulderButtonRight1 = XINPUT_GAMEPAD_RIGHT_SHOULDER,
			ShoulderButtonLeft1 = XINPUT_GAMEPAD_LEFT_SHOULDER,





			//ButtonStart = XINPUT_GAMEPAD_START,
			//ButtonBack = XINPUT_GAMEPAD_BACK,
			//StickLeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
			//StickRightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
			//ButtonLeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
			//ButtonRightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER

		};

		enum class ButtonState
		{
			up,
			up1,
			down,
			down1,
			held,
			held1
		};
	};


	class Input final : public Singleton<Input>
	{
	public:
		using ControllerKey = std::pair< XBOX360Controller::ButtonState, XBOX360Controller::ControllerButton>;
		using ControllerCommandMap = std::map<ControllerKey, InputAction>;
		ControllerCommandMap m_ConsoleCommands{};
		void MapEvent(ControllerKey key, InputAction);
		//std::vector<std::unique_ptr<XBOX360Controller>> m_Controllers{};

	private:



	};

}

