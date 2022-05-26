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

class LevelComponent:public dae::BaseComponent
{
	

public:

	LevelComponent(std::shared_ptr<dae::GameObject>&);


	~LevelComponent() = default;
	

	LevelComponent(const LevelComponent&) = delete;
	LevelComponent& operator=(const LevelComponent&) = delete;
	LevelComponent(LevelComponent&&) = delete;
	LevelComponent& operator=(LevelComponent&&) = delete;

	void Initialize(const std::string& filename);
	void Draw() const;
	void Update(float dt);
	void Render() const override;
	void HandleCollision(Rectf& actorShape, Vector2f& actorvelocity,playerMovement&);
	bool doOverlap(Vector2f l1, Vector2f r1, Vector2f l2, Vector2f r2);
	bool IsOnGround(const Rectf& actorShape, Vector2f& actorvelocity);

	Point2f rayStartFirst;
	Point2f rayEndFirst;

	Point2f rayStartMiddle;
	Point2f rayEndMiddle;

	Point2f rayStartLast;
	Point2f rayEndLast;

private:
	std::vector<std::vector<Point2f>> m_vertices;
	SVGParser m_svg_parser_;
	LevelComponent* m_level;
	/*SDL_Window* m_window;
	SDL_Renderer* m_Renderer;*/


	void DrawVertices();
};

