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

#include "resource.h"



const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;



int main()
{

	Game& game = Game::getInstance();

	int res = game.init();
	if ( res != 0 ) {
		return res;
	}

	Graphics::getInstance().initTestApp(ID_SHADER_TEST);

	Model* sphere = new Model();

	Quantum::SphereGenerator::generate(sphere);

	QuWorld* world = new QuWorld();

	for (int i = 0; i < 1000; i++) 
	{
		QuEntityRenderModel* sphereEntity = new QuEntityRenderModel();
		sphereEntity->setPosition(DirectX::XMFLOAT3(rand()/1000.f, rand() / 1000.f, rand() / 1000.f));
		sphereEntity->SetModel(sphere);
		sphereEntity->AttachToParent(world);
	}
	//sphereEntity->GetTransform().ApplyRotation(DirectX::XMQuaternionRotationRollPitchYaw(0, 1.57f, 0));



	game.openWorld(world);

	game.mainLoop();

	game.shutdown();

	return 0;
}
