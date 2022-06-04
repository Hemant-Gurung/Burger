#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "GameObject.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(std::shared_ptr<GameObject>& pGameObj, const std::string& text, const std::shared_ptr<Font>& font,SDL_Color color = { 255,255,255 })
	:BaseComponent(pGameObj), m_NeedsUpdate(true), m_Text(text), m_Font(font), m_TextTexture(nullptr),m_Color(color)
{ }

void dae::TextComponent::update(float)
{
	//m_pGameObject->GetComponent<Transform>();

	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		//const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, m_position.x, m_position.y);
		
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetPosition(const float x, const float y,const float)
{
	m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(x, y, 0);
	m_position = m_pGameObject.lock()->GetComponent<TransformComponent>()->GetPosition();

	//m_Transform.SetPosition(x, y, 0.0f);
}


