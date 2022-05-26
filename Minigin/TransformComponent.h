#pragma once
#include  "BaseComponent.h"

namespace dae
{
	class TransformComponent : public BaseComponent
	{
	public:
		TransformComponent(std::shared_ptr<GameObject>&);

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		const Vector2f& GetPosition() const { return m_Position; }
		void SetPosition(float, float, float) override;
	private:
		Vector2f m_Position;
	};

	
}
