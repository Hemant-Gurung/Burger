#pragma once
#include "Observer.h"
#include "BaseComponent.h"
//#include "box2d.h"
#include <thread>
#include "Sound_System.h"
#include  "LevelComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
namespace dae
{

	enum class PlayerState
	{
		standing,
		running,
		climbing,
		dead
	};

	class PlayerComponent : public BaseComponent
	{
	public:
		PlayerComponent(std::shared_ptr<GameObject>&, LevelComponent&);
		~PlayerComponent();

		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;

		
		void Initialize();

		void DeathCall();
		void ScoreCall();
		void ScoreAchievementUnlock();
		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();
		void ButtonsNotPressed();

		void AddObserver(std::shared_ptr<Observer> observer);
		void update(float) override;
		void Render()const override;
		void SetPosition(float, float, float) override;

		void Notify(BaseComponent&, EVENT);
		void SetVelocity(Vector2f velocity) { m_Velocity = velocity; }

		void UpdateSprite(float elapsedSec);
		void UpdateSourceRect();
		void ChangeState(PlayerState&);
		bool GetIsFlipped() { return IsTextureFlipped; }
		void FLipTexture(bool flip);
		
		Rectf& GetPlayerPos() { return m_DestRect; }
		void CallPlayerIsDead();
		//bool m_SetVelocity = false;
	protected:
		

	private:
		
		const int MAX_OBSERVERS{ 3 };
		std::vector<std::shared_ptr<Observer>> m_Observers;
		//std::shared_ptr<TransformComponent> m_TransformComponent;
		std::shared_ptr<RenderComponent> m_SpriteTexture;
		
		std::shared_ptr<dae::RenderComponent> m_PlayerIcon;
		EVENT eve;
		Vector2f m_playerPos;
		Vector2f m_Velocity{};
		float m_MoveSpeed;
		Vector2f m_Acceleration{};
		Sound_System* a;
		std::thread walk;
		std::thread die;
		

		//sprite
		Rectf m_DestRect;
		Rectf m_SrcRect;
		int m_Colums;
		int m_Rows;
		float m_AccumulatedSec;
		int m_CurrFrame;
		float m_FrameTime;
		float m_FramesPerSecond;
		bool IsMoving;
		bool IsTextureFlipped;

		PlayerState m_PlayerState{ PlayerState::standing };
		playerMovement m_playerMovement;
		 LevelComponent* m_sLevel;

		float m_SpriteSheetWidth;
		float m_SpriteSheetHeight;
		float m_SpriteSheetLeft;
		float m_SpriteSheetTop;
		float m_StartPoint;
		float m_InitialDestRectBottom;

		void InitializeDestRect();
		void InitializeSrcRect();
		void UpdatePlayerMovement(float elapsedSec);
		void ChangeFrameRate(bool lower);
		void RenderPlayerLiveCount() const;
		
	};
}

