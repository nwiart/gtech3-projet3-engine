#include "Game.h"

#include "QuWorld.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "Quantum/Generate/CapsuleGenerator.h"

#include "Model.h"
#include "TextureCube.h"

#include "QuEntityRenderModel.h"
#include "QuEntityRenderSkybox.h"
#include "QuEntityLightDirectional.h"
#include "QuEntityLightPoint.h"
#include "QuEntityPhysicsCollider.h"

#include "Quantum/Math/Math.h"

#include "resource.h"

#include <stdlib.h>
#include <time.h>

#include "EntityController.h"
#include "EntityPlanetarySystem.h"


namespace qm = Quantum::Math;



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

		EntityPlanetarySystem* ps = new EntityPlanetarySystem(6.0F, 8.0F);
		ps->setPosition(DirectX::XMFLOAT3(4.0F, 2.0F, 6.0F));
		world->attachChild(ps);
		
		// Spheres.
		for (int i = 0; i < 500; i++)
		{
			QuEntityRenderModel* model = new QuEntityRenderModel();
			model->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(0, 80.0F)));
			model->SetModel(sphere);
			world->attachChild(model);
		}

		// Boxes.
		for (int i = 0; i < 500; i++)
		{
			QuEntityRenderModel* model = new QuEntityRenderModel();
			model->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(0, 80.0F)));
			model->SetModel(box);
			world->attachChild(model);
		}

		// Capsules.
		for (int i = 0; i < 500; i++)
		{
			QuEntityRenderModel* model = new QuEntityRenderModel();
			model->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(0, 80.0F)));
			model->SetModel(capsule);
			world->attachChild(model);
		}
	}


	QuEntityPhysicsCollider* physCol = new QuEntityPhysicsCollider();
	physCol->setPosition(DirectX::XMFLOAT3(0.0F, 0.0F, 3.0F));

	QuEntityRenderModel* physModel = new QuEntityRenderModel();
	physModel->SetModel(sphere);
	physModel->setPosition(DirectX::XMFLOAT3(0.0F, 0.0F, 0.0F));

	physCol->attachChild(physModel);

	world->attachChild(physCol);


	EntityController* c = new EntityController();
	world->attachChild(c);

	QuEntityLightPoint* pointLight = new QuEntityLightPoint();
	pointLight->setIntensity(1.0F);
	c->attachChild(pointLight);



	game.openWorld(world);


	game.mainLoop();

	game.shutdown();

	return 0;
}
