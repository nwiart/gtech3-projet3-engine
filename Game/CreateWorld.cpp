#include "CreateWorld.h"
#include "QuWorld.h"
#include "QuEntityRenderModel.h"
#include "QuEntityRenderSkybox.h"
#include "QuEntityLightDirectional.h"
#include "QuEntityLightPoint.h"
#include "QuEntityPhysicsCollider.h"

#include "EntityEnemySwarm.h"
#include "MeteorShower.h"
#include "PlanetBackground.h"
#include "EntityController.h"
#include "Player.h"

#include "Game.h"

#include "ResourceLibrary.h"

CreateWorld::CreateWorld()
{
}

void CreateWorld::init()
{
	

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
		entitySkybox->setTexture(&ResourceLibrary::Get().skybox);
		world->attachChild(entitySkybox);

		//Spawn asteroid
		MeteorShower* meteorShower = new MeteorShower();
		world->attachChild(meteorShower);

		//Spawn multiple enemy
		EntityEnemySwarm* EnemySwarm = new EntityEnemySwarm();
		world->attachChild(EnemySwarm);

		// Planet background.
		PlanetBackground* planetBackground = new PlanetBackground();
		world->attachChild(planetBackground);
	}

	// Player controller.
	EntityController* c = new EntityController();
	c->setPosition(DirectX::XMFLOAT3(0, 0, -8));
	world->attachChild(c);

	Player::SetEntityController(c);

	QuEntityLightPoint* pointLight = new QuEntityLightPoint();
	pointLight->setIntensity(1.0F);
	c->attachChild(pointLight);

	Game::getInstance().openWorld(world);

	hud = new HUDGame();
	hud->createHUD();

	Player::SetHUD(hud);

	Game::getInstance().openWidget(hud);
}
