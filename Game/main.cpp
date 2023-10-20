#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi.h>

using namespace Microsoft::WRL;

#include "Game.h"



const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;



int main()
{
	Game& game = Game::getInstance();

	int res = game.init();
	if ( res != 0 ) {
		return res;
	}

	game.mainLoop();

	game.shutdown();

	return 0;
}
