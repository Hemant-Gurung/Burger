#pragma once
//#include <SDL_mixer.h>
#include "LevelComponent.h"
//#include "box2d.h"
#include "sdl_sound_system.h"
#include "SVGParser.h"
#include "Scene.h"
#include "EnemyComponent.h"
struct SDL_Window;



namespace dae
{
	
	class Minigin
	{
	public:

		const char* LEVELS[3] =
		{
			"Level_1.png",
			"Level_2.png",
			"Level_3.png"
		};
		const char* LEVEL_COLLISIONS[3] =
		{
			"../Data/Level - SDL_1.svg",
			"../Data/Level - SDL_2.svg",
			"../Data/Level - SDL_3.svg"
		};

		~Minigin()
		{
			//delete m_pLevel;
			delete ss;
		}
		void Instruction();
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
		void InitializeImgui();


	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
		//SDL_Renderer* m_Renderer;
		 //std::unique_ptr<b2World> m_World{};
		 sdl_sound_system* ss;
		 SVGParser m_svg_parser_;
		 std::vector<std::vector<Point2f>> m_vertices;


		  void PlayerOne(Scene&, LevelComponent&) const;
		  void PlayerTwo(Scene&, LevelComponent&) const;
		  void Enemy(Scene&,EnemyType&,  LevelComponent&) const;
		  void AddBurger(Scene&, LevelComponent&) const;

		  void ImguiUpdate();

		  bool isOn;
	};
}
