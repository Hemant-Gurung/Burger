#include "BaseComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"
namespace dae
{
	
	class EnemyComponent :public dae::BaseComponent
	{
	public:
		EnemyComponent(std::shared_ptr<dae::GameObject>&,EnemyType&, std::shared_ptr<LevelComponent>);
		 ~EnemyComponent() = default;

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

		void IsDead(bool);
		bool GetIsDead() { return m_IsDead; }
		void ResetEnemyPos()
		{
			if(this->m_enemyType == EnemyType::Egg)
			{
				int ra = rand() % (1 - 0 + 1) + 0;
				if (ra == 1)
				{
					m_DestRect.left = 0;
					m_DestRect.bottom = 450;
				}
				else
				{
					m_DestRect.left = 450;
					m_DestRect.bottom = 10;
				}
			}
			else
			{
				int ra = rand() % (1 - 0 + 1) + 0;
				if (ra == 1)
				{
					m_DestRect.left = 0;
					m_DestRect.bottom = 0;
				}
				else
				{
					m_DestRect.left = 450;
					m_DestRect.bottom = 250;
				}
			}
			
		}
	
	private:
		EnemyStates m_enemyState;
		EnemyType m_enemyType;
		std::shared_ptr<dae::RenderComponent> m_EnemySprite;
		std::weak_ptr<LevelComponent> m_sLevel;
		Rectf m_DestRect;
		Rectf m_SrcRect;

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
	};

}
