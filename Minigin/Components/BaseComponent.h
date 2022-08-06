#pragma once
//#include "box2d.h"
#include  "Utils/Vector2f.h"
namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		 BaseComponent(std::shared_ptr<GameObject>&) ;

		 virtual void update(float);
		 virtual void FixedUpdate();
		 virtual void Render() const ;
		 virtual void SetTexture(const std::string& );
		 virtual void SetPosition(float, float,float) ;
		 virtual Rectf GetPosition();
		 virtual std::string GetTag() const;
		// virtual void AddPhysics();
		 
	protected:
		std::weak_ptr<GameObject> m_pGameObject{};
		//glm::vec2 m_position;
		Vector2f m_position;

	private:
		void PlaySoundEffect();
	};
}