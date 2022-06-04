#pragma once
#include "BaseComponent.h"
#include "SVGParser.h"
#include "structs.h"
#include "Vector2f.h"

struct SDL_Renderer;
struct SDL_Window;

// player movement
enum  playerMovement
{
	movingleft,
	movingright,
	movingup,
	movinddown,
	idle
};

enum  EnemyStates
{
	movingLeft,
	movingRight,
	movingUp,
	movingDown,
	Dead
};

class LevelComponent:public dae::BaseComponent
{
public:

	LevelComponent(std::shared_ptr<dae::GameObject>&);

	virtual ~LevelComponent() = default;
	

	LevelComponent(const LevelComponent&) = delete;
	LevelComponent& operator=(const LevelComponent&) = delete;
	LevelComponent(LevelComponent&&) = delete;
	LevelComponent& operator=(LevelComponent&&) = delete;

	void Initialize(const std::string& filename);
	void Draw() const;
	void update(float dt) override;
	void Render() const override;
	void HandleCollision(Rectf& actorShape, Vector2f& actorvelocity,playerMovement&);
	void HandleCollision(Rectf& actorShape, Vector2f& actorvelocity, EnemyStates&);

	bool doOverlap(Vector2f l1, Vector2f r1, Vector2f l2, Vector2f r2);
	bool IsOnGround(const Rectf& actorShape, Vector2f& actorvelocity);
	Rectf& GetPlayerPositionInTheLevel() { return m_PlayerPos; }
	


	Point2f rayStartFirst;
	Point2f rayEndFirst;

	Point2f rayStartMiddle;
	Point2f rayEndMiddle;

	Point2f rayStartLast;
	Point2f rayEndLast;

protected:
	void FixedUpdate() override;
	//void update(float) override;
private:
	std::vector<std::vector<Point2f>> m_vertices;
	SVGParser m_svg_parser_;
	bool m_ShowDebugLines;
	Rectf m_PlayerPos;
	Rectf m_EnemyPos;
	//LevelComponent* m_level;
	/*SDL_Window* m_window;
	SDL_Renderer* m_Renderer;*/


	void DrawVertices();
	void GuiUpdate();
};

