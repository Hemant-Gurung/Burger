#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
void dae::RenderComponent::Render() const
{
	const auto& pos = m_Transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetPosition(float x, float y,float)
{
	m_Transform.SetPosition(x, y, 0.0f);
}