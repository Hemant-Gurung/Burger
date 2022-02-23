#include "MiniginPCH.h"
#include "TextureComponen.h"
#include  "Texture2D.h"
#include "ResourceManager.h"
void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::SetPosition(float x, float y,float)
{
	m_Transform.SetPosition(x, y, 0.0f);
}