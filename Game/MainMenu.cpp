#include "MainMenu.h"
#include "Game.h"

MainMenu::MainMenu()
{
	Title = new QuWidgetText();
	Play = new QuWidgetButton();
	Exit = new QuWidgetButton();
	Play->SetCallBack((QuWidgetButton::OnButtonClickCallback)&MainMenu::onButtonPlay);
	Exit->SetCallBack((QuWidgetButton::OnButtonClickCallback)&MainMenu::onButtonExit);
}

void MainMenu::createMainMenu(Texture2D& buttonTexture)
{
	std::string textTilte = "A Game Made By Quantum Engine";
	Title->SetPosition(DirectX::XMFLOAT2(alignCenter(textTilte.size() * 32, Game::getInstance().getRenderWidth()), 100.f));
	Title->SetText(textTilte);
	this->attachChild(Title);

	QuWidgetText* test = new QuWidgetText();
	std::string testtext = "SpaceNVADERS";
	test->SetPosition(DirectX::XMFLOAT2(alignCenter(testtext.size() * 32, Game::getInstance().getRenderWidth()), 50.f));
	test->SetText(testtext);
	this->attachChild(test);

	Play->SetSize(DirectX::XMFLOAT2(400.f, 100.f));
	Play->SetPosition(DirectX::XMFLOAT2(alignCenter(Play->GetSize().x, Game::getInstance().getRenderWidth()), 200.f));
	Play->setTexture(&buttonTexture);
	Play->createText("Play");
	this->attachChild(Play);

	Exit->SetSize(DirectX::XMFLOAT2(400.f, 100.f));
	Exit->SetPosition(DirectX::XMFLOAT2(alignCenter(Exit->GetSize().x, Game::getInstance().getRenderWidth()), 400.f));
	Exit->setTexture(&buttonTexture);
	Exit->createText("Exit");
	this->attachChild(Exit);
}

void MainMenu::onButtonPlay(int x, int y)
{
	delete(this);
	Game::getInstance().openWidget(0);
}

void MainMenu::onButtonExit(int x, int y)
{
	SendMessage(Game::getInstance().getWindow().getHwnd(), WM_CLOSE, 0, 0);
}

float MainMenu::alignCenter(float objectTo, float objectIn)
{
	float X = (objectIn - objectTo) / 2;

	return X;
}
