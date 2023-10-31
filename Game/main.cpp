#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi.h>

using namespace Microsoft::WRL;

#include "Game.h"
#include "Graphics.h"
#include "QuWorld.h"
#include "Quantum/Generate/SphereGenerator.h"

#include "QuEntityRenderModel.h"
#include "QuEntityLightDirectional.h"

#include "Quantum/Math/Math.h"

#include "resource.h"

#include <stdlib.h>
#include <time.h>



const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;



int main()
{
	srand(time(0));

	Game& game = Game::getInstance();

	int res = game.init();
	if ( res != 0 ) {
		return res;
	}


	Model* sphere = new Model();

	Quantum::SphereGenerator::generate(sphere);

	QuWorld* world = new QuWorld();

	QuEntityLightDirectional* dirLight = new QuEntityLightDirectional();
	world->attachChild(dirLight);

	QuEntity* sphereE;
	for (int i = 0; i < 50; i++) 
	{
		namespace qm = Quantum::Math;

		QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
		sphereEntity->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(0, 10.0F)));
		sphereEntity->SetModel(sphere);
		world->attachChild(sphereEntity);

		sphereE = sphereEntity;
	}
	
	for (int i = 0; i < 10; i++)
	{
		namespace qm = Quantum::Math;

		QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
		sphereEntity->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(0, 10.0F)));
		sphereEntity->SetModel(sphere);
		world->attachChild(sphereEntity);
	}



	game.openWorld(world);

	game.mainLoop();

	game.shutdown();

	return 0;
}
