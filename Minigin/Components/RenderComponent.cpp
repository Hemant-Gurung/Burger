#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "Graphics/Renderer.h"
#include "RenderComponent.h"

#include "Scenegraph/GameObject.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::RenderComponent::RenderComponent(std::shared_ptr<GameObject>& pGameObj)
	:BaseComponent(pGameObj)
{
	
}

void dae::RenderComponent::Render() const
{
	Renderer::GetInstance().RenderTexture(*m_Texture, m_position.x, m_position.y);

}

//void dae::RenderComponent::Render() const
//{
//	//const auto& pos = m_Transform.GetPosition();
//	//Renderer::GetInstance().RenderTexture(*m_Texture, m_position.x, m_position.y);
//	RenderTexture();
//}

void dae::RenderComponent::RenderLine(const float posx1, const float posy1, const float posx2, const float posy2)
{
	Renderer::GetInstance().RenderLine(posx1, posy1, posx2, posy2);
}

void dae::RenderComponent::RenderTexture(Rectf dstRect, const Rectf& srcrect,bool horizontal)
{

	//auto texture = std::make_shared<Texture2D>(m_Texture->scale(40, 40));
	Renderer::GetInstance().RenderTexture(*m_Texture, dstRect.left, dstRect.bottom, dstRect.width, dstRect.height, srcrect,horizontal);

}

void dae::RenderComponent::RenderTexture() const
{
	Renderer::GetInstance().RenderTexture(*m_Texture, m_position.x, m_position.y);
}

dae::Texture2D dae::RenderComponent::FlipTexture(const float x, const float y, const float width, const float height, const Rectf& srcrect ,bool horizontal)
{
	auto texture =  Renderer::GetInstance().FlipTexture(*m_Texture, x, y, width, height, srcrect, horizontal);
	return static_cast<Texture2D>(texture);
}

void dae::RenderComponent::RenderBox(const Rectf& box, int width, int height) const
{
	Renderer::GetInstance().RenderBox(box, width,height);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetPosition(float x, float y,float)
{
	//m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(x, y,0);
	m_pGameObject.lock()->GetComponent<TransformComponent>()->SetPosition(x,y,0);
	m_position = m_pGameObject.lock()->GetComponent<TransformComponent>()->GetPosition();

	//std::cout <<a.x<<a.y<<a.z;
	//m_Transform.SetPosition(x, y, 0.0f);
}