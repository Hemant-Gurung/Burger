#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;
	class RenderComponent :public BaseComponent
	{
	public:
		RenderComponent(std::shared_ptr<GameObject>&);
		
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		
		void RenderLine(const float posx1, const float posy1, const float posx2, const float posy2);
		void RenderTexture(Rectf dstRect,const Rectf& srcrect ,bool horizontal = false, bool vertical = false,float angle =0);
		void RenderTexture() const; 
		dae::Texture2D FlipTexture(const float x, const float y, const float width, const float height, const Rectf& ,bool horizontal = true,bool vertical = false);
		void RenderBox(const Rectf& box, int width,int height) const ;

		virtual void Render() const override;
		virtual void SetTexture(const std::string& filename) override;
		virtual void SetPosition(float x, float y,float z) override;

		
	private:
		//Transform m_Transform{};
		std::shared_ptr<Texture2D> m_Texture{};
	};
}