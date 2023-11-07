#pragma once

#include "Quantum/UI/QuWidgetButton.h"

class UIMainMenu : public QuWidget
{
public:
	UIMainMenu();
	~UIMainMenu();

	void onButtonPlay(int, int);

private:
	QuWidgetButton* button;
};

