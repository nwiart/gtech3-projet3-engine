#pragma once

#include "Timer.h"
#include "Window.h"

#include "Quantum/UI/QuMainWidget.h"

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
	void openWidget(QuWidget* widget);

	int getRenderWidth() const;
	int getRenderHeight() const;

	void visitEntity(QuEntity* entity);
	void visitUI(QuWidget* widget);


private:

	void openWorld_triggerSpawn(QuWorld* w, QuEntity* en);



private:

	Timer m_timer;
	Window m_window;

	QuWorld* m_world;
	QuWidget* m_widget;
};
