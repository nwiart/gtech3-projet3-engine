#pragma once

#include "Timer.h"
#include "Window.h"

#include "Quantum/UI/QuMainWidget.h"
#include "Quantum/UI/UISystem.h"

class QuEntity;
class QuWorld;


class Game
{
public:

	static inline Game& getInstance() { static Game g_game; return g_game; }

	Game();

	int init();
	void shutdown();

	void mainMenuLoop();

	void mainLoop();

	void openWorld(QuWorld* world);
	void openWidget(QuWidget* widget);

	int getRenderWidth() const;
	int getRenderHeight() const;

	void visitEntity(QuEntity* entity);


	inline const Timer& getTimer() const { return m_timer; }
	inline       Timer& getTimer()       { return m_timer; }

	inline QuWidget* getOpenWidget() const { return m_widget; }

	inline UISystem& getUISystem() const { return *UIsystem; }

	inline Window& getWindow() { return m_window; }

private:

	void openWorld_triggerSpawn(QuWorld* w, QuEntity* en);



private:

	Timer m_timer;
	Window m_window;

	QuWorld* m_world;
	QuWidget* m_widget;

	UISystem* UIsystem;
};
