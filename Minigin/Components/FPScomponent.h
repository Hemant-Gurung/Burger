#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
namespace dae
{
	class FPScomponent :public BaseComponent
	{
	public:
		FPScomponent(std::shared_ptr<GameObject>& ,std::shared_ptr<TextComponent> text);
		~FPScomponent() = default;
		void update(float dt) override;
	
		void Render() const override;
		void SetPosition(float, float, float) override;
		void SetText();
		void SetCurrentTime(uint64_t);
		void SetPreviousTime(uint64_t);
	private:
		std::shared_ptr<TextComponent> m_TextComponent;
		//std::shared_ptr<Transform> m_Transform;
		
		std::string m_fpsString;
		float m_fpsTimer;
		float m_FPScount;
		int m_FPS;
		float m_SecondsPerBound{};
		uint64_t m_CurrentTime;
		uint64_t m_PreviousTime;
		float m_ElapsedTime;
	};

}