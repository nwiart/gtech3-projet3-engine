#include "UISystem.h"

#include "Graphics.h"
#include "QuWidgetButton.h"


void UISystem::visitUI(QuWidget* widget)
{
	if (widget == nullptr) return;

	Graphics::getInstance().getUIRenderer()->addWidget(widget);

	for (QuWidget* child = widget->m_FirstChild; child != nullptr; child = child->m_Sibling)
	{
		visitUI(child);
	}
}

void UISystem::addWidget(QuWidget* widget)
{

}

bool UISystem::OnLeftClick(int mouseX, int mouseY, QuWidget* widget)
{
	if (widget == nullptr) return false;


	if (mouseX >= widget->GetPosition().x && mouseX <= widget->GetPosition().x + widget->GetSize().x
		&& mouseY >= widget->GetPosition().y && mouseY <= widget->GetPosition().y + widget->GetSize().y) 
	{
		QuWidgetButton* button = dynamic_cast<QuWidgetButton*>(widget);
		if (button && button->GetCallBack()) 
		{
			((button)->*(button->GetCallBack()))(mouseX, mouseY);
			return true;
		}
	}

	for (QuWidget* child = widget->m_FirstChild; child != nullptr; child = child->m_Sibling)
	{
		if (OnLeftClick(mouseX, mouseY, child))
		{
			return true;
		}
	}

	return false;
}
