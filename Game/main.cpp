#include "Game.h"

#include "Quantum/Generate/SphereGenerator.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "Quantum/Generate/CapsuleGenerator.h"
#include "Quantum/Generate/MeteorGenerator.h"


#include "Model.h"
#include "Texture2D.h"
#include "TextureCube.h"

#include "Quantum/Math/Math.h"

#include "resource.h"

// Engine entities.
#include "QuWorld.h"
#include "QuEntityRenderModel.h"
#include "QuEntityRenderSkybox.h"
#include "QuEntityLightDirectional.h"
#include "QuEntityLightPoint.h"
#include "QuEntityPhysicsCollider.h"

// Game entities.
#include "EntityController.h"
#include "Shooting.h"
#include "Bullet.h"
#include "EntityPlanetarySystem.h"
#include "EntityGravityField.h"
#include "EntityGravityAffected.h"
#include "MeteorShower.h"
#include "EntityEnemySwarm.h"
#include "Player.h"

#include "EntityParticleSmoke.h"

// Standard lib.
#include <stdlib.h>
#include <time.h>


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

	Texture2D awesome("textures/awesome.dds");
	Texture2D smoke("textures/smoke.dds");
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

		MeteorShower* meteorShower = new MeteorShower();
		world->attachChild(meteorShower);

		EntityEnemySwarm* EnemySwarm = new EntityEnemySwarm();
		world->attachChild(EnemySwarm);
	}




	// Player controller.
	EntityController* c = new EntityController();
	c->setPosition(DirectX::XMFLOAT3(0, 0, -8));
	world->attachChild(c);

	// TODO : remove later
	EntityParticleSmoke* pe0 = new EntityParticleSmoke(&smoke);
	pe0->setPosition(DirectX::XMFLOAT3(-3, -1.0F, 3));
	c->attachChild(pe0);
	EntityParticleSmoke* pe1 = new EntityParticleSmoke(&smoke);
	pe1->setPosition(DirectX::XMFLOAT3(3, -1.0F, 3));
	c->attachChild(pe1);

	Player::SetEntityController(c);

	Shooting* s = new Shooting();
	c->attachChild(s);

	QuEntityLightPoint* pointLight = new QuEntityLightPoint();
	pointLight->setIntensity(1.0F);
	c->attachChild(pointLight);

	game.openWorld(world);


	game.mainLoop();

	game.shutdown();

	return 0;
}
