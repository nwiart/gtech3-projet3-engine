#include "Game.h"

#include "QuWorld.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "Quantum/Generate/CapsuleGenerator.h"

#include "TextureCube.h"

#include "QuEntityRenderModel.h"
#include "QuEntityRenderSkybox.h"
#include "QuEntityLightDirectional.h"
#include "QuEntityLightPoint.h"

#include "Quantum/Math/Math.h"

#include "resource.h"

#include <stdlib.h>
#include <time.h>

#include "EntityController.h"



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

	QuEntityLightPoint* pointLight = new QuEntityLightPoint();
	pointLight->setIntensity(0.2F);


	Model* sphere = new Model();
	Model* box = new Model();
	Model* capsule = new Model();

	Quantum::SphereGenerator::generate(sphere);
	Quantum::BoxGenerator::generate(box, 0.5f);
	Quantum::CapsuleGenerator::generate(capsule);

	TextureCube skyboxTexture("textures/milkyway.dds");


	// Create the world.
	QuWorld* world = new QuWorld();
	{
		// Global directional light.
		QuEntityLightDirectional* dirLight = new QuEntityLightDirectional();
		dirLight->setAmbientColor(Quantum::Color(0, 10, 20));
		dirLight->setIntensity(0.5F);
		world->attachChild(dirLight);

		// Skybox.
		QuEntityRenderSkybox* entitySkybox = new QuEntityRenderSkybox();
		entitySkybox->setTexture(&skyboxTexture);
		world->attachChild(entitySkybox);

		// Spheres.
		for (int i = 0; i < 20; i++)
		{
			namespace qm = Quantum::Math;

			QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
			sphereEntity->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(0, 10.0F)));
			sphereEntity->SetModel(box);
			world->attachChild(sphereEntity);
		}

		for (int i = 0; i < 20; i++)
		{
			namespace qm = Quantum::Math;

			QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
			sphereEntity->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(0, 10.0F)));
			sphereEntity->SetModel(sphere);
			world->attachChild(sphereEntity);
		}

		for (int i = 0; i < 20; i++)
		{
			namespace qm = Quantum::Math;

			QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
			sphereEntity->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(-4.0F, 4.0F), qm::randomFloat(0, 10.0F)));
			sphereEntity->SetModel(capsule);
			world->attachChild(sphereEntity);
		}
	}

	EntityController* c = new EntityController();
	c->AttachToParent(world);
	c->attachChild(pointLight);


	game.openWorld(world);


	game.mainLoop();

	game.shutdown();

	return 0;
}
