#include "MiniginPCH.h"
#include "GameObject.h"
#include "Components/BaseComponent.h"

using namespace std;
dae::GameObject::~GameObject() = default;

void dae::GameObject::AddComponent(const std::shared_ptr<BaseComponent>& component)
{
	//add components
	m_sComponents.push_back(component);
}

void dae::GameObject::RemoveComponent(const std::shared_ptr<BaseComponent>& pComponent)
{
	auto it = std::find(m_sComponents.begin(), m_sComponents.end(), pComponent);


	if(it!=m_sComponents.end())
	{
		m_sComponents.erase(it);
	}
	else
	{
		std::cout << "ERROR:: component doesnot exist in the list\n";
	}
}


void dae::GameObject::AddChild(GameObject* go)
{
	// make iterator for childrens
	auto it = std::find(m_pChildrens.begin(), m_pChildrens.end(), go);

	//check if the child is already there
	if(it == m_pChildrens.end())
	{
		// set the parent of the gameobject as this gameobject
		go->m_pParent = this;

		//add it to the list of childrens
		m_pChildrens.push_back(go);
	}
	else
	{
		std::cout << "ERROR:: child is already attached to the gameobject\n";
	}
}

dae::GameObject* dae::GameObject::GetChildAt(int /*index*/)
{
	//return m_pChildrens.at(index);
	return nullptr;
}

void dae::GameObject::RemoveChild(GameObject* go)
{
	//m_pChildrens.erase(std::remove(m_pChildrens.begin(), m_pChildrens.end(), m_pChildrens.at(index)));
	auto it = std::find(m_pChildrens.begin(), m_pChildrens.end(), go);

	//chekck if the list contains the game objcet
	if(it!=m_pChildrens.end())
	{
		go->m_pParent = nullptr;
		m_pChildrens.erase(it);
	}
	else
	{
		std::cout << "\n";
	}
}


void dae::GameObject::Update(float dt)
{
	for (const auto& object : m_sComponents)
	{
		//update 
		object->update(dt);
	}
}

void dae::GameObject::FixedUpdate()
{

}

void dae::GameObject::Render() const
{
	for (const auto& object : m_sComponents)
	{
		//render
		object->Render();
	}
}
