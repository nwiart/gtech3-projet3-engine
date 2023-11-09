#include "HUDGame.h"

#include "Game.h"
#include "CreateWorld.h"
#include "ResourceLibrary.h"

HUDGame::HUDGame()
{
}

void HUDGame::createHUD()
{
	m_scoreValue = 0;
	scoreText = std::to_string(m_scoreValue);
	score = new QuWidgetText();
	score->SetPosition(DirectX::XMFLOAT2(alignCenter(scoreText.size() * 32, Game::getInstance().getRenderWidth()), 0.f));
	score->SetText(scoreText);

	QuWidgetButton* crosshair = new QuWidgetButton();
	crosshair->SetSize(DirectX::XMFLOAT2(128.0F, 64.0F));
	crosshair->SetPosition(DirectX::XMFLOAT2(alignCenter(crosshair->GetSize().x, Game::getInstance().getRenderWidth()), alignCenter(crosshair->GetSize().y, Game::getInstance().getRenderHeight())));
	crosshair->setTexture(&ResourceLibrary::Get().crosshair);

	this->attachChild(score);
	this->attachChild(crosshair);
}

float HUDGame::alignCenter(float objectTo, float objectIn)
{
	return (objectIn - objectTo) / 2;;
}

void HUDGame::setScore(int scoreValue)
{
	m_scoreValue += scoreValue;
	scoreText = std::to_string(m_scoreValue);
	score->SetText(scoreText);
}
