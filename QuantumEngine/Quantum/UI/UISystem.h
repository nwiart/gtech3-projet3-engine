#pragma once

#include "QuWidget.h"

class UISystem
{
public:
	void visitUI(QuWidget* widget);

	void addWidget(QuWidget* widget);

	bool OnLeftClick(int mouseX, int mouseY, QuWidget* widget);

private:


};

