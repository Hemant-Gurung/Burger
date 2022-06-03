#include "BaseComponent.h"
#include "RenderComponent.h"
#include "LevelComponent.h"
namespace dae
{
	/*enum  EnemyStates
	{
		movingLeft,
		movingRight,
		movingUp,
		movingDown,
		Dead
	};*/

	enum EnemyType
	{
		Red,
		Egg
	};


	class EnemyComponent :public BaseComponent
	{
	public:
		EnemyComponent(std::shared_ptr<GameObject>&,EnemyType&, LevelComponent&);
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
	
	private:
		EnemyStates m_enemyState;
		EnemyType m_enemyType;
		std::shared_ptr<RenderComponent> m_EnemySprite;
		LevelComponent* m_sLevel;
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

		Vector2f m_Velocity{};
		float m_MoveSpeed;
	};

}
