#include "MiniginPCH.h"
#include "GameObject.h"


using namespace std;
dae::GameObject::~GameObject() = default;

void dae::GameObject::AddComponent(const std::shared_ptr<BaseComponent>& component)
{
	//add components
	m_sComponents.push_back(component);
}


void dae::GameObject::Update(float dt)
{
	for (const auto& object : m_sComponents)
	{
		//update 
		object->update(dt);
	}
}

void dae::GameObject::Render() const
{
	for (const auto& object : m_sComponents)
	{
		//render
		object->Render();
	}
}

template <typename T>
T* dae::GameObject::GetComponent() const
{
	//template function get component
	T* component = { nullptr };
	for(int i=0;i<m_sComponents.size() && !component;++i)
	{
		component = dynamic_cast<T*>(m_sComponents[i]);
	}
	return component;
}
