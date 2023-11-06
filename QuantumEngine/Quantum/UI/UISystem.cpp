#include "UISystem.h"

#include "Graphics.h"

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
