#include "QuWidgetButton.h"

#include "QuWidgetText.h"

 QuWidgetButton::QuWidgetButton() 
{
	
}

void QuWidgetButton::createText(std::string text) 
{

	float Xpos = this->GetPosition().x +  (this->GetSize().x - text.size() * 32) / 2;
	float Ypos = this->GetPosition().y + 25;

	m_text = new QuWidgetText();
	m_text->SetSize(DirectX::XMFLOAT2(1, 1));
	m_text->SetText(text);
	m_text->SetPosition(DirectX::XMFLOAT2(Xpos,Ypos));
	this->attachChild(m_text);
}