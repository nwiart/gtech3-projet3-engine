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

#include "Quantum/UI/QuWidgetButton.h"
#include "Quantum/UI/QuWidgetText.h"
#include "Quantum/UI/UIRenderer.h"

#include "Quantum/Math/Math.h"

#include "resource.h"

#include <stdlib.h>
#include <time.h>

// Game entities.
#include "EntityController.h"
#include "Shooting.h"
#include "Bullet.h"
#include "EntityPlanetarySystem.h"
#include "EntityGravityField.h"
#include "EntityGravityAffected.h"
#include "MeteorShower.h"
#include "PlanetBackground.h"
#include "EntityEnemySwarm.h"
#include "Texture2D.h"
#include "Player.h"

#include "EntityParticleSmoke.h"

#include "ResourceLibrary.h"

// Standard lib.
#include <stdlib.h>
#include <time.h>

#include "stdafx.h"
#include "Graphics.h"

#include "MainMenu.h"
#include "PauseMenu.h"
#include <Windows.h>


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
	if (res != 0) {
		return res;
	}

	// Load resources.
	Model* sphere = new Model();
	Model* box = new Model();
	Model* capsule = new Model();

	Quantum::SphereGenerator::generate(sphere, 0.5);
	//Quantum::BoxGenerator::generate(box, DirectX0.5f);
	Quantum::CapsuleGenerator::generate(capsule);

	//Texture2D smoke("textures/smoke.dds");
	Texture2D awesome("textures/awesome.dds");
	Texture2D font("textures/font.dds");
	TextureCube skyboxTexture("textures/milkyway.dds");
	Texture2D buttonTexture("textures/button.dds");
	Texture2D smoke("textures/smoke.dds");

	MainMenu* mainMenu = new MainMenu();
	mainMenu->createMainMenu(buttonTexture);
	game.openWidget(mainMenu);


	

	/*
	QuWidget* Widget = new QuWidget();
	Widget->SetSize(DirectX::XMFLOAT2(10, 10));
	Widget->SetPosition(DirectX::XMFLOAT2(0.f, 0.f));

	QuWidgetButton* button = new QuWidgetButton();
	button->SetSize(DirectX::XMFLOAT2(500.f, 500.f/4));
	button->SetPosition(DirectX::XMFLOAT2(100.f, 100.f));
	button->setTexture(&buttonTexture);
	button->createText("option");

	QuWidgetText* text = new QuWidgetText();
	text->SetSize(DirectX::XMFLOAT2(1, 1));
	text->SetPosition(DirectX::XMFLOAT2(50.f, 450.f));
	text->SetText("QuantumEngine : made by you for you with you");
	*/
	
	//MainMenu* mainMenu = new MainMenu();
	//mainMenu->createMainMenu(buttonTexture);

	//PauseMenu* Pmenu = new PauseMenu();
	//Pmenu->createPauseMenu(buttonTexture);


	//Widget->attachChild(text);

	
	//game.openWidget(Pmenu);


	game.mainLoop();

	game.shutdown();

	return 0;
}