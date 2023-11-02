#pragma once

#include "Timer.h"
#include "Window.h"
#include "QuWorld.h"
#include "Quantum/UI/QuMainWidget.h"


class Game
{
public:

	static inline Game& getInstance() { static Game g_game; return g_game; }

	Game();

	int init();
	void shutdown();

	void mainLoop();

	void openWorld(QuWorld* world);
	void openWidget(QuWidget* widget);

	void visitEntity(QuEntity* entity);
	void visitUI(QuWidget* widget);



private:
	Timer m_timer;
	Window m_window;

	QuWorld* m_world;
	QuWidget* m_widget;
};
