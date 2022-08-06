#pragma once
// prog-2 files
#include <string>
#include <iostream>

struct Window
{
	explicit Window( const std::string& title = "Title", float width = 320.0f, 
		float height = 180.0f, bool isVSyncOn = true );

	std::string title;
	float width;
	float height;
	bool isVSyncOn;
};

struct Tag
{
	Tag();
	explicit Tag(std::string type);
	std::string type;
};

struct float4
{
	float4();
	explicit float4(float one, float two,float three,float four);
	float one;
	float two;
	float three;
	float four;
};


struct Point2f
{
	Point2f( );
	explicit Point2f( float x, float y );

	float x;
	float y;
};



struct Rectf
{
	Rectf( );
	explicit Rectf( float left, float bottom, float width, float height );
	
	float left;
	float bottom;
	float width;
	float height;

};


struct Color4f
{
	Color4f( );
	explicit Color4f( float r, float g, float b, float a );
	
	float r;
	float g;
	float b;
	float a;
};

struct Circlef
{
	Circlef( );
	explicit Circlef( const Point2f& center, float radius );
	explicit Circlef( float centerX, float centerY, float radius );

	Point2f center;
	float radius;
};


struct Ellipsef
{
	Ellipsef( );
	explicit Ellipsef( const Point2f& center, float radiusX, float radiusY );
	explicit Ellipsef( float centerX, float centerY, float radiusX, float radiusY );

	Point2f center;
	float radiusX;
	float radiusY;
};


//ENUM
enum class GamePadIndex
{
	playerOne,
	playerTwo
};




inline const char* LEVELS[3] =
{
	"Tron/Level_1.png",
	"Tron/Level_2.png",
	"Tron/Level_3.png"
};
inline const char* LEVEL_COLLISIONS[3] =
{
	//"../Data/Level - SDL_1.svg",
	"../Data/Tron/Level_1.svg",
	"../Data/Tron/Level_2.svg",
	"../Data/Tron/Level_3.svg"
};

inline const Point2f PlayerStartPosition[4] = 
{
	Point2f{429.f,466.f},Point2f{17.f,466.f},
	Point2f{15.f,78.f}, Point2f{432.f,78.f}
};

inline const Point2f EnemyStartPosition[12] =
{
	Point2f{429.f,78.f },  Point2f{17.f,78.f},Point2f{150.f,78.f},
	Point2f{429.f,350.f},  Point2f{17.f,350.f},Point2f{150.f,350.f},
	Point2f{429.f,200.f},  Point2f{17.f,200.f},Point2f{150.f,200.f},
	Point2f{429.f,466.f}, Point2f{17.f,466.f},Point2f{150.f,466.f}

};

enum EnemyType
{
	Red,
	Egg
};


//enum startPos
//{
//	Point2f = Point2f( 450.f,450.f ),
//	//Point2f = Point2f(450.f, 450.f),
//
//};