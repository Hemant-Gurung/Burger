#pragma once
#include "Transform.h"
namespace dae
{
	
	class BaseComponent
	{
	public:
		BaseComponent() = default;

		 virtual void update(float);
		 virtual void Render() const ;
		 virtual void SetTexture(const std::string& );
		 virtual void SetPosition(float, float,float) ;
		
	protected:
	
	};
}