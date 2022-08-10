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
		std::string GetTag() const override { return m_Tag; };
		void CallPlayerIsDead();

		void CallScoreNotify();

		void DestroyLive();
		void AddScore();
		void SetScore(int score) { m_Score = score; }
		void SetLives(int lives) { m_TotalLives = lives; }
		int GetScore() { return m_Score; }
		int GetLives() const { return m_TotalLives; }
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
		std::unique_ptr<RenderComponent> m_SpriteTexture;
		std::unique_ptr<RenderComponent> m_SpriteVerticalTexture;
		std::unique_ptr<dae::RenderComponent> m_PlayerIcon;
		std::unique_ptr<dae::RenderComponent> m_TurretTexture;
		std::unique_ptr<dae::RenderComponent> m_DeathExplosion;
		PlayerState m_PlayerState{ PlayerState::standing };
		playerMovement m_playerMovement;
		std::weak_ptr<LevelComponent> m_sLevel;
		std::vector<std::unique_ptr<BulletComponent>> m_Bullets;

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
		void RenderTankTurret() const;
		void PlayDeadSound();
		void WaitBeforeEnteringTheArena();
		void GuiUpdate();
		void UpdateBullet(float);

		int m_TotalLives;
		int m_Score;

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

