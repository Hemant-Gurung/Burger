#include "MiniginPCH.h"
#include "ButtonComponent.h"
#include "TextComponent.h"
ButtonComponent::ButtonComponent(std::shared_ptr<dae::GameObject>& pGameObj,std::string text, std::shared_ptr<dae::Font> font, SDL_Color color, std::function<void()> function)
	:BaseComponent(pGameObj),
     m_Color(color)
{
	m_Button = std::make_shared<dae::TextComponent>(pGameObj,text, font, color);
	m_Function = function;
}


void ButtonComponent::update(float dt)
{
	m_Button->update(dt);
}

void ButtonComponent::Render() const
{
	m_Button->Render();
}

void ButtonComponent::SetPosition(float x, float y, float z)
{
	m_Button->SetPosition(x, y, z);
}

void ButtonComponent::FixedUpdate(){}

//void ButtonComponent::Render()
//{
//	m_Button->Render();
//}

//void ButtonComponent::set(float x, float y, float z)
//{
//	m_Button->SetPosition(x, y, z);
//}

void ButtonComponent::HooverOver(const SDL_Color& /*color*/) const
{
	//m_Button->ChangeColor(color);
}

void ButtonComponent::ResetColor() const
{
	//m_Button->ChangeColor(m_Color);
}
