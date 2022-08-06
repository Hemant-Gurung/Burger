#pragma once
#include "GameObject.h"
#include "ButtonComponent.h"

class ButtonManager final
{
public:
	ButtonManager();
	~ButtonManager() = default;

	void AddButton(std::shared_ptr<dae::GameObject>& button);
	void SetStartButtonByIdx(unsigned idx);
	std::shared_ptr<ButtonComponent> GetActiveButtonComp();

	void ButtonDown();
	void ButtonUp();

private:
	std::vector<std::shared_ptr<dae::GameObject>> m_Buttons;
	bool m_CanSwitchButton = false;
	bool m_CanPressButton = false;
	int m_CurrentIdx = 0;
};

