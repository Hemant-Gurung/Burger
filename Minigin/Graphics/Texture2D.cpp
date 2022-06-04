#include "MiniginPCH.h"
#include "Texture2D.h"
#include "Renderer.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* dae::Texture2D::scale(float /*width*/, float /*height*/)
{
	SDL_Rect sourceRect;
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = 16;
	sourceRect.h = 16;

	SDL_Rect destRect;
	destRect.x = 0;
	destRect.y = 0;
	destRect.w = 40;
	destRect.h = 40;

	SDL_RenderCopy(Renderer::GetInstance().GetSDLRenderer(), m_Texture, &sourceRect, &destRect);

	return m_Texture;
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
