#include "pch.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
//#include "Minigin.h"
//#include <steam_api.h>
#include "Minigin.h"
#include  "Tron.h"


int main(int, char*[]) {

	Tron engine;

	//engine.Initialize();
	//engine.Initialize();
	// tell the resource manager where he can find the game data
	engine.Run();
    //Manang engine;
    return 0;
}
