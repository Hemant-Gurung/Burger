#include "BaseComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"
#include "BulletComponent.h"
#include "Observer.h"

namespace dae
{
	
	class EnemyComponent :public dae::BaseComponent
	{
	public:
		EnemyComponent(std::shared_ptr<dae::GameObject>&,EnemyType&, std::shared_ptr<LevelComponent>);
		 ~EnemyComponent();

		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) noexcept = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) noexcept = delete;

		void Initialize();
		void InitializeDstRect();
		void InitializeSrcRect();

		void Render() const override;
		void update(float) override;

		void UpdateSprite(float elapsedSec);
		void UpdateSourceRect();

		void UpdateEnemyMovementState(float elapsedSec);
		void UpdateEnemyMovementusingState(float elapsedSec);
		void HandleEnemyCollision();
		bool checkIfMovementIsPossible(EnemyStates&);
		Rectf& GetEnemyPos();
		void UpdatePlayerPosInLevel(const Rectf& playerpos) { m_PlayerPos = playerpos; }

		void IsDead(bool);
		bool GetIsDead() { return m_IsDead; }
		void ResetEnemyLife(bool dead);
		void ResetEnemyPos();
		Vector2f& GetVelocity() { return m_Velocity; }

		void AddObserver(std::shared_ptr<Observer> observer);
		void Notify(BaseComponent&, EVENT);
		bool CheckIfPlayerIsHit();
		bool CheckIfPlayer2IsHit();


	
	private:
		EnemyStates m_enemyState;
		EnemyType m_enemyType;
		std::unique_ptr<dae::RenderComponent> m_EnemySprite;
		std::unique_ptr<dae::RenderComponent> m_EnemySpriteVertical;

		//Bullet
		std::unique_ptr<dae::RenderComponent> m_EnemyBulletUp;
		std::unique_ptr<dae::RenderComponent> m_EnemyBulletSide;



		
		std::weak_ptr<LevelComponent> m_sLevel;
		Rectf m_DestRect;
		Rectf m_SrcRect;

		Rectf m_PlayerPos;
		float m_SpriteSheetWidth;
		float m_SpriteSheetHeight;
		float m_SpriteSheetLeft;
		float m_SpriteSheetTop;
		float m_StartPoint;
		float m_InitialDestRectBottom;

		float m_AccumulatedSec;
		int m_CurrFrame;
		float m_FrameTime;
		float m_FramesPerSecond;

		int m_Colums;
		int m_Rows;
		bool m_IsDead;

		Vector2f m_Velocity{};
		float m_MoveSpeed;

		bool m_FlipVertical, m_FlipHorizontal;
		int m_CountDamageTaken;

		void CheckIfPlayerIsInFront();
		void GenerateBullet();
		bool m_shootBullet;
		bool m_ExecuteBullet;
		float m_AccumulatedTime;
		int timetoWaitBeforeShooting;
		std::vector<std::shared_ptr<BulletComponent>> m_Bullets;
		std::vector<std::shared_ptr<Observer>> m_Observers;


	};

}
