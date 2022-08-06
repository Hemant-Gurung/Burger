#include "MiniginPCH.h"
#include "ButtonManager.h"
#include "Debug.h"


ButtonManager::ButtonManager()
{
}

void ButtonManager::AddButton(std::shared_ptr<GameObject>& button)
{
	if (!button->HasComponent<ButtonComponent>())
		Debug::LogError("ButtonManager::AddButton : 'button' has no ButtonComponent");
	m_Buttons.push_back(button);
}

void ButtonManager::SetStartButtonByIdx(unsigned idx)
{
	if (!m_Buttons[idx])
		return;
	const auto component = m_Buttons[idx]->GetComponent<ButtonComponent>();
	component->HooverOver(SDL_Color{ 255,255,255,255 });
	m_CurrentIdx = idx;
}

void ButtonManager::ButtonDown()
{
	auto component = m_Buttons[m_CurrentIdx]->GetComponent<ButtonComponent>();
	component->ResetColor();
	++m_CurrentIdx;
	if (m_CurrentIdx >= int(m_Buttons.size()))
		m_CurrentIdx = 0;
	component = m_Buttons[m_CurrentIdx]->GetComponent<ButtonComponent>();
	component->HooverOver(SDL_Color{ 255,255,255,255 });
}

std::shared_ptr<ButtonComponent> ButtonManager::GetActiveButtonComp()
{
	return m_Buttons[m_CurrentIdx]->GetComponent<ButtonComponent>();
}

void ButtonManager::ButtonUp()
{
	auto component = m_Buttons[m_CurrentIdx]->GetComponent<ButtonComponent>();
	component->ResetColor();
	--m_CurrentIdx;
	if (m_CurrentIdx < 0)
		m_CurrentIdx = int(m_Buttons.size()) - 1;
	component = m_Buttons[m_CurrentIdx]->GetComponent<ButtonComponent>();
	component->HooverOver(SDL_Color{ 255,255,255,255 });
}

