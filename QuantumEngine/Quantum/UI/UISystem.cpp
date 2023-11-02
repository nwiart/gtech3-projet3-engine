#include "UISystem.h"

void UISystem::visitUI(QuWidget* widget)
{
	if (widget == nullptr) return;

	for (QuWidget* child = widget->m_FirstChild; child != nullptr; child = child->m_Sibling)
	{
		visitUI(widget);
	}
}