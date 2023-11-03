#include "Game.h"

#include "QuWorld.h"
#include "Quantum/Generate/SphereGenerator.h"

#include "TextureCube.h"

#include "QuEntityRenderModel.h"
#include "QuEntityRenderSkybox.h"
#include "QuEntityLightDirectional.h"

#include "Quantum/Math/Math.h"

#include "resource.h"

#include <stdlib.h>
#include <time.h>

#include "EntityController.h"
#include "Shooting.h"
#include "Bullet.h"


const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;



#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	srand(time(0));

	Game& game = Game::getInstance();

	int res = game.init();
	if ( res != 0 ) {
		return res;
	}


	// Load resources.
	Model* sphere = new Model();
	Quantum::SphereGenerator::generate(sphere);

	TextureCube skyboxTexture("textures/milkyway.dds");


	// Create the world.
	QuWorld* world = new QuWorld();
	{
		// Global directional light.
		QuEntityLightDirectional* dirLight = new QuEntityLightDirectional();
		world->attachChild(dirLight);

		// Skybox.
		QuEntityRenderSkybox* entitySkybox = new QuEntityRenderSkybox();
		entitySkybox->setTexture(&skyboxTexture);
		world->attachChild(entitySkybox);

		// Spheres.
		QuEntity* sphereE;
		//for (int i = 0; i < 50; i++)
		//{
		//	namespace qm = Quantum::Math;

		//	QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
		//	sphereEntity->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(0, 10.0F)));
		//	sphereEntity->SetModel(sphere);
		//	world->attachChild(sphereEntity);

		//	sphereE = sphereEntity;
		//}

		//for (int i = 0; i < 10; i++)
		//{
		//	namespace qm = Quantum::Math;

		//	QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
		//	sphereEntity->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(0, 10.0F)));
		//	sphereEntity->SetModel(sphere);
		//	world->attachChild(sphereEntity);
		//}

		EntityController* c = new EntityController();
		world->attachChild(c);

		Shooting* s = new Shooting();
		c->attachChild(s);
	}

	game.openWorld(world);


	game.mainLoop();

	game.shutdown();

	return 0;
}
