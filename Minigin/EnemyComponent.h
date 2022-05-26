#include "BaseComponent.h"
#include "RenderComponent.h"
namespace dae
{
	enum EnemyMovement
	{
		movingLeft,
		movingRight,
		movingUp,
		movingDown
	};

	class EnemyComponent :public BaseComponent
	{
	public:
		EnemyComponent(std::shared_ptr<GameObject>&);
		~EnemyComponent();

		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) noexcept = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) noexcept = delete;

	private:

		std::shared_ptr<RenderComponent> m_EnemySprite;
		Rectf m_DestRect;
		Rectf m_SrcRect;
	};

}
