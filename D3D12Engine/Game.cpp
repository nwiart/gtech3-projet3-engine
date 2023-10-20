#include "stdafx.h"
#include "Game.h"

#include "Graphics.h"



#include "TestConstantBuffer.h"

#include "D3D12UploadHelper.h"

#include <iostream>
#include <cassert>
#include <string>
#include <ctime>



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

	while (!m_window.wantsToClose())
	{
		m_window.pollEvents();

		m_timer.tick();

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
