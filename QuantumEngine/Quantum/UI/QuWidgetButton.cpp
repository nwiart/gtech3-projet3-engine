#include "QuWidgetButton.h"

#include "QuWidgetText.h"

 QuWidgetButton::QuWidgetButton() 
{
	
}

void QuWidgetButton::createText(const char* text) 
{
	m_text = new QuWidgetText();
	m_text->SetSize(DirectX::XMFLOAT2(0.5f, 0.5f));
	m_text->SetPosition(this->GetPosition());
	m_text->SetText(text);
	this->attachChild(m_text);
}