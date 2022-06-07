#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public BaseComponent
	{
	public:
		void update(float dt) override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y,float z) override;
		
		explicit TextComponent(std::shared_ptr<GameObject>& pGameObj, const std::string& text, const std::shared_ptr<Font>& font, SDL_Color color);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		//Transform m_Transform;
		Vector2f m_position;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
		SDL_Color m_Color;
	};
}




























