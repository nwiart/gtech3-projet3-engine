#pragma once

#include "Quantum/UI/QuWidgetButton.h"
#include "Quantum/UI/QuWidgetText.h"

class PauseMenu : public QuWidget
{
public:
	PauseMenu();

	void createPauseMenu(Texture2D& buttonTexture);
	void onButtonResume(int x, int y);
	void onButtonExit(int x, int y);
	float alignCenter(float objectTo, float objectIn);

private:

	QuWidgetText* ResumeText;
	QuWidgetButton* Resume;
	QuWidgetButton* Exit;

};

