#include "UIMainMenu.h"

UIMainMenu::UIMainMenu()
{
	button = new QuWidgetButton();
	button->SetCallBack((QuWidgetButton::OnButtonClickCallback)&UIMainMenu::onButtonPlay);
}

UIMainMenu::~UIMainMenu()
{
}

void UIMainMenu::onButtonPlay(int, int)
{
	exit(0);
}
