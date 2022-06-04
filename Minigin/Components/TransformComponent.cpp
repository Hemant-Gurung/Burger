#include "MiniginPCH.h"
#include "TransformComponent.h"

dae::TransformComponent::TransformComponent(std::shared_ptr<GameObject>& pGameObj)
	:BaseComponent(pGameObj)
{
}

void dae::TransformComponent::SetPosition(float x, float y, float /*z*/)
{
	m_Position.x = x;
	m_Position.y = y;
	//m_Position.z = z;
}
