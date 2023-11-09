#include "PauseMenu.h"
#include "Game.h"

#include "Timer.h"



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


	Crosshair = new QuWidgetButton();
	Crosshair->SetSize(DirectX::XMFLOAT2(128.0F, 64.0F));
	Crosshair->SetPosition(DirectX::XMFLOAT2(alignCenter(Crosshair->GetSize().x, Game::getInstance().getRenderWidth()), 500.f));
	Crosshair->setTexture(new Texture2D("textures/crosshair.dds"));
	this->attachChild(Crosshair);
}

void PauseMenu::onButtonResume(int x, int y)
{
	Game::getInstance().getTimer().resume();
	delete(this);
	Game::getInstance().openWidget(0);
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
