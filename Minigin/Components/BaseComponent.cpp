#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

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