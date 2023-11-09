#pragma once

#include "Quantum/UI/QuWidgetButton.h"
#include "Quantum/UI/QuWidgetText.h"

class HUDGame : public QuWidget
{
public:
	HUDGame();

	void createHUD();
	float alignCenter(float objectTo, float objectIn);
	
	void setScore(int scoreValue);
private :
	std::string scoreText;
	QuWidgetText* score;

	int* m_scoreValue;

};

