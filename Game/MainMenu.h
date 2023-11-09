#pragma once

#include "Quantum/UI/QuWidgetButton.h"
#include "Quantum/UI/QuWidgetText.h"


class MainMenu : public QuWidget
{
public:
	MainMenu();

	void createMainMenu(Texture2D& buttonTexture);
	void onButtonPlay(int x, int y);
	void onButtonExit(int x, int y);
	float MainMenu::alignCenter(float objectTo, float objectIn);


private:
	QuWidgetText* Title;
	QuWidgetButton* Play;
	QuWidgetButton* Exit;

	


};

