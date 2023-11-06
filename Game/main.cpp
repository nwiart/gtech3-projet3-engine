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
#include "Shooting.h"
#include "Bullet.h"
#include "EntityPlanetarySystem.h"
#include "EntityGravityField.h"
#include "EntityGravityAffected.h"


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

		EntityPlanetarySystem* ps = new EntityPlanetarySystem(2.0F, 8.0F);
		ps->setPosition(DirectX::XMFLOAT3(20.0F, 12.0F, 24.0F));
		world->attachChild(ps);


		
		// Spheres.
		for (int i = 0; i < 100; i++)
		{
			QuEntityPhysicsCollider* physCol = new QuEntityPhysicsCollider(0.5F, MOTION_DYNAMIC);
			physCol->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-8.0F, 8.0F), qm::randomFloat(-8.0F, 8.0F), qm::randomFloat(-8.0F, 8.0F)));
			physCol->applyImpulse(DirectX::XMVectorSet(0, 0, 1, 0));

			QuEntityRenderModel* model = new QuEntityRenderModel();
			model->SetModel(sphere);
			physCol->attachChild(model);

			EntityGravityAffected* physGr = new EntityGravityAffected(physCol);
			physCol->attachChild(physGr);

			world->attachChild(physCol);
		}

		// Boxes.
		for (int i = 0; i < 200; i++)
		{
			QuEntityRenderModel* model = new QuEntityRenderModel();
			model->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(0, 80.0F)));
			model->SetModel(box);
			world->attachChild(model);
		}

		// Capsules.
		for (int i = 0; i < 200; i++)
		{
			QuEntityRenderModel* model = new QuEntityRenderModel();
			model->setPosition(DirectX::XMFLOAT3(qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(-40.0F, 40.0F), qm::randomFloat(0, 80.0F)));
			model->SetModel(capsule);
			world->attachChild(model);
		}
	}



	// Test RB.
	QuEntityPhysicsCollider* physCol = new QuEntityPhysicsCollider(3.5F, MOTION_STATIC);
	physCol->setPosition(DirectX::XMFLOAT3(0, 0, 10));

	QuEntityRenderModel* physModel = new QuEntityRenderModel();
	physModel->SetModel(sphere);
	physModel->setScale(DirectX::XMFLOAT3(7, 7, 7));

	EntityGravityField* gf = new EntityGravityField(8.0F);
	physCol->attachChild(gf);

	physCol->attachChild(physModel);

	world->attachChild(physCol);



	// Player controller.
	EntityController* c = new EntityController();
	c->setPosition(DirectX::XMFLOAT3(0, 0, -8));
	world->attachChild(c);

	QuEntityLightPoint* pointLight = new QuEntityLightPoint();
	pointLight->setIntensity(1.0F);
	c->attachChild(pointLight);

	Shooting* s = new Shooting();
	c->attachChild(s);

	game.openWorld(world);


	game.mainLoop();

	game.shutdown();

	return 0;
}
