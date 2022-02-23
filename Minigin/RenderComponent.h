#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;
	class RenderComponent :public BaseComponent
	{
	public:
		void Render() const override;
		virtual void SetTexture(const std::string& filename) override;
		virtual void SetPosition(float x, float y,float z) override;
	private:
		Transform m_Transform;
		std::shared_ptr<Texture2D> m_Texture{};
	};
}