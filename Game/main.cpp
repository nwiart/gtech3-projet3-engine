#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi.h>

using namespace Microsoft::WRL;

#include "Game.h"
#include "Graphics.h"
#include "QuWorld.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "QuEntityRenderModel.h"

#include "Quantum/Math/Math.h"

#include "resource.h"

#include <stdlib.h>
#include <time.h>



const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;


float randomFloat(float min = 0.0F, float max = 1.0F)
{
	return (rand() / (float) RAND_MAX) * (max - min) + min;
}


int main()
{
	srand(time(0));

	Game& game = Game::getInstance();

	int res = game.init();
	if ( res != 0 ) {
		return res;
	}

	Graphics::getInstance().initTestApp(ID_SHADER_TEST);

	Model* sphere = new Model();
	Model* box = new Model();

	Quantum::SphereGenerator::generate(sphere);
	Quantum::BoxGenerator::generate(box);

	QuWorld* world = new QuWorld();

	QuEntity* sphereE;
	for (int i = 0; i < 50; i++) 
	{
		//QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
		//sphereEntity->setPosition(DirectX::XMFLOAT3(0, 0, 0));
		//sphereEntity->SetModel(sphere);
		//sphereEntity->AttachToParent(world);
		QuEntityRenderModel* boxEntity = new QuEntityRenderModel();
		boxEntity->setPosition(DirectX::XMFLOAT3(0, 0, 5));
		boxEntity->SetModel(box);
		boxEntity->AttachToParent(world);
	}



	game.openWorld(world);

	game.mainLoop();

	game.shutdown();

	return 0;
}
