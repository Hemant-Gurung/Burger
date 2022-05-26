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
		std::swap(m_PreviousState, m_CurrentState);
		ZeroMemory(m_CurrentState, sizeof(XINPUT_STATE) * XUSER_MAX_COUNT);
	
		DWORD dwResult;
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			//get the state
		   dwResult = XInputGetState(i, &m_CurrentState[i]);

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
		
		//check the change in button state using xor
		auto buttonchanges = m_CurrentState->Gamepad.wButtons ^ m_PreviousState->Gamepad.wButtons;

		//check if button is pressed this frame using binary and
		buttonPressedThisFrame = buttonchanges & m_CurrentState->Gamepad.wButtons;

		//std::cout << m_CurrentState->Gamepad.wButtons << std::endl;
		//check if button is released this frame using binary and
		buttonReleasedThisFrame = buttonchanges & (~m_CurrentState->Gamepad.wButtons);

	}

	void HandleInput()
	{
		// get the instanc eof the input
		auto& pInstance = Input::GetInstance();
		std::unique_ptr<Input> pinstance = std::make_unique<Input>();

		// use for loop to go through commands 
		for (auto it = pInstance.m_ConsoleCommands.begin(); it != pInstance.m_ConsoleCommands.end(); ++it)
		{
			if(it->first.first == XBOX360Controller::ButtonState::down)
			{
				if (IsDownThisFrame(static_cast<unsigned int>(it->first.second)))
				{
					//execute the command
					it->second->Execute();

				}
			}
			else if (it->first.first == XBOX360Controller::ButtonState::held)
				// check if a button is down this frame
			{
				if (IsPressed(static_cast<unsigned int>(it->first.second)))
				{
					//execute the command
					it->second->Execute();

				}
			}
		}

	}

	bool IsDownThisFrame(unsigned int button) const { return buttonPressedThisFrame & button; }

	bool IsUpThisFrame(unsigned int button) const { return buttonReleasedThisFrame & button; }

	bool IsPressed(unsigned int button) const {

		//if(m_CurrentState!=nullptr) 
		return m_CurrentState->Gamepad.wButtons & button;

		//return false;
	}

};

//map events
void Input::MapEvent(ControllerKey key, std::unique_ptr<Command> command)
{
	m_ConsoleCommands.insert({ key,std::move(command) });

}

InputManager::InputManager()
{
	
	pImpl = new InputManagetImpl();

}

bool InputManager::ProcessInput()
{
	
	return pImpl->ProcessInput();
	
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

bool InputManager::IsDown(unsigned int button) const { return pImpl->IsDownThisFrame(static_cast<unsigned int>(button)); }

bool InputManager::IsUp(unsigned int button) const { return pImpl->IsUpThisFrame(static_cast<unsigned int>(button)); }

bool InputManager::IsPressed(unsigned int button) const { return pImpl->IsPressed(static_cast<unsigned int>(button)); }

void InputManager::HandleInput()
{
	
	pImpl->HandleInput();

}



