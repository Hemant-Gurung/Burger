#pragma once
#include "Singleton.h"
#include "utils.h"

namespace dae
{

	
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_clearColor{};

		bool m_showTrash{};
		bool m_showGame3D{};
		
	public:
		void Init(SDL_Window* window);
		void Render() ;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		
		void RenderTexture(Texture2D& texture, float x, float y, float width, float height,const Rectf& srcRect, bool horizontal) const;
		void RenderLine(const float posx1, const float posy1, const float posx2, const float posy2)const;
		void RenderBox(const Rectf&,int width,int height) const;
		SDL_Texture* FlipTexture(Texture2D& texture,const float x, const float y, const float width, const float height, const Rectf srcRect ,bool horizontal) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		bool m_CheckBox{};
	private:
		void Plot();
		void TrashTheCache();
		void TrashTheCacheWithGameObject3D();
	};


}

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

class GameObject3D
{
public:
	Transform transform;
	int ID;
};
