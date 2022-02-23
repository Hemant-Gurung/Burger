#pragma once

#include "BaseComponent.h"

namespace dae
{
	class Texture2D;
	class TextureComponent :public BaseComponent
	{
	public:
		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y,float z);
	private:
		Transform m_Transform;
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_Texture{};
	};
}