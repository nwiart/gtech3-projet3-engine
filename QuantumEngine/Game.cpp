#include "stdafx.h"
#include "Game.h"

#include "Graphics.h"
#include "QuEntityLightDirectional.h"


#include "TestConstantBuffer.h"

#include "D3D12UploadHelper.h"

#include <iostream>
#include <cassert>
#include <string>
#include <ctime>
#include "../Game/EntityController.h"	


static const char WINDOW_TITLE[] = "Direct3D 12 Test";



Game::Game()
{
	
}

int Game::init()
{
	int status;

	m_window.initialize(1280, 720, WINDOW_TITLE, false);

	status = Graphics::initialize(&m_window);
	if (status) {
		return status;
	}
}

void Game::shutdown()
{
	Graphics::shutdown();
}

void Game::mainLoop()
{
	time_t currentTime = time(0);
	time_t lastTime = currentTime;

	int frames = 0;

	QuEntityLightDirectional DirectionnalLight;

	while (!m_window.wantsToClose())
	{
		visitEntity(m_world);

		m_window.pollEvents();

		m_timer.tick();
		
		DirectionnalLight.ExecuteProcedure();
		Graphics::getInstance().renderFrame(m_timer);

		// Display FPS each second.
		frames++;
		currentTime = time(0);
		if (currentTime != lastTime) {

			std::string title = WINDOW_TITLE;
			title += " - ";
			title += std::to_string(frames);
			title += " FPS";
			
			m_window.setTitle(title.c_str());

			lastTime = currentTime;
			frames = 0;
		}
	}
}

void Game::openWorld(QuWorld* world)
{
	m_world = world;
}

void Game::visitEntity(QuEntity* entity)
{
	if (entity == nullptr) return;
	
	entity->OnUpdate(m_timer);
	entity->ExecuteProcedure();
	for (QuEntity* child = entity->m_FirstChild; child != nullptr; child = child->m_Sibling)
	{
		visitEntity(child);
	}
}
