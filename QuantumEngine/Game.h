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


	inline const Timer& getTimer() const { return m_timer; }
	inline       Timer& getTimer()       { return m_timer; }


private:

	void openWorld_triggerSpawn(QuWorld* w, QuEntity* en);



private:

	Timer m_timer;
	Window m_window;

	QuWorld* m_world;
};
