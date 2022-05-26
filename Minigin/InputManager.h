#pragma once
#define WIN32_LEAN_AND_MEAN
#include  <iostream>
#include <map>
#include <Windows.h>
#include <Xinput.h>
#include "Singleton.h"
#include "Command.h"
#include <vector>
#include "Singleton.h"
#include <map>

#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"XInput9_1_0.lib")

namespace dae
{
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
			ButtonStart = XINPUT_GAMEPAD_START,
			ButtonBack = XINPUT_GAMEPAD_BACK,
			StickLeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
			StickRightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
			ButtonLeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
			ButtonRightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER

		};

		enum class ButtonState
		{
			up,
			down,
			held
		};
	};
	class InputManager final :public Singleton<InputManager>
	{
		class InputManagetImpl;

		//std::unique_ptr<InputManagetImpl> pImpl;
		InputManagetImpl* pImpl;

	public:
		InputManager();
		~InputManager();

		
		void Update();
		bool ProcessInput();
		void HandleInput();

		bool IsDown(unsigned int button) const;
		bool IsUp(unsigned int button) const;
		bool IsPressed(unsigned int button) const;

	private:




	};

	class Input final : public Singleton<Input>
	{
	public:
		using ControllerKey = std::pair<XBOX360Controller::ButtonState, XBOX360Controller::ControllerButton>;
		using ControllerCommandMap = std::map<ControllerKey, std::unique_ptr<Command>>;
		ControllerCommandMap m_ConsoleCommands{};
		void MapEvent(ControllerKey key, std::unique_ptr<Command> command);
		std::vector<std::unique_ptr<XBOX360Controller>> m_Controllers{};
	private:



	};

}