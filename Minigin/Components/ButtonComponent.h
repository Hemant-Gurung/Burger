#pragma once
#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "Font.h"
#include "BaseComponent.h"
#include "TextComponent.h"
#include <functional>

	class ButtonComponent final : public dae::BaseComponent
	{
	public:
		ButtonComponent(std::shared_ptr<dae::GameObject>&, std::string text, std::shared_ptr<dae::Font> font, SDL_Color color, std::function<void()> function);
		virtual ~ButtonComponent() = default;

		
		void update(float) override;
		void FixedUpdate() override;
		void Render() const override;
		//void SetTransform(float x, float y, float z) override;
		void SetPosition(float, float, float) override;
		void HooverOver(const SDL_Color& color) const;
		void ExecuteButton() const { m_Function(); }
		void ResetColor() const;

	private:
		std::shared_ptr<dae::TextComponent> m_Button;
		std::function<void()> m_Function;
		SDL_Color m_Color;
	};
