#include "stdafx.h"
#include "Game.h"

#include "Graphics.h"

#include "QuWorld.h"
#include "QuEntityLightDirectional.h"

#include <iostream>
#include <cassert>
#include <string>
#include <ctime>

#include "Quantum/Physics/PhysicsWorld.h"



static const char WINDOW_TITLE[] = "Direct3D 12 Test";



Game::Game()
{
	
}

int Game::init()
{
	int status;

	m_window.initialize(1600, 900, WINDOW_TITLE, false);

	status = Graphics::initialize(&m_window);

	UIsystem = new UISystem;

	return status;
}

void Game::shutdown()
{
	Graphics::shutdown();
}

void Game::mainMenuLoop()
{
	time_t currentTime = time(0);
	time_t lastTime = currentTime;

	int frames = 0;

	while (!m_window.wantsToClose() || !m_widget)
	{
		m_window.pollEvents();

		UIsystem->visitUI(m_widget);

		m_timer.tick();

		Graphics::getInstance().renderFrame();


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

void Game::mainLoop()
{
	time_t currentTime = time(0);
	time_t lastTime = currentTime;

	int frames = 0;

	while (!m_window.wantsToClose())
	{
		m_window.pollEvents();

		if (m_world) {
			m_world->deletePendingEntities();
			
			// TODO : fixed time step.
			m_world->getPhysicsWorld()->step(m_timer.getDeltaTime());

			visitEntity(m_world);
		}

		UIsystem->visitUI(m_widget);

		m_timer.tick();

		Graphics::getInstance().renderFrame();


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



void Game::openWorld_triggerSpawn(QuWorld* w, QuEntity* en)
{
	en->OnSpawn(w);

	for (QuEntity* c = en->m_FirstChild; c; c = c->m_Sibling)
	{
		this->openWorld_triggerSpawn(w, c);
	}
}

void Game::openWorld(QuWorld* world)
{
	m_world = world;
	m_world->setOpen(true);

	this->openWorld_triggerSpawn(world, world);
}
void Game::openWidget(QuWidget* widget)
{
	m_widget = widget;
}


int Game::getRenderWidth() const
{
	return Graphics::getInstance().getRenderWidth();
}

int Game::getRenderHeight() const
{
	return Graphics::getInstance().getRenderHeight();
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
