#include "MiniginPCH.h"
#include "BaseComponent.h"

#include "SoundManager.h"
#include "Graphics/Renderer.h"
#include "Scenegraph/GameObject.h"

dae::BaseComponent::BaseComponent(std::shared_ptr<GameObject>& sGameObj)
	:m_pGameObject{sGameObj}
{
}
void dae::BaseComponent::update(float) {}

void dae::BaseComponent::FixedUpdate() {};

void dae::BaseComponent::Render() const
{
	
}

void dae::BaseComponent::SetTexture(const std::string&)
{
	
}

void dae::BaseComponent::SetPosition(float, float,float)
{

}

Rectf dae::BaseComponent::GetPosition()
{
	return Rectf{ 0,0,0,0 };
}


