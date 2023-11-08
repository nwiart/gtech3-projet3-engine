#include "Game.h"

#include "QuWorld.h"
#include "Quantum/Generate/SphereGenerator.h"
#include "Quantum/Generate/BoxGenerator.h"
#include "Quantum/Generate/CapsuleGenerator.h"
#include "Quantum/Generate/MeteorGenerator.h"


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
#include "MeteorShower.h"
#include "EntityEnemySwarm.h"
#include "Player.h"


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
