#include "MiniginPCH.h"
#include "InputManager.h"

using namespace dae;
class InputManager::InputManagetImpl
{

	XINPUT_STATE* m_CurrentState{};
	XINPUT_STATE* m_PreviousState{};

	XINPUT_KEYSTROKE m_CurrentKeyStroke{};
	WORD buttonPressedThisFrame;
	WORD buttonReleasedThisFrame;

	int _controllerIndex;
public:
	InputManagetImpl()
		:m_CurrentState(new XINPUT_STATE[XUSER_MAX_COUNT]),
		m_PreviousState(new XINPUT_STATE[XUSER_MAX_COUNT])
	{

		
		ZeroMemory(m_PreviousState, sizeof(XINPUT_STATE) * XUSER_MAX_COUNT);
		ZeroMemory(m_CurrentState, sizeof(XINPUT_STATE) * XUSER_MAX_COUNT);
		//get the state
	
	}

	~InputManagetImpl()
	{

		delete m_CurrentState;
		m_CurrentState = nullptr;
		delete m_PreviousState;
		m_PreviousState = nullptr;



	}

	bool ProcessInput()
	{
		// swap the states
		
	
		DWORD dwResult;
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			std::swap(m_PreviousState[i], m_CurrentState[i]);

			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));
			//get the state
		    dwResult = XInputGetState(i, &state);
			m_CurrentState[i] = state;
			if(dwResult == ERROR_SUCCESS)
			{
				//std::cout << "controller " << i << " connected\n";
				
			}
			else
			{
				
				//std::cout << "controller " << i << "not connected\n";
			}
		}
		
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					return false;
				}
				if (e.type == SDL_KEYDOWN) {
					
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					
				}
		
		}
		
		return true;
		
		
	}

	void Update()
	{
		
		////check the change in button state using xor
		//auto buttonchanges = m_CurrentState[int()]->Gamepad.wButtons ^ m_PreviousState->Gamepad.wButtons;

		////check if button is pressed this frame using binary and
		//buttonPressedThisFrame = buttonchanges & m_CurrentState->Gamepad.wButtons;

		////std::cout << m_CurrentState->Gamepad.wButtons << std::endl;
		////check if button is released this frame using binary and
		//buttonReleasedThisFrame = buttonchanges & (~m_CurrentState->Gamepad.wButtons);

	}

	void HandleInput()
	{
		// get the instanc eof the input
		auto& pInstance = Input::GetInstance();
		std::unique_ptr<Input> pinstance = std::make_unique<Input>();

		// use for loop to go through commands 
		for (auto it = pInstance.m_ConsoleCommands.begin(); it != pInstance.m_ConsoleCommands.end(); ++it)
		{
			if(it->first.first == XBOX360Controller::ButtonState::down || it->first.first == XBOX360Controller::ButtonState::down1)
			{
				if (IsDownThisFrame(static_cast<unsigned int>(it->first.second),it->second.playerIndex))
				{
					//execute the command
					//it->second->Execute();
					it->second.command->Execute();
				}
			}
			else if (it->first.first == XBOX360Controller::ButtonState::held || it->first.first == XBOX360Controller::ButtonState::held1)
				// check if a button is down this frame
			{
				if (IsPressed(static_cast<unsigned int>(it->first.second), it->second.playerIndex))
				{
					//execute the command
					//it->second->Execute();
					it->second.command->Execute();

				}
			}
		}

	}

	bool IsDownThisFrame(unsigned int button,GamePadIndex& playerIndex) const { return m_CurrentState[(int)playerIndex].Gamepad.wButtons & button; }

	bool IsUpThisFrame(unsigned int button, GamePadIndex& playerIndex) const { return m_CurrentState[(int)playerIndex].Gamepad.wButtons & button; }

	bool IsPressed(unsigned int button, GamePadIndex& playerIndex) const {

		//if(m_CurrentState!=nullptr) 
		return m_CurrentState[(int)playerIndex].Gamepad.wButtons & button;

		//return false;
	}

};

//map events
void Input::MapEvent(ControllerKey key, InputAction action)
{
	
	m_ConsoleCommands.insert({ key,std::move(action)});
	
}

InputManager::InputManager()
{
	
	pImpl = new InputManagetImpl();

}

bool InputManager::ProcessInput()
{
	
	auto input =  pImpl->ProcessInput();
	return input;
}

InputManager::~InputManager()
{

	pImpl->~InputManagetImpl();
	delete pImpl;
}

void InputManager::Update() 
{
	pImpl->Update();

}

bool InputManager::IsDown(unsigned int button, GamePadIndex& pIndx) const { return pImpl->IsDownThisFrame(static_cast<unsigned int>(button), pIndx); }

bool InputManager::IsUp(unsigned int button, GamePadIndex& pIndx) const { return pImpl->IsUpThisFrame(static_cast<unsigned int>(button), pIndx); }

bool InputManager::IsPressed(unsigned int button, GamePadIndex& pIndx) const { return pImpl->IsPressed(static_cast<unsigned int>(button), pIndx); }

void InputManager::HandleInput()
{
	
	pImpl->HandleInput();

}



