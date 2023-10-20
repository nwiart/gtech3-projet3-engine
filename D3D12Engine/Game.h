#pragma once

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

	Window m_window;
};
