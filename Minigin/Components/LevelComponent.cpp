#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "utils.h"

LevelComponent::LevelComponent(std::shared_ptr<dae::GameObject> pGameObject)
	:BaseComponent(pGameObject)
, m_ShowDebugLines(false),
m_IsEnemyShot(false),
m_Isused(false)
	/*,m_level()*/
	,m_vertices()
	
{
	
}

void LevelComponent::Initialize(const std::string& filename)
{
	bool success = m_svg_parser_.GetVerticesFromSvgFile( filename, m_vertices);
	if (!success) { std::cerr << "error reading svg file..\n"; }

	//draw vertices
	DrawVertices();
	// Setup renderer
	
}

void LevelComponent::Draw() const
{
	
}

void LevelComponent::update(float /*dt*/)
{
	GuiUpdate();
	
}

void LevelComponent::Render() const
{
	if (m_ShowDebugLines)
	{
		//m_pGameObject.lock()->GetComponent<dae::RenderComponent>()->RenderTexture();
		for (size_t i = 0; i < m_vertices.size(); ++i)
		{
			auto vert = m_vertices[i];
			for (size_t j = 0; j < vert.size(); ++j)
			{
				//draw all thee vertices
				if (j < vert.size() - 1)
				{
					m_pGameObject.lock()->GetComponent<dae::RenderComponent>()->RenderLine(vert[j].x, vert[j].y, vert[j + 1].x, vert[j + 1].y);
				}

				// draw the closiing vert
				if (j == vert.size() - 1)
				{
					m_pGameObject.lock()->GetComponent<dae::RenderComponent>()->RenderLine(vert[vert.size() - 1].x, vert[vert.size() - 1].y, vert[0].x, vert[0].y);

				}

			}
		}
		m_pGameObject.lock()->GetComponent<dae::RenderComponent>()->RenderLine(rayStartFirst.x, rayStartFirst.y, rayEndFirst.x, rayEndFirst.y);
		m_pGameObject.lock()->GetComponent<dae::RenderComponent>()->RenderLine(rayStartMiddle.x, rayStartMiddle.y, rayEndMiddle.x, rayEndMiddle.y);
		m_pGameObject.lock()->GetComponent<dae::RenderComponent>()->RenderLine(rayStartLast.x, rayStartLast.y, rayEndLast.x, rayEndLast.y);

	}

}

void LevelComponent::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, playerMovement& movement)
{
	
		m_PlayerPos = actorShape;
	
	utils::HitInfo h;
	
	const int lowerRay = 8;
		for (size_t i = 0; i < m_vertices.size(); i++)
		{
			switch (movement)
			{
			case playerMovement::idle:
			
				break;
			case playerMovement::movingleft:
				// <-- first
				// <-- middle
				// <-- last
				rayStartFirst = Point2f(actorShape.left + (actorShape.width / 2.0f), actorShape.bottom + actorShape.height - lowerRay);
				rayEndFirst = Point2f(actorShape.left, actorShape.bottom + actorShape.height - lowerRay);

				rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
				rayEndMiddle = Point2f(actorShape.left, actorShape.bottom + actorShape.height / 2);

				rayStartLast = Point2f{ actorShape.left + actorShape.width/2.0f,actorShape.bottom + lowerRay };
				rayEndLast = Point2f{ actorShape.left,actorShape.bottom + lowerRay };

				if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
				{
					actorShape.left = (h.intersectPoint.x);
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				else if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
				{

					actorShape.left = (h.intersectPoint.x);
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				// last
				else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
				{
					actorShape.left = (h.intersectPoint.x);
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;
				}

				break;
			case playerMovement::movingright:
				// --> first
				// --> middle
				// --> last
				rayStartFirst = Point2f(actorShape.left + (actorShape.width / 2), actorShape.bottom + actorShape.height - lowerRay);
				rayEndFirst = Point2f(actorShape.left + actorShape.width, actorShape.bottom + actorShape.height - lowerRay);

				rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
				rayEndMiddle = Point2f(actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2);

				rayStartLast = Point2f{ actorShape.left + actorShape.width / 2,actorShape.bottom + lowerRay };
				rayEndLast = Point2f{ actorShape.left + actorShape.width,actorShape.bottom + lowerRay };
				// middle
				if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
				{

					actorShape.left = (h.intersectPoint.x)- actorShape.width;
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				// first
				else if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
				{
					actorShape.left = (h.intersectPoint.x) - actorShape.width;
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				// last
				else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
				{
					actorShape.left = (h.intersectPoint.x) - actorShape.width;
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;
				}
				break;
			case playerMovement::movinddown:

				//  ^   ^   ^
				//  |   |   |
				rayStartFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom + actorShape.height / 2.f);
				rayEndFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom + actorShape.height);

				rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
				rayEndMiddle = Point2f(actorShape.left + (actorShape.width / 2.0f), actorShape.bottom + actorShape.height);

				rayStartLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom + actorShape.height / 2.f };
				rayEndLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom + actorShape.height };

				// middle
				if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
				{

					actorShape.bottom = (h.intersectPoint.y)-actorShape.height;
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				// first
				else if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
				{
					actorShape.bottom = (h.intersectPoint.y) - actorShape.height;
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				// last
				else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
				{
					actorShape.bottom = (h.intersectPoint.y) - actorShape.height;
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;
				}

				break;
			case playerMovement::movingup:

				//   |   |   |
				//   v   v	 v
				//   // three rays down
				rayStartFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom + actorShape.height / 2.f);
				rayEndFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom);

				rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
				rayEndMiddle = Point2f(actorShape.left + (actorShape.width / 2.0f), actorShape.bottom);

				rayStartLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom + actorShape.height / 2.f };
				rayEndLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom };

				// middle
				if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
				{

					actorShape.bottom = (h.intersectPoint.y);
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				// first
				else if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
				{
					actorShape.bottom = (h.intersectPoint.y);
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;

				}
				// last
				else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
				{
					actorShape.bottom = (h.intersectPoint.y);
					actorVelocity.y = 0.0f;
					actorVelocity.x = 0.0f;
				}
				break;


			}
				
		}
			
	
}

void LevelComponent::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, EnemyStates& enemystate)
{
	//ddddm_EnemyPos = actorShape;
	utils::HitInfo h;

	const int lowerRay = 8;
	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		switch (enemystate)
		{
		case EnemyStates::movingRight:
			rayStartFirst = Point2f(actorShape.left + (actorShape.width / 2), actorShape.bottom + actorShape.height - lowerRay);
			rayEndFirst = Point2f(actorShape.left + actorShape.width, actorShape.bottom + actorShape.height - lowerRay);

			rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
			rayEndMiddle = Point2f(actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2);

			rayStartLast = Point2f{ actorShape.left + actorShape.width / 2,actorShape.bottom + lowerRay };
			rayEndLast = Point2f{ actorShape.left + actorShape.width,actorShape.bottom + lowerRay };
			// middle
			if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
			{

				actorShape.left = (h.intersectPoint.x) - actorShape.width;
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			// first
			else if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
			{
				actorShape.left = (h.intersectPoint.x) - actorShape.width;
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			// last
			else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
			{
				actorShape.left = (h.intersectPoint.x) - actorShape.width;
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;
			}
			break;
		case EnemyStates::movingLeft:
			// <-- first
					// <-- middle
					// <-- last
			rayStartFirst = Point2f(actorShape.left + (actorShape.width / 2.0f), actorShape.bottom + actorShape.height - lowerRay);
			rayEndFirst = Point2f(actorShape.left, actorShape.bottom + actorShape.height - lowerRay);

			rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
			rayEndMiddle = Point2f(actorShape.left, actorShape.bottom + actorShape.height / 2);

			rayStartLast = Point2f{ actorShape.left + actorShape.width / 2.0f,actorShape.bottom + lowerRay };
			rayEndLast = Point2f{ actorShape.left,actorShape.bottom + lowerRay };

			if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
			{
				actorShape.left = (h.intersectPoint.x);
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			else if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
			{

				actorShape.left = (h.intersectPoint.x);
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			// last
			else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
			{
				actorShape.left = (h.intersectPoint.x);
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;
			}
			break;
		case EnemyStates::movingDown:
			//   |   |   |
					//   v   v	 v
					//   // three rays down
			rayStartFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom + actorShape.height / 2.f);
			rayEndFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom);

			rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
			rayEndMiddle = Point2f(actorShape.left + (actorShape.width / 2.0f), actorShape.bottom);

			rayStartLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom + actorShape.height / 2.f };
			rayEndLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom };

			// middle
			if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
			{

				actorShape.bottom = (h.intersectPoint.y);
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			// first
			else if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
			{
				actorShape.bottom = (h.intersectPoint.y);
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			// last
			else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
			{
				actorShape.bottom = (h.intersectPoint.y);
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;
			}
			break;

		default:
			break;
		case EnemyStates::movingUp:
			//  ^   ^   ^
					//  |   |   |
			rayStartFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom + actorShape.height / 2.f);
			rayEndFirst = Point2f(actorShape.left + lowerRay, actorShape.bottom + actorShape.height);

			rayStartMiddle = Point2f{ actorShape.left + (actorShape.width / 2.0f),actorShape.bottom + actorShape.height / 2.f };
			rayEndMiddle = Point2f(actorShape.left + (actorShape.width / 2.0f), actorShape.bottom + actorShape.height);

			rayStartLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom + actorShape.height / 2.f };
			rayEndLast = Point2f{ actorShape.left + actorShape.width - lowerRay,actorShape.bottom + actorShape.height };

			// middle
			if (utils::Raycast(m_vertices.at(i), rayStartMiddle, rayEndMiddle, h))
			{

				actorShape.bottom = (h.intersectPoint.y) - actorShape.height;
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			// first
			else if (utils::Raycast(m_vertices.at(i), rayStartFirst, rayEndFirst, h))
			{
				actorShape.bottom = (h.intersectPoint.y) - actorShape.height;
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;

			}
			// last
			else if (utils::Raycast(m_vertices.at(i), rayStartLast, rayEndLast, h))
			{
				actorShape.bottom = (h.intersectPoint.y) - actorShape.height;
				actorVelocity.y = 0.0f;
				actorVelocity.x = 0.0f;
			}

			break;
		}
	}
}


void LevelComponent::SetBulletPos(Rectf& bulletPos)
{
	m_BulletPos = bulletPos;
}

bool LevelComponent::checkIfEnemyIsShot()
{
	return m_IsEnemyShot;
}

void LevelComponent::SetEnemyIsShot(bool value)
{
	 m_IsEnemyShot = value;
}

bool LevelComponent::doOverlap(Vector2f l1, Vector2f r1, Vector2f l2, Vector2f r2)
{

	// To check if either rectangle is actually a line
	// For example :  l1 ={-1,0}  r1={1,1}  l2={0,-1}
	// r2={0,1}

	if (l1.x == r1.x || l1.y == r1.y || l2.x == r2.x
		|| l2.y == r2.y) {
		// the line cannot have positive overlap
		return false;
	}

	// If one rectangle is on left side of other
	if (l1.x >= r2.x || l2.x >= r1.x)
		return false;

	// If one rectangle is above other
	if (r1.y >= l2.y || r2.y >= l1.y)
		return false;

	return true;
}

bool LevelComponent::IsOnGround(const Rectf& actorShape, Vector2f& actorvelocity)
{
	Point2f rayStart{ actorShape.left + (actorShape.width / 2.f),actorShape.bottom + actorShape.height };
	Point2f rayEnd{ actorShape.left + (actorShape.width / 2.f),actorShape.bottom -1 };

	
	utils::HitInfo hit;
	if(actorvelocity.y<=0)
	{
		for(size_t i=0;i<m_vertices.size();++i)
		{
			//for (int j = 0; j < 4; ++j)
			{
				if (utils::Raycast(m_vertices.at(i), rayStart, rayEnd, hit))
				{
					return true;
				}
			}
		}
	}
	return false;
}

Rectf& LevelComponent::GetPlayerPositionInTheLevel()
{
	m_Isused = false;
	return m_PlayerPos;
}

Rectf& LevelComponent::GetPlayer2PositionInTheLevel()
{
	return m_PlayerPos2;
}

void LevelComponent::SetPlayerPosInLevel(const Rectf& pos)
{
	
	m_PlayerPos = pos;
	
}

void LevelComponent::SetPlayer2PosInLevel(const Rectf& pos)
{
	m_PlayerPos2 = pos;
}


void LevelComponent::FixedUpdate()
{
	BaseComponent::FixedUpdate();
}

void LevelComponent::DrawVertices()
{
	
	
	
		
	
}

void LevelComponent::GuiUpdate()
{
	ImGui::Begin("collider", NULL);
	//ImGui::SetWindowSize(ImVec2((float)10.f, (float)10.f));
	//ImGui::SetNextWindowPos()
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
	ImGui::Checkbox("ShowDebugLines", &m_ShowDebugLines);
	//if (ImGui::Button("Reset Speed")) {
		// This code is executed when the user clicks the button
		//this->speed = 0;
	//}
	//ImGui::SliderFloat("Speed", &this->speed, 0.0f, 10.0f);
	ImGui::End();
}


