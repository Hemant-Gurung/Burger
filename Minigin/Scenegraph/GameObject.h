#pragma once
#include "TransformComponent.h"
#include "SceneObject.h"

namespace dae
{
	class BaseComponent;
	class Texture2D;

	// todo: this should become final.
	class GameObject final :public SceneObject
	{
	public:

		
		void Update(float) override;
		void FixedUpdate() override;
		void Render() const override;

		GameObject() = default;

		//adding virtual adds more memory so if not required do not write
		 ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void AddComponent(const std::shared_ptr<BaseComponent>&);
		 void RemoveComponent(const std::shared_ptr<BaseComponent>& pComponent);

		void AddChild(GameObject* go);
		GameObject* GetChildAt(int index);
		void RemoveChild(GameObject* go);

		template<typename T>
		T* GetComponent()
		{
			//template function get component
			for (auto& tempcom : m_sComponents)
			{

				//dynamic cast the result 
				T* returntyp = dynamic_cast<T*>(tempcom.get());

				//check for nullptr
				if (returntyp != nullptr)
				{
					return returntyp;
				}

			}
			return nullptr;
		}


		

	private:
		//Transform m_Transform;
		
		std::vector<std::shared_ptr<BaseComponent>> m_sComponents{};
		std::vector<GameObject*> m_pChildrens;
		GameObject* m_pParent;
	};

	
}
