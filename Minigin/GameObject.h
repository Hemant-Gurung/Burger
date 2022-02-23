#pragma once
#include "Transform.h"
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject final 
	{
	public:
		void Update(float) ;
		void Render() const;

		GameObject() = default;

		//adding virtual adds more memory so if not required do not write
		 ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void AddComponent(const std::shared_ptr<BaseComponent>&);
		template<typename T> T* GetComponent() const; 
		template<typename T> void RemoveComponent(T*);

		void SetParent(GameObject* parent);
		GameObject* GetParent() const;

	private:
		//Transform m_Transform;
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::vector<std::shared_ptr<BaseComponent>> m_sComponents{};

	};
}
