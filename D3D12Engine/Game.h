#pragma once

#include "Timer.h"
#include "Window.h"



class Game
{
public:

	static inline Game& getInstance() { static Game g_game; return g_game; }

	Game();

	int init();
	void shutdown();

	void mainLoop();



private:

	Timer m_timer;
	Window m_window;
};
