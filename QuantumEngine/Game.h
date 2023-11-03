#pragma once

#include "Timer.h"
#include "Window.h"

class QuEntity;
class QuWorld;



class Game
{
public:

	static inline Game& getInstance() { static Game g_game; return g_game; }

	Game();

	int init();
	void shutdown();

	void mainLoop();

	void openWorld(QuWorld* world);

	int getRenderWidth() const;
	int getRenderHeight() const;

	void visitEntity(QuEntity* entity);



private:

	Timer m_timer;
	Window m_window;

	QuWorld* m_world;
};
