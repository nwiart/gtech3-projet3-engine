#include "PauseMenu.h"
#include "Game.h"

#include "Timer.h"

#include "Player.h"



PauseMenu::PauseMenu()
{
	ResumeText = new QuWidgetText();
	Resume = new QuWidgetButton();
	Exit = new QuWidgetButton();
	Resume->SetCallBack((QuWidgetButton::OnButtonClickCallback)&PauseMenu::onButtonResume);
	Exit->SetCallBack((QuWidgetButton::OnButtonClickCallback)&PauseMenu::onButtonExit);
}

void PauseMenu::createPauseMenu(Texture2D& buttonTexture)
{
	Game::getInstance().getTimer().pause();


	std::string textTilte = "PAUSED";
	ResumeText->SetPosition(DirectX::XMFLOAT2(alignCenter(textTilte.size() * 32, Game::getInstance().getRenderWidth()), 100.f));
	ResumeText->SetText(textTilte);
	this->attachChild(ResumeText);

	Resume->SetSize(DirectX::XMFLOAT2(400.f, 100.f));
	Resume->SetPosition(DirectX::XMFLOAT2(alignCenter(Resume->GetSize().x, Game::getInstance().getRenderWidth()), 300.f));
	Resume->setTexture(&buttonTexture);
	Resume->createText("Resume");
	this->attachChild(Resume);

	Exit->SetSize(DirectX::XMFLOAT2(400.f, 100.f));
	Exit->SetPosition(DirectX::XMFLOAT2(alignCenter(Exit->GetSize().x, Game::getInstance().getRenderWidth()), 500.f));
	Exit->setTexture(&buttonTexture);
	Exit->createText("Exit");
	this->attachChild(Exit);
}

void PauseMenu::onButtonResume(int x, int y)
{
	Game::getInstance().getTimer().resume();
	delete(this);

	Game::getInstance().openWidget(Player::GetHUD());
}

void PauseMenu::onButtonExit(int x, int y)
{
	SendMessage(Game::getInstance().getWindow().getHwnd(), WM_CLOSE, 0, 0);
}

float PauseMenu::alignCenter(float objectTo, float objectIn)
{
	float X = (objectIn - objectTo) / 2;

	return X;
}
