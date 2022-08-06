#pragma once
#include "Base/Observer.h"
#include "BaseComponent.h"
//#include "box2d.h"
#include <thread>
#include "Utils/Sound_System.h"
#include  "LevelComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "BulletComponent.h"

namespace dae
{

	enum class PlayerState
	{
		standing,
		running,
		climbing,
		throwingPepper,
		dead
	};

	class PlayerComponent : public BaseComponent
	{
	public:
		PlayerComponent(std::shared_ptr<GameObject>&, std::shared_ptr<LevelComponent>,std::string textureName = "PlayerTank.png");
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
		void ShootBullet();
		std::string GetTag() const override { return m_Tag; };
		void SetTag(std::string tag) { m_Tag = tag; }

		void AddObserver(std::shared_ptr<Observer> observer);
		void update(float) override;
		void Render()const override;
		void SetPosition(float, float, float) override;

		void Notify(BaseComponent&, EVENT);
		void SetVelocity(Vector2f velocity) { m_Velocity = velocity; }

		void UpdateSprite(float elapsedSec);
		void UpdateSourceRect();
		void ChangeState(PlayerState&);
		bool GetIsFlippedHorizontal()const { return IsTextureFlippedHorizontal; }
		bool GetIsFlippedVertical() const { return IsTextureFlippedVertical; }
		void FLipTextureHorozontal(bool flip);
		void FLipTextureVertical(bool flip);

		
		Rectf& GetPlayerPos() { return m_DestRect; }
		void CallPlayerIsDead();

		void CallScoreNotify();

		void ThrowPepper();
		void DestroyLive();
		void AddScore();
		void SetScore(int score) { m_Score = score; }
		void SetLives(int lives) { m_TotalLives = lives; }
		int GetScore() { return m_Score; }
		void DecreasePepper();
		int GetLives() const { return m_TotalLives; }
		int GetRemainingPepper() const { return m_Pepper; }
		void RotateTankTurret(float direction);

		void GenerateBullet();
		bool CheckPlayerBulletEnemyCollision(Rectf&);
		void InitializeDestRect();

		void SetPlayerStartPosition(const Point2f& pos)
		{
			m_DestRect.bottom = pos.y;
			m_DestRect.left = pos.x;
		}
		
		//bool m_SetVelocity = false;
	protected:
		

	private:
		
		const int MAX_OBSERVERS{ 3 };
		std::vector<std::shared_ptr<Observer>> m_Observers;
		std::shared_ptr<dae::GameObject> m_sGameObject;
		//std::shared_ptr<TransformComponent> m_TransformComponent;
		std::shared_ptr<RenderComponent> m_SpriteTexture;
		std::shared_ptr<RenderComponent> m_SpriteVerticalTexture;

		
		std::shared_ptr<dae::RenderComponent> m_PlayerIcon;
		std::shared_ptr<dae::RenderComponent> m_PepperIcon;
		std::shared_ptr<dae::RenderComponent> m_TurretTexture;
		std::shared_ptr<dae::RenderComponent> m_DeathExplosion;

		

		EVENT eve;
		Vector2f m_playerPos;
		Vector2f m_Velocity{};
		float m_MoveSpeed;
		Vector2f m_Acceleration{};
		Sound_System* a;
		std::thread walk;
		std::thread die;
		std::string m_Tag;

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
		bool IsTextureFlippedHorizontal;
		bool IsTextureFlippedVertical;

		PlayerState m_PlayerState{ PlayerState::standing };
		playerMovement m_playerMovement;
		std::weak_ptr<LevelComponent> m_sLevel;
		std::vector<std::shared_ptr<BulletComponent>> m_Bullets;
		float m_SpriteSheetWidth;
		float m_SpriteSheetHeight;
		float m_SpriteSheetLeft;
		float m_SpriteSheetTop;
		float m_StartPoint;
		float m_InitialDestRectBottom;

		void InitializeSrcRect();
		void UpdatePlayerMovement(float elapsedSec);
		void ChangeFrameRate(bool lower);
		void RenderPlayerLiveCount() const;
		void RenderPepperIcon() const ;
		void RenderTankTurret() const;
		
	
		void PlayDeadSound();
		void WaitBeforeEnteringTheArena();
		void GuiUpdate();

		int m_TotalLives;
		int m_Score;
		int m_Pepper;

		bool m_ShowDebugLines;
		float m_RotateTurret;

		bool m_shootBullet;
		bool m_ExecuteBullet;
		float m_AccumulatedTime;
		int timetoWaitBeforeShooting;
		float m_TimeToWaitBeforeEntering;
		bool m_IsPlayerShot;
		
	};
}

